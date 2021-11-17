#ifndef PROJECT_MAP_CLASS_H
#define PROJECT_MAP_CLASS_H

#include <iostream>
#include <vector>

#include "objects.h"
#include "player.h"


class Map {
public:
    Map(unsigned int width, unsigned int height); //??????????
    Map(const char* filename);
    ~Map();
    char get_team_count(); //
    char get_team_players_count();
    Object* get_map_object(unsigned int i, unsigned int j);
    int set_map_object(unsigned int i, unsigned int j, Object* obj);
    void move_player(Player* player_, unsigned int x, unsigned int y);

private:
    Object*** map_;
    unsigned int width_;
    unsigned int height_;
    char team_count_;
    std::pair<int, int>* spawns;
};

#endif //PROJECT_MAP_CLASS_H