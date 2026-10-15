#pragma once
#include <memory>
#include "battle_weapon_draw_continuation.hpp"
namespace ttyd {
struct BattleSubIconRasterRequest {
 uint32_t function=0x8011847c,callsite=0x80119170,work=0,row=0;
 BattleActMatrix matrix{};
 uint32_t label=0,enabled=0,cost=0,bp=0,flag=0,stackFlag=0;
 uint16_t icon=0;
};
struct BattleSubIconServices {
 BattleActDrawServices math;
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
 std::optional<float> (*fma)(void*,float,float,float)=nullptr;
 // Whole original DrawSubIconSub owner; must retain all partial raster work.
 std::optional<uint32_t> (*request)(void*,const BattleSubIconRasterRequest&)=nullptr;
};
class BattleSubIconCursor {
public:
 BattleSubIconCursor();~BattleSubIconCursor();
 BattleSubIconCursor(const BattleSubIconCursor&)=delete;
 BattleSubIconCursor& operator=(const BattleSubIconCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattleSubIcon(uint32_t,const BattleWeaponDrawOutput&,uint32_t,uint32_t,const BattleSubIconServices&,BattleSubIconCursor&);
};
// Original seven-row80119024/184, including actual registry lookup, two
// independent fctiwz captures, row gates, translation/concat and SubIconSub.
// Borrowed output identity/values and service owners remain immutable while
// held. Unknown private cells are required only at actual source consumption.
bool resumePackedBattleSubIcon(uint32_t work,const BattleWeaponDrawOutput&,
 uint32_t flag,uint32_t stackFlag,const BattleSubIconServices&,BattleSubIconCursor&);
}
