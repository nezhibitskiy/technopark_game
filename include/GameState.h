#ifndef FROMBOOK_GAMESTATE_H
#define FROMBOOK_GAMESTATE_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "State.h"
#include "Player.h"
#include "Map.h"

class GameState : public State {
public:
    GameState(StateStack &stack, Context context);

    void draw() override;
    bool handleEvent(const sf::Event &event) override;


private:
    Player mPlayer;
    std::vector<sf::Text> textbuf;
    sf::Font font;
    Map map;
};


#endif //FROMBOOK_GAMESTATE_H
