//
// Created by ilyas on 07.12.2021.
//

#include "message.h"
#include "game.h"

Game::Game() {
    client = new Client("0.0.0.0", "5000", &event, &request);
    state = INIT;
}
Game::~Game() {
    delete client;
}

void Game::Iteration() {
    while (state != END_OF_GAME) {
        switch(state) {
            case (INIT):
                app.render(event);
                if(app.processInput(request)){
                    app.changeState();
                    state = WAITING_FOR_GAME;
                    try
                    {
                        client->run();
                    }
                    catch (std::exception& e)
                    {
                        std::cout << "Exception: " << e.what() << "\n";
                    }
                }
                break;
            case (WAITING_FOR_GAME):

                app.render(event);
                if(app.processInput(request)){
                    app.changeState();

                    state = STARTED;
                }
                break;

            case (STARTED):
                while (event->front().getType() != EventMessage::CLOSE_GAME) {
                    while(!event->empty() && event->front().getType() != EventMessage::CLOSE_GAME){

                        app.render(event);
                        event->pop();
                    }
                    if(app.processInput(request)){

                    }
                }
                event->pop();
                state = GAME_OVER;
                app.changeState();
                while (event->front().getType() != EventMessage::WIN_TEAM) {

                }
                break;
            case(GAME_OVER):

                while(!event->empty()) {
                    app.render(event);
                    event->pop();
                }
                if (app.processInput(request)) {
                    app.changeState();
                    client->endServer();
                    state = END_OF_GAME;
                }
                break;
                //default:
                //  break;
        }
    }
    std::cout << "END OF GAME WAS HERE" << std::endl;

}