#ifndef FROMBOOK_UNIT_H
#define FROMBOOK_UNIT_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Object.h"

class Unit : public Draw::Object{
public:
    explicit Unit(unsigned int _id = 1, const std::string& filename = "../resources/ninja1.png",size_t _sizePx = 30);
    void  setPos(unsigned int x, unsigned int y) override;
    void setSize(size_t size) override;
    int getX() override;
    int getY() override;
    void setID(unsigned short _id) override;
    void draw(sf::RenderWindow & l_window) override;
    size_t getSize() override;


private:
    float sizePx;
    unsigned int id;
    sf::Sprite mUnit;
    sf::Texture texUnit;
};


#endif //FROMBOOK_UNIT_H
