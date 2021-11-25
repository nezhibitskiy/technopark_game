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
    ~Game();
private:
    Map map;
    Player* players;
    Object* objects;
    MoveHandler *moveHandler;
    AttackHandler *attackHandler;
};

#endif // PROJECT_GAME_CLASS_H