#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <unordered_map>

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
    Map *map;

    std::pair<unsigned int, unsigned int> *spawnpoints;
    char teamCount;
    char playersInTeamCount;

    std::unordered_multimap<unsigned int, Object*> objects;
    MoveHandler *moveHandler;
    AttackHandler *attackHandler;
    PutBlockHandler *putBlockHandler;
    std::queue<EventMessage> event;
    std::queue<BaseMessage> request;
};

#endif // PROJECT_GAME_CLASS_H