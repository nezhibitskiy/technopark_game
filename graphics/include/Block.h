#ifndef FROMBOOK_BLOCK_H
#define FROMBOOK_BLOCK_H


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Object.h"

class Block : public Draw::Object {
public:
    explicit Block(unsigned int _id = 0, const std::string& filename = "../resources/bricks.png");
    void setPos(unsigned int x, unsigned int y) override;
    void setID(unsigned short _id) override;
    int getX() override;
    int getY() override;
    void draw(sf::RenderWindow & l_window) override;

    ~Block() = default;

private:

    unsigned short id;
    sf::Sprite mBlock;
    sf::Texture texBlock;
};


#endif //FROMBOOK_BLOCK_H
