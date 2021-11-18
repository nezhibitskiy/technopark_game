
#include <iostream>
#include "GameState.h"

GameState::GameState(StateStack &stack, Context context) : State(stack, context), mPlayer(*context.player),
                                                           map(*context.window) {


}


void GameState::draw() {
    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Red);

    for (sf::Text &text: textbuf) {
        text.setColor(sf::Color::White);
        window.draw(text);
    }

    map.Draw();
}

bool GameState::handleEvent(const sf::Event &event) {


    // control player
    if (!mPlayer.handleEvent(event)) {

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            std::cout << "change state to Menu \n";
            requestStackPop();
            requestStackPush(States::Menu);
            std::cout << "check KEY\n";
            return true;
        }
        return false;
    }

    return true;

}