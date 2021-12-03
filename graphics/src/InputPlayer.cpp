#include <iostream>
#include "InputPlayer.h"



InputPlayer::InputPlayer(sf::RenderWindow &_mWindow) : mWindow(_mWindow) {


    mKeysMap[sf::Keyboard::A] = MoveLeft;
    mKeysMap[sf::Keyboard::D] = MoveRight;
    mKeysMap[sf::Keyboard::W] = MoveUp;
    mKeysMap[sf::Keyboard::S] = MoveDown;


}

void InputPlayer::handleEvent(const sf::Event &event,std::queue<BaseMessage> &request) /* + Элемент очереди message*/
{

    if (event.type == sf::Event::KeyReleased) {
        auto found = mKeysMap.find(event.key.code);

        if (found != mKeysMap.end()) {

            if (found->second == MoveLeft) {

                BaseMessage moveLeft(MoveHandler::MOVE_LEFT, 0);
                request.push(moveLeft);

            }
            if (found->second == MoveRight) {

                BaseMessage moveRight(MoveHandler::MOVE_RIGHT, 0);
                request.push(moveRight);

            }

            if (found->second == MoveUp) {

                BaseMessage moveUp(MoveHandler::MOVE_UP, 0);
                request.push(moveUp);

            }

            if (found->second == MoveDown) {

                BaseMessage moveDown(MoveHandler::MOVE_DOWN, 0);
                request.push(moveDown);

            }
        }

    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.key.code == sf::Mouse::Left) {

            BaseMessage putBlock(PutBlockHandler::PUT_BLOCK, 0, sf::Mouse::getPosition(mWindow).x/40,
                                      sf::Mouse::getPosition(mWindow).y/ 40);
            request.push(putBlock);
        }
        if (event.key.code == sf::Mouse::Right) {
            BaseMessage attack(AttackHandler::ATTACK, 0,sf::Mouse::getPosition(mWindow).x/40,
                                    sf::Mouse::getPosition(mWindow).y/ 40);
            request.push(attack);

        }
    }
}
