#include <iostream>
#include <vector>

#include "map_class.h"

Map::Map() {
    teamCount = 2;
    playersInTeamCount = 2;
    width = 20;
    height = 20;

    map = new Object**[height];

    for (unsigned int i = 0; i < height; i++) {
        map[i] = new Object*[width];
    }

    spawnpoints = new std::pair<unsigned int, unsigned int>[teamCount * playersInTeamCount];

    for (unsigned short i = 0; i < teamCount * playersInTeamCount; i++) {
        if (i < playersInTeamCount) {
            spawnpoints[i].second = 1;
            spawnpoints[i].first = i + 1;
        }
        else {
            spawnpoints[i].second = height - 2;
            spawnpoints[i].first = width - (i % playersInTeamCount) - 2;
        }
    }

    EndBlock *endBlocks = new EndBlock[2 * height];

    for (unsigned int i = 0; i < 2 * height; i += 2) {
        map[i / 2][0] = &endBlocks[i];
        map[i / 2][width - 1] = &endBlocks[i + 1];
    }

    endBlocks = new EndBlock[2 * (width - 1)];

    for (unsigned int j = 2; j < 2 * (width - 1); j += 2) {
        map[0][j / 2] = &endBlocks[j];
        map[height - 1][j / 2] = &endBlocks[j + 1];
    }

}

Map::~Map() {
    for (size_t i = 0; i < width; ++i) {
        delete[] map[i];
    }
    delete[] map;
}

void Map::out() {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            if (map[i][j] != nullptr) {
                std::cout << map[i][j]->getObjectSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
