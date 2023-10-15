/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** INitwork
*/

#pragma once

#include <any>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include <map>

extern "C"
{
#include "Nitwork.h"
}

namespace Nitwork {
    using actionHandler = std::function<void(std::any &, boost::asio::ip::udp::endpoint &)>;
    using handleBodyT   = std::function<void(actionHandler &, const struct header_s &)>;

    class SenderData {
        public:
            SenderData(n_id_t id, boost::asio::ip::udp::endpoint &endpoint, std::any data)
                : id(id),
                  endpoint(endpoint),
                  data(std::move(data))
            {
            }

            n_id_t id;
            boost::asio::ip::udp::endpoint endpoint;
            std::any data;
    };
    class Packet {
        public:
            Packet(n_actionType_t action, std::any body, const boost::asio::ip::udp::endpoint &endpoint)
                : action(action),
                  body(std::move(body)),
                  endpoint(endpoint)
            {
            }

            Packet(n_actionType_t action, std::any body) : action(action), body(std::move(body))
            {
            }

            Packet(const Packet &packet, const boost::asio::ip::udp::endpoint &endpoint)
                : id(packet.id),
                  action(packet.action),
                  body(packet.body),
                  endpoint(endpoint)
            {
            }
            [[nodiscard]] bool getIsResend() const
            {
                return _isResend;
            }
            void setIsResend(bool value)
            {
                _isResend = value;
            }

            n_id_t id = 0;
            n_actionType_t action;
            std::any body;
            boost::asio::ip::udp::endpoint endpoint;

        private:
            bool _isResend = false;
    };

    using actionSender = std::function<void(Packet &)>;

    class INitwork {
        public:
            virtual ~INitwork()               = default;
            INitwork(const INitwork &)        = delete;
            INitwork(const INitwork &&)       = delete;
            void operator=(const INitwork &)  = delete;
            void operator=(const INitwork &&) = delete;

            // start the NitworkServer
            virtual bool start(int port, int threadNb, int tick, const std::string &ip = "") = 0;

            virtual void stop() = 0;

            virtual bool isRunning() const = 0;

        protected:
            INitwork() = default;
            // start the NitworkServer config
            virtual bool startNitworkConfig(int port, const std::string &ip) = 0;
            // start the NitworkServer threads (context threads, clock thread, input thread and output
            // thread)
            virtual bool startNitworkThreads(int threadNb, int tick) = 0;
            // start the context threads
            virtual bool startContextThreads(int threadNb) = 0;
            // start the clock thread
            virtual bool startClockThread(int tick) = 0;
            // start the input thread inside the context (post)
            virtual void startInputHandler() = 0;
            // start the output thread inside the context (post)
            virtual void startOutputHandler() = 0;

            // start receive handler
            virtual void startReceiveHandler() = 0;
            // handler func for receive handler which handle the header
            virtual void
            headerHandler(std::size_t bytes_received, const boost::system::error_code &error) = 0;
            // handler func for headerHandler which handle the action
            virtual void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) = 0;

            // getters
            [[nodiscard]] virtual const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const = 0;
    }; // class INitwork
} // namespace Nitwork
