#include <iostream>
#include "PreparationState.h"

PreparationState::PreparationState(StateStack &stack, Context context) : State(stack, context) {

    sf::Font &font = *getContext().font;
    sf::Text GameText("Waiting room", font, 50);
    sf::FloatRect textRect = GameText.getLocalBounds();// set to center
    GameText.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    GameText.setPosition(sf::Vector2f(640 / 2.0f, 0));
    textbuf.push_back(GameText);

    sf::Text text("press space", font, 30);
    textRect = text.getLocalBounds();// set to center
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.left + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(640 / 2.0f, 480 / 2.0f));
    textbuf.push_back(text);
}

void PreparationState::draw(std::queue<EventMessage>* eventQueue) {

    sf::RenderWindow &window = *getContext().window;
    window.clear(sf::Color::Black);

    for (sf::Text &text: textbuf) {
        text.setColor(sf::Color::White);
        window.draw(text);
    }
}

bool PreparationState::handleEvent(const sf::Event &event) {


    if (event.key.code == sf::Keyboard::Space && event.type == sf::Event::KeyReleased) {
        std::cout << "change state to Game \n";
        requestStackPop();
        requestStackPush(States::Game);

    } else return false;

    return true;
}

