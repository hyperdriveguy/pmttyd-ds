#pragma once
#include "battle_audience_callback.hpp"
namespace ttyd {
struct BattleAudienceMatrixRequest {
    uint32_t function=0,axis=122;BattleActMatrix left{},right{},output{};
    std::array<float,3> xyz{};float radians=0;
};
struct BattleAudienceIconRequest {
    uint32_t function=0,flags=0,icon=0,rgba=0;BattleActMatrix matrix{};
};
struct BattleAudienceAuxServices {
    BattleAudienceCallbackServices common;void* user=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*icon)(void*,const BattleAudienceIconRequest&)=nullptr;
};
struct BattleAudienceAuxCursor {
    bool active=false,matrixPending=false;uint32_t callback=0,phase=0,step=0,index=0,pending=0,owner=0,record=0;
    std::array<BattleActMatrix,3> matrices{};BattleAudienceMatrixRequest matrix;
    BattleAudienceIconRequest icon;
};
// Complete local DispItem/DispApSrc graph. All source math/icon providers must
// acknowledge their own work; no inactive-bank assumption or geometry no-op.
bool resumePackedBattleAudienceAux(uint32_t callback,const BattleAudienceAuxServices&,BattleAudienceAuxCursor&);
void bindAudienceAuxMathAdapter(BattleAudienceAuxServices&);
}
