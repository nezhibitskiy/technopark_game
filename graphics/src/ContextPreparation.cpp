#include "ContextPreparation.h"

ContextPreparation::ContextPreparation(sf::RenderWindow &mWindow, sf::Font &mFont) : mWindow(mWindow), mFont(mFont) {

}


void ContextPreparation::Draw() {

    for (auto i : textbuf) {
        i.second->draw(mWindow);
    }
    /*if(Full){
        Full->draw(mWindow);
    }*/

}

void ContextPreparation::ShowTeams(unsigned short data) {

    /*int count = data;
    int size = 0;

    while (count != 0) {
        if ((count | 3) >> 1) {
            size++;
        }
        count >>= 1;
    }


    bool *teams = new bool[size];

    if (data / 2 == 0) {
        teams[0] = false;
    } else teams[0] = true;

    data %= 2;
    if (data % 2 == 0) {
        teams[1] = false;
    } else teams[1] = true;


    if (textbuf.empty()) {
        textbuf.resize(size);
        for (int i = 0; i < size; ++i) {
            Text *Team = new Text(mFont, "Team " + std::to_string(i), 35);
            Team->setPos(mWindow.getSize().x * (i + 1) / (3.0f), mWindow.getSize().y / 4.0f);
            textbuf[i] = Team;
        }
    }


*/
    //textbuf.clear();
    for (unsigned short i = 0; i < 2; i++) {
        if ((data >> i) & 0b1) {

            Text *Team = new Text(mFont, "Team " + std::to_string(i), 35);
            Team->setPos(mWindow.getSize().x * (i + 1) / (3.0f), mWindow.getSize().y / 4.0f);
            textbuf[i]= Team;
        } else {

        }

    }
}

void ContextPreparation::PlayerTeam(unsigned short id, unsigned short team) {


    for (auto i : textbuf) {
        if (team == i.first) {
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
        } //else textbuf[team]->setColor(sf::Color::White);

    }


}




