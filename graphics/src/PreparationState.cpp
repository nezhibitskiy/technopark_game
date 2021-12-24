#include <iostream>
#include "PreparationState.h"


PreparationState::PreparationState(StateStack &stack, Context context) : State(stack, context),
                                                                         action(*context.window, *context.font) {


    context.texBack->loadFromFile("../resources/defback.png");
    context.Back = new sf::Sprite(*context.texBack);

    Text *Wait = new Text(*getContext().font, "Waiting room", 50);
    Wait->setPos(getContext().window->getSize().x / 2.0f, 0);
    Wait->setColor(sf::Color(255, 255, 255, 255));
    textbuf.push_back(Wait);

    Text *Choice = new Text(*getContext().font, "Choose team", 25);
    Choice->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 6);
    Choice->setColor(sf::Color(255, 255, 255, 150));
    textbuf.push_back(Choice);

    Text *Tips = new Text(*getContext().font, "if you READY -> press enter", 20);
    Tips->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y * 0.8);
    Tips->setColor(sf::Color(255, 255, 255, 150));
    textbuf.push_back(Tips);

    Text *Controller = new Text(*getContext().font, "UP - W\nDOWN - S\nLEFT - A\nRIGHT - D", 20);
    Controller->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y * 0.5);
    Controller->setColor(sf::Color(255, 255, 0, 200));
    textbuf.push_back(Controller);

    Text *Mouse = new Text(*getContext().font, "MOUSE-LEFT  Attack\nMOUSE-RIGHT  Put block", 20);
    Mouse->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y * 0.7);
    Mouse->setColor(sf::Color(255, 255, 0, 200));
    textbuf.push_back(Mouse);




}

void PreparationState::draw(std::queue<EventMessage> *eventQueue) {
    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);
    window.draw(*getContext().Back);


    eventManager.EventHandle(&eventQueue->front(), &action);  // если закинуть под проверку пустой очереди не воркает на клиенте
    action.Draw();


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

    if (event.key.code == sf::Keyboard::Num1) {
        BaseMessage choice(gameServer::server::ADD_CLIENT_TO_TEAM, 0, 1);
        request->push(choice);
    }


    if (event.key.code == sf::Keyboard::Num0) {
        // Increment and wrap-around
        BaseMessage choice(gameServer::server::ADD_CLIENT_TO_TEAM, 0, 0);
        request->push(choice);
    }

    return false;

}

void PreparationState::ChangeState() {
    requestStackPop();
    requestStackPush(States::Game);
}





