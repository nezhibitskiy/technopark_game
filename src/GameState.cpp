
#include <iostream>
#include "GameState.h"

GameState::GameState(StateStack &stack, Context context) : State(stack, context), mPlayer(*context.player),map(*context.window) {

        /*Message m(DrawMapHandler::CREATE_MAP, 0, 0, 0, 0);
        mapHandler->Handle(m, &map);*/

}


void GameState::draw(std::queue<EventMessage>* eventQueue) {
    EventMessage m(0,0,640,480,4);
    eventQueue->push(m);
    while(!eventQueue->empty()){
        eventManager.EventHandle(&eventQueue->front(),&map);
        eventQueue->pop();
    }
    //map.DrawBack();
    //while not queue

    //eventManager.Register();

    map.DrawBack();

    map.Draw();
    /*map.Draw();
    mUnit.draw(*getContext().window);
    if (!request.empty()) {

        Message m = request.getEventFromQueue();
        switch (m.getType()) {
            case MoveHandler::MOVE:
                std::cout << m.getX() << " XY " << m.getY() << std::endl;
                if (m.getX() == 0) {
                    mUnit.setPos(mUnit.getX(), mUnit.getY() + m.getY() * 19);
                }
                if (m.getY() == 0) {
                    mUnit.setPos(m.getX() * 20 + mUnit.getX(), mUnit.getY());
                }
                mUnit.setPos(m.getX() * 20 + mUnit.getX(), mUnit.getY() + m.getY() * 19);

                break;
            case AttackHandler::ATTACK:
                for( auto i : mBlock){
                    if( i.getX() == m.getX() && m.getY() == i.getY()){
                        std::swap(i, mBlock.back());
                        mBlock.pop_back();
                    }
                }
                break;
            case PutBlockHandler::PUT_BLOCK:
                Block test(10);
                test.setPos(m.getX(),m.getY());
                mBlock.push_back(test);
                break;


        }
    }

    mUnit.draw(*getContext().window);
   for( auto i : mBlock){
       i.draw(*getContext().window);
   }*/
}

bool GameState::handleEvent(const sf::Event &event) {

    mPlayer.handleEvent(event, request);



    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        std::cout << "change state to Menu \n";
        requestStackPop();
        requestStackPush(States::Menu);
        std::cout << "check KEY\n";

    }


    // control player
    return true;

}