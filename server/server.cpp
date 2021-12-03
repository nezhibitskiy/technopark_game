
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

        // Register to handle the signals that indicate when the server should exit.
        signals_.add(SIGINT);   // остановка процесса с терминала
        signals_.add(SIGTERM);  // сигнал от kill
        signals_.async_wait(boost::bind(&server::handle_stop, this));

        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        boost::asio::ip::tcp::resolver resolver(io_context_);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        start_accept();
    }

    void server::run()
    {
        // Create a pool of threads to run all of the io_contexts.
        for (std::size_t i = 0; i < thread_pool_size_; ++i)
        {
            boost::shared_ptr<std::thread> thread(new std::thread(
                    boost::bind(&boost::asio::io_context::run, &io_context_)));

            threads.push_back(thread);
        }

        while (clientConnectedCount < 1) {

        }
        // QUEUE HERE
        for (unsigned int i = 0; i < clientConnectedCount; i++) {
            outputQueue[i]->push(EventMessage(EventMessage::CREATE_MAP, 0,40,40,0));
            outputQueue[i]->push(EventMessage(EventMessage::CREATE_OBJECT, 1,1,2,0));
        }

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
            clientConnectedCount++;
            new_connection_->start();
        }

        start_accept();
    }

    void server::handle_stop()
    {
        io_context_.stop();
        std::cout << "STOP COMMAND" << std::endl;

        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i]->join();
    }

} // namespace gameServer

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>

int main()
{
    try
    {
        // Initialise the server.
        std::size_t num_threads = 1;
        gameServer::server gameServer("0.0.0.0", "5000", num_threads);

        // Run the server until stopped.
        gameServer.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}