#pragma once
#include "battle_audience_aux_callback.hpp"
namespace ttyd {
struct BattlePartScalarRequest {uint32_t function=0,part=0;float value=0;std::array<float,4> coordinates{};};
struct BattlePartMaterialRequest {uint32_t function=0;int32_t handle=0;uint32_t value=0;};
struct BattlePartDrawRequest {int32_t handle=0;uint32_t pass=0;BattleActMatrix matrix{};float angle=0,scale=0;};
struct BattlePartDisplayServices {
    BattleActorServices actors;void* user=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    std::optional<float> (*scalar)(void*,const BattlePartScalarRequest&)=nullptr;
    const CameraState* (*currentCamera)(void*)=nullptr;
    std::optional<bool> (*status)(void*,uint32_t unit,int32_t)=nullptr;
    bool (*material)(void*,const BattlePartMaterialRequest&)=nullptr;
    bool (*callback)(void*,uint32_t identity,uint32_t part,uint32_t beforeAfter)=nullptr;
    bool (*draw)(void*,const BattlePartDrawRequest&,uint32_t& nestedPending)=nullptr;
};
struct BattlePartDisplayCursor {
    bool active=false,requestPending=false,white=false,status16=false;
    uint32_t part=0,unit=0,battle=0,phase=0,step=0,pending=0,target=0,order=0,color=0,callback=0,historyMode=0;
    float direction=0,floatOffset=0,gravityOffset=0,correction=0,angle=0,q=0;
    std::array<float,3> world{};const CameraState* camera=nullptr;
    std::array<BattleActMatrix,21> matrices{};
    BattleAudienceMatrixRequest matrix;BattlePartScalarRequest scalar;
    BattlePartMaterialRequest material;BattlePartDrawRequest draw;
};
// Native sine-degree adapter; the original getters read unsigned phase halves.
float nativeBattlePartPhaseOffset(uint16_t phase,bool floating);
// Ordinary part path, including all rotation orders, material/pass/history tail.
// The source special20000000 branch remains an explicit continuation boundary.
bool resumePackedBattlePartDisplay(uint32_t part,const BattlePartDisplayServices&,BattlePartDisplayCursor&);
}
