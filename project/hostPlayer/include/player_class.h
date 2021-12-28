#ifndef PROJECT_PLAYER_CLASS_H
#define PROJECT_PLAYER_CLASS_H

#include <iostream>
#include "../../include/message.h"
#include "Object.h"

#define DEFAULT_HEALTH_VALUE 5

class Player: public AbstractObject {
public:
    Player() : health(DEFAULT_HEALTH_VALUE), kills(0) { };

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

    std::pair<unsigned int, unsigned int> getSpawnpoint() const override {
        return spawnpoint;
    }

    void setTeam(char rTeam) {
        team = rTeam;
    }

    char getTeam() const override {
        return team;
    }

    void addKill() override {
        kills += 1;
    }

    unsigned int getKills() const override {
        return kills;
    }

    bool isItCover () override {
        return false;
    }

    EventMessage* addHealth(char healthCount, unsigned int id_) override {
        health += healthCount;
        if (health > DEFAULT_HEALTH_VALUE) {
            health = DEFAULT_HEALTH_VALUE;
        }
        auto healtMsg = new EventMessage(EventMessage::SET_HEALTH, id_, 0, 0, health);
        return healtMsg;
    }

    ~Player() = default;

private:
    char id;
    char health;
    char team;
    unsigned int kills;
    std::pair<unsigned int, unsigned int> spawnpoint;
};

#endif // PROJECT_PLAYER_CLASS_H