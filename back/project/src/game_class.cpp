#include <iostream>

#include "game_class.h"

Game::Game() {
    state = INIT;

    // Необходимо реализовать поля с общим размером, которые также будут передаваться в конструктор карты
    unsigned int width = 20;
    unsigned int height = 20;

    EventMessage message(EventMessage::CREATE_MAP, 0, width, height);
    event.push(message);

    map = new Map(width, height);

    unsigned int id = 0;

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

    EndBlock *endBlocks = new EndBlock[2 * height];

    Player *players = new Player[playersInTeamCount * teamCount];

    for (unsigned short i = 0; i < playersInTeamCount * teamCount; i++) {
        players[i].setTeam((char)(i / playersInTeamCount));
        players[i].saveSpawnpoint(spawnpoints[i]);
        players[i].setXY(players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);

        objects.insert(std::pair<unsigned int, Object*>(id, &players[i]));
        map->addObject(id, players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);

        EventMessage message(EventMessage::CREATE_PLAYER, id, players[i].getSpawnpoint().first,
                players[i].getSpawnpoint().second, (i / playersInTeamCount));
        event.push(message);
        id++;
    }

    for (unsigned int i = 0; i < 2 * height; i += 2) {
        objects.insert(std::pair<unsigned int, Object*>(id, &endBlocks[i]));
        map->addObject(id, i / 2, 0);
        EventMessage message1(EventMessage::CREATE_OBJECT, id, i / 2, 0);
        event.push(message1);
        id++;
        objects.insert(std::pair<unsigned int, Object*>(id, &endBlocks[i]));
        map->addObject(id, i / 2, width - 1);
        EventMessage message2(EventMessage::CREATE_OBJECT, id, i / 2, width - 1);
        event.push(message2);
        id++;
    }

    endBlocks = new EndBlock[2 * (width - 1)];

    for (unsigned int j = 2; j < 2 * (width - 1); j += 2) {
        objects.insert(std::pair<unsigned int, Object*>(id, &endBlocks[j]));
        map->addObject(id, 0, j / 2);
        EventMessage message1(EventMessage::CREATE_OBJECT, id, 0, j / 2);
        event.push(message1);
        id++;

        objects.insert(std::pair<unsigned int, Object*>(id, &endBlocks[j]));
        map->addObject(id, height - 1, j / 2);
        EventMessage message2(EventMessage::CREATE_OBJECT, id, height - 1, j / 2);
        event.push(message2);
        id++;
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
}


int Game::Iteration() {
    while (state != END_OF_GAME) {
        switch(state) {
            case (INIT):
                std::cout << "INIT STATE WAS HERE" << std::endl;
                state = WAITING_FOR_GAME;
                // Вписываются все функции частей игры и условия перехода
                // if (window.init()) {
                // state = SERVER_STARTED
                // state = CLIENT_CONNECT_TO_SERVER
                // window.find_serv()
                //}
                break;
            case (WAITING_FOR_GAME):
                std::cout << "WAITING FOR GAME WAS HERE" << std::endl;
                state = STARTED;
                break;
            case (STARTED):
                start_game();
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


    BaseMessage moveUp1(MoveHandler::MOVE_DOWN, 1);
    request.push(moveUp1);

    BaseMessage moveUp2(MoveHandler::MOVE_UP, 2);
    request.push(moveUp2);
    BaseMessage moveUp3(MoveHandler::MOVE_UP, 3);
    request.push(moveUp3);

    while (!request.empty()) {

        unsigned int initMsgCount = 0;
        EventMessage **initEventMessages = moveHandler->Handle(request.front(), map, &objects, &initMsgCount);

        if (initEventMessages != nullptr) {
            for (unsigned int i = 0; i < initMsgCount; i++) {
                event.push(*(initEventMessages[i]));
            }
            delete initEventMessages;
            initEventMessages = nullptr;
        }
        request.pop();
    }




    while (!event.empty()) {
        std::cout << "New Event Message: type: " << event.front().getType() << "; ID: " << event.front().getID() <<
                  "; X: " << event.front().getX() << "; Y: " << event.front().getY() << "; Data: " << event.front().getData() << ";\n";
        event.pop();
    }
    map->out(&objects);
    bool gameFlag = true;

    while(gameFlag) {
        char key;
        std::cin >> key;
        switch (key) {
            case('q'):
                gameFlag = false;
                break;
            case('w'): {
                BaseMessage moveUp(MoveHandler::MOVE_UP, 0);
                request.push(moveUp);
                break;
            }
            case('a'): {
                BaseMessage moveLeft(MoveHandler::MOVE_LEFT, 0);
                request.push(moveLeft);
                break;
            }
            case('s'): {
                BaseMessage moveDown(MoveHandler::MOVE_DOWN, 0);
                request.push(moveDown);
                break;
            }
            case('d'): {
                BaseMessage moveRight(MoveHandler::MOVE_RIGHT, 0);
                request.push(moveRight);
                break;
            }

            case('t'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage attackUp(AttackHandler::ATTACK, 0, playerNode->second->getX(), playerNode->second->getY() - 1);
                request.push(attackUp);
                break;
            }
            case('f'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage attackLeft(AttackHandler::ATTACK, 0, playerNode->second->getX() - 1, playerNode->second->getY());
                request.push(attackLeft);
                break;
            }
            case('g'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage attackDown(AttackHandler::ATTACK, 0, playerNode->second->getX(), playerNode->second->getY() + 1);
                request.push(attackDown);
                break;
            }
            case('h'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage attackRight(AttackHandler::ATTACK, 0, playerNode->second->getX() + 1, playerNode->second->getY());
                request.push(attackRight);
                break;
            }


            case('i'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage putBlockUp(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX(), playerNode->second->getY() - 1);
                request.push(putBlockUp);
                break;
            }
            case('j'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage putBlockLeft(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX() - 1, playerNode->second->getY());
                request.push(putBlockLeft);
                break;
            }
            case('k'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage putBlockDown(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX(), playerNode->second->getY() + 1);
                request.push(putBlockDown);
                break;
            }
            case('l'): {
                auto playerNode = objects.find(0);
                if (playerNode == objects.end()) break;
                BaseMessage putBlockRight(PutBlockHandler::PUT_BLOCK, 0, playerNode->second->getX() + 1, playerNode->second->getY());
                request.push(putBlockRight);
                break;
            }

            default: {
                break;
            }
        }
        if (!request.empty()) {

            unsigned int msgCount = 0;
            EventMessage **newEventMessages = moveHandler->Handle(request.front(), map, &objects, &msgCount);

            if (newEventMessages != nullptr) {
                for (unsigned int i = 0; i < msgCount; i++) {
                    event.push(*(newEventMessages[i]));
                }
                delete newEventMessages;
                newEventMessages = nullptr;
            }

            while (!event.empty()) {
                std::cout << "New Event Message: type: " << event.front().getType() << "; ID: " << event.front().getID() <<
                "; X: " << event.front().getX() << "; Y: " << event.front().getY() << "; Data: " << event.front().getData() << ";\n";
                event.pop();
            }

            request.pop();
            map->out(&objects);
        }

    }
}

