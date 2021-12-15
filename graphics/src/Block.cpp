
#include "Block.h"

#define SCALE 1

Block::Block(unsigned int _id, const std::string &filename,size_t _sizePx) : id(_id), winZone(false),sizePx(_sizePx) {

    if (!texBlock.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load " + filename);
    }
    texBlock.setSmooth(true);
    mBlock.setTexture(texBlock);



}

void Block::setPos(unsigned int x, unsigned int y) {
    mBlock.setPosition(x*getSize(), y*getSize());
}

void Block::draw(sf::RenderWindow &l_window) {
    setID(id);
    l_window.draw(mBlock);
}

int Block::getX() {
    return mBlock.getPosition().x;
}

void Block::setID(unsigned short _id) {
    id = _id;
    switch (id) {

        default: // back
            mBlock.setColor(sf::Color(200, 200, 200, 255));
            break;
        case 1: // block player
            mBlock.setColor(sf::Color(255, 255, 0, 255));
            break;
        case 0: // border
            mBlock.setColor(sf::Color(100, 100, 100, 255));
            break;
        case 5: // winzone
            mBlock.setColor(sf::Color(0, 255, 0, 255));
            break;
    }

}

int Block::getY() {
    return mBlock.getPosition().y;
}

bool Block::IsWinZone() {
    return winZone;
}
void Block::SetWinZone() {
    winZone = true;
}

size_t Block::getSize() {
    return sizePx;
}

void Block::setSize(size_t size) {
    sizePx = size;
    mBlock.setScale(sizePx / texBlock.getSize().x, sizePx / texBlock.getSize().y);
}
