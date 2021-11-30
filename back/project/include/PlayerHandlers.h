#ifndef CHAINOFRESPONSABILITY_PLAYERHANDLERS_H
#define CHAINOFRESPONSABILITY_PLAYERHANDLERS_H

#include "handler.h"
#include "message.h"

class AbstractRequestHandler : public AbstractHandler<BaseMessage, EventMessage, Player> {};
class AbstractEventHandler : public AbstractHandler<EventMessage, void> {};

class MoveHandler : public AbstractRequestHandler {
public:
    enum MoveType {
        MOVE_UP = 0,
        MOVE_DOWN = 1,
        MOVE_LEFT = 2,
        MOVE_RIGHT = 3,
    };
    unsigned int Handle(BaseMessage request, Map *map, Player *players, EventMessage **returnMessages) override {
        if (request.getType() > MoveHandler::MOVE_RIGHT)
            return AbstractHandler::Handle(request, map, players, returnMessages);

        unsigned int x = 0;
        unsigned int y = 0;

        if (x >= map->getWidth() || y >= map->getHeight()) return 0;

        Player *player = &players[request.getID()];


        switch (request.getType()) {
            case(MOVE_UP): {
                if (player->getY() == 0) return 0;
                x = player->getX();
                y = player->getY() - 1;
                break;
            }
            case(MOVE_DOWN): {
                if (player->getY() == (map->getHeight() - 1)) return 0;
                x = player->getX();
                y = player->getY() + 1;
                break;
            }
            case(MOVE_LEFT): {
                if (player->getX() == 0) return 0;
                x = player->getX() - 1;
                y = player->getY();
                break;
            }
            case(MOVE_RIGHT): {
                if (player->getX() == (map->getWidth() - 1)) return 0;
                x = player->getX() + 1;
                y = player->getY();
                break;
            }
        }

        Object *object = map->getObject(x, y);

        if (object == nullptr) {
            map->moveObject(player->getX(), player->getY(), x, y);
            players[request.getID()].setCoords(x, y);

            *returnMessages = new EventMessage(EventMessage::MOVE, request.getID(), x, y);
            return 1;
        }

        if (object->CanBeStandOn()) {

            object->ToDo(player);
            delete object;

            map->addObject(nullptr, x, y);

            map->moveObject(player->getX(), player->getY(), x, y);
            player->setCoords(x, y);

            *returnMessages = new EventMessage(EventMessage::MOVE, request.getID(), x, y);
            return 1;
        }
        return 0;
    }
};

#define DEFAULT_ATTACK_RADIUS 2

class AttackHandler : public AbstractRequestHandler {
public:
    enum Type {
        ATTACK = 4,
    };
    unsigned int Handle(BaseMessage request, Map *map, Player *players, EventMessage **returnMessages) override {
        if (request.getType() != AttackHandler::ATTACK)
            return AbstractHandler::Handle(request, map, players, returnMessages);

        Player *player = &players[request.getID()];
        const unsigned int x = request.getX();
        const unsigned int y = request.getY();

        if (x >= map->getWidth() || y >= map->getHeight()) return 0;

        if ((std::max(x, player->getX()) -
             std::min(x, player->getX())) > DEFAULT_ATTACK_RADIUS ||
            (std::max(y, player->getY()) -
             std::min(y, player->getY())) > DEFAULT_ATTACK_RADIUS) {
            return 0;
        }

        Object *object = map->getObject(x, y);

        if (object != nullptr) {
            if (object->Damagable()) {
                if (object->Damage(1)) {
                    if (!object->Respawn()) {
                        //Удалить объект
                        delete object;
                        map->addObject(nullptr, x, y);

                        *returnMessages = new EventMessage(EventMessage::SET_HEALTH, request.getID(), x, y);
                        return 1;
                    } else {
                        // Добавить респавн

                    }
                }
            }
        }
//            std::cout << "Attack: Player " << request.getPlayerID() << " will attack on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;

    }
};

#define DEFAULT_PUT_BLOCK_RADIUS 1

class PutBlockHandler : public AbstractRequestHandler {
public:
    enum Type {
        PUT_BLOCK = 5
    };
    unsigned int Handle(BaseMessage request, Map *map, Player *players, EventMessage **returnMessages) override {
        if (request.getType() != PutBlockHandler::PUT_BLOCK)
            return AbstractHandler::Handle(request, map, players, returnMessages);

        Player *player = &players[request.getID()];
        const unsigned int x = request.getX();
        const unsigned int y = request.getY();

        if (x >= map->getWidth() || y >= map->getHeight()) return 0;

        if ((std::max(x, player->getX()) -
             std::min(x, player->getX())) > DEFAULT_PUT_BLOCK_RADIUS ||
            (std::max(y, player->getY()) -
             std::min(y, player->getY())) > DEFAULT_PUT_BLOCK_RADIUS) {
            return 0;
        }

        Object *object = map->getObject(x, y);

        if (object == nullptr || object->CanBeStandOn()) {
            if (object != nullptr) {
                delete object;
                map->addObject(nullptr, x, y);
            }
            DefaultBlock *block = new DefaultBlock;
            map->addObject(block, x, y);

            *returnMessages = new EventMessage(EventMessage::CREATE_OBJECT, request.getID(), x, y);
            return 1;
        }

//            std::cout << "Put block: Player " << request.getPlayerID() << " will pul block on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
    }
};

#endif //CHAINOFRESPONSABILITY_PLAYERHANDLERS_H