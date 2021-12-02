//
// Created by ivan_lemon on 11/16/21.
//

#ifndef FROMBOOK_PREPARATIONSTATE_H
#define FROMBOOK_PREPARATIONSTATE_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "State.h"
#include "InputPlayer.h"
#include "../ilyas/project/include/message.h"
class PreparationState : public DrawState::State {
public:
    PreparationState(StateStack &stack, Context context);
    void draw(std::queue<EventMessage>* eventQueue) override;
    bool handleEvent(const sf::Event &event) override;
private:
    std::vector<sf::Text> textbuf;
    sf::Font font;


};


#endif //FROMBOOK_PREPARATIONSTATE_H
