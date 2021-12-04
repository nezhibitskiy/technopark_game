#include <iostream>

#include "game_class.h"

Game::Game() {
    state = INIT;
    factory = new Factory();

    // Необходимо реализовать поля с общим размером, которые также будут передаваться в конструктор карты
    unsigned int width = 5;
    unsigned int height = 10;

    EventMessage message(EventMessage::CREATE_MAP, 0, width, height,4);
    event.push(message);

    map = new Map(width, height);

    //unsigned int id = 0;

    teamCount = 2;
    playersInTeamCount = 2;

    objects.reserve(100);

    spawnpoints = new std::pair<unsigned int, unsigned int>[teamCount * playersInTeamCount];

    for (unsigned short i = 0; i < teamCount * playersInTeamCount; i++) {
        if (i < playersInTeamCount) {
            spawnpoints[i].second = 1;
            spawnpoints[i].first = i + 1;
        }
        else {
            spawnpoints[i].second = height - 2;
            spawnpoints[i].first = width - (i % playersInTeamCount) - 2;
        }
    }

    //EndBlock *endBlocks = new EndBlock[2 * height];

    //Player *players = new Player[playersInTeamCount * teamCount];

    for (unsigned short i = 0; i < playersInTeamCount * teamCount; i++) {
        auto obj = factory->createObject(ObjectClass::playerObject);
        std::pair<unsigned int, Player*> players;
        players.first = obj.first;
        players.second = dynamic_cast<Player *>(obj.second);
        players.second->setTeam((char)(i / playersInTeamCount));
        players.second->saveSpawnpoint(spawnpoints[i]);
        players.second->setXY(players.second->getSpawnpoint().first, players.second->getSpawnpoint().second);

        objects.insert(players);
        map->addObject(players.first, players.second->getSpawnpoint().first, players.second->getSpawnpoint().second);

        EventMessage message(EventMessage::CREATE_PLAYER, players.first, players.second->getSpawnpoint().first,
                players.second->getSpawnpoint().second, (i / playersInTeamCount));
        event.push(message);
    }

    for (unsigned int i = 0; i < width; ++i) {
        for (unsigned int j = 0; j < height; ++j) {
            if (i == 0 || j == 0 || (i == width - 1) || (j == height - 1)) {
                auto obj = factory->createObject(endBlockObject);
                std::pair<unsigned int, EndBlock*> block;
                block.first = obj.first;
                block.second = dynamic_cast<EndBlock *>(obj.second);
                objects.insert(block);
                map->addObject(block.first, i, j);
                EventMessage message1(EventMessage::CREATE_OBJECT, block.first, i, j);
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
    for (auto &elem : objects) {
        delete elem.second;
    }
    objects.clear();
}


int Game::Iteration() {
    while (state != END_OF_GAME) {
        switch(state) {
            case (INIT):
               // std::cout << "INIT STATE WAS HERE" << std::endl;
                app.render(&event);
                if(app.processInput(&request)){
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
                if(app.processInput(&request)){
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
                map->out(&objects);
                while (true) {
                    app.render(&event);
                    if(app.processInput(&request)){

                    }
                    start_game();

                }
                state = END_OF_GAME;
                break;
            default:
                break;

        }
    }
    std::cout << "END OF GAME WAS HERE" << std::endl;

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

