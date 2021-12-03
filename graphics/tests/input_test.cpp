#include <gtest/gtest.h>
#include <cstring>
#include "GameState.h"
#include "MenuState.h"

#include "Application.h"
#include "Player.h"
#include "StateIdentifiers.h"
#include "StateStack.h"
#include "State.h"
#include "GameState.h"




class FakeMenu {
public:
    FakeMenu() : context(w, mPlayer,"../resources/PEPSI_pl.ttf"), mStack(context), mState(mStack, context) {}

    sf::RenderWindow w;
    Player mPlayer;
    State::Context context;
    StateStack mStack;
    MenuState mState;
};

class FakeGame {
public:
    FakeGame() : context(w, mPlayer,"../resources/PEPSI_pl.ttf"), mStack(context), mState(mStack, context) {}

    sf::RenderWindow w;
    Player mPlayer;
    State::Context context;
    StateStack mStack;
    GameState mState;
};


TEST(TEST_INPUT_MENU_STATE, HandleInput) {

    FakeMenu fmenu;
    sf::Event event;
    event.type = sf::Event::KeyReleased;
    event.key.code = sf::Keyboard::Space;
    EXPECT_TRUE(fmenu.mState.handleEvent(event));
    event.key.code = sf::Keyboard::W;
    EXPECT_FALSE(fmenu.mState.handleEvent(event));
    event.key.code = sf::Keyboard::A;
    EXPECT_FALSE(fmenu.mState.handleEvent(event));
    event.key.code = sf::Keyboard::D;
    EXPECT_FALSE(fmenu.mState.handleEvent(event));
    event.key.code = sf::Keyboard::S;
    EXPECT_FALSE(fmenu.mState.handleEvent(event));
}


TEST(TEST_INPUT_GAME_STATE, HandleInput) {

    FakeGame fgame;
    sf::Event event;
    event.type = sf::Event::KeyReleased;

    event.key.code = sf::Keyboard::W;
    EXPECT_TRUE(fgame.mState.handleEvent(event));

    event.key.code = sf::Keyboard::A;
    EXPECT_TRUE(fgame.mState.handleEvent(event));

    event.key.code = sf::Keyboard::D;
    EXPECT_TRUE(fgame.mState.handleEvent(event));

    event.key.code = sf::Keyboard::S;
    EXPECT_TRUE(fgame.mState.handleEvent(event));

    

}

