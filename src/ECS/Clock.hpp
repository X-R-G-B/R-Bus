#pragma once

#include <chrono>
#include <cstddef>
#include <vector>

class Clock {
    public:
        Clock() = default;

        std::size_t create();
        std::size_t elapsedSecondesSince(std::size_t id);     // 1
        std::size_t elapsedMillisecondsSince(std::size_t id); // 10^-3
        std::size_t elapsedNanosecondsSince(std::size_t id);  // 10^-9
        void restart(std::size_t id);

    private:
        std::vector<std::chrono::high_resolution_clock::time_point> _clocks;
};
