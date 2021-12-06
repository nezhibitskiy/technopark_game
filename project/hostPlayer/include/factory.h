#ifndef PROJECT_TP_FACTORY_H
#define PROJECT_TP_FACTORY_H

#include <iostream>

#include "Object.h"
#include "player_class.h"
#include "defaultBlock.h"
#include "endBlock.h"



enum ObjectClass {
    playerObject = 0,
    defaultBlockObject = 1,
    endBlockObject = 2,
};

class Factory {
public:
    Factory() : id_count(0) {}
    //template <typename classObject>
    std::pair<unsigned int, Object*> createObject(unsigned short val) {
        switch (val) {
            case playerObject: {
                auto player = new Player();
                return std::make_pair(id_count++, player);
            }
            case defaultBlockObject: {
                auto block = new DefaultBlock();
                return std::make_pair(id_count++, block);
            }
            case endBlockObject: {
                auto block = new EndBlock();
                return std::make_pair(id_count++, block);
            }
            default: {
                return std::make_pair(0, nullptr);
            }
        }
    }

private:
    unsigned int id_count;
};



#endif //PROJECT_TP_FACTORY_H
