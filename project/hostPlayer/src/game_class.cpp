#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <unordered_map>

#include "game_class.h"

#define DRAW -1
#define GAME_TIME 100


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

    // Необходимо реализовать поля с общим размером, которые также будут передаваться в конструктор карты

    unsigned int width = 20;
    unsigned int height = 20;

    EventMessage message(EventMessage::CREATE_MAP, 0, width, height, 4);
    event.push(message);

    map = new Map(width, height);

    zone = new Zone(3, width, height);
    EventMessage createZone(EventMessage::CREATE_ZONE, 0, zone->getX(), zone->getY(), zone->getRad());
    event.push(createZone);

    //playerIds = new unsigned int[teamCount * playersInTeamCount];

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
        std::cout << "PLAYER CREATED  ID:" << players.first << " X: " << players.second->getSpawnpoint().first << " Y:"
                  << players.second->getSpawnpoint().second << " TEAM: " << playerTeams.at(i).second << std::endl;
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

    /*for (unsigned long i = map->getWidth() / 5; i < map->getWidth() - 1; i += map->getWidth() / 5) {
        for (unsigned long j = map->getHeight() / 5; j < map->getHeight() - 1; j += map->getHeight() / 5) {
            auto heal = factory->createObject(healPot);
            map->addObject(heal.first, i, j);
            objects.insert(heal);
            EventMessage createHeal(EventMessage::CREATE_OBJECT, healingPotion::ID, i, j);
            event.push(createHeal);
        }
    }*/
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distY(2, map->getHeight() - 3);
    //x = distX(rng);
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
                    //app.changeState();
                    state = WAITING_FOR_GAME;

                    BaseMessage connectedClient(gameServer::server::CONNECTING_CLIENT, 0);
                    request.push(connectedClient);

                    //BaseMessage clientChooseTeam(gameServer::server::ADD_CLIENT_TO_TEAM, 0, 0);
                    // request.push(clientChooseTeam);


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
                //finish = start;
                std::cout << finish.tv_sec - start.tv_sec << std::endl;
                std::cout << "START: " << start.tv_sec << " END: " << finish.tv_sec << std::endl;
                // while (finish.tv_sec - start.tv_sec < GAME_TIME && !gameServer.closeGameReq) {
                EventMessage sendTimeFirst(EventMessage::SEND_TIME, GAME_TIME, 0, 0, 0);
                event.push(sendTimeFirst);
                int counter = 1;
                while (GAME_TIME - finish.tv_sec + start.tv_sec >= 0) {
                    if (finish.tv_sec - start.tv_sec - prev >= 1) {
                        prev = finish.tv_sec - start.tv_sec;
                        std::cout << "SEND TIME: " << GAME_TIME - (unsigned short) prev << std::endl;
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

                    app.render(&event); // check !event.empty())
                    if (!event.empty()) {
                        event.pop();
                    }
                    if (app.processInput(&request)) {
                    }
                    start_game();
                    clock_gettime(CLOCK_MONOTONIC, &finish);
                    //std::cout << finish.tv_sec - start.tv_sec << std::endl;
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
            std::cout << "Connecting client. Avaliable teams: " << teamAvailable << std::endl;
        } else if (newMessage.getType() == gameServer::server::ADD_CLIENT_TO_TEAM) {
            if (newMessage.getX() < maxTeams && playersInTeamsCount[newMessage.getX()] < maxPlayersInTeams) {
                std::cout << "Received client team choose" << std::endl;
                for (int i = 0; i < playerTeams.size(); ++i) {
                    if (playerTeams.at(i).first == newMessage.getID()) {
                        request.pop();
                        return;
                    }
                }
                //std::cout << "ADD PLAYER TO TEAM" << std::endl;
                playersInTeamsCount[newMessage.getX()]++;
                playerTeams.emplace_back(std::make_pair(newMessage.getID(), newMessage.getX()));
                EventMessage addPlayer(EventMessage::PLAYER_ADDED_TO_TEAM, newMessage.getID(), newMessage.getX(), 0,
                                       newMessage.getX());
                event.push(addPlayer);
                std::cout << "Add client to team. Team: " << newMessage.getX() << std::endl;
            }
            //std::cout << "request.size: " << request.size() << std::endl;
            request.pop();
            //std::cout << "request.size: " << request.size() << std::endl;

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

        //std::cout << "X: " << point.first << " Y: " << point.second << std::endl;
        //std::cout << "KEY: " << key << std::endl;
        /*for (auto& elem : objects) {
            std::cout << "OBJECTS X: " << elem.second->getX() << " Y: " << elem.second->getY() << std::endl;
        }*/
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
                    && point.second + 1 <= zone->getY() + zone->getRad() &&
                    point.second + 1 >= zone->getY() - zone->getRad()) {
                    q.push(std::make_pair(point.first, point.second + 1));
                } else {
                    std::cout << "FALSE2" << std::endl;
                    return false;
                }

                if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                    && point.second - 1 <= zone->getY() + zone->getRad() &&
                    point.second - 1 >= zone->getY() - zone->getRad()) {
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
                        std::cout << "FALSE1" << std::endl;
                        return false;
                    }
                    if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                        && point.second + 1 <= zone->getY() + zone->getRad() &&
                        point.second + 1 >= zone->getY() - zone->getRad()) {

                        q.push(std::make_pair(point.first, point.second + 1));
                    } else {
                        std::cout << "FALSE2" << std::endl;
                        return false;
                    }
                    if (point.first <= zone->getX() + zone->getRad() && point.first >= zone->getX() - zone->getRad()
                        && point.second - 1 <= zone->getY() + zone->getRad() &&
                        point.second - 1 >= zone->getY() - zone->getRad()) {
                        q.push(std::make_pair(point.first, point.second - 1));
                    } else {
                        std::cout << "FALSE3" << std::endl;
                        return false;
                    }
                    if (point.first - 1 <= zone->getX() + zone->getRad() &&
                        point.first - 1 >= zone->getX() - zone->getRad()
                        && point.second <= zone->getY() + zone->getRad() &&
                        point.second >= zone->getY() - zone->getRad()) {
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
    std::cout << "PLAYERS COUNT: " << playerTeams.size() << std::endl;
    std::cout << "TEAMS: ";
    for (int i = 0; i < playerTeams.size(); ++i) {
        std::cout << (int) playerTeams.at(i).second << " ";
    }
    std::cout << std::endl;
    bool flag = false;
    for (int i = 0; i < playerTeams.size(); ++i) {
        flag = false;
        auto it = objects.find(playerTeams.at(i).first);   // playerIds[i]
        if ((it->second->getX() > zone->getX() - zone->getRad()) && (it->second->getX() < zone->getX() + zone->getRad())
            && (it->second->getY() > zone->getY() - zone->getRad()) &&
            (it->second->getY() < zone->getY() + zone->getRad())) {
            if (move(it->second->getX(), it->second->getY())) {
                /*if (teams.empty()) {
                    teams.emplace_back(std::make_pair(1, it->second->getTeam()));

                }*/
                for (int k = 0; k < teams.size(); ++k) {  // ?????????????????????
                    if (teams.at(k).second == it->second->getTeam()) {
                        teams.at(k).first += 1;
                        std::cout << "PAST PARE:  kolvo:" << teams.at(k).first << " TEAM: " << teams.at(k).second << std::endl;
                        flag = true;
                    }
                }
                if (!flag) {
                    teams.emplace_back(std::make_pair(1, it->second->getTeam()));
                    std::cout << "NEW PARE: kolvo: " << teams.at(teams.size() - 1).first << " TEAM: " << teams.at(teams.size() - 1).second << std::endl;
                }
            }
        }
    }
    for (int i = 0; i < teams.size(); ++i) {
        std::cout << "KOLVO: " << teams.at(i).first << "TEAM: " << teams.at(i).second << std::endl;
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
    std::cout << "RES" << std::endl;
    for (int i = 0; i < res.size(); ++i) {
        std::cout << res.at(i) << " ";
    }
    std::cout << std::endl;

    // определяем киллы
    for (int i = 0; i < playerTeams.size(); ++i) {
        auto it = objects.find(playerTeams.at(i).first);
        std::cout << "Player team " << (int)playerTeams.at(i).second << " KILLS: " << it->second->getKills() << std::endl;
    }
    std::vector<int> finalTeams;
    if (res.empty()) {
        auto killsTeam = new unsigned int[maxTeams]; //teamCount
        for (int i = 0; i < maxTeams; ++i) {  // teamCount
            killsTeam[i] = 0;
        }
        for (int i = 0; i < playerTeams.size(); ++i) {
            auto it = objects.find(playerTeams.at(i).first);  // playerIds[i]
            for (int j = 0; j < maxTeams; ++j) {  //teamCount
                if (it->second->getTeam() == j) {
                    killsTeam[j] += it->second->getKills();
                }
            }
        }
        /*for (int i = 0; i < teamCount; ++i) {
        }*/
        biggest = killsTeam[0];
        finalTeams.emplace_back(0);
        for (int i = 1; i < maxTeams; ++i) {   //teamCount
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
        std::cout << "ELSE " << std::endl;
        bool fl = false;
        std::vector<std::pair<unsigned int, int>> killsTeam;
        for (int i = 0; i < playerTeams.size(); ++i) {
            flag = false;
            auto it = objects.find(playerTeams.at(i).first);  //playerIds[i]
            for (int re: res) {
                if (it->second->getTeam() == re) {
                    for (int k = 0; k < killsTeam.size(); ++k) {  // ?????????????????????
                        if (killsTeam.at(k).second == it->second->getTeam()) {
                            killsTeam.at(k).first += it->second->getKills();
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

