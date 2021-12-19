#ifndef PROJECT_TP_FACTORY_H
#define PROJECT_TP_FACTORY_H

#include <iostream>

#include "Object.h"
#include "player_class.h"
#include "defaultBlock.h"
#include "endBlock.h"
#include "healingPotion.h"


enum ObjectClass {
    defaultBlockObject = 0,
    endBlockObject = 1,
    healPot = 2
};

class Factory {
public:
    Factory() : id_count(0) {}

    //template <typename classObject>
    std::pair<unsigned int, Object *> createObject(unsigned short val) {
        switch (val) {
            case defaultBlockObject: {
                auto block = new DefaultBlock();
                return std::make_pair(id_count++, block);
            }
            case endBlockObject: {
                auto block = new EndBlock();
                return std::make_pair(id_count++, block);
            }
            case healPot: {
                auto heal = new healingPotion();
                return std::make_pair(id_count++, heal);
            }
            default: {
                return std::make_pair(0, nullptr);
            }
        }
    }

    std::pair<unsigned int, Player *> createPlayer() {
        auto player = new Player();
        return std::make_pair(id_count++, player);
    }

private:
    unsigned int id_count;
};

#endif //PROJECT_TP_FACTORY_H
