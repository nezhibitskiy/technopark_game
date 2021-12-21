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
        if(!ipPlayer.empty() && isValid()) {
            IPAdr *ip = convertIP(ipPlayer);

            BaseMessage IpMessage(IpHandler::IP, ((ip->ip[0] << 8) | ip->ip[1]),
                               ((ip->ip[2] << 8) | ip->ip[3]), ip->port);
            request->push(IpMessage);
            delete ip;
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

ClientConnectState::IPAdr *ClientConnectState::convertIP(std::string &ip) {
    char ch;
    std::stringstream s(ip);
    auto *ipAdr = new IPAdr;
    unsigned char i = 0;
    unsigned short tmp = 0;
    while (s) {
        s >> tmp >> ch;
        if (ch == '.') {
            ipAdr->ip[i] = tmp;
            i++;
        } else if (ch == ':') {
            ipAdr->ip[i] = tmp;
            i++;
            s >> ipAdr->port;
            break;
        }
    }
    return ipAdr;
}

bool ClientConnectState::isValid() {

    return (3 == std::count(ipPlayer.begin(), ipPlayer.end(), '.') && std::count(ipPlayer.begin(), ipPlayer.end(), ':') == 1);
}

