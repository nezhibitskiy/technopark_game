
#include "InputPlayer.h"
#define SIZE_PIXELS 30

InputPlayer::InputPlayer(sf::RenderWindow &_mWindow) : mWindow(_mWindow) {


    mKeysMap[sf::Keyboard::A] = MoveLeft;
    mKeysMap[sf::Keyboard::D] = MoveRight;
    mKeysMap[sf::Keyboard::W] = MoveUp;
    mKeysMap[sf::Keyboard::S] = MoveDown;

    mMouseMap[sf::Mouse::Left] = Hit;
    mMouseMap[sf::Mouse::Right] = Build;

}

void InputPlayer::handleEvent(const sf::Event &event, std::queue<BaseMessage> &request)
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

        auto found = mMouseMap.find(event.mouseButton.button);
        if (found->second == Build) {

            BaseMessage putBlock(PutBlockHandler::PUT_BLOCK, 0, sf::Mouse::getPosition(mWindow).x / SIZE_PIXELS,
                                 sf::Mouse::getPosition(mWindow).y / SIZE_PIXELS);
            request.push(putBlock);
        }
        if (found->second == Hit) {
            BaseMessage attack(AttackHandler::ATTACK, 0, sf::Mouse::getPosition(mWindow).x / SIZE_PIXELS,
                               sf::Mouse::getPosition(mWindow).y / SIZE_PIXELS);
            request.push(attack);

        }
    }
}
