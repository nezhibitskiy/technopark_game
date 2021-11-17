#include "Unit.h"

Unit::Unit(unsigned int _id): id(_id) {
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
    texUnit.loadFromFile("Boo.png");
    mUnit.setScale(1/6.4,1/6.56);
    mUnit.setTexture(texUnit);
}

void Unit::setPos(float x, float y) {
    mUnit.setPosition(x,y);
}

sf::Sprite Unit::getSprite() {
    return mUnit;
}

void Unit::SetScale() {

}



