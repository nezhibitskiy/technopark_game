#ifndef PROJECT_PLAYER_CLASS_H
#define PROJECT_PLAYER_CLASS_H

#include <iostream>
#include "message.h"
#include "Object.h"

#define DEFAULT_HEALTH_VALUE 5

class Player: public Object {
public:
    Player() : health(DEFAULT_HEALTH_VALUE) { };

    bool Damagable() const override { return true; };

    unsigned char Damage(const unsigned char damageValue) override {
        if (health > damageValue) {
            health -= damageValue;
            return health;
        }
        return 0;
    };

    bool Respawn() override {
        health = DEFAULT_HEALTH_VALUE;
        x = spawnpoint.first;
        y = spawnpoint.second;
        return true;
    }
    bool CanBeStandOn() const override { return false; };

    void saveSpawnpoint(std::pair<unsigned int, unsigned int> recievedSpawnpoint) {
        spawnpoint = recievedSpawnpoint;
    }

    char getObjectSymbol() override {
        if (team == 0) {
            return 'o';
        }
        if (team == 1) {
            return 'x';
        }

    };

    std::pair<unsigned int, unsigned int> getSpawnpoint() const {
        return spawnpoint;
    }

    unsigned int getX() const {
        return x;
    }

    unsigned int getY() const {
        return y;
    }

    void setCoords(unsigned int rX, unsigned int rY) {
        x = rX;
        y = rY;
    }

    void setTeam(char rTeam) {
        team = rTeam;
    }

    ~Player() override {  };

private:
    char id;
    char health;
    char team;
    unsigned int x;
    unsigned int y;
    std::pair<unsigned int, unsigned int> spawnpoint;
};

#endif // PROJECT_PLAYER_CLASS_H