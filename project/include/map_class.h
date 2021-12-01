#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>
#include "endBlock.h"
#include "defaultBlock.h"

class Map {
public:
    Map();
    Map(const char* filename);
    ~Map();
    void addObject(unsigned int id, unsigned int x, unsigned int y) {
        map[y][x] = id;
    }
    char getTeamCount() {
        return teamCount;
    };
    char getPlayersInTeamCount() {
        return playersInTeamCount;
    };
    std::pair<unsigned int, unsigned int> getPlayerSpawnpoint(char playerID) {
        return spawnpoints[playerID];
    }
    unsigned int getObjectId(unsigned int x, unsigned int y) {
        return map[y][x];
    }
    void moveObject(unsigned int oldX, unsigned int oldY, unsigned int newX, unsigned int newY) {
        std::swap(map[oldY][oldX], map[newY][newX]);
    }

    size_t getWidth() const {
        return width;
    }

    size_t getHeight() const {
        return height;
    }

private:
    size_t width;
    size_t height;
    //Object*** map;
    unsigned int** map;
    std::pair<unsigned int, unsigned int> *spawnpoints;
    char teamCount;
    char playersInTeamCount;
};

#endif //PROJECT_MAP_CLASS_H