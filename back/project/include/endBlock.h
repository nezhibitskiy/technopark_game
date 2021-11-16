//
// Created by ilyas on 16.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_ENDBLOCK_H
#define CHAINOFRESPONSABILITY_ENDBLOCK_H

#include "Object.h"

class EndBlock : public Object {
    bool Damagable() const override {
        return false;
    };
    bool CanBeStandOn() const override {
        return false;
    };
    char getObjectSymbol() override {
        return '#';
    }
public:
    EndBlock() = default;
    ~EndBlock() = default;
};

#endif //CHAINOFRESPONSABILITY_ENDBLOCK_H
