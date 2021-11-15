#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>

#include "objects.h"


class Map {
public:
    Map(unsigned int width, unsigned int height);
    ~Map();
    char get_team_count(); //
    char get_team_players_count();


private:
    Object*** map_;
    size_t width_;
    size_t height_;
    char team_count_;
    std::pair<int, int>* spawns;
};

#endif //PROJECT_MAP_CLASS_H