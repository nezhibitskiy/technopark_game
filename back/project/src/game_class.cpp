#include <iostream>

#include "game_class.h"

Game::Game() : map() {
    players = new Player[map.getPlayersInTeamCount() * map.getTeamCount()];
    for (unsigned short i = 0; i < map.getPlayersInTeamCount() * map.getTeamCount(); i++) {
        players[i].setTeam((i / map.getPlayersInTeamCount()));
        players[i].saveSpawnpoint(map.getPlayerSpawnpoint(i));
        map.addObject(&players[i], players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);
        players[i].setCoords(players[i].getSpawnpoint().first, players[i].getSpawnpoint().second);
    }

    moveHandler = new MoveHandler;
}

void Game::start_game() {
    MessageQueue event;
    event.addEventToQueue(MoveHandler::MOVE, 1, 1, 3);
    event.addEventToQueue(MoveHandler::MOVE, 0, 5, 7);

    map.out();
    while(!event.empty()) {
        moveHandler->Handle(event.getEventFromQueue(), &map, players);
        map.out();
    }

//    mapa_.out();
//    while(true) {
//        char key;
//        std::cin >> key;
//        if (key == 'q') {
//            break;
//        } else if (key == 'w') {
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
//            players_[0].move_top();
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
//        } else if (key == 'a') {
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
//            players_[0].move_left();
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
//        } else if (key == 's') {
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
//            players_[0].move_bottom();
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
//        } else if (key == 'd') {
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
//            players_[0].move_right();
//            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
//        }
//        mapa_.out();
//    }
}

