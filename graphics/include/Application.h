#ifndef FROMBOOK_APPLICATION_H
#define FROMBOOK_APPLICATION_H


#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "InputPlayer.h"
#include "StateStack.h"


class Application {
public:
    explicit Application();

    void Run();
    void changeState();
    bool processInput(std::queue<BaseMessage>* request);
    void render(std::queue<EventMessage>* eventQueue);

private:


    void registerStates();


private:

    sf::RenderWindow mWindow;
    InputPlayer mPlayer;
    StateStack mStateStack;

};


#endif //FROMBOOK_APPLICATION_H
