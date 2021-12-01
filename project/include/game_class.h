#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "player_class.h"
#include "map_class.h"

class Game {
public:
    Game();
    void start_game();
    void addObject(Object* obj, unsigned int x, unsigned int y);
    bool deleteObject(unsigned int id);
    static unsigned int fabr();
    ~Game();
private:
    Map* map;
    Player* players;
    std::unordered_multimap<unsigned int, Object*> mapObjects;
};

#endif // PROJECT_GAME_CLASS_H