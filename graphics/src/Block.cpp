
#include "Block.h"

#define SCALE 1

Block::Block(unsigned int _id, const std::string &filename) : id(_id), winZone(false) {

    if (!texBlock.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load " + filename);
    }
    texBlock.setSmooth(true);
    mBlock.setTexture(texBlock);
    mBlock.setScale(SCALE, SCALE);


}

void Block::setPos(unsigned int x, unsigned int y) {
    mBlock.setPosition(x, y);
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
            mBlock.setColor(sf::Color(255, 255, 255, 200));
            break;
        case 1: // block player
            mBlock.setColor(sf::Color(255, 255, 0, 255));
            break;
        case 0: // border
            mBlock.setColor(sf::Color(255, 0, 0, 255));
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
