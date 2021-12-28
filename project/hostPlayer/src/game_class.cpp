#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <unordered_map>

#include "game_class.h"

#define DRAW -1
#define GAME_TIME 30


Game::Game() : gameServer(4) {
    state = PREINIT;
    playerIds = new unsigned int[4];
    playersCount = 0;
    maxTeams = 2;
    maxPlayersInTeams = 2;
    playersInTeamsCount = new unsigned int[maxTeams];
    for (char i = 0; i < maxTeams; i++)
        playersInTeamsCount[i] = 0;
}

Game::~Game() {
    delete moveHandler;
    delete attackHandler;
    delete putBlockHandler;

    for (auto &elem: objects) {
        delete elem.second;
    }
    objects.clear();
    delete factory;
    delete zone;
    delete playersInTeamsCount;
    delete spawnpoints;
    delete playerIds;
}

void Game::CreateMap() {
    factory = new Factory(playersCount);

    unsigned int width = 20;
    unsigned int height = 20;

    EventMessage message(EventMessage::CREATE_MAP, 0, width, height, 4);
    event.push(message);

    map = new Map(width, height);

    zone = new Zone(3, width, height);
    EventMessage createZone(EventMessage::CREATE_ZONE, 0, zone->getX(), zone->getY(), zone->getRad());
    event.push(createZone);

    objects.reserve(10);
    createHeals();
    spawnpoints = new std::pair<unsigned int, unsigned int>[playersCount];

    for (unsigned int i = 0; i < playerTeams.size(); i++) {
        if (playerTeams.at(i).second == 0) {
            spawnpoints[i].second = 1;
            spawnpoints[i].first = i + 1;
        } else {
            spawnpoints[i].second = height - 2;
            spawnpoints[i].first = width - (i % maxPlayersInTeams) - 2;
        }
    }

    for (unsigned int i = 0; i < playerTeams.size(); i++) {
        auto players = factory->createPlayer(playerTeams.at(i).first);
        players.second->setTeam(playerTeams.at(i).second);
        players.second->saveSpawnpoint(spawnpoints[i]);
        players.second->setXY(players.second->getSpawnpoint().first, players.second->getSpawnpoint().second);

        objects.insert(players);
        map->addObject(players.first, players.second->getSpawnpoint().first, players.second->getSpawnpoint().second);

        EventMessage createPlayer(EventMessage::CREATE_PLAYER, players.first, players.second->getSpawnpoint().first,
                                  players.second->getSpawnpoint().second, playerTeams.at(i).second);
        event.push(createPlayer);
        EventMessage setHealth(EventMessage::SET_HEALTH, players.first, 0, 0, DEFAULT_HEALTH_VALUE);
        event.push(setHealth);
    }

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            if (i == 0 || j == 0 || (i == width - 1) || (j == height - 1)) {
                auto obj = factory->createObject(endBlockObject);
                objects.insert(obj);
                map->addObject(obj.first, i, j);
                EventMessage createEndBlock(EventMessage::CREATE_OBJECT, EndBlock::ID, i, j);
                event.push(createEndBlock);
            }
        }
    }

    moveHandler = new MoveHandler;
    attackHandler = new AttackHandler;
    putBlockHandler = new PutBlockHandler;
    moveHandler->SetNext(attackHandler)->SetNext(putBlockHandler);
}

void Game::createHeals() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distY(2, map->getHeight() - 3);
    unsigned int prev = 1;
    for (int i = 1; i < 5; ++i) {
        std::uniform_int_distribution<std::mt19937::result_type> distX(prev, prev + map->getWidth() / 4 - 1);
        unsigned long x = distX(rng);
        unsigned long y = distY(rng);
        unsigned int key = map->getObject(x, y);
        if (objects.find(key) != objects.end()) {
            continue;
        }
        auto heal = factory->createObject(healPot);
        map->addObject(heal.first, x, y);
        objects.insert(heal);
        EventMessage createHeal(EventMessage::CREATE_OBJECT, healingPotion::ID, x, y);
        event.push(createHeal);
        prev += map->getWidth() / 4 - 1;
    }
}

