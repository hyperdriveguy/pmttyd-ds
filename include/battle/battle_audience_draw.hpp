#pragma once
#include "battle_actor.hpp"
#include "battle_act_draw.hpp"
namespace ttyd {
struct BattleAudienceTexture {uint32_t bank=0,index=0,width=0,height=0;};
struct BattleAudiencePoint {float x=0,y=0,z=0,u=0,v=0;};
struct BattleAudienceQuad {
    uint32_t slot=0;BattleAudienceTexture texture;BattleActMatrix matrix{};
    uint32_t rgba=0;std::array<BattleAudiencePoint,4> points{};
};
struct BattleAudienceGXRequest {uint32_t function=0;std::array<int32_t,7> arguments{};};
struct BattleAudienceDrawServices {
    BattleActorServices actors;void* user=nullptr;
    std::optional<uint32_t> (*camera)(void*,uint32_t index)=nullptr;
    std::optional<BattleActMatrix> (*view)(void*,uint32_t camera)=nullptr;
    std::optional<std::array<float,6>> (*vectors)(void*,uint32_t camera)=nullptr; // eyeXYZ,atXYZ
    std::optional<uint8_t> (*mapMode)(void*)=nullptr;
    bool (*request)(void*,const BattleAudienceGXRequest&)=nullptr;
    std::optional<BattleAudienceTexture> (*texture)(void*,uint32_t bank,uint8_t index)=nullptr;
    std::optional<BattleActMatrix> (*scale)(void*,float,float,float)=nullptr;
    std::optional<std::array<float,3>> (*vector)(void*,const BattleActMatrix&,const std::array<float,3>&)=nullptr;
    std::optional<BattleActMatrix> (*concat)(void*,const BattleActMatrix&,const BattleActMatrix&)=nullptr;
    std::optional<double> (*sine)(void*,double)=nullptr;
    std::optional<double> (*cosine)(void*,double)=nullptr;
    float (*fma)(void*,float,float,float)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    bool (*quad)(void*,const BattleAudienceQuad&)=nullptr;
};
std::optional<int32_t> packedBattleAudienceFront(int32_t slot,const BattleAudienceDrawServices&);
// Whole DispAudience local body. Writes original dimensions/normal/flip fields;
// prepare each source pass once, then replay retained geometry for other views.
bool drawPackedBattleAudience(const BattleAudienceDrawServices&);
}
