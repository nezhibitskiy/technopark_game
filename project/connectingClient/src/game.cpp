
#include "message.h"
#include "game.h"

Game::Game() : event(), request() {
    state = PREINIT;
    event = new std::queue<EventMessage>;
    request = new std::queue<BaseMessage>;
//    std::cout << "Event queue size: " << event->size() << std::endl;
}

Game::~Game() {
    delete client;
}

void Game::Iteration() {
    while (state != END_OF_GAME) {
        switch (state) {
            case (PREINIT):
                app.render(event);
                if (app.processInput(request)) {

                    app.changeState();
                    state = INIT;
                }
                break;
            case (INIT): // join write IP
                app.render(event);
                if (app.processInput(request)) {
                    if (request->empty()) break;
                    if (request->front().getType() == IpHandler::IP) {
                        try {
                            std::string ip = std::to_string(((request->front().getID() >> 8) & 0xFF)) + "."
                                    + std::to_string(((request->front().getID()) & 0xFF)) + "."
                                    + std::to_string(((request->front().getX() >> 8) & 0xFF)) + "."
                                    + std::to_string(((request->front().getX()) & 0xFF));
                            std::string port = std::to_string(request->front().getY());

//                            std::cout << ip << ":" << port << std::endl;

                            client = new Client(ip, port, event, request);

                            client->run();
                        }
                        catch (std::exception &e) {
                            std::cout << "Exception: " << e.what() << "\n";
//                            delete client;
                            client = nullptr;
                            break;
                        }
                        state = WAITING_FOR_GAME;
                        app.changeState();
                    }
                }
                break;
            case (WAITING_FOR_GAME):

                while (!event->empty()) {

                    app.render(event);
                    event->pop();

                }
                if (app.processInput(request)) {
                    app.changeState();

                    state = WAITING_FOR_SERVER_START;
                }
                break;
            case (WAITING_FOR_SERVER_START):
                if (!event->empty()) {
                    if (event->front().getType() == EventMessage::CREATE_MAP)
                        state = STARTED;
                    else
                        event->pop();
                }
                break;

            case (STARTED):
                while (event->front().getType() != EventMessage::CLOSE_GAME) {

                    while (!event->empty() && event->front().getType() != EventMessage::CLOSE_GAME) {

                        app.render(event);
                        event->pop();

                    }
                    if (app.processInput(request)) {

                    }
                }

                event->pop();
                state = GAME_OVER;
                app.changeState();
                while (event->front().getType() != EventMessage::WIN_TEAM) {

                }
                break;
            case (GAME_OVER):

                while (!event->empty()) {
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