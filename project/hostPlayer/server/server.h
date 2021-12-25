#ifndef BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H
#define BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <thread>

#include "connection.h"

namespace gameServer {

    class server
            : private boost::noncopyable
    {
    public:
        enum Type {
            CONNECTING_CLIENT = 6,
            ADD_CLIENT_TO_TEAM = 7
        };

        explicit server(std::size_t rClientCount);

        bool init(const std::string& address, const std::string& port);
        /// Run the hostPlayer's io_context loop.
        void Run(EventMessage tmpEventMsg);
        BaseMessage **CheckRequests(unsigned int *reqMsgCount);
        /// Handle a request to stop the hostPlayer.
        void CloseServer();

        bool closeGameReq;

    private:
        /// Initiate an asynchronous accept operation.
        void StartAccept();

        /// Handle completion of an asynchronous accept operation.
        void HandleAccept(const boost::system::error_code& e);

        /// The number of threads that will call io_context::run().
        std::size_t thread_pool_size_;

        std::size_t clientCount;

        /// The io_context used to perform asynchronous operations.
        boost::asio::io_context io_context_;

        /// The signal_set is used to register for process termination notifications.
        boost::asio::signal_set signals_;

        /// Acceptor used to listen for incoming connections.
        boost::asio::ip::tcp::acceptor acceptor_;

        /// The next connection to be accepted.
        std::vector<boost::shared_ptr<Connection>> connectionVector;

        std::queue<BaseMessage> **inputQueue;
        std::queue<EventMessage> **outputQueue;

        std::vector<boost::shared_ptr<std::thread>> threads;

        /// The handler for all incoming requests.
//            Router<Response(*)(const Request &request)> request_router;
    };

} // namespace gameServer

#endif //BOOST_ASIO_SERVER_ASYNC_HTTP_SERVER_H