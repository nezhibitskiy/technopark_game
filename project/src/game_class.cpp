#include <iostream>

#include "defaultBlock.h"
#include "endBlock.h"
#include "Object.h"
#include "game_class.h"
#include "map_class.h"
#include "player_class.h"


unsigned int id_num = 0;


Game::Game() : map(new Map()) {
    mapObjects.reserve(map->getHeight());
}

unsigned int Game::fabr() {
    return id_num++;
}

void Game::addObject(Object *obj, unsigned int x, unsigned int y) {
    unsigned int id = fabr();
    map->addObject(id, x, y);
    mapObjects.insert(std::make_pair(id, obj));
}

bool Game::deleteObject(unsigned int id) {
    auto it = mapObjects.find(id);
    while (it != mapObjects.end()) {
        if (it->first == id) {
            mapObjects.erase(it);
            return true;
        }
        it++;
    }
    return false;
}


void Game::start_game() {
   //serv iter
   //gui iter
}

