#include <iostream>
#include "EndState.h"

EndState::EndState(StateStack &stack, DrawState::State::Context context) : State(stack, context) {
    Text *GameOver = new Text(*getContext().font, "GAME OVER", 50);
    GameOver->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(GameOver);

}

void EndState::draw(std::queue<EventMessage> *eventQueue) {

    sf::RenderWindow &window = *getContext().window;
    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());
    window.draw(backgroundShape);

    Text *IdTeam = nullptr;
    EventMessage endMessage = eventQueue->front();
    if( endMessage.getType() == EventMessage::WIN_TEAM) {

        if (endMessage.getID() == 65535) {
            IdTeam = new Text(*getContext().font, " DRAW ", 40);
        } else {
            IdTeam = new Text(*getContext().font, "winner -  TEAM "+std::to_string(endMessage.getID()), 40);

        }
        IdTeam->setPos(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        IdTeam->draw(window);

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
