#pragma once
#include "field_actors.hpp"
#include <array>
namespace ttyd {
// Native XYZ triangle floats; attribute 80000000(hex) is dynamic-object domain,
// cull1 source back-face policy. owner is catalog key, not geometry pointer.
struct FieldObjectTriangle {
    std::array<float,9> vertices{};
    uint32_t owner=0,attributes=0x80000000,cull=1;
    bool ordinarySolid=false; // Source name filter known and accepts this model.
};
// Source mobjHitEntry mode0 with default object scale and Y rotation.
// Model draw scalar and pose deformation do not affect this collision primitive.
void buildFieldObjectCollision(const FieldActorAssets&,uint32_t room,std::optional<int32_t> story,
                              uint32_t interior,uint32_t system,std::vector<FieldObjectTriangle>&);
}
