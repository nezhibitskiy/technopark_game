//
// Created by ivan_lemon on 11/17/21.
//

#ifndef FROMBOOK_MAP_H
#define FROMBOOK_MAP_H

#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <map>
#include "Unit.h"
#include "Block.h"
#include "Heart.h"
#include "Object.h"
#include "Text.h"

class Object;

class DrawMap {
public:

    explicit DrawMap(sf::RenderWindow &mWindow, sf::Font &font);

    ~DrawMap();

    void DrawMapInit(unsigned int _width, unsigned int _height, unsigned int countUnits);

    void DrawPlayerInit(unsigned short id, unsigned int x, unsigned int y, unsigned int team);

    void DrawBack();

    void SetHp(unsigned short id, int hpVal);

    void SetUnits(unsigned short id, unsigned int x, unsigned int y);

    void SetBlocks(unsigned short id, unsigned int x, unsigned int y);

    void SetZone(unsigned int x, unsigned int y, unsigned int rad);

    void SetTime(unsigned short id);

    void SetPotion(unsigned int x, unsigned int y);

    void DeleteDraw(unsigned short id, unsigned int x, unsigned int y);

    void SetKills(unsigned short id,unsigned short kills);

    void SetHit(unsigned short id, int hpVal);

    void ReturnUnitState();


    void Draw();

private:

    unsigned int width = 0;
    unsigned int height = 0;



    Text *time = nullptr;
    sf::Font &mfont;
    sf::RenderWindow &mWindow;
    std::vector<Unit *> mUnits;
    Draw::Object ***mObjects;
    Block **mBlocks;
    std::vector<Heart *> hp;
    std::map<unsigned short,int> killsCount;
    std::vector<Text*> Kills;
   // Heart *h;


};


#endif //FROMBOOK_MAP_H
