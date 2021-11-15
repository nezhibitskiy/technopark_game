#include <iostream>
#include <vector>

#include "map_class.h"

Map::Map(unsigned int width, unsigned int height) {
    width_ = width;
    height_ = height;
    //не то
    /*map_ = new Object *[width_];
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
    }*/

    /*map_ = new Object**[height_];
    for (size_t i = 0; i < height; ++i) {
        map_[i] = new Object*[width_];
    }
    auto block1 = new Block(1, 1);
    map_[1][1] = block1;
    if (map_[1][1]->Damagable()) {
        if (map_[1][1]->Damage(1)) {
            map_[1][1]->Respawn();
        } else {
            delete map_[1][1];
        }
    }*/
}

Map::~Map() {
    for (size_t i = 0; i < width_; ++i) {
        delete[] map_[i];
    }
    delete[] map_;
}
