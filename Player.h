//
// Created by ilyas on 14.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_PLAYER_H
#define CHAINOFRESPONSABILITY_PLAYER_H

#include "Object.h"

class Player: private Object {
private:
    char id;
    char health;
public:
    bool Damagable() override { return true; };
    bool Damage(const char damageValue) {
        if (health > damageValue) {
            health -= damageValue;
            return false;
        }
        return true;
    };
    bool Respawn() override {
        return false;
    }
    bool CanBeStandOn() override { return false; };

    ~Player() override {  };
};

#endif //CHAINOFRESPONSABILITY_PLAYER_H
