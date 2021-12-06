#ifndef CHAINOFRESPONSABILITY_PLAYERHANDLERS_H
#define CHAINOFRESPONSABILITY_PLAYERHANDLERS_H

#include "handler.h"
#include "message.h"
#include <unordered_map>

class AbstractRequestHandler : public AbstractHandler<BaseMessage, EventMessage, std::unordered_multimap<unsigned int, Object*>> {};
class AbstractEventHandler : public AbstractHandler<EventMessage, void> {};

class MoveHandler : public AbstractRequestHandler {
public:
    enum MoveType {
        MOVE_UP = 0,
        MOVE_DOWN = 1,
        MOVE_LEFT = 2,
        MOVE_RIGHT = 3,
    };
    EventMessage **Handle(BaseMessage request, Map *map, std::unordered_multimap<unsigned int, Object*> *hashTable, unsigned int *returnMsgCount, Factory* factory) override {
        if (request.getType() > MoveHandler::MOVE_RIGHT)
            return AbstractHandler::Handle(request, map, hashTable, returnMsgCount, factory);

        unsigned int x = 0;
        unsigned int y = 0;

        if (x >= map->getWidth() || y >= map->getHeight()) return nullptr;

        auto playerNode = hashTable->find(request.getID());

        if (playerNode == hashTable->end()) return nullptr;

        Object *player = playerNode->second;

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

        auto objectNode = hashTable->find(map->getObject(x, y));

        if (objectNode == hashTable->end()) {

            map->moveObject(player->getX(), player->getY(), x, y);
            player->setXY(x, y);

            *returnMsgCount = 1;
            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = new EventMessage(EventMessage::MOVE, playerNode->first, x, y);
            return returnMessages;
        }
        Object *object = objectNode->second;
        if (object->CanBeStandOn()) {

            object->ToDo(player); // Необходимо добавить ответ в виде сообщений

            map->moveObject(player->getX(), player->getY(), x, y);
            player->setXY(x, y);

            *returnMsgCount = 2;
            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = new EventMessage(EventMessage::DELETE, objectNode->first, x, y);
            returnMessages[1] = new EventMessage(EventMessage::MOVE, playerNode->first, x, y);

            hashTable->erase(objectNode);

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
    EventMessage **Handle(BaseMessage request, Map *map, std::unordered_multimap<unsigned int, Object*> *hashTable, unsigned int *returnMsgCount, Factory* factory) override {
        if (request.getType() != AttackHandler::ATTACK)
            return AbstractHandler::Handle(request, map, hashTable, returnMsgCount, factory);


        auto playerNode = hashTable->find(request.getID());
        if (playerNode == hashTable->end()) return nullptr;

        Object *player = playerNode->second;

        const unsigned int x = request.getX();
        const unsigned int y = request.getY();

        if (x >= map->getWidth() || y >= map->getHeight()) return nullptr;

        if ((std::max(x, player->getX()) -
             std::min(x, player->getX())) > DEFAULT_ATTACK_RADIUS ||
            (std::max(y, player->getY()) -
             std::min(y, player->getY())) > DEFAULT_ATTACK_RADIUS) {
            return nullptr;
        }

        auto objectNode = hashTable->find(map->getObject(x, y));

        if (objectNode != hashTable->end()) {
            Object *object = objectNode->second;
            if (object->Damagable()) {
                unsigned char leftHealth = object->Damage(1);
                if (leftHealth == 0) {
                    std::cout << "KILLS   " << player->getKills() << std::endl;
                    if (!object->Respawn()) {
                        //Удалить объект

                        *returnMsgCount = 1;
                        EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
                        returnMessages[0] = new EventMessage(EventMessage::DELETE, objectNode->first, x, y);

                        hashTable->erase(objectNode);
                        return returnMessages;
                    } else {
                        player->addKill();
                        unsigned int xSpawnpoint = object->getSpawnpoint().first;
                        unsigned int ySpawnpoint = object->getSpawnpoint().second;
                        // проверить, есть ли в этой клетке что-то

                        map->moveObject(x, y, xSpawnpoint, ySpawnpoint);

                        *returnMsgCount = 2;
                        EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
                        returnMessages[0] = new EventMessage(EventMessage::SET_HEALTH, objectNode->first, x, y, DEFAULT_HEALTH_VALUE);
                        returnMessages[1] = new EventMessage(EventMessage::MOVE, objectNode->first, xSpawnpoint, ySpawnpoint);

                        return returnMessages;
                    }
                }
                else {
                    *returnMsgCount = 1;
                    EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
                    returnMessages[0] = new EventMessage(EventMessage::SET_HEALTH, objectNode->first, x, y, leftHealth);
                    return returnMessages;
                }
            }
        }
    }
};

#define DEFAULT_PUT_BLOCK_RADIUS 1

class PutBlockHandler : public AbstractRequestHandler {
public:
    enum Type {
        PUT_BLOCK = 5
    };
    EventMessage **Handle(BaseMessage request, Map *map, std::unordered_multimap<unsigned int, Object*> *hashTable, unsigned int *returnMsgCount, Factory* factory) override {
        if (request.getType() != PutBlockHandler::PUT_BLOCK)
            return AbstractHandler::Handle(request, map, hashTable, returnMsgCount, factory);

        auto playerNode = hashTable->find(request.getID());
        if (playerNode == hashTable->end()) return nullptr;

        Object *player = playerNode->second;

        const unsigned int x = request.getX();
        const unsigned int y = request.getY();

        if (x >= map->getWidth() || y >= map->getHeight()) return nullptr;

        if ((std::max(x, player->getX()) -
             std::min(x, player->getX())) > DEFAULT_PUT_BLOCK_RADIUS ||
            (std::max(y, player->getY()) -
             std::min(y, player->getY())) > DEFAULT_PUT_BLOCK_RADIUS) {
            return nullptr;
        }

        auto objectNode = hashTable->find(map->getObject(x, y));

        EventMessage *deleteMessage = nullptr;
        if (objectNode != hashTable->end()) {
            Object *object = objectNode->second;
            if (!object->CanBeStandOn()) {
                return nullptr;
            }

            deleteMessage = new EventMessage(EventMessage::DELETE, objectNode->first, x, y);
            hashTable->erase(objectNode);
        }


        auto obj = factory->createObject(defaultBlockObject);
        std::pair<unsigned int, DefaultBlock*> block;
        block.first = obj.first;
        block.second = dynamic_cast<DefaultBlock *>(obj.second);

        map->addObject(block.first, x, y);
        hashTable->insert(block);

        if (deleteMessage != nullptr) {
            *returnMsgCount = 2;
            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = deleteMessage;
            returnMessages[1] = new EventMessage(EventMessage::CREATE_OBJECT, DefaultBlock::ID, x, y);
            return returnMessages;
        } else {
            *returnMsgCount = 1;
            EventMessage **returnMessages = new EventMessage*[*returnMsgCount];
            returnMessages[0] = new EventMessage(EventMessage::CREATE_OBJECT, DefaultBlock::ID, x, y);
            return returnMessages;
        }
    }
};

#endif //CHAINOFRESPONSABILITY_PLAYERHANDLERS_H