#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <unordered_map>

#include "game_class.h"

#define DRAW -1
#define GAME_TIME 30


Game::Game() :gameServer(4)
{
    state = PREINIT;

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
}

void Game::CreateMap() {
    factory = new Factory();

    // Необходимо реализовать поля с общим размером, которые также будут передаваться в конструктор карты

    unsigned int width = 20;
    unsigned int height = 20;

    EventMessage message(EventMessage::CREATE_MAP, 0, width, height, 4);
    event.push(message);

    map = new Map(width, height);

    zone = new Zone(3, width, height);
    EventMessage createZone(EventMessage::CREATE_ZONE, 0, zone->getX(), zone->getY(), zone->getRad());
    event.push(createZone);

    teamCount = 2;
    playersInTeamCount = 2;
    playerIds = new unsigned int[teamCount * playersInTeamCount];

    objects.reserve(10);

    spawnpoints = new std::pair<unsigned int, unsigned int>[teamCount * playersInTeamCount];

    for (unsigned short i = 0; i < teamCount * playersInTeamCount; i++) {
        if (i < playersInTeamCount) {
            spawnpoints[i].second = 1;
            spawnpoints[i].first = i + 1;
        } else {
            spawnpoints[i].second = height - 2;
            spawnpoints[i].first = width - (i % playersInTeamCount) - 2;
        }
    }

    EventMessage createHeal(EventMessage::CREATE_OBJECT, healingPotion::ID, width / 2, height / 2);
    event.push(createHeal);

    for (unsigned short i = 0; i < playersInTeamCount * teamCount; i++) {
        auto players = factory->createPlayer();
        playerIds[i] = players.first;
        players.second->setTeam((char) (i / playersInTeamCount));
        players.second->saveSpawnpoint(spawnpoints[i]);
        players.second->setXY(players.second->getSpawnpoint().first, players.second->getSpawnpoint().second);


        objects.insert(players);
        map->addObject(players.first, players.second->getSpawnpoint().first, players.second->getSpawnpoint().second);

        EventMessage createPlayer(EventMessage::CREATE_PLAYER, players.first, players.second->getSpawnpoint().first,
                                  players.second->getSpawnpoint().second, (i / playersInTeamCount));

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


int Game::Iteration() {
    while (state != END_OF_GAME) {
        switch (state) {
            case (PREINIT):
                app.render(&event);
                if (app.processInput(&request)) {

                        app.changeState();
                        state = INIT;
                }
                break;
            case (INIT):
                app.render(&event);
                if (app.processInput(&request)) {

                    /// Place for address output and port input
                    if (gameServer.init("0.0.0.0", "5000")) {
                        app.changeState();
                        state = WAITING_FOR_GAME;
                    } else {
                        /// ADD "TRY AGAIN" MESSAGE
                        std::cout << "Error during server start" << std::endl;
                    }
                }
                break;
            case (WAITING_FOR_GAME):
                for (size_t i = 0; i < teamCount * playersInTeamCount; ++i) {
                    
                }
                app.render(&event);
                if (app.processInput(&request)) {
                    app.changeState();
                    state = STARTED;
                    CreateMap();
                }
                break;
            case (STARTED): {
                while (!request.empty()) {
                    unsigned int initMsgCount = 0;
                    EventMessage **initEventMessages = moveHandler->Handle(request.front(), map, &objects, &initMsgCount, factory);

                    if (initEventMessages != nullptr) {
                        for (unsigned int i = 0; i < initMsgCount; i++) {
                            event.push(*(initEventMessages[i]));
                        }
                        delete initEventMessages;
                        initEventMessages = nullptr;
                    }
                    request.pop();
                }

                long start = clock();
                long prev = 0;
                while ((clock() - start) / CLOCKS_PER_SEC != GAME_TIME && !gameServer.closeGameReq) {
                    if ((clock() - start) / CLOCKS_PER_SEC - prev > 10) {
                        prev = (clock() - start) / CLOCKS_PER_SEC;
                        EventMessage sendTime(EventMessage::SEND_TIME, prev, 0, 0, 0);
                    }
                    unsigned int receivedMsgCount = 0;

                    BaseMessage **receivedMsg = gameServer.CheckRequests(&receivedMsgCount);
                    if (receivedMsg != nullptr) {
                        for (unsigned int i = 0; i < receivedMsgCount; i++) {
                            request.push(*receivedMsg[i]);
                        }
                    }

                    while(!event.empty()){
                        gameServer.Run(event.front());
                        app.render(&event);
                        event.pop();
                    }

                    if (app.processInput(&request)) {

                    }
                    start_game();
                }
                app.changeState();

                EventMessage closeGame(EventMessage::CLOSE_GAME, 0, 0, 0, 0);
                event.push(closeGame);
                state = GAME_OVER;

                int res = getWinTeam();
                EventMessage winTeam((res < 0) ? EventMessage::DRAW_GAME : EventMessage::WIN_TEAM, res, 0, 0, 0);
                event.push(winTeam);
                std::cout << "WIN: " << res << std::endl;
                break;
            }

            case (GAME_OVER): {
                while(!event.empty()){
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
        //std::cout << "X: " << point.first << " Y: " << point.second << std::endl;
        //std::cout << "KEY: " << key << std::endl;
        /*for (auto& elem : objects) {
            std::cout << "OBJECTS X: " << elem.second->getX() << " Y: " << elem.second->getY() << std::endl;
        }*/
        map->out(&objects);
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
                    std::cout << "FALSE1" << std::endl;
                    return false;
                }
                if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                    && point.second + 1 <= zone->getY() + zone->getRad() && point.second + 1 >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first, point.second + 1));
                } else {
                    std::cout << "FALSE2" << std::endl;
                    return false;
                }
                if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                    && point.second - 1 <= zone->getY() + zone->getRad() && point.second - 1 >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first, point.second - 1));
                } else {
                    std::cout << "FALSE3" << std::endl;
                    return false;
                }
                if (point.first - 1 <= zone->getX() + zone->getRad() && point.first - 1 >= zone->getX() - zone->getRad()
                    && point.second <= zone->getY() + zone->getRad() && point.second >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first - 1, point.second));
                } else {
                    std::cout << "FALSE4" << std::endl;
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
                std::cout << "HERE" << std::endl;
                if (it->second->isItCover()) {
                    continue;
                } else {
                    passed.emplace_back(std::make_pair(point.first, point.second));

                    if (point.first + 1 <= zone->getX() + zone->getRad() && point.first + 1 >= zone->getX() - zone->getRad()
                        && point.second <= zone->getY() + zone->getRad() && point.second >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first + 1, point.second));
                    } else {
                        std::cout << "FALSE1" << std::endl;
                        return false;
                    }
                    if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                        && point.second + 1 <= zone->getY() + zone->getRad() && point.second + 1 >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first, point.second + 1));
                    } else {
                        std::cout << "FALSE2" << std::endl;
                        return false;
                    }
                    if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                        && point.second - 1 <= zone->getY() + zone->getRad() && point.second - 1 >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first, point.second - 1));
                    } else {
                        std::cout << "FALSE3" << std::endl;
                        return false;
                    }
                    if (point.first - 1 <= zone->getX() + zone->getRad() && point.first - 1 >= zone->getX() - zone->getRad()
                        && point.second <= zone->getY() + zone->getRad() && point.second >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first - 1, point.second));
                    } else {
                        std::cout << "FALSE4" << std::endl;
                        return false;
                    }
                }

            }
        }

    }
    std::cout << "TRUE" << std::endl;
    return true;
}

