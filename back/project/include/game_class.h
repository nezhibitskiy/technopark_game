#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <vector>

#include "player_class.h"
#include "map_class.h"
#include "PlayerHandlers.h"

class Game {
public:
    Game();
    void start_game();
    ~Game() {
        delete moveHandler;
    }
private:
    Map map;
    Player* players;
    MoveHandler *moveHandler;
};

#endif // PROJECT_GAME_CLASS_H