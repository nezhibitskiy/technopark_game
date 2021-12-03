
#include "Block.h"

Block::Block(unsigned int _id, const std::string& filename): id(0){

    if(!texBlock.loadFromFile(filename)){
        throw std::runtime_error("Failed to load " + filename);
    }
    mBlock.setTexture(texBlock);


}

void Block::setPos(unsigned int x, unsigned int y) {
    mBlock.setPosition(x,y);
}

void Block::draw(sf::RenderWindow &l_window) {
    l_window.draw(mBlock);
}

int Block::getX() {
    return mBlock.getPosition().x;
}
void Block::setID(unsigned short _id) {
    id =_id;
    switch(id){
        case 10:
            mBlock.setColor(sf::Color(220, 220, 0, 255));
            break;
        case 0:
            mBlock.setColor(sf::Color(255, 255, 255, 255));
    }

}

int Block::getY() {
    return mBlock.getPosition().y;
}