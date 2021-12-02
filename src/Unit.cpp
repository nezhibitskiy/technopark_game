#include "Unit.h"

Unit::Unit(unsigned int _id, const std::string& filename): id(_id){
    switch (id) {
        case 10:
            mUnit.setColor(sf::Color(250, 70, 70, 255));
            break;
        case 1:
            mUnit.setColor(sf::Color(140, 200, 140, 255));
            break;
        default:
            mUnit.setColor(sf::Color(70, 230, 70, 255));
            break;
    }
    if(!texUnit.loadFromFile(filename)){
        throw std::runtime_error("Failed to load texture" + filename);
    }

    mUnit.setOrigin(texUnit.getSize().x * 0.5 ,
                    texUnit.getSize().y * 0.5);
    mUnit.setTexture(texUnit);


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



