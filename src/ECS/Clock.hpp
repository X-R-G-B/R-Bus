#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <vector>
#include <chrono>

class Clock {
    public:
        Clock() = default;

        std::size_t create(bool deferStart = false)
        {
            if (deferStart) {
                _clocks.emplace_back(std::nullopt);
            } else {
                _clocks.emplace_back(std::chrono::high_resolution_clock::now());
            }
            return _clocks.size() - 1;
        }

        std::size_t elapsedMillisecondsSince(std::size_t id)
        {
            if (!_clocks[id].has_value()) {
                return static_cast<std::size_t>(-1);
            }
            auto val = std::chrono::high_resolution_clock::now() - _clocks[id].value();
            return std::chrono::duration_cast<std::chrono::milliseconds>(val).count();
        }

        std::size_t elapsedSecondsSince(std::size_t id)
        {
            if (!_clocks[id].has_value()) {
                return static_cast<std::size_t>(-1);
            }
            auto val = std::chrono::high_resolution_clock::now() - _clocks[id].value();
            return std::chrono::duration_cast<std::chrono::seconds>(val).count();
        }

        std::size_t elapsedNanosecondsSince(std::size_t id)
        {
            if (!_clocks[id].has_value()) {
                return static_cast<std::size_t>(-1);
            }
            auto val = std::chrono::high_resolution_clock::now() - _clocks[id].value();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(val).count();
        }

        void decreaseSeconds(std::size_t id, std::size_t seconds)
        {
            if (!_clocks[id].has_value()) {
                return;
            }
            _clocks[id].value() += std::chrono::seconds(seconds);
        }

        void decreaseMilliseconds(std::size_t id, std::size_t milliseconds)
        {
            if (!_clocks[id].has_value()) {
                return;
            }
            _clocks[id].value() += std::chrono::milliseconds(milliseconds);
        }

        void decreaseNanoseconds(std::size_t id, std::size_t nanoseconds)
        {
            if (!_clocks[id].has_value()) {
                return;
            }
            _clocks[id].value() += std::chrono::nanoseconds(nanoseconds);
        }

        void restart(std::size_t id)
        {
            _clocks[id] = std::chrono::high_resolution_clock::now();
        }

    private:
        std::vector<std::optional<std::chrono::high_resolution_clock::time_point>> _clocks;
};
