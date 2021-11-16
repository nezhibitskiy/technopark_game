//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_HANDLER_H
#define CHAINOFRESPONSABILITY_HANDLER_H

#include "message.h"
#include "map_class.h"
#include "player_class.h"

class Handler {
public:
    virtual Handler *SetNext(Handler *handler) = 0;
    virtual void Handle(Message request, Map *map, Player *players) = 0;
};

class AbstractHandler : public Handler {
private:
    Handler *next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    Handler *SetNext(Handler *handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    void Handle(Message request, Map *map) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request, map);
        }
        else {
            // default action for unknown request
            std::cout << " Action was left untouched.\n";
            return; // nullptr
        }
    }
};

#endif //CHAINOFRESPONSABILITY_HANDLER_H
