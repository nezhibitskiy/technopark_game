//
// Created by ilyas on 16.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_PLAYERHANDLERS_H
#define CHAINOFRESPONSABILITY_PLAYERHANDLERS_H

#include "handler.h"

class MoveHandler : public AbstractHandler {
public:
    enum MoveType {
        MOVE = 0,
    };
    void Handle(Message request, Map *map, Player *players) override {
        if (request.getType() == MoveHandler::MOVE) {
            // Check if player can move and if true, move him

//            std::cout << "Move: Player " << request.getPlayerID() << " will move on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map);
        }
    }
};

#endif //CHAINOFRESPONSABILITY_PLAYERHANDLERS_H
