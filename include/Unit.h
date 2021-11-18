//
// Created by ivan_lemon on 11/17/21.
//

#ifndef FROMBOOK_UNIT_H
#define FROMBOOK_UNIT_H


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Object.h"

class Unit : public Object{
public:
    explicit Unit(unsigned int _id = 0, const std::string& filename = "../resources/Boo.png");
    void  setPos(float x, float y) override;
    void draw(sf::RenderWindow & l_window) override;
    ~Unit() = default;

private:
    unsigned int id;
    sf::Sprite mUnit;
    sf::Texture texUnit;
};


#endif //FROMBOOK_UNIT_H
