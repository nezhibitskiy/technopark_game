#include <iostream>
#include <ostream>
#include <string>

#include "Application.h"
#include "clientNetwork.h"


class game {
public:
    enum State {
        INIT = 0,
        WAITING_FOR_GAME = 1,
        STARTED = 2,
        END_OF_GAME = 3
    };

    game() {
        client = new Client("0.0.0.0", "5000", &event, &request);
    }
    ~game() {
        delete client;
    }
    Client *client;

    void Iteration() {
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
                        while(!event->empty()){

                            app.render(event);
                            event->pop();
                        }
                        if(app.processInput(request)){

                        }
                    }

                    state = END_OF_GAME;
                    break;
                    //default:
                    //  break;

            }
        }
        std::cout << "END OF GAME WAS HERE" << std::endl;

    }

    State state;
    Application app;
    std::queue<EventMessage> *event;
    std::queue<BaseMessage> *request;
};


int main()
{
    game game;

    game.Iteration();
    return 0;
}