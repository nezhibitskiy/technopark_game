#ifndef PROJECT_TP_ENDSTATE_H
#define PROJECT_TP_ENDSTATE_H


#include "State.h"
#include "Text.h"
#include "EventManager.h"

class EndState : public DrawState::State {
public:
    EndState(StateStack &stack, Context context);
    ~EndState(){
        for(auto text : textbuf)
            delete text;
    }
    void draw(std::queue<EventMessage>* eventQueue) override;
    bool handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) override;
    void ChangeState() override;
private:
    std::vector<Text*> textbuf;
    sf::Font font;
    Draw::EventManager eventManager;


};


#endif //PROJECT_TP_ENDSTATE_H
