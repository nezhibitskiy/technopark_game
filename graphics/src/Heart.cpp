#include "Heart.h"


Heart::Heart(const std::string &filename) {
    if(!texHeart.loadFromFile(filename)){
        throw std::runtime_error("Failed to load " + filename);
    }
    mHeart.setTexture(texHeart);

}

void Heart::setPos(unsigned int x, unsigned int y) {
    mHeart.setOrigin(texHeart.getSize().x * 0.5 ,texHeart.getSize().y * 0.5);
    mHeart.setPosition(x,y);
}

int Heart::getX() {
    return mHeart.getPosition().x;
}

int Heart::getY() {
    return mHeart.getPosition().y;
}

void Heart::draw(sf::RenderWindow &l_window) {
    l_window.draw(mHeart);
}

void Heart::setID(unsigned short _id) {
        id = _id;
}

