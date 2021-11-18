#ifndef PROJECT_PLAYER_CLASS_H
#define PROJECT_PLAYER_CLASS_H

#include <iostream>
#include "Object.h"

class Player: public Object {
public:
    Player(char i) : id(i) {

    }

    bool Damagable() const override { return true; };

    bool Damage(const char damageValue) override {
        if (health > damageValue) {
            health -= damageValue;
            return false;
        }
        return true;
    };

    bool Respawn() override {

        return false;
    }
    bool CanBeStandOn() const override { return false; };

    void saveSpawnpoint(std::pair<unsigned int, unsigned int> recievedSpawnpoint) {
        spawnpoint = recievedSpawnpoint;
        return;
    }

    char getObjectSymbol() override {
        if (team == 0) {
            return 'o';
        }
        if (team == 1) {
            return 'x';
        }
        return 0;
    }

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

    ~Player() {  };

private:
    char id;
    char health;
    char team;
    unsigned int x;
    unsigned int y;
    std::pair<unsigned int, unsigned int> spawnpoint;
};

//struct Player : public Object {
//    void set_conditions(size_t x, size_t y);
//
//    size_t get_x() const;
//
//    size_t get_y() const;
//
//    void move_left();
//
//    void move_right();
//
//    void move_top();
//
//    void move_bottom();
//
//private:
//    //size_t id_;
//    //size_t hp_;
//    size_t x_;
//    size_t y_;
//    //size_t armor_;
//    //size_t team_;
//    //size_t cursor;
//};


#endif // PROJECT_PLAYER_CLASS_H