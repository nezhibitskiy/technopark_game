#include <iostream>
#include "Map.h"

#define SIZE_PIXELS 30

DrawMap::DrawMap(sf::RenderWindow &mWindow) : mWindow(mWindow) {

}


void DrawMap::DrawBack() {

    mWindow.clear(sf::Color::White);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {

            mBlocks[i][j].setPos(i * SIZE_PIXELS, j * SIZE_PIXELS);
            mBlocks[i][j].draw(mWindow);
        }
    }
    for (int i = 0; i < hp.size(); ++i) {
        hp[i]->setPos((i + 1) * SIZE_PIXELS, mWindow.getSize().y - SIZE_PIXELS);
        hp[i]->draw(mWindow);
    }


}

void DrawMap::SetHp(unsigned short id, int hpVal) {


    hp.resize(hpVal);
    for (int i = 0; i < hp.size(); ++i) {

        hp[i] = new Heart;
    }

}


void DrawMap::SetUnits(unsigned short id, unsigned int x, unsigned int y) {

    mUnits[id]->setPos(x * SIZE_PIXELS + SIZE_PIXELS / 2, y * SIZE_PIXELS + SIZE_PIXELS / 2);

}

void DrawMap::SetBlocks(unsigned short id, unsigned int x, unsigned int y) {

    mBlocks[x][y].setID(id);

}

void DrawMap::Draw() {
    for (auto i: mUnits) {
        i->draw(mWindow);
    }


}


void DrawMap::DrawMapInit(unsigned int _width, unsigned int _height, unsigned int countUnits) {

    width = _width;
    height = _height;


    mObjects.resize(width * height, nullptr);



    mBlocks = new Block *[width];
    for (int i = 0; i < width; ++i)
        mBlocks[i] = new Block[height];


    mUnits.resize(countUnits);
    for (auto &mUnit: mUnits) {
        mUnit = new Unit;
    }


}

void DrawMap::DrawPlayerInit(unsigned short id, unsigned int x, unsigned int y, unsigned int team) {

    mUnits[id]->setPos(x * SIZE_PIXELS + SIZE_PIXELS / 2, y * SIZE_PIXELS + SIZE_PIXELS / 2);
    if (id == 0) {
        mUnits[id]->setID(10);
    } else mUnits[id]->setID(team);

}





