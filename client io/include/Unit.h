//
// Created by ivan_lemon on 11/17/21.
//

#ifndef FROMBOOK_UNIT_H
#define FROMBOOK_UNIT_H


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Object.h"

class Unit : public Object{
public:
    explicit Unit(unsigned int _id);
    sf::Sprite getSprite() override;
    void  setPos(float x, float y) override;
    ~Unit() = default;
    void SetScale();
private:
    unsigned int id;
    sf::Sprite mUnit;
    sf::Texture texUnit;
};


#endif //FROMBOOK_UNIT_H
