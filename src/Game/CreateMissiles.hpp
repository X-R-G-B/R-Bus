#include "GameCustomTypes.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"

namespace Systems {
    std::string getMissileId(missileTypes_e type);
    void createMissile(Types::Position pos, Types::Missiles &typeOfMissile);
}
