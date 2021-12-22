#include <iostream>
#include "Map.h"

#define DEFAULT_SIZE 30

DrawMap::DrawMap(sf::RenderWindow &mWindow, sf::Font &font) : mWindow(mWindow), mfont(font) {

}

DrawMap::~DrawMap() {
    for (int i = 0; i < width; ++i)
        delete[] mBlocks[i];
    delete[] mBlocks;
}

void DrawMap::DrawBack() {


    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {

            mBlocks[i][j].setPos(i, j);
            mBlocks[i][j].draw(mWindow);
        }
    }

    /*for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (mObjects[i][j] != nullptr) {
                mObjects[i][j]->setPos(i, j);
                mObjects[i][j]->draw(mWindow);
            }
        }
    }*/


    for (int i = 0; i < hp.size(); ++i) {
        hp[i]->setPos((i + 1) * hp[i]->getSize(), mWindow.getSize().y - hp[i]->getSize());
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


        if (hp.size() > hpVal) {
            for (int i = 0; i < hpVal; ++i) {
                hp[i]->setID(1);
            }
            for (int i = hpVal; i < hp.size(); ++i) {
                hp[i]->setID(0);
            }
        } else {
            hp.resize(hpVal);
            for (auto &i: hp) {
                i = new Heart;
                i->setID(1);
            }

        }


}


void DrawMap::SetUnits(unsigned short id, unsigned int x, unsigned int y) {


    mUnits[id]->setPos(x, y);

}

void DrawMap::SetBlocks(unsigned short id, unsigned int x, unsigned int y) {

    if (mBlocks[x][y].IsWinZone() && id != 1) {
        mBlocks[x][y].setID(5);
    } else mBlocks[x][y].setID(id);

}

void DrawMap::Draw() {

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (mObjects[i][j] != nullptr) {

                mObjects[i][j]->draw(mWindow);
            }
        }
    }


    for (auto i: mUnits) {
        i->draw(mWindow);
    }
    if (time != nullptr) {
        time->draw(mWindow);
    } else SetTime(floor(clock.getElapsedTime().asSeconds()));


}


void DrawMap::DrawMapInit(unsigned int _width, unsigned int _height, unsigned int countUnits) {

    width = _width;
    height = _height;


    mBlocks = new Block *[width];
    for (int i = 0; i < width; ++i) {
        mBlocks[i] = new Block[height];

    }

    mObjects = new Draw::Object **[width];
    for (int i = 0; i < width; ++i) {
        mObjects[i] = new Draw::Object *[height];
    }

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            mObjects[i][j] = nullptr;
        }
    }

    mUnits.resize(countUnits);
    for (auto &mUnit: mUnits) {
        mUnit = new Unit;
    }


}

void DrawMap::DrawPlayerInit(unsigned short id, unsigned int x, unsigned int y, unsigned int team) {


    mUnits[id]->setPos(x, y);
    if (id == 0 && team == 0) {
        mUnits[id]->setID(10);
    } else if (id == 0 && team == 1) {
        mUnits[id]->setID(20);
    } else mUnits[id]->setID(team + 1);

}

void DrawMap::SetTime(unsigned short id) {

    if (id <= 10) {
        time = new Text(mfont, std::to_string(id), 40);
        time->setColor(sf::Color::Red);
        time->setPos(mWindow.getSize().x / 2, (height) * mBlocks[0]->getSize());

    } else {
        time = new Text(mfont, std::to_string(id), 30);
        time->setPos(mWindow.getSize().x / 2, height * mBlocks[0]->getSize());
    }


}

void DrawMap::SetPotion(unsigned int x, unsigned int y) {

    h = new Heart;
    h->setID(10);
    mObjects[x][y] = h;
    h->setPos(x * mBlocks[0]->getSize()+mBlocks[0]->getSize()/2  , y * mBlocks[0]->getSize()+mBlocks[0]->getSize()/2 );
}

void DrawMap::DeleteDraw(unsigned short id, unsigned int x, unsigned int y) {

    mObjects[x][y] = nullptr;

}




