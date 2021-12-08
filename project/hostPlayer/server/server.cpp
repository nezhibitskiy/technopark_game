
#include "server.h"

#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>

namespace gameServer {
    server::server(const std::string& address, const std::string& port,
                   std::size_t thread_pool_size)
            : thread_pool_size_(thread_pool_size),
              signals_(io_context_),
              acceptor_(io_context_)
    {
        clientConnectedCount = 0;
        inputQueue = new std::queue<BaseMessage>*[thread_pool_size];
        outputQueue = new std::queue<EventMessage>*[thread_pool_size];

        // Register to handle the signals that indicate when the hostPlayer should exit.
        signals_.add(SIGINT);   // остановка процесса с терминала
        signals_.add(SIGTERM);  // сигнал от kill
        signals_.async_wait(boost::bind(&server::closeServer, this));

        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        start_accept();
    }
    void server::init() {
        // Create a pool of threads to run all of the io_contexts.
        for (std::size_t i = 0; i < thread_pool_size_; ++i)
        {
            boost::shared_ptr<std::thread> thread(new std::thread(
                    boost::bind(&boost::asio::io_context::run, &io_context_)));

            threads.push_back(thread);
        }
    }

    void server::run(EventMessage tmpEventMsg)
    {
        // QUEUE HERE
        for (unsigned int i = 0; i < connectionVector.size(); i++) {
            if (connectionVector[i]->is_open()) {
                if (tmpEventMsg.getID() - 1 == i &&
                tmpEventMsg.getType() != EventMessage::CREATE_OBJECT &&
                tmpEventMsg.getType() != EventMessage::WIN_TEAM) {
                    EventMessage msgForCurrentPlayer(tmpEventMsg.getType(),
                                                     0,
                                                     tmpEventMsg.getX(),
                                                     tmpEventMsg.getY(),
                                                     tmpEventMsg.getData());
                    outputQueue[i]->push(msgForCurrentPlayer);
                } else if (tmpEventMsg.getID() == 0 &&
                tmpEventMsg.getType() != EventMessage::CREATE_OBJECT &&
                tmpEventMsg.getType() != EventMessage::WIN_TEAM) {
                    EventMessage msgForCurrentPlayer(tmpEventMsg.getType(),
                                                     i + 1,
                                                     tmpEventMsg.getX(),
                                                     tmpEventMsg.getY(),
                                                     tmpEventMsg.getData());
                    outputQueue[i]->push(msgForCurrentPlayer);
                } else
                    outputQueue[i]->push(tmpEventMsg);
            }
        }
    }
    BaseMessage **server::checkRequests(unsigned int *reqMsgCount)
    {
        std::vector<std::pair<unsigned short, BaseMessage>> returnVector;

        for (unsigned short i = 0; i < connectionVector.size(); i++) {
            if (!inputQueue[i]->empty()) {
                while (!inputQueue[i]->empty()) {
                    returnVector.emplace_back(i, inputQueue[i]->front());
                    inputQueue[i]->pop();
                }
            }
        }

        if (returnVector.empty()) return nullptr;
        *reqMsgCount = returnVector.size();
        BaseMessage **returnMessages = new BaseMessage*[*reqMsgCount];
        for (unsigned int i = 0; i < *reqMsgCount; i++) {
            returnMessages[i] = new BaseMessage(returnVector[i].second.getType(),
                                                returnVector[i].first + 1,
                                                returnVector[i].second.getX(),
                                                returnVector[i].second.getY());
        }
        return returnMessages;
    }

    void server::closeServer() {
        if (io_context_.stopped()) return;
        for (std::size_t i = 0; i < connectionVector.size(); ++i) {
            connectionVector[i]->stop();
        }
        io_context_.stop();
        // Wait for all threads in the pool to exit.
        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i]->join();
    }

    void server::start_accept()
    {
        connectionVector.push_back(boost::shared_ptr<Connection>(new Connection(io_context_,
                                                                 &(outputQueue[connectionVector.size()]),
                                                                 &(inputQueue[connectionVector.size()]))));

        acceptor_.async_accept(connectionVector[connectionVector.size() - 1]->socket(),
                               boost::bind(&server::handle_accept, this,
                                           boost::asio::placeholders::error));
    }

    void server::handle_accept(const boost::system::error_code& e)
    {
        if (!e)
        {
            connectionVector[connectionVector.size() - 1]->start();
        }

        start_accept();
    }

} // namespace gameServer

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>

//int main()
//{
//
//
//    return 0;
//}