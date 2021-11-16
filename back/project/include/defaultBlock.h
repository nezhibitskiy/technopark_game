//
// Created by ilyas on 16.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_DEFAULTBLOCK_H
#define CHAINOFRESPONSABILITY_DEFAULTBLOCK_H

#include "Object.h"

class DefaultBlock : public Object {
    bool Damagable() override {
        return true;
    };
    bool Damage(const char damageValue) override {
        return damageValue > 0; // Если умер - возвращает true
    };
    bool CanBeStandOn() override {
        return false;
    };
    char getObjectSymbol() override {
        return 'o';
    }
    ~DefaultBlock() = default;
};

#endif //CHAINOFRESPONSABILITY_DEFAULTBLOCK_H
