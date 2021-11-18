#ifndef PROJECT_ENDBLOCK_H
#define PROJECT_ENDBLOCK_H

#include "Object.h"

class EndBlock : public Object {
public:
    EndBlock() = default;
    bool Damagable() const override {
        return false;
    };
    bool CanBeStandOn() const override {
        return false;
    };
    char getObjectSymbol() override {
        return '#';
    }
    ~EndBlock() = default;
};

#endif // PROJECT_ENDBLOCK_H
