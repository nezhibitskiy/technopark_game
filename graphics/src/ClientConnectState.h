#ifndef PROJECT_TP_CLIENTCONNECTSTATE_H
#define PROJECT_TP_CLIENTCONNECTSTATE_H

#include "State.h"
#include "Text.h"

class ClientConnectState : public DrawState::State {
public:
    ClientConnectState(StateStack &stack,Context context);

    ~ClientConnectState() {
        for (auto text: textbuf)
            delete text;
    }

    void draw(std::queue<EventMessage> *eventQueue) override;

    bool handleEvent(const sf::Event &event, std::queue<BaseMessage> *request) override;

    void ChangeState() override;

    std::string &getIP();

private:
    std::vector<Text *> textbuf;
    sf::Font font;
    std::string ipPlayer;

};


#endif //PROJECT_TP_CLIENTCONNECTSTATE_H
