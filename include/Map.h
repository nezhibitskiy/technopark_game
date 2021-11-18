//
// Created by ivan_lemon on 11/17/21.
//

#ifndef FROMBOOK_MAP_H
#define FROMBOOK_MAP_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Unit.h"
#include "Block.h"

class Map {
public:

    explicit Map( sf::RenderWindow &mWindow);

    void Draw();

private:
    sf::RenderWindow &mWindow;
    std::vector<Unit> mUnits;
    Block mBlock;

};


#endif //FROMBOOK_MAP_H
