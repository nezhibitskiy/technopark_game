#ifndef PROJECT_DEFAULTBLOCK_H
#define PROJECT_DEFAULTBLOCK_H

#include "Object.h"

class DefaultBlock : public Object {
public:
    DefaultBlock() = default;
    bool Damagable() const override {
        return true;
    };
    bool Damage(const char damageValue) override {
        return damageValue > 0; // Если умер - возвращает true
    };
    bool CanBeStandOn() const override {
        return false;
    };
    char getObjectSymbol() override {
        return 'o';
    }
    ~DefaultBlock() = default;
};

#endif // PROJECT_DEFAULTBLOCK_H