int Game::Iteration() {
    while (state != END_OF_GAME) {
        switch (state) {
            case (PREINIT):
                app.render(&event);
                if (app.processInput(&request)) {

                    app.changeState();
                    app.changeState();
                    state = INIT;
                }
                break;
            case (INIT):
                /// Place for address output and port input
                if (gameServer.init("0.0.0.0", "5000")) {
                    state = WAITING_FOR_GAME;

                    BaseMessage connectedClient(gameServer::server::CONNECTING_CLIENT, 0);
                    request.push(connectedClient);
                } else {
                    /// ADD "TRY AGAIN" MESSAGE
                    std::cout << "Error during server start" << std::endl;
                }
                break;
            case (WAITING_FOR_GAME): {
                unsigned int receivedMsgCount = 0;
                BaseMessage **receivedMsg = gameServer.CheckRequests(&receivedMsgCount);
                if (receivedMsg != nullptr) {
                    for (unsigned int i = 0; i < receivedMsgCount; i++) {
                        request.push(*receivedMsg[i]);
                    }
                }

                if (!request.empty()) {
                    waitingForGame();
                }

                if (!event.empty()) {
                    gameServer.Run(event.front());
                }

                app.render(&event);
                while (!event.empty()) {
                    event.pop();
                }

                if (app.processInput(&request)) {
                    /// Place for address output and port input
                    app.changeState();
                    CreateMap();
                    state = STARTED;
                }
                break;
            }
            case (STARTED): {
                while (!request.empty()) {
                    unsigned int initMsgCount = 0;
                    EventMessage **initEventMessages = moveHandler->Handle(request.front(), map, &objects,
                                                                           &initMsgCount, factory);
                    if (initEventMessages != nullptr) {
                        for (unsigned int i = 0; i < initMsgCount; i++) {
                            event.push(*(initEventMessages[i]));
                        }
                        delete initEventMessages;
                        initEventMessages = nullptr;
                    }
                    request.pop();
                }

                struct timespec start, finish;
                long prev = 0;
                clock_gettime(CLOCK_MONOTONIC, &start);
                clock_gettime(CLOCK_MONOTONIC, &finish);

                EventMessage sendTimeFirst(EventMessage::SEND_TIME, GAME_TIME, 0, 0, 0);
                event.push(sendTimeFirst);
                int counter = 1;
                while (GAME_TIME - finish.tv_sec + start.tv_sec >= 0) {
                    if (finish.tv_sec - start.tv_sec - prev >= 1) {
                        prev = finish.tv_sec - start.tv_sec;
                        EventMessage sendTime(EventMessage::SEND_TIME, GAME_TIME - prev, 0, 0, 0);
                        event.push(sendTime);
                    }
                    if (finish.tv_sec - start.tv_sec >= 30 * counter) {
                        createHeals();
                        counter++;
                    }

                    unsigned int receivedMsgCount = 0;
                    BaseMessage **receivedMsg = gameServer.CheckRequests(&receivedMsgCount);
                    if (receivedMsg != nullptr) {
                        for (unsigned int i = 0; i < receivedMsgCount; i++) {
                            request.push(*receivedMsg[i]);
                        }
                    }

                    if (!event.empty()) {
                        gameServer.Run(event.front());
                    }

                    app.render(&event);
                    if (!event.empty()) {
                        event.pop();
                    }
                    if (app.processInput(&request)) {
                    }
                    start_game();
                    clock_gettime(CLOCK_MONOTONIC, &finish);
                }
                app.changeState();

                EventMessage closeGame(EventMessage::CLOSE_GAME, 0, 0, 0, 0);
                event.push(closeGame);
                state = GAME_OVER;

                int res = getWinTeam();
                EventMessage winTeam((res < 0) ? EventMessage::DRAW_GAME : EventMessage::WIN_TEAM, res, 0, 0, 0);
                event.push(winTeam);
                break;
            }
            case (GAME_OVER): {
                while (!event.empty()) {
                    gameServer.Run(event.front());
                    app.render(&event);
                    event.pop();
                }

                if (app.processInput(&request)) {
                    app.changeState();
                    gameServer.CloseServer();
                    state = END_OF_GAME;
                }
                break;
            }
            case (END_OF_GAME):
                break;
        }
    }
    return EXIT_SUCCESS;
}

