#include "State.h"
#include "StateStack.h"

State::Context::Context(sf::RenderWindow &window, Player &player, std::string _fontfile = "textures/PEPSI_pl.ttf")
        : window(&window), player(&player), fontfile(_fontfile) {
    sf::Font *textfont = new sf::Font;
    if(!textfont->loadFromFile(fontfile)){
        throw std::runtime_error("Failed to load " + fontfile);
    }
    font = textfont;
}



void State::requestStackPush(States::ID stateID) {
    mStack->pushState(stateID);
}

void State::requestStackPop() {
    mStack->popState();
}

void State::requestStateClear() {
    mStack->clearStates();
}

State::Context State::getContext() const {
    return mContext;
}
