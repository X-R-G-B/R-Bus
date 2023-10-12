
#include "Clock.hpp"
#include <chrono>

std::size_t Clock::create(bool deferStart)
{
    if (deferStart) {
        _clocks.emplace_back(std::nullopt);
    } else {
        _clocks.emplace_back(std::chrono::high_resolution_clock::now());
    }
    return _clocks.size() - 1;
}

std::size_t Clock::elapsedMillisecondsSince(std::size_t id)
{
    if (!_clocks[id].has_value()) {
        return static_cast<std::size_t>(-1);
    }
    auto val = std::chrono::high_resolution_clock::now() - _clocks[id].value();
    return std::chrono::duration_cast<std::chrono::milliseconds>(val).count();
}

std::size_t Clock::elapsedSecondsSince(std::size_t id)
{
    if (!_clocks[id].has_value()) {
        return static_cast<std::size_t>(-1);
    }
    auto val = std::chrono::high_resolution_clock::now() - _clocks[id].value();
    return std::chrono::duration_cast<std::chrono::seconds>(val).count();
}

std::size_t Clock::elapsedNanosecondsSince(std::size_t id)
{
    if (!_clocks[id].has_value()) {
        return static_cast<std::size_t>(-1);
    }
    auto val = std::chrono::high_resolution_clock::now() - _clocks[id].value();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(val).count();
}

void Clock::restart(std::size_t id)
{
    _clocks[id] = std::chrono::high_resolution_clock::now();
}
