#pragma once
#include "field_wall.hpp"
#include <array>
#include <optional>
#include <string>
namespace ttyd {
struct EventEntry;
struct FieldBeroFloor {bool resolved=false,hit=false;float y=0;};
struct FieldBeroPositionServices {
 void* user=nullptr;
 bool (*center)(void*,const std::string&,FieldWallVector&)=nullptr;
 FieldBeroFloor (*floor)(void*,FieldWallVector origin,FieldWallVector direction,float length)=nullptr;
 float (*sine)(void*,float radians)=nullptr;
 float (*cosine)(void*,float radians)=nullptr;
 float (*negativeMultiplySubtract)(void*,float,float,float)=nullptr; // -(a*b-c), fused then negated.
 float (*multiplyAdd)(void*,float,float,float)=nullptr;
};
struct FieldBeroPosition {
 FieldWallVector start,finish,recovery;
};
// Resolved-value body of bero_get_position_normal. Original world lookup/ray
// and SDK numerical services are explicit; no callback ABI/side-effect claim.
std::optional<FieldBeroPosition> fieldBeroNormalPosition(std::array<int32_t,3> xyz,
 const std::string& name,int32_t direction,int32_t mode,float distance,const FieldBeroPositionServices&);
struct FieldBeroStartPublication {
 std::array<int32_t,3> words{};
 FieldWallVector position;
};
// READF -> LW10..12 and subsequent fetched-word float values. Finite fctiwz
// admission only; raw EVT alias routing and exceptional conversion stay separate.
std::optional<FieldBeroStartPublication> fieldBeroStartPublication(FieldWallVector);
bool publishFieldBeroStart(EventEntry&,const FieldBeroStartPublication&);
}
