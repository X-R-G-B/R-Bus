#include <chrono>
#include "Clock.hpp"

std::size_t Clock::create()
{
    _clocks.emplace_back(std::chrono::high_resolution_clock::now());
    return _clocks.size() - 1;
}

std::size_t Clock::elapsedMillisecondsSince(std::size_t id)
{
    auto val = std::chrono::high_resolution_clock::now() - _clocks[id];
    return std::chrono::duration_cast<std::chrono::milliseconds>(val).count();
}

std::size_t Clock::elapsedSecondesSince(std::size_t id)
{
    auto val = std::chrono::high_resolution_clock::now() - _clocks[id];
    return std::chrono::duration_cast<std::chrono::seconds>(val).count();
}

std::size_t Clock::elapsedNanosecondsSince(std::size_t id)
{
    auto val = std::chrono::high_resolution_clock::now() - _clocks[id];
    return std::chrono::duration_cast<std::chrono::nanoseconds>(val).count();
}

void Clock::restart(std::size_t id)
{
    _clocks[id] = std::chrono::high_resolution_clock::now();
}
