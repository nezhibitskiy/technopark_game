//
// Created by ilyas on 14.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_OBJECT_H
#define CHAINOFRESPONSABILITY_OBJECT_H

class Object {
public:
    virtual bool Damagable() const = 0;
    virtual unsigned char Damage(const unsigned char damageValue) {
        return 1;                               // Если умер - возвращает true
    }
    virtual bool Respawn() {
        return false;
    }
    virtual bool CanBeStandOn() const { return true; };
    virtual void ToDo(Object *player) {}
    virtual char getObjectSymbol() {
        return '0';
    }
    virtual ~Object() = default;
};

#endif //CHAINOFRESPONSABILITY_OBJECT_H
