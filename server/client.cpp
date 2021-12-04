#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>

#include "message.h"

using boost::asio::ip::tcp;

EventMessage parse(const boost::array<unsigned char, 14> &data) {
    EventMessage returnMessage((data[0] | data[1] << 8), (data[2] | data[3] << 8),
                              (data[4] | data[5] << 8 | data[6] << 16 | data[7] << 24),
                              (data[8] | data[9] << 8 | data[10] << 16 | data[11] << 24),
                              (data[12] | data[13] << 8));
    return returnMessage;
}

boost::array<unsigned char, 12> pack(const BaseMessage &data) {
    boost::array<unsigned char, 12> returnValue;
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

    return returnValue;
}

class Client
{
public:
    Client(boost::asio::io_context& io_context,
           const std::string& server, const std::string& port)
            : resolver_(io_context),
              socket_(io_context)
    {
        inputQueue = new std::queue<EventMessage>;
        outputQueue = new std::queue<BaseMessage>;

        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        resolver_.async_resolve(server, port,
                                boost::bind(&Client::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::results));
    }

    void run(boost::asio::io_context *io_context) {
        std::vector<boost::shared_ptr<std::thread> > threads;
        for (std::size_t i = 0; i < 2; ++i)
        {
            boost::shared_ptr<std::thread> thread(new std::thread(
                    boost::bind(&boost::asio::io_context::run, io_context)));

            threads.push_back(thread);
        }

        outputQueue->push(BaseMessage(0, 3,0,0));
        outputQueue->push(BaseMessage(0, 2,0,0));


        // Wait for all threads in the pool to exit.
        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i]->join();
    }
private:
    void handle_resolve(const boost::system::error_code& err,
                        const tcp::resolver::results_type& endpoints)
    {
        if (!err)
        {
            // Attempt a connection to each endpoint in the list until we
            // successfully establish a connection.
            boost::asio::async_connect(socket_, endpoints,
                                       boost::bind(&Client::handle_connect, this,
                                                   boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error 1: " << err.message() << "\n";
        }
    }

    void handle_connect(const boost::system::error_code& err)
    {
        if (!err)
        {
            boost::asio::async_read(socket_, boost::asio::buffer(inputBuffer_),
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&Client::handle_read, this,
                                                boost::asio::placeholders::error));

            while(outputQueue->empty() && (socket_.is_open())) {}

            if (socket_.is_open()) {
                outputBuffer_ = pack(outputQueue->front());

                // The connection was successful. Send the request.
                boost::asio::async_write(socket_, boost::asio::buffer(outputBuffer_.data(), outputBuffer_.size()),
                                         boost::bind(&Client::handle_write_request, this,
                                                     boost::asio::placeholders::error));
            }
        }
        else
        {
            std::cout << "Error 2: " << err.message() << "\n";


            // MAKE TIMER AND ATTEMPT COUNT FOR RECONNECTION
//            resolver_.async_resolve(request_.host, request_.port,
//                            boost::bind(&Client::handle_resolve, this,
//                                    boost::asio::placeholders::error,
//                                    boost::asio::placeholders::results));
        }
    }

    void handle_write_request(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Read the response status line. The response_ streambuf will
            // automatically grow to accommodate the entire line. The growth may be
            // limited by passing a maximum size to the streambuf constructor.
            outputQueue->pop();

            while(outputQueue->empty() && (socket_.is_open())) {}

            if (socket_.is_open()) {
                outputBuffer_ = pack(outputQueue->front());

                // The connection was successful. Send the request.
                boost::asio::async_write(socket_, boost::asio::buffer(outputBuffer_.data(), outputBuffer_.size()),
                                         boost::bind(&Client::handle_write_request, this,
                                                     boost::asio::placeholders::error));
            }
        }
        else
        {
            std::cout << "Error 3: " << err.message() << "\n";
        }
    }

    void handle_read(const boost::system::error_code& err)
    {
        if (!err)
        {

//            std::istream response_stream(&response_buf_);
//            response_.body = std::string(std::istreambuf_iterator<char>(response_stream), std::istreambuf_iterator<char>());
//            std::cout << response_.body << std::endl;
            // Continue reading remaining data until EOF.

            std::cout << inputBuffer_.data() << std::endl;

            EventMessage inputMessage = parse(inputBuffer_);
            std::cout << "Input message type: " << inputMessage.getType();
            std::cout << "; Input message ID: " << inputMessage.getID();
            std::cout << "; Input message X: " << inputMessage.getX();
            std::cout << "; Input message Y: " << inputMessage.getY();
            std::cout << "; Input message Data: " << inputMessage.getData() << ";" << std::endl;

            inputQueue->push(inputMessage);

            boost::asio::async_read(socket_, boost::asio::buffer(inputBuffer_),
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&Client::handle_read, this,
                                                boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error 6: " << err << "\n";
        }
    }


private:
    tcp::resolver resolver_;
    tcp::socket socket_;

    boost::asio::streambuf response_buf_;


    boost::array<unsigned char, 14> inputBuffer_;
    boost::array<unsigned char, 12> outputBuffer_;


    std::queue<EventMessage> *inputQueue;
    std::queue<BaseMessage> *outputQueue;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;

        Client c(io_context, "0.0.0.0", "5000");
        c.run(&io_context);
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}