#include <iostream>
#include "Map.h"

DrawMap::DrawMap(sf::RenderWindow &mWindow) : mWindow(mWindow) {


}


void DrawMap::DrawBack() {
    mWindow.clear(sf::Color::White);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {

            mBlocks[i][j].setPos(i * 40, j * 40);
            mBlocks[i][j].draw(mWindow);
        }
    }

    for (int i = 0; i < hp.size(); ++i) {
        hp[i]->setPos(i * 60 + 60, height - 40);
        hp[i]->draw(mWindow);
    }

}

void DrawMap::SetHp(unsigned short id, int hpVal) {
    hp.resize(hpVal);
}


void DrawMap::SetUnits(unsigned short id, unsigned int x, unsigned int y) {

    mUnits[id]->setPos(x * 40 + 20, y * 40 + 20);

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

    /* hp.resize(3);
     for (int i = 0; i < hp.size(); ++i) {
         auto *h = new Heart;
         hp[i] = h;
     }*/

    mBlocks = new Block *[width * 40]; // двумерный массив для блоков на всю карту
    for (int i = 0; i < width * 40; ++i)
        mBlocks[i] = new Block[height];

    mUnits.resize(countUnits);
    for (auto &mUnit: mUnits) {
        mUnit = new Unit;
    }


}

void DrawMap::DrawPlayerInit(unsigned short id, unsigned int x, unsigned int y, unsigned int team) {

    mUnits[id]->setPos(x * 40 + 20, y * 40 + 20);
    if (id == 0) {
        mUnits[id]->setID(10);
    } else mUnits[id]->setID(team);

}





