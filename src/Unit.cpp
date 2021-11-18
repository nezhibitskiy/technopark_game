#include "Unit.h"

Unit::Unit(unsigned int _id, const std::string& filename): id(_id) {
    switch (id) {
        case 10:
            mUnit.setColor(sf::Color(220, 70, 70, 255));
            break;
        case 1:
            mUnit.setColor(sf::Color(140, 220, 140, 255));
            break;
        default:
            mUnit.setColor(sf::Color(70, 220, 70, 255));
            break;
    }
    if(!texUnit.loadFromFile(filename)){
        throw std::runtime_error("Failed to load texture" + filename);
    }
    mUnit.setScale(1/6.4,1/6.56);
    mUnit.setTexture(texUnit);
}

void Unit::setPos(float x, float y) {
    mUnit.setPosition(x,y);
}


void Unit::draw(sf::RenderWindow &l_window) {
        l_window.draw(mUnit);
}




