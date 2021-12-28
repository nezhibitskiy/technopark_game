#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <unordered_map>
#include <random>

#include "player_class.h"
#include "map_class.h"
#include "PlayerHandlers.h"
#include "server.h"

#include "Application.h"

class Zone {
public:
    Zone(unsigned int rad_, unsigned int width, unsigned int height) : rad(rad_) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distX(1 + rad, width - rad - 1);
        x = distX(rng);
        std::uniform_int_distribution<std::mt19937::result_type> distY(1 + rad, height - rad - 1);
        y = distY(rng);
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
        PREINIT = -1,
        INIT = 0,
        WAITING_FOR_GAME = 1,
        STARTED = 2,
        GAME_OVER = 3,
        END_OF_GAME = 4
    };
    Game();
    void CreateMap();
    void waitingForGame();
    int Iteration();
    void createHeals();
    void start_game();
    bool move(unsigned int x, unsigned y);
    int getWinTeam();
    ~Game();


private:
    gameServer::server gameServer;

    State state;
    Map *map;

    std::pair<unsigned int, unsigned int> *spawnpoints;
    char playersCount;
    unsigned int* playersInTeamsCount;
    char maxPlayersInTeams;
    char maxTeams;
    unsigned int* playerIds;
    std::vector<std::pair<unsigned int, char>> playerTeams;

    std::unordered_multimap<unsigned int, Object*> objects;
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