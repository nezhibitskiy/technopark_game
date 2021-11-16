#include <iostream>

#include "game_class.h"
#include "player_class.h"
#include "map_class.h"




Game::Game(size_t players_number, std::vector<std::pair<size_t, size_t>> coords) : mapa_(10, 10) {
    players_ = new Player[players_number];
    for (int i = 0; i < players_number; ++i) {
        players_[i].set_conditions(coords.at(i).first, coords.at(i).second);
        mapa_.change_map(coords.at(i).first, coords.at(i).second, player);
    }
}

void Game::start_game() {
    mapa_.out();
    while(true) {
        char key;
        std::cin >> key;
        if (key == 'q') {
            break;
        } else if (key == 'w') {
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
            players_[0].move_top();
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
        } else if (key == 'a') {
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
            players_[0].move_left();
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
        } else if (key == 's') {
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
            players_[0].move_bottom();
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
        } else if (key == 'd') {
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), clear_field);
            players_[0].move_right();
            mapa_.change_map(players_[0].get_x(), players_[0].get_y(), player);
        }
        mapa_.out();
    }
}

