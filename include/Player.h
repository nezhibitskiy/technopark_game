//
// Created by ivan_lemon on 11/15/21.
//

#ifndef FROMBOOK_PLAYER_H
#define FROMBOOK_PLAYER_H


#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <SFML/Window/Event.hpp>

class Player {

public:
    enum Action {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Hit
    };

    Player();

    bool handleEvent(const sf::Event &event); /* + Элемент очереди message*/


private:
    std::map<sf::Keyboard::Key, Action> mKeysMap;

};


#endif //FROMBOOK_PLAYER_H
