#include <iostream>
#include "EndState.h"

EndState::EndState(StateStack &stack, DrawState::State::Context context) : State(stack, context) {
    Text *GameOver = new Text(*getContext().font, "GAME OVER", 50);
    GameOver->setPos(getContext().window->getSize().x / 2.0f, 0);
    GameOver->setColor(sf::Color::Red);
    textbuf.push_back(GameOver);

    Text *Tips = new Text(*getContext().font, "click ESC for CLOSE", 20);
    Tips->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 1.5f);
    Tips->setColor(sf::Color::Red);
    textbuf.push_back(Tips);

}

void EndState::draw(std::queue<EventMessage> *eventQueue) {

    sf::RenderWindow &window = *getContext().window;
    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());
    window.draw(backgroundShape);

    Text *IdTeam;

    std::string text;
    std::string kills = "player kills - 0";
    EventMessage endMessage = eventQueue->front();




    if (endMessage.getType() == EventMessage::WIN_TEAM) {
        text = "winner -  TEAM " + std::to_string(endMessage.getID());

    } else if (endMessage.getType() == EventMessage::DRAW_GAME) {
        text = " DRAW ";
    }
    IdTeam = new Text(*getContext().font, text, 40);
    IdTeam->setPos(window.getSize().x / 2.0f, window.getSize().y / 3.0f);
    IdTeam->draw(window);


    for (auto text: textbuf) {
        text->draw(window);
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
