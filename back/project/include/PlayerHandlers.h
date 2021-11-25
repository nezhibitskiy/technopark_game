//
// Created by ilyas on 16.11.2021.
//

#ifndef CHAINOFRESPONSABILITY_PLAYERHANDLERS_H
#define CHAINOFRESPONSABILITY_PLAYERHANDLERS_H

#include "handler.h"

class MoveHandler : public AbstractHandler {
public:
    enum MoveType {
        MOVE = 0,
    };
    void Handle(Message request, Map *map, Player *players) override {
        if (request.getType() == MoveHandler::MOVE) {
            // Check if player can move and if true, move him
            if ((std::max(request.getX(), players[request.getPlayerID()].getX()) -
                 std::min(request.getX(), players[request.getPlayerID()].getX())) > 1 ||
                 (std::max(request.getY(), players[request.getPlayerID()].getY()) -
                 std::min(request.getY(), players[request.getPlayerID()].getY())) > 1) {
                return;
            }

            if (map->getObject(request.getX(), request.getY()) != nullptr) {
                if (map->getObject(request.getX(), request.getY())->CanBeStandOn()) {
                    // Обработчик объекта
                    // Если обработали - удаляем
                    map->moveObject(players[request.getPlayerID()].getX(),
                                    players[request.getPlayerID()].getY(),
                                    request.getX(),
                                    request.getY());
                    players[request.getPlayerID()].setCoords(request.getX(), request.getY());
                }
            } else {
                map->moveObject(players[request.getPlayerID()].getX(),
                                players[request.getPlayerID()].getY(),
                                request.getX(),
                                request.getY());
                players[request.getPlayerID()].setCoords(request.getX(), request.getY());
            }
//            std::cout << "Move: Player " << request.getPlayerID() << " will move on x: " << request.getX();
//            std::cout << " y: " << request.getY() << std::endl;
            return;
        } else {
            return AbstractHandler::Handle(request, map, players);
        }
    }
};

class AttackHandler : public AbstractHandler {
public:
    enum Type {
        ATTACK = 1,
    };
    void Handle(Message request, Map *map, Player *players) override {
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

class PutBlockHandler : public AbstractHandler {
public:
    enum Type {
        PUT_BLOCK = 2
    };
    void Handle(Message request, Map *map, Player *players) override {
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
