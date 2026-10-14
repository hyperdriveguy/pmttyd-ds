#pragma once
#include "display_queue.hpp"
#include <cstddef>
namespace ttyd {
// Source0x5c-byte record; alpha at0x28,3x4 matrix at0x2c. Position/radius
// and retained settings are requests; matrix/texture prep has a separate owner.
struct ShadowRecord {
    uint16_t flags=0;uint8_t type=0,variant=0;
    std::array<float,3> position{};
    float radius=0,height=0;int16_t collision=0,camera=0;
    float scale=0,inner=0,outer=0;uint8_t alpha=0;
    std::array<uint8_t,3> padding{};std::array<float,12> matrix{};
};
static_assert(sizeof(ShadowRecord)==0x5c);
static_assert(offsetof(ShadowRecord,matrix)==0x2c && offsetof(ShadowRecord,alpha)==0x28);
struct ShadowAuxiliaryState {uint32_t flags=0,counter=0;};
struct ShadowMainCursor {uint8_t step=0;bool active=false;};
// Native owner for the source primary pool and retained request settings.
// GPU image allocation, collision preparation and deferred rendering are separate.
class ShadowWork {
public:
    // Source boot settings:enabled1,type2,collision on,height20 engine units;
    // color0x0000006e is source RGBA black with alpha110, not C2D packed ABGR.
    // Primary pool64 records; two auxiliary resets are independent settings.
    uint32_t flags=0,counter=0,enabled=1,color=0x0000006e;
    uint32_t preparationGate=1,type=2,collisionEnabled=1;
    float height=20;
    std::array<ShadowAuxiliaryState,2> auxiliary{};
    std::optional<uint32_t> enter(std::array<float,3>,float radius);
    bool main(DisplayQueue&,ShadowMainCursor&);
    uint32_t count() const{return count_;}
    ShadowRecord* record(uint32_t i){return i<records_.size()?&records_[i]:nullptr;}
    const ShadowRecord* record(uint32_t i)const{return i<records_.size()?&records_[i]:nullptr;}
private:
    uint32_t count_=0;std::array<ShadowRecord,64> records_{};
};
}
