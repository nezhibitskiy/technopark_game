#ifndef FROMBOOK_GAMESTATE_H
#define FROMBOOK_GAMESTATE_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <queue>
#include "State.h"
#include "InputPlayer.h"
#include "Map.h"
#include "DrawHandlers.h"
#include "EventManager.h"
#include "message.h"

class GameState : public DrawState::State {
public:
    GameState(StateStack &stack, Context context);

    void draw(std::queue<EventMessage>* eventQueue) override;
    bool handleEvent(const sf::Event &event,std::queue<BaseMessage>* request) override;
    void ChangeState() override;

private:
    InputPlayer mPlayer;
    DrawMap map;
    std::queue<BaseMessage> request;
   // EventMessage event;
    Draw::EventManager eventManager;
};


#endif //FROMBOOK_GAMESTATE_H
