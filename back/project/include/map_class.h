#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>
#include "endBlock.h"
#include "defaultBlock.h"

class Map {
public:
    Map();
    ~Map();
    void addObject(Object *object, unsigned int x, unsigned int y) {
        map[y][x] = object;
    }
    char getTeamCount() {
        return teamCount;
    };
    char getPlayersInTeamCount() {
        return playersInTeamCount;
    };
    std::pair<unsigned int, unsigned int> getPlayerSpawnpoint(unsigned short playerID) {
        return spawnpoints[playerID];
    }
    void out();
private:
    size_t width;
    size_t height;
    Object*** map;
    std::pair<unsigned int, unsigned int> *spawnpoints;
    char teamCount;
    char playersInTeamCount;
};

#endif //PROJECT_MAP_CLASS_H