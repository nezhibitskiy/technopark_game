#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <vector>

#include "player_class.h"
#include "map_class.h"


class Game {
public:
    explicit Game(size_t players_number, std::vector<std::pair<size_t, size_t>> coords);
    void start_game();

private:
    Map mapa_;
    Player* players_;
};

#endif // PROJECT_GAME_CLASS_H