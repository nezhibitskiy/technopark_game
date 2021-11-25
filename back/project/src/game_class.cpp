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
}

void Game::start_game() {
    MessageQueue request;
//    request.addEventToQueue(MoveHandler::MOVE, 1, 1, 3);
//    request.addEventToQueue(MoveHandler::MOVE, 0, 5, 7);

    map.out();
    bool gameFlag = true;

    while(gameFlag) {
        char key;
        std::cin >> key;
        switch (key) {
            case('q'):
                gameFlag = false;
                break;
            case('w'):
                request.addEventToQueue(MoveHandler::MOVE, 0, players[0].getX(), players[0].getY() - 1);
                break;
            case('a'):
                request.addEventToQueue(MoveHandler::MOVE, 0, players[0].getX() - 1, players[0].getY());
                break;
            case('s'):
                request.addEventToQueue(MoveHandler::MOVE, 0, players[0].getX(), players[0].getY() + 1);
                break;
            case('d'):
                request.addEventToQueue(MoveHandler::MOVE, 0, players[0].getX() + 1, players[0].getY());
                break;
            default:
                break;
        }
        if (!request.empty()) {
            moveHandler->Handle(request.getEventFromQueue(), &map, players);
            map.out();
        }
    }
}

