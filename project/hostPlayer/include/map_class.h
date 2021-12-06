#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>
#include "endBlock.h"
#include "defaultBlock.h"

#include <unordered_map>


class Map {
public:
    Map(unsigned int width, unsigned int height);
    ~Map();
    void addObject(unsigned int id, unsigned int x, unsigned int y) {
        map[y][x] = id;
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
};

#endif //PROJECT_MAP_CLASS_H