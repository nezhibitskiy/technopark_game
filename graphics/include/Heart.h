#ifndef TEST_HEART_H
#define TEST_HEART_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Object.h"

class Heart : public DrawObject{
public:
    Heart(const std::string& filename = "../resources/heart.png");
    void setPos(unsigned int x, unsigned int y) override;
    int getX() override;
    int getY() override;
    void draw(sf::RenderWindow & l_window) override;
private:
    sf::Sprite mHeart;
    sf::Texture texHeart;
};


#endif //TEST_HEART_H
