#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <vector>

#include "map_class.h"
#include "player.h"

class Game {
public:
    explicit Game(unsigned int width, unsigned int height);
    void start_game();

private:
    Map mapa_;
    Player** players_;
};

#endif // PROJECT_GAME_CLASS_H