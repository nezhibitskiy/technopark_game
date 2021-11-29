#include <iostream>

#include "game_class.h"

Game::Game() : map() {
    players = new Player[map.getPlayersInTeamCount() * map.getTeamCount()];
    for (unsigned short i = 0; i < map.getPlayersInTeamCount() * map.getTeamCount(); i++) {
        players[i].setTeam((char)(i / map.getPlayersInTeamCount()));
        players[i].saveSpawnpoint(map.getPlayerSpawnpoint(i));
        map.addObject(&players[i], players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);
        players[i].setCoords(players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);
    }

    moveHandler = new MoveHandler;
}

Game::~Game() {
    delete moveHandler;
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
    map.out();
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
            default: {
                break;
            }
        }
        if (!request.empty()) {
            moveHandler->Handle(request.front(), &map, players);
            request.pop();
            map.out();
        }
    }
}

