#include <sstream>
#include "Application.h"
#include "MenuState.h"
#include "GameState.h"
#include "PreparationState.h"


Application::Application()
        : mWindow(sf::VideoMode(640,480), "BLOCK WARS", sf::Style::Close)
        , mPlayer(mWindow)
        , mStateStack(DrawState::State::Context(mWindow, mPlayer,"../resources/PEPSI_pl.ttf"))

{
    mWindow.setKeyRepeatEnabled(false);
    registerStates();
    mStateStack.pushState(States::Menu);   /// начальная регулировка состояния
}

void Application::Run()
{

    while (mWindow.isOpen())
    {

        sf::Event event;
        while (mWindow.pollEvent(event)) {
            processInput(event);
        }

            if (mStateStack.isEmpty())
                mWindow.close();

        std::queue<EventMessage> eventQueue;
            render(&eventQueue);
    }
}

void  Application::processInput(sf::Event event) // внешний while
{

        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();

}


void Application::render(std::queue<EventMessage>* eventQueue)
{
    mWindow.clear();

    mStateStack.draw(eventQueue);

    mWindow.display();
}


void Application::registerStates()
{
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<PreparationState>(States::Preparation);


}
