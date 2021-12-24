#include <SFML/Graphics/Sprite.hpp>
#include "State.h"
#include "StateStack.h"

DrawState::State::Context::Context(sf::RenderWindow &window, InputPlayer &player, std::string _fontfile = "PEPSI_pl.ttf",std::string _backfile = "../resources/back.png")
        : window(&window), player(&player), fontfile(_fontfile), backfile(_backfile) {
    sf::Font *textfont = new sf::Font;
    sf::Texture *_texBack = new sf::Texture;
    if(!_texBack->loadFromFile(backfile)){
        throw std::runtime_error("Failed to load " + backfile);
    }
    if(!textfont->loadFromFile(fontfile)){
        throw std::runtime_error("Failed to load " + fontfile);
    }
    texBack = _texBack;
    font = textfont;
    Back = new sf::Sprite(*texBack);
    Back->setColor(sf::Color(255, 255, 255, 150));

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
