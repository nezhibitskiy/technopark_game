
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
              acceptor_(io_context_),
              new_connection_()
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
        for (unsigned int i = 0; i < clientConnectedCount; i++) {
            outputQueue[i]->push(tmpEventMsg);
        }
    }
    BaseMessage **server::checkRequests(unsigned int *reqMsgCount)
    {
        std::vector<BaseMessage> returnVector;

        for (unsigned int i = 0; i < clientConnectedCount; i++) {
            if (!inputQueue[i]->empty()) {
                while (!inputQueue[i]->empty()) {
                    returnVector.push_back(inputQueue[i]->front());
                    inputQueue[i]->pop();
                }
            }
        }

        if (returnVector.size() == 0) return nullptr;
        *reqMsgCount = returnVector.size();
        BaseMessage **returnMessages = new BaseMessage*[*reqMsgCount];
        for (unsigned int i = 0; i < *reqMsgCount; i++) {
            returnMessages[i] = new BaseMessage(returnVector[i].getType(),
                                                i + 1,
                                                returnVector[i].getX(),
                                                returnVector[i].getY());
        }
        return returnMessages;
    }

    void server::closeServer() {
        if (io_context_.stopped()) return;
        for (std::size_t i = 0; i < clientConnectedCount; ++i) {
            connectionVector[i]->stop();
        }
        io_context_.stop();
        // Wait for all threads in the pool to exit.
        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i]->join();
    }

    void server::start_accept()
    {
        new_connection_.reset(new Connection(io_context_,
                &(outputQueue[clientConnectedCount]),
                &(inputQueue[clientConnectedCount])));

        acceptor_.async_accept(new_connection_->socket(),
                               boost::bind(&server::handle_accept, this,
                                           boost::asio::placeholders::error));
    }

    void server::handle_accept(const boost::system::error_code& e)
    {
        if (!e)
        {
            connectionVector.push_back(new_connection_);
            clientConnectedCount++;
            new_connection_->start();
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