#include <iostream>
#include "PreparationState.h"


PreparationState::PreparationState(StateStack &stack, Context context) : State(stack, context) {


    Text *Wait = new Text(*getContext().font, "Waiting room", 50);
    Wait->setPos(getContext().window->getSize().x / 2.0f, 0);
    Wait->setColor(sf::Color::Red);
    textbuf.push_back(Wait);

    Text *Tips = new Text(*getContext().font, "if you READY -> press enter", 20);
    Tips->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y * 0.8);
    Tips->setColor(sf::Color::Red);
    textbuf.push_back(Tips);

    Text *Controller = new Text(*getContext().font, "UP - W\nDOWN - S\nLEFT - A\nRIGHT - D", 20);
    Controller->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y * 0.5);
    Controller->setColor(sf::Color::Yellow);
    textbuf.push_back(Controller);

    Text *Mouse = new Text(*getContext().font, "MOUSE-LEFT  Attack\nMOUSE-RIGHT  Put block", 20);
    Mouse->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y * 0.7);
    Mouse->setColor(sf::Color::Cyan);
    textbuf.push_back(Mouse);




}

void PreparationState::draw(std::queue<EventMessage> *eventQueue) {

    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);






    Text *ip = new Text(*getContext().font, ipPlayer, 30);
    ip->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 4.0f);
    ip->draw(*getContext().window);

    for (auto text: textbuf) {

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

void PreparationState::DrawTeams() {

}


