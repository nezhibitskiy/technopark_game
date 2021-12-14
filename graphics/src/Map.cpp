#include <iostream>
#include "Map.h"

DrawMap::DrawMap(sf::RenderWindow &mWindow) : mWindow(mWindow) {

}

DrawMap::~DrawMap() {
    for (int i = 0; i < width; ++i)
        delete[] mBlocks[i];
    delete[] mBlocks;
}

void DrawMap::DrawBack() {

    mWindow.clear(sf::Color::Black);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {

            mBlocks[i][j].setPos(i, j);
            mBlocks[i][j].draw(mWindow);
        }
    }
    for (int i = 0; i < hp.size(); ++i) {
        hp[i]->setPos((i + 1), mWindow.getSize().y);
        hp[i]->draw(mWindow);
    }


}

void DrawMap::SetZone(unsigned int x, unsigned int y, unsigned int rad) {


    for (int i = x - rad; i < rad + x; ++i) {
        for (int j = y - rad; j < rad + y; ++j) {
            mBlocks[i][j].setID(5);
            mBlocks[i][j].SetWinZone();
        }
    }
}

void DrawMap::SetHp(unsigned short id, int hpVal) {
    if (id == 0) {
        hp.resize(hpVal);
        for (int i = 0; i < hp.size(); ++i) {
            hp[i] = new Heart;
        }
    }
}


void DrawMap::SetUnits(unsigned short id, unsigned int x, unsigned int y) {

    mUnits[id]->setPos(x,y);


}

void DrawMap::SetBlocks(unsigned short id, unsigned int x, unsigned int y) {

    if (mBlocks[x][y].IsWinZone() && id != 1) {
        mBlocks[x][y].setID(5);
    } else mBlocks[x][y].setID(id);

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

    mUnits[id]->setPos(x, y);
    if (id == 0) {
        mUnits[id]->setID(0);
    } else mUnits[id]->setID(team+1);

}



