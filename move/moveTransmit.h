//
// Created by ilyas on 14.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_MOVETRANSMIT_H
#define CHAINOFRESPONSABILITY_MOVETRANSMIT_H

#include "handler.h"

class MoveEventHandler : public AbstractHandler {
public:
    enum MoveType {
        MOVE_UP = 0,
        MOVE_DOWN = 1,
        MOVE_LEFT = 2,
        MOVE_RIGHT = 3,
    };
    void Handle(Request request/*Event event*/) override {
        if (request.getType() >= MoveType::MOVE_UP && request.getType() <= MoveType::MOVE_RIGHT) {
            std::cout << "Move: Player " << request.getPlayerID() << " will move on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request);
        }
    }
};


#endif //CHAINOFRESPONSABILITY_MOVETRANSMIT_H
