#include "field_map_npc_bootstrap.hpp"
#include "event_address_space.hpp"
namespace ttyd {
FieldMapNpcBootstrap::FieldMapNpcBootstrap(EventAddressSpace& m,FieldNpcRegistryBinding& b,const FieldMapNpcAssets& a,uint32_t base,uint32_t area):memory_(m),binding_(b),assets_(a),base_(base),area_(area){}
std::unique_ptr<FieldMapNpcBootstrap> FieldMapNpcBootstrap::prepare(EventAddressSpace& m,FieldNpcRegistryBinding& b,const FieldMapNpcAssets& a,uint32_t base,uint32_t area,uint32_t map,uint32_t field){
 auto result=std::unique_ptr<FieldMapNpcBootstrap>(new FieldMapNpcBootstrap(m,b,a,base,area));
 if(!a.mapTo(m,base))return {};
 result->mapped_=a.count()!=0;
 const auto gate=prepareFieldMapNpcHistory(m,field,map);if(!gate)return {};
 result->gate_=*gate;return result;
}
FieldMapNpcBootstrap::~FieldMapNpcBootstrap(){if(mapped_)memory_.unregisterRam(base_);}
bool FieldMapNpcBootstrap::advance(BattlePoseClock clock,const FieldMapNpcBattleAdmission& battle){
 while(!ready()){
  // The original parser pops a row before its history test. Capture that test
  // once, including a false result, across native resource-admission holds.
  if(!skip_){skip_=skipFieldMapNpc(memory_,gate_,ordinal_);if(!skip_)return false;}
  if(!*skip_){const auto input=assets_.input(ordinal_,base_,area_);if(!input || !binding_.registerMapNpc(*input,clock,battle))return false;}
  ++ordinal_;skip_.reset();
 }
 return true;
}
}
