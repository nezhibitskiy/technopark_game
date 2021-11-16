#ifndef PROJECT_TP_PLAYER_H
#define PROJECT_TP_PLAYER_H

#include "objects.h"

class Player: public Object {
private:
    char id_;
    char health_;
    //int armor_;
    unsigned int x_;
    unsigned int y_;
public:
    Player(char i, unsigned int x, unsigned int y) :id_(i), x_(x), y_(y), health_(3) {}
    ~Player() override = default;
    void AddHealth(const char value) {
        health_ += value;
    }
    bool Damagable() override { return true; };
    bool Damage(const char damageValue) override {
        if (health_ > damageValue) {
            health_ -= damageValue;
            return false;
        }
        return true;
    };
    bool Respawn() override {
        return false;
    }
    bool CanBeStandOn() override { return false; };
};

#endif //PROJECT_TP_PLAYER_H
