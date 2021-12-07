//
// Created by ilyas on 07.12.2021.
//

#ifndef PROJECT_TP_CLIENTNETWORK_H
#define PROJECT_TP_CLIENTNETWORK_H

#include <iostream>

#include <boost/asio.hpp>
#include <thread>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>

#include "message.h"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(const std::string& server, const std::string& port,
           std::queue<EventMessage> **rInputQueue,
           std::queue<BaseMessage> **rOutputQueue);

    void run();
    void endServ();

private:
    void handle_resolve(const boost::system::error_code& err,
                        const tcp::resolver::results_type& endpoints);

    void handle_connect(const boost::system::error_code& err);

    void handle_stop();

    void handle_write_request(const boost::system::error_code& err);

    void handle_read(const boost::system::error_code& err);

private:

    std::vector<boost::shared_ptr<std::thread> > threads;
    boost::asio::io_context io_context;
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::signal_set signals_;
    boost::asio::streambuf response_buf_;


    boost::array<unsigned char, 14> inputBuffer_;
    boost::array<unsigned char, 12> outputBuffer_;


    std::queue<EventMessage> *inputQueue;
    std::queue<BaseMessage> *outputQueue;
};



#endif //PROJECT_TP_CLIENTNETWORK_H
