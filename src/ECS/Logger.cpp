/*
** EPITECH PROJECT, 2023
** Raytracer
** File description:
** Logger.cpp
*/

#include "Logger.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include "Registry.hpp"
#include "date/date.h"

namespace Logger {
    void fatal(const std::string &message)
    {
        Registry::getInstance().getLogger().fatal(message);
    }
    void error(const std::string &message)
    {
        Registry::getInstance().getLogger().error(message);
    }
    void warn(const std::string &message)
    {
        Registry::getInstance().getLogger().warn(message);
    }
    void info(const std::string &message)
    {
        Registry::getInstance().getLogger().info(message);
    }
    void debug(const std::string &message)
    {
        Registry::getInstance().getLogger().debug(message);
    }
    void trace(const std::string &message)
    {
        Registry::getInstance().getLogger().trace(message);
    }

    Logger::Logger(LogLevel logLevel)
        : _logLevel(logLevel)
#ifdef _WIN32
          ,
          _hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
#endif
    {
    }

    void Logger::fatal(const std::string &message)
    {
        if (_logLevel < LogLevel::Fatal) {
            return;
        }
        Logger::print(LogLevel::Fatal, "FATAL", message);
    }

    void Logger::error(const std::string &message)
    {
        if (_logLevel < LogLevel::Error) {
            return;
        }
        Logger::print(LogLevel::Error, "ERROR", message);
    }

    void Logger::warn(const std::string &message)
    {
        if (_logLevel < LogLevel::Warn) {
            return;
        }
        Logger::print(LogLevel::Warn, "WARN", message);
    }

    void Logger::info(const std::string &message)
    {
        if (_logLevel < LogLevel::Info) {
            return;
        }
        Logger::print(LogLevel::Info, "INFO", message);
    }

    void Logger::debug(const std::string &message)
    {
#ifdef NDEBUG
        return;
#else
        if (_logLevel < LogLevel::Debug) {
            return;
        }
        Logger::print(LogLevel::Debug, "DEBUG", message);
#endif
    }

    void Logger::trace(const std::string &message)
    {
#ifdef NDEBUG
        return;
#else
        if (_logLevel < LogLevel::Trace) {
            return;
        }
        Logger::print(LogLevel::Trace, "TRACE", message);
#endif
    }

    void Logger::print(LogLevel levelT, const std::string &level, const std::string &message)
    {
        auto const now = std::chrono::system_clock::now();
        auto it        = _callbacks.find(levelT);
        std::stringstream s;
        std::string mes;

        {
            using namespace date;
            s << now << " [" << level << "] " << message;
        }
        mes = s.str();

#ifdef _WIN32
        static std::map<LogLevel, WORD> colors = {
            {LogLevel::Fatal,       static_cast<WORD>(FOREGROUND_RED | FOREGROUND_INTENSITY)                    },
            {LogLevel::Error,       static_cast<WORD>(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)  },
            {LogLevel::Warn,        static_cast<WORD>(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY)},
            {LogLevel::Info,        static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) },
            {LogLevel::Debug,       static_cast<WORD>(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)},
            {LogLevel::Trace,       static_cast<WORD>(FOREGROUND_RED | FOREGROUND_INTENSITY)                    },
            {LogLevel::MAXLOGLEVEL, static_cast<WORD>(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)      },
        };

        if (_hConsole != INVALID_HANDLE_VALUE) {
            SetConsoleTextAttribute(_hConsole, colors[levelT]);
            std::cerr << mes << std::endl;
            SetConsoleTextAttribute(_hConsole, colors[LogLevel::MAXLOGLEVEL]);
        } else {
            std::cerr << message << std::endl;
        }

#else
        static std::map<LogLevel, std::string> colors = {
            {LogLevel::Fatal,       "\033[31m"},
            {LogLevel::Error,       "\033[33m"},
            {LogLevel::Warn,        "\033[34m"},
            {LogLevel::Info,        "\033[32m"},
            {LogLevel::Debug,       "\033[38m"},
            {LogLevel::Trace,       "\033[30m"},
            {LogLevel::MAXLOGLEVEL, "\033[0m" },
        };

        std::cerr << colors[levelT] << mes << colors[LogLevel::MAXLOGLEVEL] << std::endl;
#endif

        if (it != _callbacks.end()) {
            for (auto &it1 : it->second) {
                it1.second(mes);
            }
        }
    }

    void Logger::subscribeCallback(
        LogLevel type,
        const std::string &name,
        std::function<void(const std::string &)> callback)
    {
        if (_callbacks.find(type) == _callbacks.end()) {
            _callbacks.emplace(type, std::map<std::string, std::function<void(const std::string &)>>());
        }
        _callbacks[type].emplace(name, callback);
    }

    void Logger::unsubscribeCallback(LogLevel type, const std::string &name)
    {
        if (_callbacks.find(type) == _callbacks.end()) {
            return;
        }
        if (_callbacks[type].find(name) == _callbacks[type].end()) {
            return;
        }
        _callbacks[type].erase(name);
    }

    void Logger::setLogLevel(LogLevel logLevel)
    {
        _logLevel = logLevel;
    }

    LogLevel Logger::getLogLevel() const
    {
        return _logLevel;
    }
} // namespace Logger
