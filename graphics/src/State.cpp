#include "State.h"
#include "StateStack.h"

DrawState::State::Context::Context(sf::RenderWindow &window, InputPlayer &player, std::string _fontfile = "PEPSI_pl.ttf")
        : window(&window), player(&player), fontfile(_fontfile) {
    sf::Font *textfont = new sf::Font;
    if(!textfont->loadFromFile(fontfile)){
        throw std::runtime_error("Failed to load " + fontfile);
    }
    font = textfont;
}



void DrawState::State::requestStackPush(States::ID stateID) {
    mStack->pushState(stateID);
}

void DrawState::State::requestStackPop() {
    mStack->popState();
}

void DrawState::State::requestStateClear() {
    mStack->clearStates();
}

DrawState::State::Context DrawState::State::getContext() const {
    return mContext;
}
