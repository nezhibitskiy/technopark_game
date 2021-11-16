#ifndef PROJECT_GAME_CLASS_H
#define PROJECT_GAME_CLASS_H

#include <iostream>
#include <vector>

#include "player_class.h"
#include "map_class.h"

class Game {
public:
    Game(); // size_t players_number, std::vector<std::pair<size_t, size_t>> coords);
    void start_game();
    void printMap() {
        map.out();
    }
    void addObject(Object* object, unsigned int x, unsigned int y) {
        map.addObject(object, x, y);
    }
private:
    Map map;
    Player* players;
};

#endif // PROJECT_GAME_CLASS_H