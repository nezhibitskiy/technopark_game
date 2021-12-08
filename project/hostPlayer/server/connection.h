#ifndef BOOST_ASIO_SERVER_CONNECTION_H
#define BOOST_ASIO_SERVER_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>

#include "response.h"
#include "request.h"
// #include "router.h"
#include "message.h"
#include <queue>

namespace gameServer {

    class Connection
            : public boost::enable_shared_from_this<Connection>,
              private boost::noncopyable
    {
    public:
        explicit Connection(boost::asio::io_context& io_context,
                std::queue<EventMessage> **rOutputQueue,
                std::queue<BaseMessage> **rInputQueue);

        boost::asio::ip::tcp::socket& socket();
        void start();

    private:
        void handle_read(const boost::system::error_code& e);
        void handle_write(const boost::system::error_code& e);

    private:
        /// Socket for the connection.
        boost::asio::ip::tcp::socket socket_;

        boost::array<unsigned char, 12> inputBuffer_;
        boost::array<unsigned char, 14> outputBuffer_;

        boost::asio::streambuf response_buf_;

        std::queue<BaseMessage> *inputQueue;
        std::queue<EventMessage> *outputQueue;
    };
} // namespace gameServer

#endif //BOOST_ASIO_SERVER_CONNECTION_H