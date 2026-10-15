#pragma once
#include "field_map_npc_assets.hpp"
#include "field_map_npc_history.hpp"
#include "field_npc_registry_owner.hpp"
namespace ttyd {
// Retained synchronous setupDataBase NPC continuation using precompiled parsed
// tags. Binding/facade outlive this cursor. No event parser opcodes are claimed.
class FieldMapNpcBootstrap {
public:
 static std::unique_ptr<FieldMapNpcBootstrap> prepare(EventAddressSpace&,FieldNpcRegistryBinding&,const FieldMapNpcAssets&,uint32_t inputBase,uint32_t area,uint32_t map,uint32_t field);
 ~FieldMapNpcBootstrap();
 FieldMapNpcBootstrap(const FieldMapNpcBootstrap&)=delete;
 FieldMapNpcBootstrap& operator=(const FieldMapNpcBootstrap&)=delete;
 bool advance(BattlePoseClock,const FieldMapNpcBattleAdmission& = {});
 bool ready() const{return ordinal_==assets_.count();}
 unsigned ordinal() const{return ordinal_;}
private:
 FieldMapNpcBootstrap(EventAddressSpace&,FieldNpcRegistryBinding&,const FieldMapNpcAssets&,uint32_t,uint32_t);
 EventAddressSpace& memory_;FieldNpcRegistryBinding& binding_;FieldMapNpcAssets assets_;
 uint32_t base_,area_;PackedMapNpcHistoryGate gate_;unsigned ordinal_=0;
 std::optional<bool> skip_;bool mapped_=false;
};
}
