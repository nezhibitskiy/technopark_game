#include "Unit.h"
#define SCALE 0.75
Unit::Unit(unsigned int _id, const std::string& filename): id(_id){

    if(!texUnit.loadFromFile(filename)){
        throw std::runtime_error("Failed to load texture" + filename);
    }
    texUnit.setSmooth(true);
    mUnit.setOrigin(texUnit.getSize().x * 0.5 ,
                    texUnit.getSize().y * 0.5);
    mUnit.setTexture(texUnit);
    mUnit.setScale(SCALE,SCALE);

}

void Unit::setPos(unsigned int x, unsigned int y) {

    mUnit.setPosition(x,y);
}



void Unit::draw(sf::RenderWindow &l_window) {
        l_window.draw(mUnit);
}

int Unit::getX() {
    return mUnit.getPosition().x;
}

int Unit::getY() {
    return mUnit.getPosition().y;
}

void Unit::setID(unsigned short _id) {
    id = _id;
    switch (id) {
        case 0:
            mUnit.setColor(sf::Color(70, 70, 250, 255));
            break;
        case 1:
            mUnit.setColor(sf::Color(250, 70, 70, 255));
            break;
        case 10:
            mUnit.setColor(sf::Color(80, 80, 100, 255));
            break;
    }
}



