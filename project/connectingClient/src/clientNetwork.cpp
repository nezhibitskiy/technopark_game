//
// Created by ilyas on 07.12.2021.
//

#include "clientNetwork.h"

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

Client::Client(const std::string& server, const std::string& port,
       std::queue<EventMessage> **rInputQueue,
       std::queue<BaseMessage> **rOutputQueue)
        : resolver_(io_context),
          signals_(io_context),
          socket_(io_context)
{
    // Register to handle the signals that indicate when the hostPlayer should exit.
    signals_.add(SIGINT);   // остановка процесса с терминала
    signals_.add(SIGTERM);  // сигнал от kill
    signals_.async_wait(boost::bind(&Client::endServ, this));

    inputQueue = new std::queue<EventMessage>;
    outputQueue = new std::queue<BaseMessage>;

    *rInputQueue = inputQueue;
    *rOutputQueue = outputQueue;

    // Start an asynchronous resolve to translate the hostPlayer and service names
    // into a list of endpoints.
    resolver_.async_resolve(server, port,
                            boost::bind(&Client::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::results));
}



void Client::run() {
    for (std::size_t i = 0; i < 2; ++i)
    {
        boost::shared_ptr<std::thread> thread(new std::thread(
                boost::bind(&boost::asio::io_context::run, &io_context)));

        threads.push_back(thread);
    }

}
void Client::endServ() {
    // Wait for all threads in the pool to exit.
    io_context.stop();
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void Client::handle_resolve(const boost::system::error_code& err,
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
void Client::handle_connect(const boost::system::error_code& err)
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

void Client::handle_write_request(const boost::system::error_code& err)
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
        EventMessage inputMessage(EventMessage::CLOSE_GAME, 0, 0, 0, 0);
        inputQueue->push(inputMessage);
        io_context.stop();
    }
}

void Client::handle_read(const boost::system::error_code& err)
{
    if (!err)
    {

        // std::cout << inputBuffer_.data() << std::endl;

        EventMessage inputMessage = parse(inputBuffer_);
//            std::cout << "Input message type: " << inputMessage.getType();
//            std::cout << "; Input message ID: " << inputMessage.getID();
//            std::cout << "; Input message X: " << inputMessage.getX();
//            std::cout << "; Input message Y: " << inputMessage.getY();
//            std::cout << "; Input message Data: " << inputMessage.getData() << ";" << std::endl;

        inputQueue->push(inputMessage);

        boost::asio::async_read(socket_, boost::asio::buffer(inputBuffer_),
                                boost::asio::transfer_at_least(1),
                                boost::bind(&Client::handle_read, this,
                                            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error 6: " << err << "\n";
        EventMessage inputMessage(EventMessage::CLOSE_GAME, 0, 0, 0, 0);
        inputQueue->push(inputMessage);
        io_context.stop();
    }
}