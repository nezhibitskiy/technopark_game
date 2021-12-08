#include <iostream>
#include "PreparationState.h"


PreparationState::PreparationState(StateStack &stack, Context context) : State(stack, context) {


    Text *Wait = new Text(*getContext().font, "Waiting room", 50);
    Wait->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(Wait);
    Text *Tips = new Text(*getContext().font, "press enter", 30);
    Tips->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.0f);
    textbuf.push_back(Tips);


}

void PreparationState::draw(std::queue<EventMessage> *eventQueue) {

    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);

    for (auto text: textbuf) {
        text->setColor(sf::Color::Red);
        text->draw(*getContext().window);
    }
}

bool PreparationState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request = nullptr) {


    if (event.key.code == sf::Keyboard::Return && event.type == sf::Event::KeyReleased) {
        std::cout << "change state to Game \n";
        return true;

    } else return false;


}

void PreparationState::ChangeState() {
    requestStackPop();
    requestStackPush(States::Game);
}

