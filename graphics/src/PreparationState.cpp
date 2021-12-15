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

    Text *ip = new Text(*getContext().font, ipPlayer, 30);
    ip->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 4.0f);
    ip->draw(*getContext().window);

    for (auto text: textbuf) {
        text->setColor(sf::Color::Red);
        text->draw(*getContext().window);
    }



}

bool PreparationState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request) {


    if (event.key.code == sf::Keyboard::Return && event.type == sf::Event::KeyReleased) {
        std::cout << "change state to Game \n";


        return true;

    }


    if (event.type == sf::Event::TextEntered) {

        if (event.text.unicode > 45 &&  event.text.unicode < 60) {
            std::cout << event.text.unicode<<std::endl;
            ipPlayer += static_cast<char>(event.text.unicode);

        }else if(event.text.unicode == 8 ){
            ipPlayer.pop_back();
        }
    }
    return false;

}

void PreparationState::ChangeState() {
    requestStackPop();
    requestStackPush(States::Game);
}

std::string &PreparationState::getIP() {
    return ipPlayer;
}

