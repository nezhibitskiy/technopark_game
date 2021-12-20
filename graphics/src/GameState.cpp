#include <cmath>
#include "GameState.h"

GameState::GameState(StateStack &stack, Context context) : State(stack, context), mPlayer(*context.player),
                                                           map(*context.window) {


}


void GameState::draw(std::queue<EventMessage> *eventQueue) {

    if (!eventQueue->empty()) {
        eventManager.EventHandle(&eventQueue->front(), &map);
    }
    getContext().window->clear(sf::Color::Black);
    map.DrawBack();
    map.Draw();
    Text *time = new Text(*getContext().font, std::to_string(floor(clock.getElapsedTime().asSeconds())), 30);
    time->setPos(getContext().window->getSize().x / 2, getContext().window->getSize().y - time->getSize());
    time->draw(*getContext().window);
}

bool GameState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request) {


    mPlayer.handleEvent(event, *request);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {

        std::cout << "change state to Menu \n";
        return true;


    }

}

void GameState::ChangeState() {

    requestStackPush(States::End);
}
