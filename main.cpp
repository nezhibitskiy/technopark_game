#include <iostream>
#include <queue>

#include "move.h"


class AttackHandler : public AbstractHandler {
public:
    void Handle(Message request) override {
        if (request.getType() == Message::ATTACK) {
            std::cout << "Attack: Player " << request.getPlayerID() << " will attack on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request);
        }
    }
};

class PutBlockHandler : public AbstractHandler {
public:
    void Handle(Message request) override {
        if (request.getType() == Message::PUT_BLOCK) {
            std::cout << "Put block: Player " << request.getPlayerID() << " will pul block on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request);
        }
    }
};

void ClientCode(Handler &handler) {
    MessageQueue event;
    event.addEventToQueue(Message::MOVE, 1, 1, 1);
    event.addEventToQueue(Message::ATTACK, 0, 2, 1);
    event.addEventToQueue(Message::PUT_BLOCK, 2, 1, 2);

    while (!event.empty())
    {
        Message tmpMsg = event.getEventFromQueue();
        std::cout << "Player " << tmpMsg.getPlayerID() << ": I want to ";
        switch (tmpMsg.getType()) {
            case (Message::MOVE):
                std::cout << "move";
                break;
            case (Message::ATTACK):
                std::cout << "attack";
                break;
            case (Message::PUT_BLOCK):
                std::cout << "put block";
                break;
        }
        std::cout << std::endl;

        handler.Handle(tmpMsg);
    }
}

class Game {
public:
    void initGame() {
        //init
        // while(is init)
        // {

        // }
    }
};

int main() {

    // init
    // while ...
    // graphic.iteration
    // game.iteartion
    // server.iteration
    // client.iteration
    auto *move = new MoveHandler;
    auto *attack = new AttackHandler;
    auto *putBlock = new PutBlockHandler;
    move->SetNext(attack)->SetNext(putBlock);

    std::cout << "Chain: Move > Attack > Put Block\n\n";
    ClientCode(*move);
    std::cout << "\n";
    std::cout << "Subchain: Attack > Put Block\n\n";
    ClientCode(*attack);

    delete move;
    delete attack;
    delete putBlock;

    return 0;
}