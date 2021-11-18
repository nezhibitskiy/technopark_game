#ifndef FROMBOOK_APPLICATION_H
#define FROMBOOK_APPLICATION_H


#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Player.h"
#include "StateStack.h"

class Application {
public:
    explicit Application();

    void Run();


private:
    void processInput();

    void render();

    void registerStates();


private:

    sf::RenderWindow mWindow;
    Player mPlayer;
    StateStack mStateStack;

};


#endif //FROMBOOK_APPLICATION_H
