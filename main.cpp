#include <iostream>
#include <queue>

#include "move.h"

#include "Player.h"

class AttackHandler : public AbstractHandler {
public:
    enum Type {
        ATTACK = 1,
    };
    void Handle(Message request, Map *map) override {
        if (request.getType() == AttackHandler::ATTACK) {
            std::cout << "Attack: Player " << request.getPlayerID() << " will attack on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map);
        }
    }
};

class PutBlockHandler : public AbstractHandler {
public:
    enum Type {
        PUT_BLOCK = 2
    };
    void Handle(Message request, Map *map) override {
        if (request.getType() == PutBlockHandler::PUT_BLOCK) {
            std::cout << "Put block: Player " << request.getPlayerID() << " will pul block on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map);
        }
    }
};

class SetEnchantHandler : public AbstractHandler {
public:
    enum Type {
        SET_ENCHANT = 3
    };
    // EventMessage* Handle(EventMessage request) override
    void Handle(Message request, Map *map) override {
        if (request.getType() == SetEnchantHandler::SET_ENCHANT) {
            std::cout << "Set enchant: Player " << request.getPlayerID() << " will set enchant on x: " << request.getX();
            std::cout << " y: " << request.getY() << std::endl;
            //  if (check_can_player_move_on_coords())
            //      map movePlayer
            //      EventMessage* newMessage = new EventMessage(MoveHandler::MOVE, playerId, x, y);
                    return; // newMessage
            // else return nullptr;
        } else {
            return AbstractHandler::Handle(request, map);
        }
    }
};

void ClientCode(Handler &handler) {
    MessageQueue event;
    event.addEventToQueue(MoveHandler::MOVE, 1, 1, 1);
    event.addEventToQueue(AttackHandler::ATTACK, 0, 2, 1);
    event.addEventToQueue(PutBlockHandler::PUT_BLOCK, 2, 1, 2);
    event.addEventToQueue(SetEnchantHandler::SET_ENCHANT, 0, 2, 5);

    Map map;
    Player player;

    while (!event.empty())
    {
        Message tmpMsg = event.getEventFromQueue();
        std::cout << "Player " << tmpMsg.getPlayerID() << ": I want to ";
        switch (tmpMsg.getType()) {
            case (MoveHandler::MOVE):
                std::cout << "move";
                break;
            case (AttackHandler::ATTACK):
                std::cout << "attack";
                break;
            case (PutBlockHandler::PUT_BLOCK):
                std::cout << "put block";
                break;
        }
        std::cout << std::endl;

        handler.Handle(tmpMsg, &map);
    }
}

class Core {
public:
    void iteration(/* RequestQueue *requestQueue, EventQueue *eventQueue */) {
        // if (requestQueue.empty) { return }
        // tmpMsg = RequestQueue.getElementFromQueue()
        // newMessage = handler.Handle(tmpMsg);
        // if (newMessage != nullptr)
            // EventQueue.add(newMessage);
        // else return
    }

    void clientIteration(/* RequestQueue *requestQueue, EventQueue *eventQueue */) {
        // if (requestQueue.empty) { return }
        // tmpMsg = RequestQueue.getElementFromQueue()
        // newMessage = handler.Handle(tmpMsg);
        // if (newMessage != nullptr)
        // EventQueue.add(newMessage);
        // else return
    }
};

class Game {
public:
    void initGame() {
        //init
        // while(is init)
        // {

        // }
    }

    void runGame() {
        while (gameRuns) {
            // core.iteration(EventMessage message)
            // gui.iteration
            // server.iteration
        }
    };
private:
    bool gameRuns;
    // Map *map
    // Player *players
    // Block *blocks
    // Object *objects

};

int main() {
    Game game;
    // init
    // while ...
    // graphic.iteration
    // game.iteartion
    // server.iteration
    // client.iteration
    auto *move = new MoveHandler;
    auto *attack = new AttackHandler;
    auto *putBlock = new PutBlockHandler;
    auto *enchant = new SetEnchantHandler;
    move->SetNext(attack)->SetNext(putBlock)->SetNext(enchant);

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