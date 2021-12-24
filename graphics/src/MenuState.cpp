#include "MenuState.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <cmath>

MenuState::MenuState(StateStack &stack, State::Context context) : State(stack, context), textbuf(), mOptionIndex(0) {

    Text *Menu = new Text(*getContext().font, "Menu", 50);
    Menu->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(Menu);

    Text *TipEnter = new Text(*getContext().font, "press space", 20);
    TipEnter->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 9.0f * 7);
    textbuf.push_back(TipEnter);

    Text *create = new Text(*getContext().font, "create", 40);
    create->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.5f);
    activeTextbuf.push_back(create);

    Text *join = new Text(*getContext().font, "join", 40);
    join->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.0f);
    activeTextbuf.push_back(join);

    updateActiveText();

}

void MenuState::draw(std::queue<EventMessage> *eventQueue) {


    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);



    window.draw(*getContext().Back);
    window.setView(window.getDefaultView());

    for (auto text: textbuf)
        text->draw(window);

    for (auto text: activeTextbuf)
        text->draw(window);


}

bool MenuState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request = nullptr) {

    if (event.key.code == sf::Keyboard::Space) {
        if (mOptionIndex == OptionNames::Create) {
            std::cout << "SERVER\n";
            return true;
        }
        if (mOptionIndex == OptionNames::Join) {
            std::cout << "SERVER\n";
            return true;
        }


    }
    if (event.key.code == sf::Keyboard::Up) {
        // Decrement and wrap-around
        if (mOptionIndex > 0)
            mOptionIndex--;

        updateActiveText();
    }


    if (event.key.code == sf::Keyboard::Down) {
        // Increment and wrap-around
        if (mOptionIndex < activeTextbuf.size() - 1)
            mOptionIndex++;

        updateActiveText();
    }

    return false;


}

void MenuState::ChangeState() {
    requestStackPop();
    requestStackPush(States::Join);

}




void MenuState::updateActiveText() {

    if (activeTextbuf.empty())
        return;

    // White all texts
    for (auto text: activeTextbuf)
        text->setColor(sf::Color::White);

    // Red the selected text
    activeTextbuf[mOptionIndex]->setColor(sf::Color::Yellow);
}




