#pragma once
#include "source_icon_position.hpp"
// Retained source stock-EXP icons, not decimal text. Count at B+f04 caps100:
// ones use scale 0.35, tens0.5, both original icon 404/flags 16/alpha255.
// Source world/UI coordinates are not native lower-screen touch pixels.
namespace ttyd {
struct BattleStockExpDrawServices {
    void* user=nullptr;std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    bool (*icon)(void*,const SourceIconPositionRequest&)=nullptr;
};
struct BattleStockExpDrawCursor {
    bool active=false;uint32_t phase=0,battle=0,index=0,pending=0;int32_t count=0,ones=0,tens=0;
    float x=0;SourceIconPositionRequest icon;
};
bool drawBattleStockExp(const BattleStockExpDrawServices&,BattleStockExpDrawCursor&);
}
