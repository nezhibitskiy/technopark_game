//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_HANDLER_H
#define CHAINOFRESPONSABILITY_HANDLER_H

template<typename Message, typename Map,typename Objects>
class Handler {
public:
    virtual Handler *SetNext(Handler *handler) = 0;
    virtual void Handle(Message request, Map *map, Objects *players) = 0;
};

template<typename Message, typename Map,typename Objects>
class AbstractHandler : public Handler<Message,Map,Objects> {
private:
    Handler<Message,Map,Objects> *next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    Handler<Message,Map,Objects> *SetNext(Handler<Message,Map,Objects> *handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    void Handle(Message request, Map *map, Objects *players) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request, map, players);
        }
        else {
            // default action for unknown request
            std::cerr << " Action was left untouched.\n";
            return; // nullptr
        }
    }
};

#endif //CHAINOFRESPONSABILITY_HANDLER_H