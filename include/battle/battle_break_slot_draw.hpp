#pragma once
#include "source_icon_matrix.hpp"
#include "shadow_display.hpp"
namespace ttyd {
struct BattleBreakSlotPlaneRequest {BattleActMatrix matrix{};uint32_t texture=0,rgba=0,flags=0;};
struct BattleBreakSlotDrawServices {
    void* user=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
    std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
    bool (*scissor)(void*,const std::array<uint32_t,4>&)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    bool (*icon)(void*,const BattleAudienceIconRequest&)=nullptr;
    const SourceIconMatrixServices* sourceIcon=nullptr;
    bool (*plane)(void*,const BattleBreakSlotPlaneRequest&)=nullptr;
};
struct BattleBreakSlotDrawCursor {
    bool active=false,queued=false;uint32_t phase=0,index=0,step=0,base=0,reel=0,table=0,pending=0,color=0,category=0,data=0,candidate=0,gp=0;
    int32_t state=0;std::array<uint32_t,4> scissor{};
    std::array<BattleActMatrix,8> matrices{};BattleAudienceMatrixRequest matrix;
    BattleAudienceIconRequest icon;SourceIconMatrixCursor iconCursor;BattleBreakSlotPlaneRequest plane;
};
// Full parent802036d4. A missing real texture-plane provider remains a hold.
bool drawBattleBreakSlot(const BattleBreakSlotDrawServices&,BattleBreakSlotDrawCursor&);
}
