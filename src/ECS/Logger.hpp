/*
** EPITECH PROJECT, 2023
** Raytracer
** File description:
** Logger.hpp
*/

#pragma once

#include <functional>
#include <map>
#include <string>

#ifdef _WIN32
    #define NOGDI  // All GDI defines and routines
    #define NOUSER // All USER defines and routines
    #ifdef BOOST_OS_WINDOWS
        #define _WIN32_WINNT 0x0501
        #if _WIN32_WINNT <= 0x0501
            #define BOOST_ASIO_DISABLE_IOCP
            #define BOOST_ASIO_ENABLE_CANCELIO
        #endif
    #endif
extern "C" {
    #include <windows.h>
}
    // Because raylib uses these names as function parameters
    #undef near
    #undef far
    #undef min
    #undef max
#endif

namespace Logger {
    enum class LogLevel : int {
        NOLOG       = -1,
        Fatal       = 0,
        Error       = 1,
        Warn        = 2,
        Info        = 3,
        Debug       = 4,
        Trace       = 5,
        MAXLOGLEVEL = 6,
    };

    void fatal(const std::string &message);
    void error(const std::string &message);
    void warn(const std::string &message);
    void info(const std::string &message);
    void debug(const std::string &message);
    void trace(const std::string &message);

    /**
     * @brief Logger
     *
     * LogLevel:
     * LogLevel::NOLOG - no messages
     * LogLevel::Fatal - only `fatal` are displayed
     * LogLevel::Error - `error` and `fatal` are displayed
     * LogLevel::Warn - `warn`, `error` and `fatal` are displayed
     * LogLevel::Info - `info`, `warn`, `error` and `fatal` are displayed
     * LogLevel::Debug - `debug`, `info`, `warn`, `error` and `fatal` are
     * displayed LogLevel::Trace - `trace`, `debug`, `info`, `warn`, `error` and
     * `fatal` are displayed
     *
     * Attention:
     * `debug` and `trace` will only do something in debug mode compilation.
     * (`set(CMAKE_BUILD_TYPE Debug)` in `CMakeLists.txt`)
     */
    class Logger {
        public:
            Logger(LogLevel logLevel = LogLevel::Info);
            /**
             * @brief Logger fata
             *
             * Means that something critical is broken, and the application
             * cannot continue to do any more useful work without the
             * intervention of an engineer.
             *
             * @param message the message
             */
            void fatal(const std::string &message);
            /**
             * @brief Logger error
             *
             * The ERROR log level is used to represent error conditions in
             * an application that prevent a specific operation from running,
             * but the application itself can continue working even if it is
             * at a reduced level of functionality or performance.
             *
             * @param message the message
             */
            void error(const std::string &message);
            /**
             * @brief Logger warn
             *
             * Messages logged at the WARN level typically indicate that
             * something unexpected happened, but the application can recover
             * and continue to function normally.
             *
             * @param message the message
             */
            void warn(const std::string &message);
            /**
             * @brief Logger info
             *
             * INFO-level messages indicate events in the system that are
             * significant to the business purpose of the application. Such
             * events are logged to show that the system is operating normally.
             *
             * @param message the message
             */
            void info(const std::string &message);
            /**
             * @brief Logger debug
             *
             * The DEBUG level is used for logging messages that help
             * developers find out what went wrong during a debugging session.
             *
             * @param message the message
             */
            void debug(const std::string &message);
            /**
             * @brief Logger trace
             *
             * The TRACE level is used for tracing the path of code execution
             * in a program.
             *
             * @param message the message
             */
            void trace(const std::string &message);
            /**
             * @brief Logger subscribe
             *
             * @param type the type to subscribe for
             * @param name the name
             * @param callback the callback
             */
            void subscribeCallback(
                LogLevel type,
                const std::string &name,
                std::function<void(const std::string &)> callback);
            /**
             * @brief Logger remove
             *
             * @param type the type
             * @param name the name
             */
            void unsubscribeCallback(LogLevel type, const std::string &name);
            /**
             * @brief set log level
             *
             * @param logLevel the log level
             */
            void setLogLevel(LogLevel logLevel);
            /**
             * @brief get log level
             *
             * @return the log level
             */
            [[nodiscard]] LogLevel getLogLevel() const;

        private:
            void print(LogLevel levelT, const std::string &level, const std::string &message);
            std::map<LogLevel, std::map<std::string, std::function<void(const std::string &)>>> _callbacks;
            LogLevel _logLevel;

#ifdef _WIN32
            HANDLE _hConsole;
#endif
    };
} // namespace Logger
