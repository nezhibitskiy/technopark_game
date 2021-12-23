#include "Unit.h"

Unit::Unit(unsigned int _id, const std::string &filename, size_t _sizePx) : id(_id), sizePx(_sizePx) {

    if (!texUnit.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture" + filename);
    }
    texUnit.setSmooth(true);

    mUnit.setOrigin(texUnit.getSize().x * 0.5,
                    texUnit.getSize().y * 0.5);
    mUnit.setTexture(texUnit);


}

void Unit::setPos(unsigned int x, unsigned int y) {

  mUnit.setPosition(x * getSize() + getSize() / 2, y * getSize() + getSize() / 2);
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
        case 10:
            mUnit.setColor(sf::Color(150, 150, 255, 255));
            break;
        case 20:
            mUnit.setColor(sf::Color(150, 255, 150, 255));
            break;
        case 2:
            mUnit.setColor(sf::Color(70, 255, 70, 255));
            break;
        case 1:
            mUnit.setColor(sf::Color(80, 80, 255, 255));
            break;
        case 5:
            mUnit.setColor(sf::Color(255, 80, 80, 255));
            break;

    }
}

size_t Unit::getSize() {
    return sizePx;
}

void Unit::setSize(size_t size) {
    sizePx = size;
    mUnit.setScale(sizePx / texUnit.getSize().x, sizePx / texUnit.getSize().y);
}



