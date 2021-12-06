#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <unordered_map>

#include "game_class.h"

#define DRAW -1
#define GAME_TIME 30


Game::Game() {
    state = INIT;
    factory = new Factory();

    // Необходимо реализовать поля с общим размером, которые также будут передаваться в конструктор карты
    unsigned int width = 16;
    unsigned int height = 16;

    EventMessage message(EventMessage::CREATE_MAP, 0, width, height, 4);
    event.push(message);

    map = new Map(width, height);

    zone = new Zone();
    zone->setXY((0 + 8) + rand() % ((width - 8) - 8 + 1), (0 + 8) + rand() % ((height - 8) - 8 + 1), 8);
    EventMessage createZone(EventMessage::CREATE_ZONE, 0, zone->getX(), zone->getY(), zone->getRad());
    event.push(createZone);

    teamCount = 2;
    playersInTeamCount = 2;
    playerIds = new unsigned int[teamCount * playersInTeamCount];

    objects.reserve(100);

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
                EventMessage message1(EventMessage::CREATE_OBJECT, EndBlock::ID, i, j);
                event.push(message1);
            }
        }
    }


    moveHandler = new MoveHandler;
    attackHandler = new AttackHandler;
    putBlockHandler = new PutBlockHandler;
    moveHandler->SetNext(attackHandler)->SetNext(putBlockHandler);
}

Game::~Game() {
    delete moveHandler;
    delete attackHandler;
    delete putBlockHandler;
    // Добавить очистку объектов хэш таблицы
    /*auto it = objects.begin();
    while (it != objects.end()) {
        delete it->second;
        it++;
    }*/
    for (auto &elem: objects) {
        delete elem.second;
    }
    objects.clear();
    delete factory;
    delete zone;
}


int Game::Iteration() {
    long start = clock();
    while (state != END_OF_GAME) {
        switch (state) {
            case (INIT):
                // std::cout << "INIT STATE WAS HERE" << std::endl;
                app.render(&event);
                if (app.processInput(&request)) {
                    app.changeState();
                    state = WAITING_FOR_GAME;
                }


                // Вписываются все функции частей игры и условия перехода
                // if (window.init()) {
                // state = SERVER_STARTED
                // state = CLIENT_CONNECT_TO_SERVER
                // window.find_serv()
                //}
                break;
            case (WAITING_FOR_GAME):

                // std::cout << "WAITING FOR GAME WAS HERE" << std::endl;
                app.render(&event);
                if (app.processInput(&request)) {
                    app.changeState();
                    BaseMessage moveUp1(MoveHandler::MOVE_DOWN, 1);
                    request.push(moveUp1);

                    BaseMessage moveUp2(MoveHandler::MOVE_UP, 2);
                    request.push(moveUp2);
                    BaseMessage moveUp3(MoveHandler::MOVE_UP, 3);
                    request.push(moveUp3);
                    state = STARTED;

                }


                break;
            case (STARTED):
                while ((clock() - start) / CLOCKS_PER_SEC != GAME_TIME) {
                    map->out(&objects);

                    app.render(&event);
                    if (app.processInput(&request)) {

                    }
                    start_game();
                }
                state = GAME_OVER;
                break;

            case (GAME_OVER):
                int res = getWinTeam();
                EventMessage winTeam(EventMessage::WIN_TEAM, res, 0, 0, 0);
                event.push(winTeam);
                std::cout << "WIN: " << res << "    END OF GAME WAS HERE" << std::endl;

                app.render(&event);
                if (app.processInput(&request)) {
                    state = END_OF_GAME;
                }
                break;
        }
    }


    return EXIT_SUCCESS;
}

void Game::start_game() {


//    BaseMessage moveUp1(MoveHandler::MOVE_DOWN, 1);
//    request.push(moveUp1);
//
//    BaseMessage moveUp2(MoveHandler::MOVE_UP, 2);
//    request.push(moveUp2);
//    BaseMessage moveUp3(MoveHandler::MOVE_UP, 3);
//    request.push(moveUp3);


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
        map->out(&objects);
    }




    /* while (!event.empty()) {
         std::cout << "New Event Message: type: " << event.front().getType() << "; ID: " << event.front().getID() <<
                   "; X: " << event.front().getX() << "; Y: " << event.front().getY() << "; Data: " << event.front().getData() << ";\n";
         event.pop();
     }*/

    bool gameFlag = true;

