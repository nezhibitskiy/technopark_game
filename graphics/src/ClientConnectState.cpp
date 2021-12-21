#include <iostream>
#include <sstream>

#include "ClientConnectState.h"


ClientConnectState::ClientConnectState(StateStack &stack, DrawState::State::Context context) : State(stack, context) {
    Text *Join = new Text(*getContext().font, "Join the game", 50);
    Join->setPos(getContext().window->getSize().x / 2.0f, 0);
    textbuf.push_back(Join);

    Text *Tips = new Text(*getContext().font, "write your IP", 30);
    Tips->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 2.0f);
    textbuf.push_back(Tips);
    Text *TipsEnter = new Text(*getContext().font, "then click ENTER", 20);
    TipsEnter->setPos(getContext().window->getSize().x / 2.0f, getContext().window->getSize().y / 1.8f);
    textbuf.push_back(TipsEnter);
}

void ClientConnectState::draw(std::queue<EventMessage> *eventQueue) {

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

bool ClientConnectState::handleEvent(const sf::Event &event, std::queue<BaseMessage> *request) {

    if (event.key.code == sf::Keyboard::Return && event.type == sf::Event::KeyReleased) {
        if (!ipPlayer.empty() && isValid()) {
            auto *ip = new std::pair<std::string, std::string>;
            *ip = convertIP(ipPlayer);
            auto x = boost::asio::ip::address_v4::from_string(ip->first).to_uint();
            BaseMessage IpMessage(IpHandler::IP, 0, x, std::stol(ip->second));
            request->push(IpMessage);
            return true;
        }
        ipPlayer = "";
        return false;


    }


    if (event.type == sf::Event::TextEntered) {

        if (event.text.unicode > 45 && event.text.unicode < 60) {
            std::cout << event.text.unicode << std::endl;
            ipPlayer += static_cast<char>(event.text.unicode);

        } else if (event.text.unicode == 8) {
            ipPlayer.pop_back();
        }

    }
    return false;
}

void ClientConnectState::ChangeState() {

    requestStackPop();
    requestStackPush(States::Preparation);
}

std::pair<std::string, std::string> &ClientConnectState::convertIP(std::string &ip) {
    std::string address;
    std::string port;
    int temp;
    char ch;
    std::stringstream s(ip);
    while (s) {
        s >> temp >> ch;
        address += std::to_string(temp);
        if (ch == ':') {
            s >> port;
            break;
        } else address += ch;


    }
    auto *k = new std::pair<std::string, std::string>;
    k->first = address;
    k->second = port;
    return *k;

}

bool ClientConnectState::isValid() {

    return (3 == std::count(ipPlayer.begin(), ipPlayer.end(), '.') &&
            std::count(ipPlayer.begin(), ipPlayer.end(), ':') == 1);
}

