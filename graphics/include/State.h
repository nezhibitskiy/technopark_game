#ifndef FROMBOOK_STATE_H
#define FROMBOOK_STATE_H

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "StateIdentifiers.h"
#include <queue>
#include "message.h"
class StateStack;

class InputPlayer;
namespace DrawState {
    class State {

    public:

        typedef std::unique_ptr<State> Ptr;

        struct Context {
            Context(sf::RenderWindow &window, InputPlayer &player, std::string fontfile);

            std::string fontfile;
            sf::Font *font;
            sf::RenderWindow *window;
            InputPlayer *player;
        };


        State(StateStack &stack, Context context) : mStack(&stack), mContext(context) {}

        virtual ~State() = default;

        virtual void draw(std::queue<EventMessage>* eventQueue) = 0;


        virtual bool handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) = 0;
        virtual void ChangeState() = 0;

    protected:
        void requestStackPush(States::ID stateID);

        void requestStackPop();

        void requestStateClear();

        Context getContext() const;


    private:
        StateStack *mStack;
        Context mContext;
    };

}
#endif //FROMBOOK_STATE_H
