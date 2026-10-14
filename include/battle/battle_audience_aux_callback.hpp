#pragma once
#include "source_icon_matrix.hpp"
namespace ttyd {
struct BattleAudienceAuxServices {
    BattleAudienceCallbackServices common;void* user=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*icon)(void*,const BattleAudienceIconRequest&)=nullptr;
    const SourceIconMatrixServices* sourceIcon=nullptr;
};
struct BattleAudienceAuxCursor {
    bool active=false,matrixPending=false;uint32_t callback=0,phase=0,step=0,index=0,pending=0,owner=0,record=0;
    std::array<BattleActMatrix,3> matrices{};BattleAudienceMatrixRequest matrix;
    BattleAudienceIconRequest icon;SourceIconMatrixCursor iconCursor;
};
// Complete local DispItem/DispApSrc graph. All source math/icon providers must
// acknowledge their own work; no inactive-bank assumption or geometry no-op.
bool resumePackedBattleAudienceAux(uint32_t callback,const BattleAudienceAuxServices&,BattleAudienceAuxCursor&);
void bindAudienceAuxMathAdapter(BattleAudienceAuxServices&);
}
