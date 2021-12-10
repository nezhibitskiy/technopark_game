#include <iostream>
#include <vector>

#include "map_class.h"


Map::Map(unsigned int width, unsigned int height) : width(width), height(height) {

    map = new unsigned int *[height];

    for (unsigned int i = 0; i < height; i++) {
        map[i] = new unsigned int[width];
        for (unsigned int j = 0; j < width; j++) map[i][j] = 400000U;
    }


}

Map::~Map() {
    for (size_t i = 0; i < width; ++i) {
        delete[] map[i];
    }
    delete[] map;
}

void Map::out(std::unordered_multimap<unsigned int, Object*> *hashTable) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            /*auto objectNode = hashTable->find(map[i][j]);
            if (objectNode != hashTable->end()) {
                std::cout << objectNode->second->getObjectSymbol() << " ";
            } else {
                std::cout << ". ";
            }*/
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

