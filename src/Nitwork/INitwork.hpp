/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** INitwork
*/

#pragma once

#include <any>
#include <map>
#include <boost/array.hpp>
#include <boost/asio.hpp>

extern "C"
{
#include "Nitwork.h"
}

namespace Nitwork {
    using actionHandler = std::function<void(std::any &, boost::asio::ip::udp::endpoint &)>;
    using handleBodyT = std::function<void(
        const struct header_s &,
        actionHandler &
    )>;

    class SenderData {
        public:
            SenderData(boost::asio::ip::udp::endpoint &endpoint, std::any data)
                : endpoint(endpoint),
                  data(std::move(data))
            {
            }

            boost::asio::ip::udp::endpoint endpoint;
            std::any data;
    };
    struct packet_s {
            n_actionType_t action;
            std::any body;
    };

    class INitwork {
        public:
            virtual ~INitwork() = default;
            INitwork(const INitwork &) = delete;
            INitwork(const INitwork &&) = delete;
            void operator=(const INitwork &) = delete;
            void operator=(const INitwork &&) = delete;

            // start the NitworkServer
            virtual bool start(int port, int threadNb, int tick, const std::string &ip = "") = 0;

            virtual void stop() = 0;
        protected:
            INitwork() = default;
            // start the NitworkServer config
            virtual bool startNitworkConfig(int port, const std::string &ip) = 0;
            // start the NitworkServer threads (context threads, clock thread, input thread and output thread)
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
            virtual void headerHandler(
                std::size_t bytes_received,
                const boost::system::error_code &error) = 0;
            // handler func for headerHandler which handle the action
            virtual void handleBodyAction(
                const struct header_s header,
                const boost::asio::ip::udp::endpoint &endpoint) = 0;

            // getters
            [[nodiscard]] virtual const std::map<
                enum n_actionType_t,
                actionHandler
            >& getActionToSendHandlers() const = 0;
    }; // class INitwork
} // namespace NitworkServer