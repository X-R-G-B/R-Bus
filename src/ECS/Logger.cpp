/*
** EPITECH PROJECT, 2023
** Raytracer
** File description:
** Logger.cpp
*/

#include "Logger.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <string>

namespace Logger {
    Logger::Logger(int logLevel) : _logLevel(logLevel)
    {
    }

    void Logger::fatal(const std::string &message)
    {
        if (_logLevel < _LOG_FATAL_VALUE) {
            return;
        }
        Logger::print(_LOG_FATAL_VALUE, "FATAL", message);
    }

    void Logger::error(const std::string &message)
    {
        if (_logLevel < _LOG_ERROR_VALUE) {
            return;
        }
        Logger::print(_LOG_ERROR_VALUE, "ERROR", message);
    }

    void Logger::warn(const std::string &message)
    {
        if (_logLevel < _LOG_WARN_VALUE) {
            return;
        }
        Logger::print(_LOG_WARN_VALUE, "WARN", message);
    }

    void Logger::info(const std::string &message)
    {
        if (_logLevel < _LOG_INFO_VALUE) {
            return;
        }
        Logger::print(_LOG_INFO_VALUE, "INFO", message);
    }

    void Logger::debug(const std::string &message)
    {
#ifdef NDEBUG
        return;
#else
        if (_logLevel < _LOG_DEBUG_VALUE) {
            return;
        }
        Logger::print(_LOG_DEBUG_VALUE, "DEBUG", message);
#endif
    }

    void Logger::trace(const std::string &message)
    {
#ifdef NDEBUG
        return;
#else
        if (_logLevel < _LOG_TRACE_VALUE) {
            return;
        }
        Logger::print(_LOG_TRACE_VALUE, "TRACE", message);
#endif
    }

    void Logger::print(
        int levelT,
        const std::string &level,
        const std::string &message)
    {
#ifdef __linux__
        static std::map<int, std::string> colors = {
            {_LOG_FATAL_VALUE, "\033[31m"},
            {_LOG_ERROR_VALUE, "\033[33m"},
            {_LOG_WARN_VALUE,  "\033[34m"},
            {_LOG_INFO_VALUE,  "\033[32m"},
            {_LOG_DEBUG_VALUE, "\033[38m"},
            {_LOG_TRACE_VALUE, "\033[30m"},
            {_LOG_RESET_COLOR, "\033[0m" },
        };
#elif __APPLE__
        static std::map<int, std::string> colors = {
            {_LOG_FATAL_VALUE, "\033[31m"},
            {_LOG_ERROR_VALUE, "\033[33m"},
            {_LOG_WARN_VALUE,  "\033[34m"},
            {_LOG_INFO_VALUE,  "\033[32m"},
            {_LOG_DEBUG_VALUE, "\033[38m"},
            {_LOG_TRACE_VALUE, "\033[30m"},
            {_LOG_RESET_COLOR, "\033[0m" },
        };
#else
        static std::map<int, std::string> colors =
            {
                {_LOG_FATAL_VALUE, ""},
                {_LOG_ERROR_VALUE, ""},
                {_LOG_WARN_VALUE,  ""},
                {_LOG_INFO_VALUE,  ""},
                {_LOG_DEBUG_VALUE, ""},
                {_LOG_TRACE_VALUE, ""},
                {_LOG_RESET_COLOR, ""},
        }
#endif

        std::time_t rawtime = 0;
        struct tm *timeinfo = nullptr;
        std::string time;
        std::string mes;
        auto it = _callbacks.find(levelT);

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        time     = std::asctime(timeinfo);
        time.erase(time.find_last_of("\n"));
        mes = time + " [" + level + "] " + message;
        std::cerr << colors[levelT] << mes << colors[_LOG_RESET_COLOR]
                  << std::endl;
        if (it != _callbacks.end()) {
            for (auto &it1 : it->second) {
                it1.second(mes);
            }
        }
    }

    void Logger::subscribeCallback(
        int type,
        const std::string &name,
        std::function<void(const std::string &)> callback)
    {
        if (_callbacks.find(type) == _callbacks.end()) {
            _callbacks.emplace(
                type,
                std::map<
                    std::string,
                    std::function<void(const std::string &)>>());
        }
        _callbacks[type].emplace(name, callback);
    }

    void Logger::unsubscribeCallback(int type, const std::string &name)
    {
        if (_callbacks.find(type) == _callbacks.end()) {
            return;
        }
        if (_callbacks[type].find(name) == _callbacks[type].end()) {
            return;
        }
        _callbacks[type].erase(name);
    }
} // namespace Logger
