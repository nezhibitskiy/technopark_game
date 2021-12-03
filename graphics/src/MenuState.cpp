#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cmath>

MenuState::MenuState(StateStack &stack, State::Context context) : State(stack, context), textbuf() {

    sf::Font &font = *getContext().font;
    sf::Text MenuText("Menu", font, 50);
    sf::FloatRect textRect = MenuText.getLocalBounds();// set to center
    MenuText.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    MenuText.setPosition(sf::Vector2f(640 / 2.0f, 0));
    textbuf.push_back(MenuText);

    sf::Text text("press space", font, 30);
    textRect = text.getLocalBounds();// set to center
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.left + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(640 / 2.0f, 480 / 2.0f));
    textbuf.push_back(text);

    sf::Text exit("press backspace for exit", font, 30);
    textRect = exit.getLocalBounds();// set to center
    exit.setOrigin(textRect.left + textRect.width / 2.0f, textRect.left + textRect.height / 2.0f);
    exit.setPosition(text.getPosition() + sf::Vector2f(0.f, 30.f));
    textbuf.push_back(exit);

    updateActiveText();
}

void MenuState::draw(std::queue<EventMessage>* eventQueue) {


    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::White);
    window.setView(window.getDefaultView());

    for (sf::Text &text: textbuf)
        window.draw(text);


}

bool MenuState::handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) {

    if (event.key.code == sf::Keyboard::Space) {
        std::cout << "SPACE\n";
         return true;

        std::cout << "change state to Waiting room \n";
        // отдельная функция


    }else return false;
    /*else if (event.key.code == sf::Keyboard::BackSpace && event.type == sf::Event::KeyReleased) {
        std::cout << "change state to Close window \n";
        requestStackPop();
        requestStateClear();

    } else return false;

    return true;*/

}

void MenuState::ChangeState() {
    requestStackPop();
    requestStackPush(States::Preparation);
}



void MenuState::updateActiveText() {

    for (sf::Text &text: textbuf)
        text.setColor(sf::Color::Red);

}




