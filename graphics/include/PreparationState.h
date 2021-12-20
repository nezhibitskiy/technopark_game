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
#include "message.h"
#include "Text.h"
#include "TextField.h"

class PreparationState : public DrawState::State {
public:
    PreparationState(StateStack &stack, Context context);
    ~PreparationState(){
        for(auto text : textbuf)
            delete text;
    }
    void draw(std::queue<EventMessage>* eventQueue) override;
    bool handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) override;
    void ChangeState() override;
    std::string& getIP();
    void DrawTeams(/*параметры каждой команды из event*/);

private:
    std::vector<Text*> textbuf;
    sf::Font font;
    std::string ipPlayer;

};


#endif //FROMBOOK_PREPARATIONSTATE_H
