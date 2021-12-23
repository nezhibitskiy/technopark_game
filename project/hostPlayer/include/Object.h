//
// Created by ilyas on 14.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_OBJECT_H
#define CHAINOFRESPONSABILITY_OBJECT_H

#include "message.h"

class Object {
public:
    virtual bool Damagable() const = 0;
    virtual unsigned char Damage(const unsigned char damageValue) = 0;
    virtual bool Respawn() = 0;
    virtual bool CanBeStandOn() const { return true; };
    virtual EventMessage* ToDo(Object *player, unsigned int id_) = 0;
    virtual char getObjectSymbol() = 0;
    virtual unsigned int getX() = 0;
    virtual unsigned int getY() = 0;
    virtual void setXY(unsigned int x, unsigned int y) = 0;
    virtual void addKill() = 0;
    virtual unsigned int getKills() const = 0;
    virtual bool isItCover() { return false; }
    virtual char getTeam() const = 0;
    virtual std::pair<unsigned int, unsigned int> getSpawnpoint() const = 0;
    virtual EventMessage* addHealth(char healthCount, unsigned int id_) {};
    virtual ~Object() = default;
};

class AbstractObject : public Object {
public:
    unsigned char Damage(const unsigned char damageValue) override {
        return 1;                               // Если умер - возвращает true
    }
    bool Respawn() override {
        return false;
    }
    virtual bool CanBeStandOn() const { return true; };
    EventMessage* ToDo(Object *player, unsigned int id_) override { return nullptr; }
    char getObjectSymbol() override {
        return '0';
    }

    unsigned int getX() override { return x; }
    unsigned int getY() override { return y; }
    void addKill() override {}
    unsigned int getKills() const override { return 0; }
    virtual char getTeam() const { return 0; }
    virtual bool isItCover() { return false; }

    void setXY(unsigned int rX, unsigned int rY) override {
        x = rX;
        y = rY;
    }
    std::pair<unsigned int, unsigned int> getSpawnpoint() const override {
        return std::pair<unsigned int, unsigned int>(x, y);
    }
    ~AbstractObject() override = default;

private:
    unsigned int x;
    unsigned int y;
};

#endif //CHAINOFRESPONSABILITY_OBJECT_H
