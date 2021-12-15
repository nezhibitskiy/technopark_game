#ifndef TEST_HEART_H
#define TEST_HEART_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Object.h"

class Heart : public Draw::Object{
public:
    Heart(const std::string& filename = "../resources/heart.png",size_t _sizePx = 30);
    void setPos(unsigned int x, unsigned int y) override;
    int getX() override;
    int getY() override;
    void setID(unsigned short _id) override;
    void draw(sf::RenderWindow & l_window) override;
    size_t getSize() override;
    void setSize(size_t size) override {};
private:
    float sizePx;
    unsigned short id;
    sf::Sprite mHeart;
    sf::Texture texHeart;
};


#endif //TEST_HEART_H
