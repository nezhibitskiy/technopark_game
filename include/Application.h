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


private:
    void processInput(sf::Event event);

    void render(std::queue<EventMessage>* eventQueue);

    void registerStates();


private:

    sf::RenderWindow mWindow;
    InputPlayer mPlayer;
    StateStack mStateStack;

};


#endif //FROMBOOK_APPLICATION_H
