#include "Heart.h"


Heart::Heart(const std::string &filename, size_t _sizePx) : sizePx(_sizePx) {
    if (!texHeart.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load " + filename);
    }
    mHeart.setTexture(texHeart);

}

void Heart::setPos(unsigned int x, unsigned int y) {
    mHeart.setOrigin(texHeart.getSize().x * 0.5, texHeart.getSize().y * 0.5);
    mHeart.setPosition(x * getSize(), y - getSize());
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
    switch (id) {
        case 0:
            mHeart.setColor(sf::Color(255, 255, 255, 255));
            break;
        case 1:
            mHeart.setColor(sf::Color(10, 10, 255, 255));
            break;
    }
}

size_t Heart::getSize() {
    return sizePx;
}

