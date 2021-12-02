//
// Created by ivan_lemon on 11/17/21.
//

#ifndef FROMBOOK_MAP_H
#define FROMBOOK_MAP_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include "Unit.h"
#include "Block.h"
#include "Heart.h"

class DrawMap {
public:

    explicit DrawMap( sf::RenderWindow &mWindow);
    void DrawMapInit(unsigned int _width,unsigned int _height,unsigned int countUnits);
    ~DrawMap(){

    }
    void DrawBack();
    void SetHp(unsigned short id,int hpVal);
    void SetUnits(unsigned short id, unsigned int x, unsigned int y );
    void SetBlocks(unsigned short id, unsigned int x, unsigned int y);
    void Draw();
private:
    unsigned int width;
    unsigned int height;
    sf::RenderWindow &mWindow;
    std::vector<Unit*> mUnits;
    Block **mBlocks;
    std::vector<Heart*> hp;

};


#endif //FROMBOOK_MAP_H
