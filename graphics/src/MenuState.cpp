#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cmath>

MenuState::MenuState(StateStack &stack, State::Context context) : State(stack, context), textbuf() {

    Text *Menu = new Text(*getContext().font, "Menu", 50);
    Menu->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(Menu);

    Text *TipEnter = new Text(*getContext().font, "press space", 20);
    TipEnter->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.0f);
    textbuf.push_back(TipEnter);

    Text *TipExit = new Text(*getContext().font, "press backspace for exit", 20);
    TipExit->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 4.0f * 3);
    textbuf.push_back(TipExit);

    updateActiveText();
}

void MenuState::draw(std::queue<EventMessage> *eventQueue) {


    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);
    window.setView(window.getDefaultView());

    for (auto text: textbuf)
        text->draw(window);


}

bool MenuState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request = nullptr) {

    if (event.key.code == sf::Keyboard::Space) {
        std::cout << "SPACE\n";
        return true;

    } else return false;


}

void MenuState::ChangeState() {
    requestStackPop();
    requestStackPush(States::Preparation);
}


void MenuState::updateActiveText() {

    for (auto  text: textbuf)
        text->setColor(sf::Color::White);

}




