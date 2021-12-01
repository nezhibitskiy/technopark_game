#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>
#include "endBlock.h"
#include "defaultBlock.h"

#include <unordered_map>


class Map {
public:
    Map();
    ~Map();
    void addObject(unsigned int id, unsigned int x, unsigned int y) {
        map[y][x] = id;
    }
    char getTeamCount() const {
        return teamCount;
    };
    char getPlayersInTeamCount() const {
        return playersInTeamCount;
    };
    std::pair<unsigned int, unsigned int> getPlayerSpawnpoint(unsigned short playerID) const {
        return spawnpoints[playerID];
    }
    unsigned int getObject(unsigned int x, unsigned int y) {
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
    void out(std::unordered_multimap<unsigned int, Object*> *hashTable);
private:
    size_t width;
    size_t height;
    unsigned int** map;
    std::pair<unsigned int, unsigned int> *spawnpoints;
    char teamCount;
    char playersInTeamCount;
};

#endif //PROJECT_MAP_CLASS_H