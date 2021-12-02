#include "StateStack.h"

#include <cassert>



StateStack::StateStack(DrawState::State::Context context)
        : mStack(), mPendingList(), mContext(context), mStatesMap() {
}


void StateStack::draw(std::queue<EventMessage>* eventQueue) {

    for (DrawState::State::Ptr &state :  mStack) state->draw(eventQueue);

}

void StateStack::handleEvent(const sf::Event &event) {

    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
        if (!(*itr)->handleEvent(event))
            break;
    }

    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID) {
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState() {
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates() {
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const {
    return mStack.empty();
}

DrawState::State::Ptr StateStack::createState(States::ID stateID) {
    auto found = mStatesMap.find(stateID);
    assert(found != mStatesMap.end());

    return found->second();
}

void StateStack::applyPendingChanges() {
    for(PendingChange change : mPendingList) {
        switch (change.action) {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;

            case Pop:
                mStack.pop_back();
                break;

            case Clear:
                mStack.clear();
                break;
        }
    }

    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
        : action(action), stateID(stateID) {
}
