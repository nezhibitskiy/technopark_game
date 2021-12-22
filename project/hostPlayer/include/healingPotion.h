#ifndef PROJECT_TP_HEALINGPOTION_H
#define PROJECT_TP_HEALINGPOTION_H

#include "Object.h"

#define DEFAULT_HEALING 2

class healingPotion : public AbstractObject {
    bool Damagable() const override {
        return true;
    }

    unsigned char Damage(const unsigned char damageValue) override {
        if (damageValue > health)
            return 0;
        else return (health - damageValue);
    }
    bool CanBeStandOn() const override {
        return false;        // ???????????????????????????????????????
    }
    bool isItCover() override { return false; }

    ~healingPotion() override = default;

    EventMessage* ToDo(Object* player) override {
        return player->addHealth(DEFAULT_HEALING);
    }

    unsigned char health = 1;
public:
    enum {
        ID = 2
    };
};


#endif //PROJECT_TP_HEALINGPOTION_H
