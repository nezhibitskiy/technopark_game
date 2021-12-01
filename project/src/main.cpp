#include <iostream>
#include <vector>

#include "game_class.h"



int main() {
    std::vector<std::pair<size_t, size_t>> coords;
    coords.emplace_back(std::make_pair(5, 5));
    Game my_game();
    my_game.start_game();
    return 0;
}
