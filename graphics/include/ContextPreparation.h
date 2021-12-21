//
// Created by ivan_lemon on 12/20/21.
//

#ifndef PROJECT_TP_CONTEXTPREPARATION_H
#define PROJECT_TP_CONTEXTPREPARATION_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Text.h"
#include "Unit.h"

class ContextPreparation {
public:
    explicit ContextPreparation(sf::RenderWindow &mWindow, sf::Font &mFont);
    ~ContextPreparation() = default;
    void Draw();
    void ShowTeams(unsigned short data);
    void PlayerTeam(unsigned short id,unsigned short team);
private:
    sf::Font &mFont;
    sf::RenderWindow &mWindow;
    std::vector<Text*> textbuf;

};


#endif //PROJECT_TP_CONTEXTPREPARATION_H
