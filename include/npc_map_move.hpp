#pragma once
#include "field_wall.hpp"
#include <optional>
namespace ttyd {
struct NpcMapMove {FieldWallVector root;FieldWallHit contact;};
struct NpcMapMoveServices {
    void* user=nullptr;
    FieldWallVector (*direction)(void*,float dx,float dz)=nullptr; // angleABf then sincosf.
    float (*length)(void*,float x,float z)=nullptr; // Source SDK norm provider.
    float (*normalLength)(void*,float x,float z)=nullptr; // Separate products, then sum.
    float (*angle)(void*,float x,float z)=nullptr; // Field-heading degrees.
    FieldWallHit (*ray)(void*,FieldWallVector,FieldWallVector,float extent)=nullptr; // Null filter.
    void (*publishContact)(void*,const FieldWallHit&)=nullptr; // Caller+2fc before second query.
};
// Finite no-reflection/filter0 body. nullopt is a native provider/domain
// frontier; a contained null contact is the original miss result.
std::optional<NpcMapMove> checkNpcMapSide(FieldWallVector,float width,float height,
    float dx,float dz,uint32_t flags,const NpcMapMoveServices&);
// Original npcMain: retain first contact, rerun tentative delta on contact.
std::optional<NpcMapMove> advanceNpcMapSide(FieldWallVector,float width,float height,
    float dx,float dz,uint32_t flags,const NpcMapMoveServices&);
}
