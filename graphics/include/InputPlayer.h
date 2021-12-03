//
// Created by ivan_lemon on 11/15/21.
//

#ifndef FROMBOOK_PLAYER_H
#define FROMBOOK_PLAYER_H


#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <SFML/Window/Event.hpp>
#include "message.h"
#include "PlayerHandlers.h"
#include "Unit.h"


class InputPlayer {

public:
    enum Action {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Hit
    };

    InputPlayer(sf::RenderWindow &mWindow);

    void handleEvent(const sf::Event &event,std::queue<BaseMessage> &request); /* + Элемент очереди message*/


private:
    sf::RenderWindow &mWindow;
    std::map<sf::Keyboard::Key, Action> mKeysMap;

};


#endif //FROMBOOK_PLAYER_H
