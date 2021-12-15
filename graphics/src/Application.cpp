#include <sstream>
#include "Application.h"
#include "MenuState.h"
#include "GameState.h"
#include "PreparationState.h"
#include "EndState.h"
#include "ClientConnectState.h"


Application::Application()
        : mWindow(sf::VideoMode(600,650), "BLOCK WARS", sf::Style::Close), mPlayer(mWindow),
          mStateStack(DrawState::State::Context(mWindow, mPlayer, "../resources/PEPSI_pl.ttf")) {
    mWindow.setKeyRepeatEnabled(false);
    registerStates();
    mStateStack.pushState(States::Menu);
    mStateStack.applyPendingChanges();  /// начальная регулировка состояния
}

void Application::Run() {

    sf::Event event;
    mWindow.pollEvent(event);
    // processInput(*event);


    if (mStateStack.isEmpty())
        mWindow.close();


    std::queue<EventMessage> eventQueue;
    render(&eventQueue);

}

bool Application::processInput(std::queue<BaseMessage>* request) // внешний while
{
    sf::Event event;
    mWindow.pollEvent(event);
    if (event.type == sf::Event::Closed){

        mWindow.close();
    }


    return mStateStack.handleEvent(event,request);


}

void Application::changeState() // внешний while
{
    mStateStack.ChangeState();
}


void Application::render(std::queue<EventMessage> *eventQueue) {
    mWindow.clear();

    mStateStack.draw(eventQueue);

    mWindow.display();
}


void Application::registerStates() {

    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<ClientConnectState>(States::Join);
    mStateStack.registerState<PreparationState>(States::Preparation);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<EndState>(States::End);

}
