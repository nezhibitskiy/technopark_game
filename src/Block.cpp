
#include "Block.h"

Block::Block(unsigned int _id, const std::string& filename): id(_id){

    if(!texBlock.loadFromFile(filename)){
        throw std::runtime_error("Failed to load " + filename);
    }
    mBlock.setTexture(texBlock);
}

void Block::setPos(float x, float y) {
    mBlock.setPosition(x,y);
}

void Block::draw(sf::RenderWindow &l_window) {
    l_window.draw(mBlock);
}