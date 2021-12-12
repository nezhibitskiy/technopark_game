#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <unordered_map>

#include "player_class.h"
#include "map_class.h"
#include "PlayerHandlers.h"
#include "server.h"

#include "Application.h"

class Zone {
public:
    void setXY(unsigned int x_, unsigned int y_, unsigned int rad_) {
        x = x_;
        y = y_;
        rad = rad_;
    }

    unsigned int getX() const {
        return x;
    }

    unsigned int getY() const {
        return y;
    }

    unsigned int getRad() const {
        return rad;
    }

private:
    unsigned int x;
    unsigned int y;
    unsigned int rad;
};

class Game {
public:
    enum State {
        INIT = 0,
        WAITING_FOR_GAME = 1,
        STARTED = 2,
        GAME_OVER = 3,
        END_OF_GAME = 4
    };
    Game();
    void CreateMap();
    int Iteration();
    void start_game();
    bool move(unsigned int x, unsigned y);
    int getWinTeam();
    ~Game();


private:
    gameServer::server gameServer;

    State state;
    Map *map;

    std::pair<unsigned int, unsigned int> *spawnpoints;
    char teamCount;
    char playersInTeamCount;

    std::unordered_multimap<unsigned int, Object*> objects;
    unsigned int* playerIds;
    Zone* zone;
    Factory* factory;
    MoveHandler *moveHandler;
    AttackHandler *attackHandler;
    PutBlockHandler *putBlockHandler;
    std::queue<EventMessage> event;
    std::queue<BaseMessage> request;

    Application app;

};

#endif // PROJECT_GAME_CLASS_H