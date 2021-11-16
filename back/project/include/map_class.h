#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>

typedef enum key {
    player = 0,
    end_block = 1,
    clear_field = 2
};

class field {
public:
    key fld_;
    void *data_;
};

class Map {
public:
    Map(size_t width, size_t height);
    ~Map();
    void out();
    void change_map(size_t x, size_t y, key fld);
private:
    size_t width_;
    size_t height_;
    field** map_;
};

#endif //PROJECT_MAP_CLASS_H