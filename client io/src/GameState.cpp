
#include <iostream>
#include "GameState.h"

GameState::GameState(StateStack &stack, Context context) : State(stack, context), mPlayer(*context.player),map(*context.window) {
    font.loadFromFile("PEPSI_pl.ttf");

    sf::Text GameText("Game", font, 50);
    sf::FloatRect textRect = GameText.getLocalBounds();// set to center
    GameText.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    GameText.setPosition(sf::Vector2f(640 / 2.0f, 0));
    textbuf.push_back(GameText);

    sf::Text text("press ESC", font, 30);
    textRect = text.getLocalBounds();// set to center
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.left + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(640 / 2.0f, 480 / 2.0f));
    textbuf.push_back(text);
}


void GameState::draw() {
    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Red);

    for(sf::Text& text : textbuf) {
        text.setColor(sf::Color::White);
        window.draw(text);
    }

    map.Draw();
}

bool GameState::handleEvent(const sf::Event &event) {

    mPlayer.handleEvent(event); // control player
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        std::cout << "change state to Menu \n";
        requestStackPop();
        requestStackPush(States::Menu);
    }

    return true;
}