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


    void applyPendingChanges();

public:
    explicit StateStack(DrawState::State::Context context);

    template<typename T>
    void registerState(States::ID stateID);

    void draw(std::queue<EventMessage>* eventQueue);

    bool handleEvent(const sf::Event &event,std::queue<BaseMessage>* request);

    void ChangeState();

    void pushState(States::ID stateID);

    void popState();

    void clearStates();

    bool isEmpty() const;


private:
    DrawState::State::Ptr createState(States::ID stateID);


    struct PendingChange {
        explicit PendingChange(Action action, States::ID stateID = States::None);
        Action action;
        States::ID stateID;
    };


    std::vector<DrawState::State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;
    DrawState::State::Context mContext;
    std::map<States::ID, std::function<DrawState::State::Ptr()>> mStatesMap;
};


template<typename T>
void StateStack::registerState(States::ID stateID) {
    mStatesMap[stateID] = [this]() {
        return DrawState::State::Ptr(new T(*this, mContext));
    };
}


#endif //FROMBOOK_STATESTACK_H
