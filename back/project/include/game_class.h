#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <vector>

#include "player_class.h"
#include "map_class.h"
#include "PlayerHandlers.h"

class Game {
public:
    enum State {
        INIT = 0,
        WAITING_FOR_GAME = 1,
        STARTED = 2
    };
    Game();
    int Iteration();
    void start_game();
    ~Game();
private:
    State state;
    Map map;
    Player* players;
    Object* objects;
    MoveHandler *moveHandler;
    AttackHandler *attackHandler;
    std::queue<EventMessage> event;
    std::queue<BaseMessage> request;
};

#endif // PROJECT_GAME_CLASS_H