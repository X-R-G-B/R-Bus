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
        void restart(std::size_t id);

    private:
        std::vector<std::optional<std::chrono::high_resolution_clock::time_point>> _clocks;
};
