#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems::ParallaxSystems {
    std::vector<std::function<void(std::size_t, std::size_t)>> getParallaxSystems();
}
