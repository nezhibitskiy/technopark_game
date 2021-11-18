//
// Created by ilyas on 14.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_ATTACKTRANSMIT_H
#define CHAINOFRESPONSABILITY_ATTACKTRANSMIT_H

#include "handler.h"

class AttackEventHandler : public AbstractHandler {
public:
    enum AttackType {
        ATTACK_UP = 4,
        ATTACK_DOWN = 5,
        ATTACK_LEFT = 6,
        ATTACK_RIGHT = 7,
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

#endif //CHAINOFRESPONSABILITY_ATTACKTRANSMIT_H
