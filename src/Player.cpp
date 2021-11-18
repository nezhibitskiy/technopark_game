#include <iostream>
#include "Player.h"

Player::Player()
{

    mKeysMap[sf::Keyboard::A] = MoveLeft;
    mKeysMap[sf::Keyboard::D] = MoveRight;
    mKeysMap[sf::Keyboard::W] = MoveUp;
    mKeysMap[sf::Keyboard::S] = MoveDown;
    // mKeysMap -- Hit;

}

void Player::handleEvent(const sf::Event& event) /* + Элемент очереди message*/
{
    if (event.type == sf::Event::KeyReleased)
    {
        auto found = mKeysMap.find(event.key.code);

        if (found != mKeysMap.end() ){

            if(found->second == MoveLeft)
                std::cout << "Left"<< std::endl;
            if(found->second == MoveRight)
                std::cout << "Right"<< std::endl;
            if(found->second == MoveUp)
                std::cout << "Up"<< std::endl;
            if(found->second == MoveDown)
                std::cout << "Down"<< std::endl;

            // добавить в request  queue

        }

    } // клики мыши аналогично else if(){}
}