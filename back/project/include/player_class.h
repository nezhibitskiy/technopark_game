#ifndef PROJECT_PLAYER_CLASS_H
#define PROJECT_PLAYER_CLASS_H

#include <iostream>
#include "message.h"
#include "Object.h"

#define DEFAULT_HEALTH_VALUE 5

class Player: public AbstractObject {
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
        setXY(spawnpoint.first, spawnpoint.second);
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

    void setTeam(char rTeam) {
        team = rTeam;
    }

    ~Player() = default;

private:
    char id;
    char health;
    char team;
    std::pair<unsigned int, unsigned int> spawnpoint;
};

#endif // PROJECT_PLAYER_CLASS_H