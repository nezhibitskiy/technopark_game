//
// Created by ilyas on 31.10.2021.
//

#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H



template<typename Message, typename T>
class BaseEventHandler {
public:
    //virtual EventHandler *SetNext(EventHandler *handler) = 0;
    virtual bool CanHandle(Message *event) = 0;
    virtual void Handler(Message *event,T *t) = 0;
};

/*
template<typename Message, typename ReturnMessage, typename Map = DrawMap>
class DrawAbstractHandler : public Handler<Message, ReturnMessage, Map> {
private:
    DrawHandler<Message, ReturnMessage, Map> *next_handler_;

public:
    DrawAbstractHandler() : next_handler_(nullptr) {
    }
    DrawHandler<Message, ReturnMessage, Map> *SetNext(DrawHandler<Message, ReturnMessage, Map> *handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    ReturnMessage **Handle(Message request, Map *map, unsigned int *returnMsgCount) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request, map, returnMsgCount);
        }
        else {
            // default action for unknown request
            std::cerr << " Action was left untouched.\n";
            return 0; // nullptr
        }
    }
};
*/

#endif //TEST_HANDLER_H
