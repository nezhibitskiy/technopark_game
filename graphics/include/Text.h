#ifndef PROJECT_TP_TEXT_H
#define PROJECT_TP_TEXT_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Object.h"

class Text : public Draw::Object {
public:
    Text(sf::Font &font, const std::string &text, int size);

    void setPos(unsigned int x, unsigned int y) override;

    int getX() override {};

    int getY() override {};

    void setID(unsigned short _id) override {};

    void setColor();

    void draw(sf::RenderWindow &l_window) override;

private:

    sf::Text mText;

};


#endif //PROJECT_TP_TEXT_H
