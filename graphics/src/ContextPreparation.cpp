#include "ContextPreparation.h"

ContextPreparation::ContextPreparation(sf::RenderWindow &mWindow, sf::Font &mFont) : mWindow(mWindow), mFont(mFont) {

}


void ContextPreparation::Draw() {
    for (auto text: textbuf) {
        text->draw(mWindow);
    }

}

void ContextPreparation::ShowTeams(unsigned short data) {

    int count = 0;
    int size = 0;

    while (data != 0) {
        if ((data & 3) >> 1) {
            count++;
        }
        if ((data | 3) >> 1) {
            size++;
        }
        data >>= 1;
    }



    if (textbuf.empty()) {
        textbuf.resize(size);
        for (int i = 0; i < size; ++i) {
            Text *Team = new Text(mFont, "Team " + std::to_string(i), 35);
            Team->setPos(mWindow.getSize().x * (i + 1) / (3.0f), mWindow.getSize().y / 4.0f);
            Team->setColor(sf::Color::White);
            textbuf[i] = Team;
        }
    }


}

void ContextPreparation::PlayerTeam(unsigned short id, unsigned short team) {


        for (int i = 0; i < textbuf.size(); ++i) {
            if (team == i) {
                switch (team) {
                    case 0:
                        textbuf[team]->setColor(sf::Color::Blue);
                        break;
                    case 1:
                        textbuf[team]->setColor(sf::Color::Red);
                        break;
                    default:
                        textbuf[team]->setColor(sf::Color::White);
                }
            } else textbuf[i]->setColor(sf::Color::White);

        }


}




