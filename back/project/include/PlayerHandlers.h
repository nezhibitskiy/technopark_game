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
    EventMessage **Handle(BaseMessage request, Map *map, Player *players, unsigned int *returnMsgCount) override {
        if (request.getType() > MoveHandler::MOVE_RIGHT)
            return AbstractHandler::Handle(request, map, players, returnMsgCount);

        unsigned int x = 0;
        unsigned int y = 0;

        if (x >= map->getWidth() || y >= map->getHeight()) return nullptr;

        Player *player = &players[request.getID()];


        switch (request.getType()) {
            case(MOVE_UP): {
                if (player->getY() == 0) return nullptr;
                x = player->getX();
                y = player->getY() - 1;
                break;
            }
            case(MOVE_DOWN): {
                if (player->getY() == (map->getHeight() - 1)) return nullptr;
                x = player->getX();
                y = player->getY() + 1;
                break;
            }
            case(MOVE_LEFT): {
                if (player->getX() == 0) return nullptr;
                x = player->getX() - 1;
                y = player->getY();
                break;
            }
            case(MOVE_RIGHT): {
                if (player->getX() == (map->getWidth() - 1)) return nullptr;
                x = player->getX() + 1;
                y = player->getY();
                break;
            }
        }

        Object *object = map->getObject(x, y);

        if (object == nullptr) {
            map->moveObject(player->getX(), player->getY(), x, y);
            players[request.getID()].setCoords(x, y);

            *returnMsgCount = 1;
            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = new EventMessage(EventMessage::MOVE, request.getID(), x, y);
            return returnMessages;
        }

        if (object->CanBeStandOn()) {

            object->ToDo(player);
            delete object;

            map->addObject(nullptr, x, y);

            map->moveObject(player->getX(), player->getY(), x, y);
            player->setCoords(x, y);

            *returnMsgCount = 1;
            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = new EventMessage(EventMessage::MOVE, request.getID(), x, y);
            return returnMessages;
        }
        return nullptr;
    }
};

#define DEFAULT_ATTACK_RADIUS 2

class AttackHandler : public AbstractRequestHandler {
public:
    enum Type {
        ATTACK = 4,
    };
    EventMessage **Handle(BaseMessage request, Map *map, Player *players, unsigned int *returnMsgCount) override {
        if (request.getType() != AttackHandler::ATTACK)
            return AbstractHandler::Handle(request, map, players, returnMsgCount);

        Player *player = &players[request.getID()];
        const unsigned int x = request.getX();
        const unsigned int y = request.getY();

        if (x >= map->getWidth() || y >= map->getHeight()) return nullptr;

        if ((std::max(x, player->getX()) -
             std::min(x, player->getX())) > DEFAULT_ATTACK_RADIUS ||
            (std::max(y, player->getY()) -
             std::min(y, player->getY())) > DEFAULT_ATTACK_RADIUS) {
            return nullptr;
        }

        Object *object = map->getObject(x, y);

        if (object != nullptr) {
            if (object->Damagable()) {
                unsigned char leftHealth = object->Damage(1);
                if (leftHealth == 0) {
                    if (!object->Respawn()) {
                        //Удалить объект
                        delete object;
                        map->addObject(nullptr, x, y);

                        *returnMsgCount = 1;
                        EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
                        returnMessages[0] = new EventMessage(EventMessage::DELETE, request.getID(), x, y);
                        return returnMessages;
                    } else {
                        // Добавить респавн
                        map->moveObject(x, y, 5, 5);

                        *returnMsgCount = 2;
                        EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
                        returnMessages[0] = new EventMessage(EventMessage::SET_HEALTH, request.getID(), x, y, DEFAULT_HEALTH_VALUE);
                        returnMessages[1] = new EventMessage(EventMessage::MOVE, request.getID(), 5, 5);
                        return returnMessages;
                    }
                }
                else {
                    *returnMsgCount = 1;
                    EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
                    returnMessages[0] = new EventMessage(EventMessage::SET_HEALTH, request.getID(), x, y, leftHealth);
                    return returnMessages;
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
    EventMessage **Handle(BaseMessage request, Map *map, Player *players, unsigned int *returnMsgCount) override {
        if (request.getType() != PutBlockHandler::PUT_BLOCK)
            return AbstractHandler::Handle(request, map, players, returnMsgCount);

        Player *player = &players[request.getID()];
        const unsigned int x = request.getX();
        const unsigned int y = request.getY();

        if (x >= map->getWidth() || y >= map->getHeight()) return nullptr;

        if ((std::max(x, player->getX()) -
             std::min(x, player->getX())) > DEFAULT_PUT_BLOCK_RADIUS ||
            (std::max(y, player->getY()) -
             std::min(y, player->getY())) > DEFAULT_PUT_BLOCK_RADIUS) {
            return nullptr;
        }

        Object *object = map->getObject(x, y);

        if (object == nullptr || object->CanBeStandOn()) {
            if (object != nullptr) {
                delete object;
                map->addObject(nullptr, x, y);
            }
            DefaultBlock *block = new DefaultBlock;
            map->addObject(block, x, y);

            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = new EventMessage(EventMessage::CREATE_OBJECT, request.getID(), x, y);
            return returnMessages;
        }

//            std::cout << "Put block: Player " << request.getPlayerID() << " will pul block on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
    }
};

#endif //CHAINOFRESPONSABILITY_PLAYERHANDLERS_H