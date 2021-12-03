//
// Created by ivan_lemon on 11/15/21.
//

#ifndef FROMBOOK_MENUSTATE_H
#define FROMBOOK_MENUSTATE_H


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "State.h"
#include "message.h"
class MenuState : public DrawState::State {
public:
    MenuState(StateStack &stack, State::Context context);

    void draw(std::queue<EventMessage>* eventQueue) override;
    void updateActiveText();
    bool handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) override;
    void ChangeState() override;
private:
    std::vector<sf::Text> textbuf;
    sf::Font font;
};


#endif //FROMBOOK_MENUSTATE_H
