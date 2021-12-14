#ifndef PROJECT_TP_TEXTFIELD_H
#define PROJECT_TP_TEXTFIELD_H


#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>

class TextField : public sf::Transformable, public sf::Drawable{
public:
    TextField(unsigned int maxChars = 20) :
            m_size(maxChars),
            m_rect(sf::Vector2f(15 * m_size, 20)), // 15 pixels per char, 20 pixels height, you can tweak
            m_hasfocus(false)
    {
        m_font.loadFromFile("../resources/PEPSI_pl.ttf");
        m_rect.setOutlineThickness(2);
        m_rect.setFillColor(sf::Color::White);
        m_rect.setOutlineColor(sf::Color(127,127,127));
        m_rect.setPosition(this->getPosition());
    }

    const std::string getText() const{
        return m_text;
    }

    void setPosition(float x, float y){
        sf::Transformable::setPosition(x, y);
        m_rect.setPosition(x, y);
    }

    bool contains(sf::Vector2f point) const{
        return m_rect.getGlobalBounds().contains(point);
    }

    void setFocus(bool focus){
        m_hasfocus = focus;
        if (focus){
            m_rect.setOutlineColor(sf::Color::Blue);
        }
        else{
            m_rect.setOutlineColor(sf::Color(127, 127, 127)); // Gray color
        }
    }

    void handleInput(sf::Event e){
        if (!m_hasfocus || e.type != sf::Event::TextEntered)
            return;

        if (e.text.unicode == 8){   // Delete key
            m_text = m_text.substr(0, m_text.size() - 1);
        }
        else if (m_text.size() < m_size){
            m_text += e.text.unicode;
        }
    }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {};

private:
    unsigned int m_size;
    sf::Font m_font;
    std::string m_text;
    sf::RectangleShape m_rect;
    bool m_hasfocus;
};


#endif //PROJECT_TP_TEXTFIELD_H
