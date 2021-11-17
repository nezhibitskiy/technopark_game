#ifndef PROJECT_TP_BLOCK_H
#define PROJECT_TP_BLOCK_H

#include "objects.h"

class Block : public Object {
public:
    Block(int x, int y, bool damage) : x_(x), y_(y), is_damagable(damage) {}
    ~Block() override = default;
    bool Damagable() override { return is_damagable; };
    bool Damage(const char damageValue) override {
        if (damageValue > 0) {
            return true;
        }
        return false;
    }
    bool CanBeStandOn() override { return false; };
    /*void To_Do(Object *player) override {
        //
    }*/

private:
    int x_;
    int y_;
    bool is_damagable;
};


#endif //PROJECT_TP_BLOCK_H
