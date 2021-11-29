#ifndef CHAINOFRESPONSABILITY_PLAYERHANDLERS_H
#define CHAINOFRESPONSABILITY_PLAYERHANDLERS_H

#include "handler.h"
#include "message.h"
#include "map_class.h"
#include "player_class.h"

class AbstractRequestHandler : public AbstractHandler<BaseMessage, Map, Player> {};
class AbstractEventHandler : public AbstractHandler<EventMessage, Map, Object> {};

class MoveHandler : public AbstractRequestHandler {
public:
    enum MoveType {
        MOVE_UP = 0,
        MOVE_DOWN = 1,
        MOVE_LEFT = 2,
        MOVE_RIGHT = 3,
    };
    void Handle(BaseMessage request, Map *map, Player *players) override {
        if (request.getType() >= MoveHandler::MOVE_UP && request.getType() <= MoveHandler::MOVE_RIGHT) {
            // Check if player can move and if true, move him
            unsigned int x = 0;
            unsigned int y = 0;

            switch (request.getType()) {
                case(MOVE_UP): {
                    if (players[request.getID()].getY() == 0) return;
                    x = players[request.getID()].getX();
                    y = players[request.getID()].getY() - 1;
                    break;
                }
                case(MOVE_DOWN): {
                    if (players[request.getID()].getY() == (map->getHeight() - 1)) return;
                    x = players[request.getID()].getX();
                    y = players[request.getID()].getY() + 1;
                    break;
                }
                case(MOVE_LEFT): {
                    if (players[request.getID()].getX() == 0) return;
                    x = players[request.getID()].getX() - 1;
                    y = players[request.getID()].getY();
                    break;
                }
                case(MOVE_RIGHT): {
                    if (players[request.getID()].getX() == (map->getWidth() - 1)) return;
                    x = players[request.getID()].getX() + 1;
                    y = players[request.getID()].getY();
                    break;
                }
            }

            if (map->getObject(x, y) != nullptr) {
                Object *object = map->getObject(x, y);

                if (object->CanBeStandOn()) {
                    // Обработчик объекта
                    // Если обработали - удаляем
                    map->moveObject(players[request.getID()].getX(), players[request.getID()].getY(), x, y);
                    players[request.getID()].setCoords(x, y);
                }
            } else {
                map->moveObject(players[request.getID()].getX(), players[request.getID()].getY(), x, y);
                players[request.getID()].setCoords(x, y);
            }


//            std::cout << "Move: Player " << request.getPlayerID() << " will move on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map, players);
        }
    }
};

class AttackHandler : public AbstractRequestHandler {
public:
    enum Type {
        ATTACK = 4,
    };
    void Handle(BaseMessage request, Map *map, Player *players) override {
        if (request.getType() == AttackHandler::ATTACK) {
            if (map->getObject(request.getX(), request.getY()) != nullptr) {
                if (map->getObject(request.getX(), request.getY())->Damagable()) {
                    if (map->getObject(request.getX(), request.getY())->Damage(1)) {
                        if (!map->getObject(request.getX(), request.getY())->Respawn()) {
                            //Удалить объект
                        }
                    }
                }
            }
//            std::cout << "Attack: Player " << request.getPlayerID() << " will attack on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map, players);
        }
    }
};

class PutBlockHandler : public AbstractRequestHandler {
public:
    enum Type {
        PUT_BLOCK = 5
    };
    void Handle(BaseMessage request, Map *map, Player *players) override {
        if (request.getType() == PutBlockHandler::PUT_BLOCK) {
//            std::cout << "Put block: Player " << request.getPlayerID() << " will pul block on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map, players);
        }
    }
};

#endif //CHAINOFRESPONSABILITY_PLAYERHANDLERS_H