#include <iostream>

#include "defaultBlock.h"
#include "endBlock.h"
#include "Object.h"
#include "game_class.h"
#include "map_class.h"
#include "player_class.h"

#define MAX_ALPHA 30

unsigned int id_num = 0;


Game::Game() : map(new Map()) {
    objects_count = map->getHeight();
    mapObjects.reserve(objects_count);
    objects_count = 0;
}

void Game::grow() {
    //mapObjects.reserve(objects_count * 2);
    std::unordered_multimap<unsigned int, std::pair<unsigned int, Object*>> objects_;
    objects_.reserve(objects_count * 2);
    for (auto &elem : mapObjects) {
        unsigned int id = elem.second.first;
        unsigned int hash_ = hash(id);
        objects_.insert(std::make_pair(hash_, std::make_pair(id, elem.second.second)));
    }
    mapObjects = objects_;
}

unsigned int Game::fabr() {
    return id_num++;
}

unsigned int Game::hash(unsigned int id) {
    return (id % mapObjects.bucket_count());
}

void Game::addObject(Object *obj, unsigned int x, unsigned int y) {
    if ((objects_count / mapObjects.size()) > MAX_ALPHA) {
        grow();
    }
    objects_count++;
    unsigned int id = fabr();
    map->addObject(id, x, y);
    unsigned int hash_ = hash(id);
    mapObjects.insert(std::make_pair(hash_, std::make_pair(id, obj)));
}

bool Game::deleteObject(unsigned int id) {
    auto it = mapObjects.find(id);
    while (it != mapObjects.end()) {
        if (it->second.first == id) {
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

