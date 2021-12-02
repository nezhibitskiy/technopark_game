#ifndef TEST_DRAWHANDLERS_H
#define TEST_DRAWHANDLERS_H



#include "Map.h"
#include "Handler.h"
#include "../ilyas/project/include/message.h"
namespace Draw {
    class EventHandler : public BaseEventHandler<EventMessage, DrawMap> {
    };

    class MoveHandler : public EventHandler {
    public:
        enum MoveType {
            MOVE = 3
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == MoveHandler::MOVE) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, DrawMap *map) override {
            map->SetUnits(event->getID(), event->getX(), event->getY());
        }


    };

    class MapHandler : public EventHandler {
    public:
        enum Type {
            CREATE_MAP = 0
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == MapHandler::CREATE_MAP) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, DrawMap *map) override {
            map->DrawMapInit(event->getX(), event->getY(), event->getData());
        }


    };

    class ObjectHandler : public EventHandler {
    public:
        enum Type {
            CREATE_OBJECT = 1
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == ObjectHandler::CREATE_OBJECT) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, DrawMap *map) override {
            map->SetBlocks(10, event->getX(), event->getY());
        }


    };

    class HealthHandler : public EventHandler {
    public:
        enum Type {
            SET_HEALTH = 2
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == HealthHandler::SET_HEALTH) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, DrawMap *map) override {
            map->SetHp(0, event->getData());
        }


    };

    class DeleteHandler : public EventHandler {
    public:
        enum Type {
            DELETE = 4
        };

        bool CanHandle(EventMessage *event) override {
            if (event->getType() == DeleteHandler::DELETE) {
                return true;
            } else return false;
        }

        void Handler(EventMessage *event, DrawMap *map) override {
            map->SetBlocks(0, event->getX(), event->getY());
        }


    };
}
/*class ObjectHandler : public AbstractEventHandler {
public:
    enum Type {
        CREATE_OBJECT = 1
    };

    EventMessage **Handle(BaseMessage request, Map *map, unsigned int *returnMsgCount) override {
        if (request.getType() != ObjectHandler::CREATE_OBJECT)
            return DrawAbstractHandler::Handle(request, map, returnMsgCount);

        return nullptr;
    }
};

class HealthHandler : public AbstractEventHandler {
public:
    enum Type {
        SET_HEALTH = 2
    };

    EventMessage **Handle(BaseMessage request, Map *map, unsigned int *returnMsgCount) override {
        if (request.getType() != HealthHandler::SET_HEALTH)
            return AbstractHandler::Handle(request, map, returnMsgCount);

        return nullptr;
    }
};

class DeleteHandler : public AbstractEventHandler {
public:
    enum Type {
        DELETE = 4
    };

    EventMessage **Handle(BaseMessage request, Map *map, unsigned int *returnMsgCount) override {
        if (request.getType() != DeleteHandler::SET_HEALTH)
            return AbstractHandler::Handle(request, map, returnMsgCount);

        return nullptr;
    }
};


/*
class DrawMoveHandler : public AbstractDrawHandler<Message, DrawMap> {
public:
    enum MoveType {
        MOVE = 0,
    };

    void Handle(Message request, DrawMap *map) override {
        if (request.getType() == DrawMoveHandler::MOVE) {
            map->SetUnits(request.getPlayerID(), request.getX(), request.getY());
            return;
        } else {
            return AbstractDrawHandler::Handle(request, map);
        }
    }

};

class DrawMapHandler : public AbstractDrawHandler<Message, DrawMap> {
public:
    enum MapType {
        CREATE_MAP = 1,
    };

    void Handle(Message request, DrawMap *map) override {
        if (request.getType() == DrawMapHandler::CREATE_MAP) {
            map->DrawMapInit(640, 480, 4);
            return;
        } else {
            return AbstractDrawHandler::Handle(request, map);
        }
    }

};

class DrawHealthHandler : public AbstractDrawHandler<Message, DrawMap> {
public:
    enum MapType {
        SET_HEALTH = 3,
    };

    void Handle(Message request, DrawMap *map) override {

    }

};

class DrawDeleteHandler : public AbstractDrawHandler<Message, DrawMap> {
public:
    enum MapType {
        DELETE = 4,
    };

    void Handle(Message request, DrawMap *map) override {

    }

};

class DrawBlockHandler : public AbstractDrawHandler<Message, DrawMap> {
public:
    enum BlockType {
        PUT_BLOCK = 2,
    };

    void Handle(Message request, DrawMap *map) override {
        if (request.getType() == DrawBlockHandler::PUT_BLOCK) {
            map->SetBlocks(10, request.getX(), request.getY());

        } else if (request.getType() == DrawMapHandler::CREATE_MAP) {
            map->DrawMapInit(640, 480, 4);

        } else if (request.getType() == DrawMoveHandler::MOVE) {
            map->SetUnits(request.getPlayerID(), request.getX(), request.getY());
        } else if (request.getType() == DrawHealthHandler::SET_HEALTH) {
            map->SetHp(0, 2);
        } else if (request.getType() == DrawDeleteHandler::DELETE) {
            map->SetBlocks(0, request.getX(), request.getY());
        } else {
            return AbstractDrawHandler::Handle(request, map);
        }

        return;
    }

};
*/
#endif //TEST_DRAWHANDLERS_H
