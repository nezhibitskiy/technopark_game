#ifndef FROMBOOK_STATE_H
#define FROMBOOK_STATE_H

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "StateIdentifiers.h"

class StateStack;

class Player;

class State {

public:

    typedef std::unique_ptr<State> Ptr;

    struct Context {
        Context(sf::RenderWindow &window, Player &player, std::string fontfile);
        std::string fontfile;
        sf::Font *font;
        sf::RenderWindow *window;
        Player *player;
    };


    State(StateStack &stack, Context context): mStack(&stack), mContext(context) {}
    virtual ~State() = default;

    virtual void draw() = 0;



    virtual bool handleEvent(const sf::Event &event) = 0;


protected:
    void requestStackPush(States::ID stateID);

    void requestStackPop();

    void requestStateClear();

    Context getContext() const;


private:
    StateStack *mStack;
    Context mContext;
};


#endif //FROMBOOK_STATE_H
