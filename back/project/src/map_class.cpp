#include <iostream>
#include <vector>

#include "map_class.h"

Map::Map(size_t width, size_t height) {
    width_ = width;
    height_ = height;
    map_ = new field *[width_];
    for (size_t i = 0; i < width_; ++i) {
        map_[i] = new field[height_];
    }
    for (size_t i = 0; i < width_; ++i) {
        for (size_t j = 0; j < height_; ++j) {
            if (i == 0 || i == width_ - 1 || j == 0 || j == height_ - 1) {
                map_[i][j].fld_ = end_block;
            } else {
                map_[i][j].fld_ = clear_field;
            }
        }
    }
}

Map::~Map() {
    for (size_t i = 0; i < width_; ++i) {
        delete[] map_[i];
    }
    delete[] map_;
}

void Map::out() {
    for (size_t i = 0; i < width_; ++i) {
        for (size_t j = 0; j < height_; ++j) {
            if (map_[i][j].fld_ == player) {
                std::cout << "U ";
            }else if (map_[i][j].fld_ == end_block) {
                std::cout << "# ";
            } else if (map_[i][j].fld_ == clear_field) {
                std::cout << "* ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Map::change_map(size_t x, size_t y, key fld) {
    map_[x][y].fld_ = fld;
}