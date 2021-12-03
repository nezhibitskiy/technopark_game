
#ifndef BOOST_ASIO_SERVER_ROUTER_H
#define BOOST_ASIO_SERVER_ROUTER_H

#include <map>
#include "message.h"

namespace gameServer {
    template<typename Handler>
    class MessageManager {
    public:
        void addHandler(unsigned char messageType, Handler handler) {
            m_Handlers.emplace(messageType, handler);
        }

        Response processRoute(const unsigned char &messageType, const EventMessage &EventMessage) {
            auto handler_it = m_Handlers.find(messageType);

            if (handler_it != m_Handlers.end()) {
                return (handler_it->second)(EventMessage);
            }

            Response response;
            response.status_code = 404;
            response.status_message = "Not Found";
            response.http_version = "HTTP/1.0";
            response.body = "Not Found";
            return response;
        }

    private:
        std::map<unsigned char, Handler> m_Handlers;
    };
}


#endif //BOOST_ASIO_SERVER_ROUTER_H