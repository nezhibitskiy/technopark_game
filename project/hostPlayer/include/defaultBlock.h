//
// Created by ilyas on 16.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_DEFAULTBLOCK_H
#define CHAINOFRESPONSABILITY_DEFAULTBLOCK_H

#include "Object.h"

#define STANDART_HEAL 2

class DefaultBlock : public AbstractObject {
    bool Damagable() const override {
        return true;
    }
    unsigned char Damage(const unsigned char damageValue) override {
        if (damageValue > health)
            return 0; // Если умер - возвращает true
        else return (health - damageValue);
    }
    bool CanBeStandOn() const override {
        return false;
    }
    bool isItCover() override { return true; }
    char getObjectSymbol() override {
        return '4';
    }

    void ToDo(Object* player) override {
        //player.
    }

    ~DefaultBlock() override = default;

    unsigned char health = 1;

public:
    enum {
        ID = 1
    };
};

#endif //CHAINOFRESPONSABILITY_DEFAULTBLOCK_H
