
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




class FakeMenuFont {
public:
    FakeMenuFont() : context(w, mPlayer,"" ), mStack(context), mState(mStack, context) {}

    sf::RenderWindow w;
    Player mPlayer;
    State::Context context;
    StateStack mStack;
    MenuState mState;
};

class FakeGameFont {
public:
    FakeGameFont() : context(w, mPlayer,""), mStack(context), mState(mStack, context) {}

    sf::RenderWindow w;
    Player mPlayer;
    State::Context context;
    StateStack mStack;
    GameState mState;
};

class FakeBlock{
public:
    FakeBlock() : fBlock(0,""){}
    Block fBlock;
};

class FakeUnit{
public:
    FakeUnit() : fUnit(0,""){}
    Unit fUnit;
};

TEST(TEST_RESOURCE_FONT_MENU, FontCheck) {

    try {
        FakeMenuFont fmenu;
    }
    catch(std::runtime_error){

    }

}


TEST(TEST_RESOURCE_FONT_GAME, FontCheck) {

    try {
        FakeGameFont fgame;
    }
    catch(std::runtime_error){

    }

}


TEST(TEST_RESOURCE_BLOCK, TextureCheck) {

    try {
        FakeBlock fBlock;
    }
    catch(std::runtime_error){

    }

}

TEST(TEST_RESOURCE_UNIT, TextureCheck) {

    try {
        FakeUnit fUnit;
    }
    catch(std::runtime_error){

    }

}