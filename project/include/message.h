//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_MESSAGE_H
#define CHAINOFRESPONSABILITY_MESSAGE_H

#include <queue>

class BaseMessage{
public:
    BaseMessage(unsigned short Type, unsigned short ID, unsigned int x = 0, unsigned int y = 0)
    : type(Type), ID(ID), x(x), y(y) {} ;
    unsigned short getType() const {
        return type;
    }
    unsigned short getID() const {
        return ID;
    }
    unsigned int getX() const {
        return x;
    }
    unsigned int getY() const {
        return y;
    }
private:
    unsigned short type;
    unsigned short ID;
    unsigned int x;
    unsigned int y;
};


class EventMessage : public BaseMessage {
public:
    enum {
        CREATE_MAP = 0,
        CREATE_ZONE = 1,
        CREATE_PLAYER = 2,
        CREATE_OBJECT = 3,
        SET_HEALTH = 4,
        MOVE = 5,
        DELETE = 6,
        WIN_TEAM = 7,
        DRAW_GAME = 8,
        CLOSE_GAME = 9
    };
    EventMessage(unsigned short Type, unsigned short ID,
                 unsigned int X, unsigned int Y, unsigned short Data = 0) :
                    BaseMessage(Type, ID, X, Y), data(Data) {};
    unsigned short getData() const {
        return data;
    }

private:
    unsigned short data;
};

//template <typename T>
//class MessageQueue {
//public:
//    void addEventToQueue(unsigned short type, unsigned short playerID, unsigned int x, unsigned int y) {
//        T message(type, playerID, x, y);
//        queue.push(message);
//    }
//    T getEventFromQueue() {
//        T tmp = queue.front();
//        queue.pop();
//        return tmp;
//    }
//    bool empty() const {
//        return queue.empty();
//    }
//private:
//    std::queue<T> queue;
//};

#endif // CHAINOFRESPONSABILITY_MESSAGE_H
