
#include "Block.h"

Block::Block(unsigned int _id): id(_id){

    texBlock.loadFromFile("block.png");
    mBlock.setTexture(texBlock);
}

void Block::setPos(float x, float y) {
    mBlock.setPosition(x,y);
}
sf::Sprite Block::getSprite(){
    return  mBlock;
}