void Game::waitingForGame() {
    if (!request.empty()) {
        BaseMessage newMessage = request.front();
        if (newMessage.getType() == gameServer::server::CONNECTING_CLIENT &&
            playersCount < maxPlayersInTeams * maxTeams) {
            for (int i = 0; i < playersCount; ++i) {
                if (playerIds[i] == newMessage.getID()) {
                    request.pop();
                    return;
                }
            }

            playerIds[playersCount] = newMessage.getID();
            playersCount++;
            request.pop();
            unsigned short teamAvailable = 0;
            for (unsigned int i = 0; i < maxTeams; ++i) {
                if (playersInTeamsCount[i] < maxPlayersInTeams) {
                    teamAvailable |= 1U << i;
                }
            }
            EventMessage availableTeamsMsg(EventMessage::AVAILABLE_TEAMS, 0, 0, 0, teamAvailable);
            event.push(availableTeamsMsg);
        } else if (newMessage.getType() == gameServer::server::ADD_CLIENT_TO_TEAM) {
            if (newMessage.getX() < maxTeams && playersInTeamsCount[newMessage.getX()] < maxPlayersInTeams) {
                for (auto & playerTeam : playerTeams) {
                    if (playerTeam.first == newMessage.getID()) {
                        request.pop();
                        return;
                    }
                }
                playersInTeamsCount[newMessage.getX()]++;
                playerTeams.emplace_back(std::make_pair(newMessage.getID(), newMessage.getX()));
                EventMessage addPlayer(EventMessage::PLAYER_ADDED_TO_TEAM, newMessage.getID(), newMessage.getX(), 0,
                                       newMessage.getX());
                event.push(addPlayer);
            }
            request.pop();
        } else
            request.pop();
    }
}

void Game::start_game() {

    if (!request.empty()) {
        unsigned int msgCount = 0;
        EventMessage **newEventMessages = moveHandler->Handle(request.front(), map, &objects, &msgCount, factory);
        if (newEventMessages != nullptr) {
            for (unsigned int i = 0; i < msgCount; i++) {
                event.push(*(newEventMessages[i]));
            }
            delete newEventMessages;
            newEventMessages = nullptr;
        }
        request.pop();
    }
}

bool Game::move(unsigned int x, unsigned int y) {
    std::queue<std::pair<unsigned int, unsigned int>> q;
    q.push(std::make_pair(x, y));
    std::vector<std::pair<unsigned int, unsigned int>> passed;
    while (!q.empty()) {
        std::pair<unsigned int, unsigned int> point = q.front();
        q.pop();
        unsigned int key = map->getObject(point.first, point.second);

        auto it = objects.find(key);
        if (it == objects.end()) {
            bool flag = false;
            for (auto &i: passed) {
                if (i.first == point.first && i.second == point.second) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                continue;
            } else {
                passed.emplace_back(std::make_pair(point.first, point.second));

                if (point.first + 1 <= zone->getX() + zone->getRad() && point.first + 1 >= zone->getX() - zone->getRad()
                    && point.second <= zone->getY() + zone->getRad() && point.second >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first + 1, point.second));
                } else {
                    return false;
                }
                if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                    && point.second + 1 <= zone->getY() + zone->getRad() &&
                    point.second + 1 >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first, point.second + 1));
                } else {
                    return false;
                }

                if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                    && point.second - 1 <= zone->getY() + zone->getRad() &&
                    point.second - 1 >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first, point.second - 1));
                } else {
                    return false;
                }
                if (point.first - 1 <= zone->getX() + zone->getRad() && point.first - 1 >= zone->getX() - zone->getRad()
                    && point.second <= zone->getY() + zone->getRad() && point.second >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first - 1, point.second));
                } else {
                    return false;
                }

            }
        } else {
            bool flag = false;
            for (auto &i: passed) {
                if (i.first == point.first && i.second == point.second) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                continue;
            } else {
                if (it->second->isItCover()) {
                    continue;
                } else {
                    passed.emplace_back(std::make_pair(point.first, point.second));
                    if (point.first + 1 <= zone->getX() + zone->getRad() &&
                        point.first + 1 >= zone->getX() - zone->getRad()
                        && point.second <= zone->getY() + zone->getRad() &&
                        point.second >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first + 1, point.second));
                    } else {
                        return false;
                    }
                    if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                        && point.second + 1 <= zone->getY() + zone->getRad() &&
                        point.second + 1 >= zone->getY() - zone->getRad()) {

                        q.push(std::make_pair(point.first, point.second + 1));
                    } else {
                        return false;
                    }
                    if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                        && point.second - 1 <= zone->getY() + zone->getRad() &&
                        point.second - 1 >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first, point.second - 1));
                    } else {
                        return false;
                    }
                    if (point.first - 1 <= zone->getX() + zone->getRad() &&
                        point.first - 1 >= zone->getX() - zone->getRad()
                        && point.second <= zone->getY() + zone->getRad() &&
                        point.second >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first - 1, point.second));
                    } else {
                        return false;
                    }
                }

            }
        }

    }
    return true;
}

