#include "Map.h"

Map::Map(sf::RenderWindow &mWindow) : mWindow(mWindow) {

}

void Map::Draw() {
    for(int i =0 ;i < 16 ; ++i){
        for(int j = 0; j < 11 ; ++j){

            mBlock.setPos(i*40,j*39);

            mBlock.draw(mWindow);
        }
    }

    Unit m(0);
    Unit m_enemy(10);
    Unit m_enemy_1(10);
    Unit m_ally(1);
    m.setPos(40,39);
    m_enemy.setPos(560,39);
    m_enemy_1.setPos(400,195);
    m_ally.setPos(80,39);
    m.draw(mWindow);
    m_enemy.draw(mWindow);
    m_enemy_1.draw(mWindow);
    m_ally.draw(mWindow);

}


