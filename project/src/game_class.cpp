#include <iostream>

#include "game_class.h"

Game::Game() : gameServer("0.0.0.0", "5000", 4) {
    state = INIT;
    factory = new Factory();

    // Необходимо реализовать поля с общим размером, которые также будут передаваться в конструктор карты
    unsigned int width = 16;
    unsigned int height = 16;

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

    for (unsigned int i = 0; i < 2 * height; i += 2) {
        auto obj = factory->createObject(endBlockObject);
        std::pair<unsigned int, EndBlock*> block;
        block.first = obj.first;
        block.second = dynamic_cast<EndBlock *>(obj.second);
        objects.insert(block);
        map->addObject(block.first, i / 2, 0);
        EventMessage message1(EventMessage::CREATE_OBJECT, block.first, i / 2, 0);
        event.push(message1);

        obj = factory->createObject(endBlockObject);
        block.first = obj.first;
        block.second = dynamic_cast<EndBlock *>(obj.second);
        objects.insert(block);
        map->addObject(block.first, i / 2, width - 1);
        EventMessage message2(EventMessage::CREATE_OBJECT, block.first, i / 2, width - 1);
        event.push(message2);
    }

    for (unsigned int j = 2; j < 2 * (width - 1); j += 2) {
        auto obj = factory->createObject(endBlockObject);
        std::pair<unsigned int, EndBlock*> block;
        block.first = obj.first;
        block.second = dynamic_cast<EndBlock *>(obj.second);
        objects.insert(block);
        map->addObject(block.first, 0, j / 2);
        EventMessage message1(EventMessage::CREATE_OBJECT, block.first, 0, j / 2);
        event.push(message1);

        obj = factory->createObject(endBlockObject);
        block.first = obj.first;
        block.second = dynamic_cast<EndBlock *>(obj.second);
        objects.insert(block);
        map->addObject(block.first, height - 1, j / 2);
        EventMessage message2(EventMessage::CREATE_OBJECT, block.first, height - 1, j / 2);
        event.push(message2);


    }


    moveHandler = new MoveHandler;
    attackHandler = new AttackHandler;
    putBlockHandler = new PutBlockHandler;
    moveHandler->SetNext(attackHandler)->SetNext(putBlockHandler);

    gameServer.init();
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

                unsigned int receivedMsgCount = 0;

                while (true) {

                    BaseMessage **receivedMsg = gameServer.checkRequests(&receivedMsgCount);
                    if (receivedMsg != nullptr) {
                        for (unsigned int i = 0; i < receivedMsgCount; i++) {
                            request.push(*receivedMsg[i]);
                        }
                    }

                    while(!event.empty()){
                        gameServer.run(event.front());
                        app.render(&event);
                        event.pop();
                    }
                    if(app.processInput(&request)){

                    }
                    start_game();

                }
                state = END_OF_GAME;
                break;
            //default:
              //  break;

        }
    }
    std::cout << "END OF GAME WAS HERE" << std::endl;

    return EXIT_SUCCESS;
}

void Game::start_game() {

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
        // map->out(&objects);
    }



//    if (!request.empty()) {
//
//        unsigned int msgCount = 0;
//        EventMessage **newEventMessages = moveHandler->Handle(request.front(), map, &objects, &msgCount, factory);
//
//        if (newEventMessages != nullptr) {
//            for (unsigned int i = 0; i < msgCount; i++) {
//                event.push(*(newEventMessages[i]));
//            }
//            delete newEventMessages;
//            newEventMessages = nullptr;
//        }
//
//        /*while (!event.empty()) {
//            std::cout << "New Event Message: type: " << event.front().getType() << "; ID: " << event.front().getID() <<
//            "; X: " << event.front().getX() << "; Y: " << event.front().getY() << "; Data: " << event.front().getData() << ";\n";
//            event.pop();
//        }*/
//
//        request.pop();
//        map->out(&objects);
//    }

//    }
}

