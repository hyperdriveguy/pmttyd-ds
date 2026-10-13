#pragma once
#include "field_actors.hpp"
#include <array>
namespace ttyd {
struct FieldObjectTriangle {
    std::array<float,9> vertices{};
    uint32_t owner=0,attributes=0x80000000,cull=1;
};
// Source mobjHitEntry mode0 with default object scale and Y rotation.
// Model draw scalar and pose deformation do not affect this collision primitive.
void buildFieldObjectCollision(const FieldActorAssets&,uint32_t room,std::optional<int32_t> story,
                              uint32_t interior,uint32_t system,std::vector<FieldObjectTriangle>&);
}
