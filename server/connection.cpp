#include "connection.h"

#include <boost/bind/bind.hpp>
#include <iostream>

#include <string>

BaseMessage parse(const boost::array<unsigned char, 12> &data) {
    BaseMessage returnMessage((data[0] | data[1] << 8), (data[2] | data[3] << 8),
            (data[4] | data[5] << 8 | data[6] << 16 | data[7] << 24),
            (data[8] | data[9] << 8 | data[10] << 16 | data[11] << 24));
    return returnMessage;
}

boost::array<unsigned char, 14> pack(const EventMessage &data) {
    boost::array<unsigned char, 14> returnValue;
    returnValue[0] = data.getType() & 0xFF;
    returnValue[1] = (data.getType() >> 8) & 0xFF;

    returnValue[2] = data.getID() & 0xFF;
    returnValue[3] = (data.getID() >> 8) & 0xFF;

    returnValue[4] = data.getX() & 0xFF;
    returnValue[5] = (data.getX() >> 8) & 0xFF;
    returnValue[6] = (data.getX() >> 16) & 0xFF;
    returnValue[7] = (data.getX() >> 24) & 0xFF;


    returnValue[8] = data.getY() & 0xFF;
    returnValue[9] = (data.getY() >> 8) & 0xFF;
    returnValue[10] = (data.getY() >> 16) & 0xFF;
    returnValue[11] = (data.getY() >> 24) & 0xFF;

    returnValue[12] = data.getData() & 0xFF;
    returnValue[13] = (data.getData() >> 8) & 0xFF;

    return returnValue;
}

namespace gameServer {
    Connection::Connection(boost::asio::io_context& io_context,
                           std::queue<EventMessage> **rOutputQueue,
                           std::queue<BaseMessage> **rInputQueue) : socket_(io_context)
    {
        inputQueue = new std::queue<BaseMessage>;
        outputQueue = new std::queue<EventMessage>;
        *rOutputQueue = outputQueue;
        *rInputQueue = inputQueue;
    }

    boost::asio::ip::tcp::socket& Connection::socket()
    {
        return socket_;
    }

    void Connection::start()
    {
        boost::asio::async_read(socket_, boost::asio::buffer(inputBuffer_),
                                boost::asio::transfer_at_least(1),
                                boost::bind(&Connection::handle_read, shared_from_this(),
                                            boost::asio::placeholders::error));
        while(outputQueue->empty() && (socket_.is_open())) {}
        if (socket_.is_open()) {
            outputBuffer_ = pack(outputQueue->front());

            boost::asio::async_write(socket_, boost::asio::buffer(outputBuffer_.data(), outputBuffer_.size()),
                                     boost::bind(&Connection::handle_write, shared_from_this(),
                                                 boost::asio::placeholders::error));
        }

    }

    void Connection::handle_read(const boost::system::error_code& e)
    {
        if (!e)
        {
            std::cout << inputBuffer_.data() << std::endl;

            BaseMessage inputMessage = parse(inputBuffer_);
            std::cout << "Input message type: " << inputMessage.getType();
            std::cout << "; Input message ID: " << inputMessage.getID();
            std::cout << "; Input message X: " << inputMessage.getX();
            std::cout << "; Input message Y: " << inputMessage.getY() << ";" << std::endl;

            inputQueue->push(inputMessage);

            boost::asio::async_read(socket_, boost::asio::buffer(inputBuffer_),
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&Connection::handle_read, shared_from_this(),
                                                boost::asio::placeholders::error));
        }
        else {
            std::cout << "readHandler error" << std::endl;
        }
    }

    void Connection::handle_write(const boost::system::error_code& e)
    {
        if (!e)
        {
            outputQueue->pop();
            while (outputQueue->empty() && (socket_.is_open())) {

            }

            if (socket_.is_open()) {
                outputBuffer_ = pack(outputQueue->front());

                boost::asio::async_write(socket_, boost::asio::buffer(outputBuffer_.data(), outputBuffer_.size()),
                                         boost::bind(&Connection::handle_write, shared_from_this(),
                                                     boost::asio::placeholders::error));
            } else {
                // Initiate graceful connection closure.
                boost::system::error_code ignored_ec;
                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            }
        }
    }
}
