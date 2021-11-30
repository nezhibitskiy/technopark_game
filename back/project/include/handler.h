//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_HANDLER_H
#define CHAINOFRESPONSABILITY_HANDLER_H

#include "map_class.h"
#include "player_class.h"

template<typename Message, typename ReturnMessage, typename Map, typename Objects>
class Handler {
public:
    virtual Handler *SetNext(Handler *handler) = 0;
    virtual unsigned int Handle(Message request, Map *map, Objects *objects, ReturnMessage **returnMessage) = 0;
};

template<typename Message, typename ReturnMessage, typename Objects = Object, typename Map = Map>
class AbstractHandler : public Handler<Message, ReturnMessage, Map, Objects> {
private:
    Handler<Message, ReturnMessage, Map, Objects> *next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    Handler<Message, ReturnMessage, Map, Objects> *SetNext(Handler<Message, ReturnMessage, Map, Objects> *handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    unsigned int Handle(Message request, Map *map, Objects *objects, ReturnMessage **returnMessage) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request, map, objects, returnMessage);
        }
        else {
            // default action for unknown request
            std::cerr << " Action was left untouched.\n";
            return 0; // nullptr
        }
    }
};

#endif //CHAINOFRESPONSABILITY_HANDLER_H