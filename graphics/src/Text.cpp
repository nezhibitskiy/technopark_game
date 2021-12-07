#include "Text.h"


Text::Text(sf::Font &font,const std::string& text,int size): mText(text, font, size) {

}

void Text::setPos(unsigned int x, unsigned int y) {
    sf::FloatRect textRect = mText.getLocalBounds();// set to center
    mText.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    mText.setPosition(sf::Vector2f(x, y));
}

void Text::draw(sf::RenderWindow &l_window) {
    l_window.draw(mText);
}

void Text::setColor(const sf::Color color) {
  mText.setFillColor(color);
}


