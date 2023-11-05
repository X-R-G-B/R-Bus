#pragma once

#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Json.hpp"

namespace Systems {
    void addPhysicsToEntity(nlohmann::json jsonObject, const Types::Position &originPos);
}