//    while(gameFlag) {
//        char key;
//        std::cin >> key;
//        //проверка айдишников
//        for (auto &el : objects) {
//            std::cout << el.first << " ";
//        }
//        std::cout << std::endl;
//        switch (key) {
//            case('q'):
//                gameFlag = false;
//                break;
//            case('w'): {
//                BaseMessage moveUp(MoveHandler::MOVE_UP, 0);
//                request.push(moveUp);
//                break;
//            }
//            case('a'): {
//                BaseMessage moveLeft(MoveHandler::MOVE_LEFT, 0);
//                request.push(moveLeft);
//                break;
//            }
//            case('s'): {
//                BaseMessage moveDown(MoveHandler::MOVE_DOWN, 0);
//                request.push(moveDown);
//                break;
//            }
//            case('d'): {
//                BaseMessage moveRight(MoveHandler::MOVE_RIGHT, 0);
//                request.push(moveRight);
//                break;
//            }
//
//            case('t'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage attackUp(AttackHandler::ATTACK, 0, playerNode->second->getX(), playerNode->second->getY() - 1);
//                request.push(attackUp);
//                break;
//            }
//            case('f'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage attackLeft(AttackHandler::ATTACK, 0, playerNode->second->getX() - 1, playerNode->second->getY());
//                request.push(attackLeft);
//                break;
//            }
//            case('g'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage attackDown(AttackHandler::ATTACK, 0, playerNode->second->getX(), playerNode->second->getY() + 1);
//                request.push(attackDown);
//                break;
//            }
//            case('h'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage attackRight(AttackHandler::ATTACK, 0, playerNode->second->getX() + 1, playerNode->second->getY());
//                request.push(attackRight);
//                break;
//            }
//
//
//            case('i'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage putBlockUp(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX(), playerNode->second->getY() - 1);
//                request.push(putBlockUp);
//                break;
//            }
//            case('j'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage putBlockLeft(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX() - 1, playerNode->second->getY());
//                request.push(putBlockLeft);
//                break;
//            }
//            case('k'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage putBlockDown(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX(), playerNode->second->getY() + 1);
//                request.push(putBlockDown);
//                break;
//            }
//            case('l'): {
//                auto playerNode = objects.find(0);
//                if (playerNode == objects.end()) break;
//                BaseMessage putBlockRight(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX() + 1, playerNode->second->getY());
//                request.push(putBlockRight);
//                break;
//            }
//
//            default: {
//                break;
//            }
//        }

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

        /*while (!event.empty()) {
            std::cout << "New Event Message: type: " << event.front().getType() << "; ID: " << event.front().getID() <<
            "; X: " << event.front().getX() << "; Y: " << event.front().getY() << "; Data: " << event.front().getData() << ";\n";
            event.pop();
        }*/

        request.pop();
        map->out(&objects);
    }

//    }
}

bool Game::move(unsigned int x, unsigned int y) {
    std::queue<std::pair<unsigned int, unsigned int>> q;
    q.push(std::make_pair(x, y));
    std::vector<std::pair<unsigned int, unsigned int>> passed;
    while (!q.empty()) {
        std::pair<unsigned int, unsigned int> point = q.front();
        q.pop();
        unsigned int key = map->getObject(point.first, point.second);
        if (key == 400000U) {
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

                if (point.first + 1 < map->getWidth() && point.first + 1 > 0  && point.second < map->getHeight() && point.second > 0) {
                    q.push(std::make_pair(point.first + 1, point.second));
                } else {
                    std::cout << "FALSE" << std::endl;
                    return false;
                }
                if (point.first < map->getWidth() && point.first > 0 && point.second + 1 < map->getHeight() && point.second + 1 > 0) {
                    q.push(std::make_pair(point.first, point.second + 1));
                } else {
                    std::cout << "FALSE" << std::endl;
                    return false;
                }
                if (point.first < map->getWidth() && point.first > 0 && point.second - 1 < map->getHeight() && point.second - 1 > 0) {
                    q.push(std::make_pair(point.first, point.second - 1));
                } else {
                    std::cout << "FALSE" << std::endl;
                    return false;
                }
                if (point.first - 1 < map->getWidth() && point.first - 1 > 0 && point.second < map->getHeight() && point.second > 0) {
                    q.push(std::make_pair(point.first - 1, point.second));
                } else {
                    std::cout << "FALSE" << std::endl;
                    return false;
                }

            }
        } else {
            bool flag = false;
            auto it = objects.find(key);
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

                    if (point.first + 1 < map->getWidth() && point.first + 1 > 0  && point.second < map->getHeight() && point.second > 0) {
                        q.push(std::make_pair(point.first + 1, point.second));
                    } else {
                        std::cout << "FALSE" << std::endl;
                        return false;
                    }
                    if (point.first < map->getWidth() && point.first > 0 && point.second + 1 < map->getHeight() && point.second + 1 > 0) {
                        q.push(std::make_pair(point.first, point.second + 1));
                    } else {
                        std::cout << "FALSE" << std::endl;
                        return false;
                    }
                    if (point.first < map->getWidth() && point.first > 0 && point.second - 1 < map->getHeight() && point.second - 1 > 0) {
                        q.push(std::make_pair(point.first, point.second - 1));
                    } else {
                        std::cout << "FALSE" << std::endl;
                        return false;
                    }
                    if (point.first - 1 < map->getWidth() && point.first - 1 > 0 && point.second < map->getHeight() && point.second > 0) {
                        q.push(std::make_pair(point.first - 1, point.second));
                    } else {
                        std::cout << "FALSE" << std::endl;
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
        /*if ((it->second->getX() > zone->getX() - zone->getRad()) && (it->second->getX() < zone->getX() + zone->getRad())
            && (it->second->getY() > zone->getY() - zone->getRad()) &&
            (it->second->getY() < zone->getY() + zone->getRad()))*/
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
        return finalTeams.at(0);
    }
}

