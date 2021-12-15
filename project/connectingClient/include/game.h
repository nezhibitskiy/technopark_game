//
// Created by ilyas on 07.12.2021.
//

#ifndef PROJECT_TP_GAME_H
#define PROJECT_TP_GAME_H

#include "Application.h"
#include "clientNetwork.h"

class Game {
public:
    Game();
    ~Game();
    void Iteration();

private:
    enum State {
        PREINIT = -1,
        INIT = 0,
        WAITING_FOR_GAME = 1,
        WAITING_FOR_SERVER_START = 2,
        STARTED = 3,
        GAME_OVER = 4,
        END_OF_GAME = 5
    };

    Client *client;
    State state;
    Application app;
    std::queue<EventMessage> *event;
    std::queue<BaseMessage> *request;
};

#endif //PROJECT_TP_GAME_H
