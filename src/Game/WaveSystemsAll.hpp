#include <functional>
#include <vector>

namespace Systems {
    std::vector<std::function<void(std::size_t, std::size_t)>> getWaveSystems();
}
