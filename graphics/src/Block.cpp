
#include "Block.h"
#define SCALE 1
Block::Block(unsigned int _id, const std::string& filename): id(_id){

    if(!texBlock.loadFromFile(filename)){
        throw std::runtime_error("Failed to load " + filename);
    }
    texBlock.setSmooth(true);
    mBlock.setTexture(texBlock);
    mBlock.setScale(SCALE,SCALE);


}

void Block::setPos(unsigned int x, unsigned int y) {
    mBlock.setPosition(x,y);
}

void Block::draw(sf::RenderWindow &l_window) {
    setID(id);
    l_window.draw(mBlock);
}

int Block::getX() {
    return mBlock.getPosition().x;
}
void Block::setID(unsigned short _id) {
    id =_id;
    switch(id){

        case 0:
            mBlock.setColor(sf::Color(255, 255, 255, 200));
            break;
        default:
            mBlock.setColor(sf::Color(255, 255, 0, 255));

    }

}

int Block::getY() {
    return mBlock.getPosition().y;
}