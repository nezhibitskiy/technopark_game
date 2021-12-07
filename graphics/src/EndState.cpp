#include <iostream>
#include "EndState.h"

EndState::EndState(StateStack &stack, DrawState::State::Context context) : State(stack, context) {
    Text *GameOver = new Text(*getContext().font, "GAME OVER", 50);
    GameOver->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(GameOver);
    Text *Winner = new Text(*getContext().font, "winner: ", 30);
    Winner->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.0f);
    textbuf.push_back(Winner);
}

void EndState::draw(std::queue<EventMessage> *eventQueue) {
    /*while(!eventQueue->empty()){
        eventManager.EventHandle(&eventQueue->front(),nullptr);
        eventQueue->pop();
    }*/
    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);

    for (auto text: textbuf) {
        text->setColor();
        text->draw(*getContext().window);
    }
}

bool EndState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request) {
    if (event.key.code == sf::Keyboard::Escape && event.type == sf::Event::KeyReleased) {
        std::cout << "CLOSE window \n";
        return true;

    } else return false;
}

void EndState::ChangeState() {
    requestStackPop();
    requestStateClear();
}
