#pragma once

#include <vector>

class IPlugin {
    public:
        virtual ~IPlugin() = default;
        virtual void initPlugin() = 0;
        virtual std::vector<std::function<void(std::size_t, std::size_t)>> getSystems() = 0;
};