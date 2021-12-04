
#include <iostream>
#include "GameState.h"

GameState::GameState(StateStack &stack, Context context) : State(stack, context), mPlayer(*context.player),map(*context.window) {

        /*Message m(DrawMapHandler::CREATE_MAP, 0, 0, 0, 0);
        mapHandler->Handle(m, &map);*/

}


void GameState::draw(std::queue<EventMessage>* eventQueue) {


        eventManager.EventHandle(&eventQueue->front(),&map);




    map.DrawBack();
    map.Draw();

}

bool GameState::handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) {

    mPlayer.handleEvent(event, *request);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        std::cout << "change state to Menu \n";
        return true;
        std::cout << "check KEY\n";

    }

}

void GameState::ChangeState() {
    requestStackPop();
    requestStateClear();
}
