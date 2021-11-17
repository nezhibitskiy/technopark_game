#ifndef FROMBOOK_STATESTACK_H
#define FROMBOOK_STATESTACK_H


#include <functional>
#include <map>
#include "State.h"

class StateStack  {
public:
    enum Action {
        Push,
        Pop,
        Clear,
    };


public:
    explicit StateStack(State::Context context);

    template<typename T>
    void registerState(States::ID stateID);

    void draw();

    void handleEvent(const sf::Event &event);

    void pushState(States::ID stateID);

    void popState();

    void clearStates();

    bool isEmpty() const;


private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();


    struct PendingChange {
        explicit PendingChange(Action action, States::ID stateID = States::None);
        Action action;
        States::ID stateID;
    };


    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;
    State::Context mContext;
    std::map<States::ID, std::function<State::Ptr()>> mStatesMap;
};


template<typename T>
void StateStack::registerState(States::ID stateID) {
    mStatesMap[stateID] = [this]() {
        return State::Ptr(new T(*this, mContext));
    };
}


#endif //FROMBOOK_STATESTACK_H
