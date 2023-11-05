#include "B-luga-physics/ECSCustomTypes.hpp"
#include "GameCustomTypes.hpp"

namespace Systems {
    std::string getMissileId(missileTypes_e type);
    void createMissile(Types::Position pos, Types::Missiles &typeOfMissile);
    std::size_t createPlayerMissile(Types::Position pos, Types::Missiles &typeOfMissile);
    missileTypes_e getMissileTypeFromId(const std::string &id);
    std::vector<std::function<void(std::size_t, std::size_t)>> getBulletsSystems();
} // namespace Systems
