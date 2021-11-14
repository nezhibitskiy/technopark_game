//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_MOVE_H
#define CHAINOFRESPONSABILITY_MOVE_H

#include "handler.h"

class MoveHandler : public AbstractHandler {
public:
    enum MoveType {
        MOVE = 0,
    };
    void Handle(Message request, Map *map) override {
        if (request.getType() == MoveHandler::MOVE) {
            std::cout << "Move: Player " << request.getPlayerID() << " will move on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map);
        }
    }
};

#endif // CHAINOFRESPONSABILITY_MOVE_H