int Game::getWinTeam() {
    std::vector<std::pair<int, int>> teams;
    for (int i = 0; i < teamCount * playersInTeamCount; ++i) {
        auto it = objects.find(playerIds[i]);
        if ((it->second->getX() > zone->getX() - zone->getRad()) && (it->second->getX() < zone->getX() + zone->getRad())
            && (it->second->getY() > zone->getY() - zone->getRad()) &&
            (it->second->getY() < zone->getY() + zone->getRad())) {
            if (move(it->second->getX(), it->second->getY())) {

                if (teams.empty()) {
                    teams.emplace_back(std::make_pair(1, it->second->getTeam()));
                }
                for (int k = 0; k < teams.size(); ++k) {  // ?????????????????????
                    if (teams.at(k).second == it->second->getTeam()) {
                        teams.at(k).first += 1;
                    } else {
                        teams.emplace_back(std::make_pair(1, it->second->getTeam()));
                    }
                }
            }
        }
    }
    // выяснить, какая тима имеет больше застроившихся игроков
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

    // определяем киллы
    std::vector<int> finalTeams;
    if (res.empty()) {
        auto killsTeam = new unsigned int[teamCount];
        for (int i = 0; i < teamCount; ++i) {
            killsTeam[i] = 0;
        }
        for (int i = 0; i < teamCount * playersInTeamCount; ++i) {
            auto it = objects.find(playerIds[i]);
            for (int j = 0; j < teamCount; ++j) {
                if (it->second->getTeam() == j) {
                    killsTeam[j] += it->second->getKills();
                }
            }
        }
        for (int i = 0; i < teamCount; ++i) {
        }
        biggest = killsTeam[0];
        finalTeams.emplace_back(0);
        for (int i = 1; i < teamCount; ++i) {
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
        std::vector<std::pair<unsigned int, int>> killsTeam;
        for (int i = 0; i < teamCount * playersInTeamCount; ++i) {
            auto it = objects.find(playerIds[i]);
            for (int re : res) {
                if (it->second->getTeam() == re) {
                    for (int k = 0; k < killsTeam.size(); ++k) {  // ?????????????????????
                        if (killsTeam.at(k).second == it->second->getTeam()) {
                            killsTeam.at(k).first += it->second->getKills();
                        } else {
                            killsTeam.emplace_back(std::make_pair(it->second->getKills(), it->second->getTeam()));
                        }
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
            return 0;

        return finalTeams.at(0);

    }
}

