//
// Created by ilyas on 14.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_OBJECT_H
#define CHAINOFRESPONSABILITY_OBJECT_H

class Object {
public:
    virtual bool Damagable() = 0;
    virtual bool Damage(const char damageValue) {
        return false;                               // Если умер - возвращает true
    };
    virtual bool Respawn() {
        return false;
    }
    virtual bool CanBeStandOn() = 0;
    virtual void ToDo(Object *player) {}
    virtual ~Object() = default;
};

#endif //CHAINOFRESPONSABILITY_OBJECT_H
