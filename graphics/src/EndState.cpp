#include <iostream>
#include "EndState.h"

EndState::EndState(StateStack &stack, DrawState::State::Context context) : State(stack, context) {
    Text *GameOver = new Text(*getContext().font, "GAME OVER", 50);
    GameOver->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(GameOver);
    Text *Winner = new Text(*getContext().font, "winner team", 30);
    Winner->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.0f);
    textbuf.push_back(Winner);
}

void EndState::draw(std::queue<EventMessage> *eventQueue) {



    EventMessage endMessage = eventQueue->front();
    Text *IdTeam;
    if (endMessage.getType() == EventMessage::WIN_TEAM) {
        IdTeam = new Text(*getContext().font, std::to_string(endMessage.getID()), 40);
        IdTeam->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 1.8f);
        IdTeam->draw(*getContext().window);
    }


    for (auto text: textbuf) {
        text->setColor(sf::Color::Red);
        text->draw(*getContext().window);
    }
}

bool EndState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request = nullptr) {
    if (event.key.code == sf::Keyboard::Escape && event.type == sf::Event::KeyReleased) {
        std::cout << "CLOSE window \n";
        return true;

    } else return false;
}

void EndState::ChangeState() {
    requestStackPop();
    requestStateClear();

}
