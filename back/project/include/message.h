//
// Created by ilyas on 31.10.2021.
//

#ifndef CHAINOFRESPONSABILITY_MESSAGE_H
#define CHAINOFRESPONSABILITY_MESSAGE_H

#include <queue>

class Message {
public:
    Message(unsigned short rType, unsigned short rPlayerID, unsigned int rX, unsigned int rY) :
            type(rType), playerID(rPlayerID), x(rX), y(rY) {};
    unsigned short getType() const {
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
    unsigned short type;
    unsigned short playerID;
    unsigned int x;
    unsigned int y;
};

class MessageQueue {
public:
    void addEventToQueue(unsigned short type, unsigned short playerID, unsigned int x, unsigned int y) {
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

#endif // CHAINOFRESPONSABILITY_MESSAGE_H
