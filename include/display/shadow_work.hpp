#pragma once
#include "display_queue.hpp"
#include "camera_depth.hpp"
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
// Mutable source shadowConfig rows. These scalar boot settings are recovered
// logic, not an image/asset pack; words3c/44 remain opaque retained metadata.
struct ShadowProfile {
    uint32_t followCamera4=0;
    std::array<float,3> eye{},up{},at{};
    float left=0,top=0,nearPlane=0,farPlane=0;
    uint32_t projectionKind=0,word3c=0,sizeIndex=0,word44=0;
};
static_assert(sizeof(ShadowProfile)==72);
static_assert(offsetof(ShadowProfile,projectionKind)==0x38);
struct ShadowAuxiliaryState {
    uint32_t flags=2,counter=0,profileIndex=0,color=0x00000040;
    CameraViewMatrix view{};CameraProjectionMatrix projection{};
};
struct ShadowMainCursor {uint8_t step=0;bool active=false;};
// Native owner for the source primary pool and retained request settings.
// GPU image allocation, collision preparation and deferred rendering are separate.
class ShadowWork {
public:
    // Source boot settings:profileIndex1,type2,collision on,height20 engine units;
    // color0x0000006e is source RGBA black with alpha110, not C2D packed ABGR.
    // Primary pool64 records; two auxiliary resets are independent settings.
    uint32_t flags=0,counter=0,profileIndex=1,color=0x0000006e;
    std::array<ShadowProfile,2> profiles{{
        {0,{-100,256,335},{0,1,0},{0,0,0},-20,20,356,741,0,3,0,3230599741u},
        {1,{135,40,41},{0,1,0},{0,0,0},-240,240,80,10000,1,3,0,0}
    }};
    std::array<uint16_t,4> profileSizes{{400,256,128,64}};
    ShadowProfile* selectedProfile(){return profileIndex<profiles.size()?&profiles[profileIndex]:nullptr;}
    CameraViewMatrix view{};
    CameraProjectionMatrix projection{};
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
