//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_HANDLER_H
#define CHAINOFRESPONSABILITY_HANDLER_H

#include "message.h"

class Handler {
public:
    virtual Handler *SetNext(Handler *handler) = 0;
    virtual void Handle(Message request) = 0;
};

class AbstractHandler : public Handler {
    /**
     * @var Handler
     */
private:
    Handler *next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    Handler *SetNext(Handler *handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    void Handle(Message request) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request);
        }
        else {
            // default action for unknown request
            std::cout << " Action was left untouched.\n";
        }
    }
};

#endif //CHAINOFRESPONSABILITY_HANDLER_H