int Game::getWinTeam() {
    std::vector<std::pair<int, int>> teams;
    bool flag = false;
    for (auto & playerTeam : playerTeams) {
        flag = false;
        auto it = objects.find(playerTeam.first);   // playerIds[i]
        if ((it->second->getX() > zone->getX() - zone->getRad()) && (it->second->getX() < zone->getX() + zone->getRad())
            && (it->second->getY() > zone->getY() - zone->getRad()) &&
            (it->second->getY() < zone->getY() + zone->getRad())) {
            if (move(it->second->getX(), it->second->getY())) {
                for (auto & team : teams) {
                    if (team.second == it->second->getTeam()) {
                        team.first += 1;
                        flag = true;
                    }
                }
                if (!flag) {
                    teams.emplace_back(std::make_pair(1, it->second->getTeam()));
                }
            }
        }
    }

    std::vector<int> res;
    unsigned int biggest;
    if (teams.size() == 1) {
        return teams.at(0).second;
    } else if (!teams.empty()) {
        biggest = teams.at(0).first;
        res.emplace_back(teams.at(0).second);
        for (int i = 1; i < teams.size(); ++i) {
            if (teams.at(i).first > biggest) {
                res.clear();
                res.emplace_back(teams.at(i).second);
            } else if (teams.at(i).first == biggest) {
                res.emplace_back(teams.at(i).second);
            } else {
                continue;
            }
        }
    }
    if (res.size() == 1) {
        return res.at(0);
    }

    std::vector<int> finalTeams;
    if (res.empty()) {
        auto killsTeam = new unsigned int[maxTeams];
        for (int i = 0; i < maxTeams; ++i) {
            killsTeam[i] = 0;
        }

        for (auto & playerTeam : playerTeams) {
            auto it = objects.find(playerTeam.first);
            for (int j = 0; j < maxTeams; ++j) {
                if (it->second->getTeam() == j) {
                    killsTeam[j] += it->second->getKills();
                }
            }
        }

        biggest = killsTeam[0];
        finalTeams.emplace_back(0);
        for (int i = 1; i < maxTeams; ++i) {
            if (killsTeam[i] > biggest) {
                biggest = killsTeam[i];
                finalTeams.clear();
                finalTeams.emplace_back(i);
            } else if (killsTeam[i] == biggest) {
                delete[] killsTeam;
                return DRAW;
            } else {
                continue;
            }
        }
        delete[] killsTeam;
        return finalTeams.at(0);
    } else {
        bool fl = false;
        std::vector<std::pair<unsigned int, int>> killsTeam;
        for (auto & playerTeam : playerTeams) {
            flag = false;
            auto it = objects.find(playerTeam.first);
            for (int re: res) {
                if (it->second->getTeam() == re) {
                    for (auto & k : killsTeam) {
                        if (k.second == it->second->getTeam()) {
                            k.first += it->second->getKills();
                            flag = true;
                        }
                    }
                    if (!flag) {
                        killsTeam.emplace_back(std::make_pair(it->second->getKills(), it->second->getTeam()));
                    }
                }
            }
        }
        if (!killsTeam.empty()) {
            biggest = killsTeam.at(0).first;
            finalTeams.emplace_back(killsTeam.at(0).second);
            for (int i = 1; i < killsTeam.size(); ++i) {
                if (killsTeam.at(i).first > biggest) {
                    biggest = killsTeam.at(i).first;
                    finalTeams.clear();
                    finalTeams.emplace_back(killsTeam.at(i).second);
                } else if (killsTeam.at(i).first == biggest) {
                    return DRAW;
                } else {
                    continue;
                }
            }
        }
        if (finalTeams.empty())
            return DRAW;

        return finalTeams.at(0);

    }
}

