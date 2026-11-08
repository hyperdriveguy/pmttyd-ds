#include "battle_kill_unit_event.hpp"
#include "battle_packed_unit_initialization.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Args=0x85000000, Global=0x8041eb90, Battle=0x84000000,
                   Unit=0x86000000, Other=0x87000000;
struct Op {char kind;uint32_t at,value;bool operator==(const Op&o)const{return kind==o.kind&&at==o.at&&value==o.value;}};
struct Fixture {
 EventEntry event;BattleUnitEventServices units;BattleActorServices actors;
 std::map<uint32_t,uint32_t> memory;std::vector<Op> trace;
 bool null=false,mutateRegistry=false,mutateClear=false;int chargeReplacement=-1;
 Fixture(uint32_t mode=0){
  event.arguments=Args;memory={{Args,2},{Args+4,mode},{Global,Battle},{Unit+0x104,0x40},{Unit+0x1c,0x80}};
  units.memory.user=this;units.user=this;units.values.user=this;
  units.memory.word=[](void*p,uint32_t at){auto&f=*static_cast<Fixture*>(p);assert(f.memory.count(at));auto v=f.memory.at(at);f.trace.push_back({'R',at,v});return v;};
  units.memory.storeWord=[](void*p,uint32_t at,uint32_t v){auto&f=*static_cast<Fixture*>(p);f.trace.push_back({'W',at,v});f.memory[at]=v;};
  units.memory.storeHalf=[](void*p,uint32_t at,uint16_t v){auto&f=*static_cast<Fixture*>(p);f.trace.push_back({'H',at,v});f.memory[at]=v;};
  units.memory.storeByte=[](void*p,uint32_t at,uint8_t v){auto&f=*static_cast<Fixture*>(p);f.trace.push_back({'B',at,v});f.memory[at]=v;if(f.mutateClear){f.memory[Args+4]=2;f.memory[Global]=Other;}if(at==Unit+0x129&&v==157&&f.chargeReplacement>=0)f.memory[at]=uint32_t(f.chargeReplacement);};
  units.values.resolve=[](void*p,EventEntry&,int32_t v){auto&f=*static_cast<Fixture*>(p);f.trace.push_back({'V',uint32_t(v),0});return v;};
  units.translate=[](void*p,EventEntry&,int32_t v){auto&f=*static_cast<Fixture*>(p);f.trace.push_back({'T',uint32_t(v),0});return v;};
  units.unit=[](void*p,uint32_t b,int32_t id){auto&f=*static_cast<Fixture*>(p);assert(b==Battle&&id==2);f.trace.push_back({'U',b,uint32_t(id)});if(f.mutateRegistry)f.memory[Args+4]=1;return f.null?0:Unit;};
  actors.memory=units.memory;actors.user=this;
  actors.byte=[](void*p,uint32_t at){auto&f=*static_cast<Fixture*>(p);assert(f.memory.count(at));auto v=f.memory.at(at);f.trace.push_back({'r',at,v});return uint8_t(v);};
 }
 std::vector<Op> expected(uint32_t mode)const{
  std::vector<Op> out{{'R',Args,2},{'V',2,0},{'T',2,0},{'R',Global,Battle},{'U',Battle,2},{'R',Args+4,mode}};
  if(null)return out;
  out.push_back({'H',Unit+0x10c,0});for(unsigned i=0;i<30;++i)out.push_back({'B',Unit+0x118+i,0});
  out.push_back({'r',Unit+0x129,0});out.push_back({'B',Unit+0x129,157});
  const auto fresh=chargeReplacement<0?157:uint32_t(chargeReplacement);out.push_back({'r',Unit+0x129,fresh});
  if(fresh>=128)out.push_back({'B',Unit+0x129,0});else if(fresh>99)out.push_back({'B',Unit+0x129,99});
  for(unsigned i=0;i<6;++i){auto at=Unit+0xae8+12*i;out.push_back({'B',at,255});for(unsigned j=1;j<4;++j)out.push_back({'B',at+j,0});out.push_back({'W',at+4,0});}
  if(mode&2){out.push_back({'R',Unit+0x104,0x40});out.push_back({'W',Unit+0x104,0x20040});}
  else {out.push_back({'B',Unit+0x135,1});if(!(mode&1)){out.push_back({'R',Unit+0x1c,0x80});out.push_back({'W',Unit+0x1c,0x80000080});}}
  return out;
 }
};
}
int main(){
 for(uint32_t mode:{0u,1u,2u,3u,4u,0xffffffffu}){Fixture f(mode);assert(battleKillUnitEvent(f.event,f.units,f.actors)==2);assert(f.trace==f.expected(mode));for(unsigned i=0;i<6;++i)assert(!f.memory.count(Unit+0xae8+12*i+8));}
 {Fixture f(0xffffffff);f.null=true;f.units.memory.storeHalf=nullptr;f.units.memory.storeByte=nullptr;f.units.memory.storeWord=nullptr;f.actors={};assert(battleKillUnitEvent(f.event,f.units,f.actors)==2);assert(f.trace==f.expected(0xffffffff));}
 {Fixture f;f.mutateRegistry=true;assert(battleKillUnitEvent(f.event,f.units,f.actors)==2);assert(f.trace==f.expected(1));}
 {Fixture f;f.mutateClear=true;assert(battleKillUnitEvent(f.event,f.units,f.actors)==2);assert(f.trace==f.expected(0));assert(f.memory[Global]==Other&&f.memory[Unit+0x1c]==0x80000080);}
 for(int replacement:{0,77,120,200}){Fixture f;f.chargeReplacement=replacement;assert(battleKillUnitEvent(f.event,f.units,f.actors)==2);assert(f.trace==f.expected(0));assert(f.memory[Unit+0x129]==uint32_t(replacement>=128?0:replacement>99?99:replacement));}
 std::cout<<"Whole KillUnit all raw modes/capture order and exact ClearStatus fresh charge read/store trace pass\n";
}
