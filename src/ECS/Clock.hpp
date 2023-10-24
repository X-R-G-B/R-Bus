#pragma once

#include <chrono>
#include <cstddef>
#include <optional>
#include <vector>

class Clock {
    public:
        Clock() = default;

        std::size_t create(bool deferStart = false);
        std::size_t elapsedSecondsSince(std::size_t id);      // 1
        std::size_t elapsedMillisecondsSince(std::size_t id); // 10^-3
        std::size_t elapsedNanosecondsSince(std::size_t id);  // 10^-9
        void decreaseSeconds(std::size_t id, std::size_t seconds);
        void decreaseMilliseconds(std::size_t id, std::size_t milliseconds);
        void decreaseNanoseconds(std::size_t id, std::size_t nanoseconds);
        void restart(std::size_t id);
        void restart(std::size_t id, std::time_t time);

    private:
        std::vector<std::optional<std::chrono::high_resolution_clock::time_point>> _clocks;
};
