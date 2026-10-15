#pragma once
#include "battle_actor.hpp"
#include "shadow_display.hpp"
namespace ttyd {
struct BattleStageVertex {std::array<float,3> position{};std::array<uint8_t,4> color{};};
// Source mode 80(hex) quad/count 4 or90(hex) triangle/count 3. Whole primitive
// acknowledgement prevents an immediate GPU Begin surviving a provider hold.
struct BattleStagePrimitive {uint32_t mode=0,count=0;std::array<BattleStageVertex,4> vertices{};};
struct BattleStageMath {
    uint32_t function=0;std::array<float,3> axis{},vector{},output{};float angle=0;
    std::array<float,12> matrix{};
};
struct BattleStageDisplayServices {
    BattleActorServices actors;void* user=nullptr;
    const CameraState* (*camera)(void*,uint32_t)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
    bool (*primitive)(void*,const BattleStagePrimitive&)=nullptr;
    bool (*math)(void*,BattleStageMath&)=nullptr;
};
// index is source record/quad progress; segment is spotlight wedge progress.
// Queued math/primitive state captures source inputs independently.
struct BattleStageDisplayCursor {
    bool active=false,queued=false;uint32_t callback=0,owner=0,phase=0,step=0,index=0,segment=0,pending=0;
    const CameraState* camera=nullptr;ShadowGXRequest request;BattleStagePrimitive primitive;BattleStageMath math;
    std::array<float,3> axis{},vector{},previous{},start{},center{};float width=0;
};
bool resumePackedBattleStageDisplay(uint32_t callback,const BattleStageDisplayServices&,BattleStageDisplayCursor&);
// Explicit finite native adapter: source instruction graph, native sin/cos and
// 1/sqrt estimate. Not Gekko frsqrte/libm/FPSCR or bit parity.
bool nativeBattleStageMath(BattleStageMath&);
}
