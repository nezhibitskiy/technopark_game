//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_MESSAGE_H
#define CHAINOFRESPONSABILITY_MESSAGE_H

#include <queue>

class Message {
public:
    enum Type {
        MOVE        = 0,
        ATTACK      = 1,
        PUT_BLOCK   = 2
    };
    Message(Type rType, unsigned short rPlayerID, unsigned int rX, unsigned int rY) :
            type(rType), playerID(rPlayerID), x(rX), y(rY) {};
    Type getType() const {
        return type;
    }
    unsigned short getPlayerID() const {
        return playerID;
    }
    unsigned int getX() const {
        return x;
    }
    unsigned int getY() const {
        return y;
    }

private:
    Type type;
    unsigned short playerID;
    unsigned int x;
    unsigned int y;

};

class MessageQueue {
public:
    void addEventToQueue(Message::Type type, unsigned short playerID, unsigned int x, unsigned int y) {
        Message message(type, playerID, x, y);
        queue.push(message);
    }
    Message getEventFromQueue() {
        Message tmp = queue.front();
        queue.pop();
        return tmp;
    }
    bool empty() const {
        return queue.empty();
    }
private:
    std::queue<Message> queue;
};

#endif //CHAINOFRESPONSABILITY_MESSAGE_H
