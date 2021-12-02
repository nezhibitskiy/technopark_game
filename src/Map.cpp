#include "Map.h"

DrawMap::DrawMap(sf::RenderWindow &mWindow) : mWindow(mWindow) {


}


void DrawMap::DrawBack() {
    mWindow.clear(sf::Color::White);
    for (int i = 0; i < width / 40; ++i) {
        for (int j = 0; j < height / 40 - 2; ++j) {

            mBlocks[i][j].setPos(i * 40, j * 40);
            mBlocks[i][j].draw(mWindow);
        }
    }

    for (int i = 0; i < hp.size(); ++i) {
        hp[i]->setPos(i * 60 + 60, height  - 40);
        hp[i]->draw(mWindow);
    }

}

void DrawMap::SetHp(unsigned short id,int hpVal){
    hp.resize(hpVal);
}


void DrawMap::SetUnits(unsigned short id, unsigned int x, unsigned int y) {

    mUnits[id]->setPos(x * 40 + mUnits[id]->getX(), y * 40 + mUnits[id]->getY());

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

    hp.resize(3);
    for (int i = 0; i < hp.size(); ++i) {
        auto *h = new Heart;
        hp[i] = h;
    }

    mBlocks = new Block *[width / 40]; // двумерный массив для блоков на всю карту
    for (int i = 0; i < width / 40; ++i)
        mBlocks[i] = new Block[height / 40];

    mUnits.resize(countUnits);
    Unit *player = new Unit(0);
    player->setPos(20, 20);
    mUnits[0] = player;
    for (int i = 1; i < countUnits / 2; ++i) {

        Unit *u = new Unit(1);
        u->setPos(60, 60);
        mUnits[i] = u;

    }
    for (int i = countUnits / 2; i < countUnits; ++i) {

        Unit *u = new Unit(10);
        u->setPos(620 - i * 40, 380);
        mUnits[i] = u;
    }


}




