#include <iostream>

#include "game_class.h"

Game::Game() : map() {

    objects.reserve(map.getPlayersInTeamCount() * map.getTeamCount());


//    players = new Object[map.getPlayersInTeamCount() * map.getTeamCount()];
//    for (unsigned short i = 0; i < map.getPlayersInTeamCount() * map.getTeamCount(); i++) {
//        players[i].setTeam((char)(i / map.getPlayersInTeamCount()));
//        players[i].saveSpawnpoint(map.getPlayerSpawnpoint(i));
//        map.addObject(&players[i], players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);
//        players[i].setCoords(players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);
//    }

    moveHandler = new MoveHandler;
    attackHandler = new AttackHandler;
    putBlockHandler = new PutBlockHandler;
    moveHandler->SetNext(attackHandler)->SetNext(putBlockHandler);
}

Game::~Game() {
    delete moveHandler;
    delete attackHandler;
    delete putBlockHandler;
    delete [] players;
}


int Game::Iteration() {

    // INIT

    while (state != STARTED) {

        switch(state) {
            case (INIT):
                // Вписываются все функции частей игры и условия перехода
                // if (window.init()) {
                // state = SERVER_STARTED
                // state = CLIENT_CONNECT_TO_SERVER
                // window.find_serv()
                //}
                break;

        }
    }

    return EXIT_SUCCESS;
}

void Game::start_game() {
    map.out(&objects);
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
                BaseMessage attackUp(AttackHandler::ATTACK, 0, players[0].getX(), players->getY() - 1);
                request.push(attackUp);
                break;
            }
            case('f'): {
                BaseMessage attackLeft(AttackHandler::ATTACK, 0, players[0].getX() - 1, players->getY());
                request.push(attackLeft);
                break;
            }
            case('g'): {
                BaseMessage attackDown(AttackHandler::ATTACK, 0, players[0].getX(), players->getY() + 1);
                request.push(attackDown);
                break;
            }
            case('h'): {
                BaseMessage attackRight(AttackHandler::ATTACK, 0, players[0].getX() + 1, players->getY());
                request.push(attackRight);
                break;
            }


            case('i'): {
                BaseMessage putBlockUp(PutBlockHandler::PUT_BLOCK, 0, players[0].getX(), players->getY() - 1);
                request.push(putBlockUp);
                break;
            }
            case('j'): {
                BaseMessage putBlockLeft(PutBlockHandler::PUT_BLOCK, 0, players[0].getX() - 1, players->getY());
                request.push(putBlockLeft);
                break;
            }
            case('k'): {
                BaseMessage putBlockDown(PutBlockHandler::PUT_BLOCK, 0, players[0].getX(), players->getY() + 1);
                request.push(putBlockDown);
                break;
            }
            case('l'): {
                BaseMessage putBlockRight(PutBlockHandler::PUT_BLOCK, 0, players[0].getX() + 1, players->getY());
                request.push(putBlockRight);
                break;
            }

            default: {
                break;
            }
        }
        if (!request.empty()) {

            unsigned int msgCount = 0;
            EventMessage **newEventMessages = moveHandler->Handle(request.front(), &map, &objects, &msgCount);

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
            map.out(&objects);
        }

    }
}

