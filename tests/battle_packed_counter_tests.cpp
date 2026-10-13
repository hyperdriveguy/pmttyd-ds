#include "battle_packed_counter.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <set>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};BattleCounterWork work;
 std::set<std::pair<uint32_t,int32_t>> statuses;std::vector<std::pair<uint32_t,int32_t>> calls;
 uint32_t flags=0;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x4000)));work.init();}
 void w(uint32_t at,uint32_t v){assert(vm.memory.writeWord(at,v));}
 void b(uint32_t at,uint8_t v){assert(vm.memory.writeByte(at,v));}
 BattlePackedCounterServices services(){BattlePackedCounterServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.status=[](void* self,uint32_t u,int32_t type){auto& f=*static_cast<Fixture*>(self);f.calls.emplace_back(u,type);return f.statuses.count({u,type})!=0;};return s;}
 int32_t check(){return packedBattleCheckCounter(work,0x1000,0x2000,0x3000,0x4000,flags,services());}
};
int main(){
 Fixture empty;assert(empty.check()==0 && empty.work.words[0]==1 && empty.flags==0);
 // Null attacker returns before resetting count or reading any service.
 empty.work.words[10]=123;assert(packedBattleCheckCounter(empty.work,0,0,0,0,empty.flags,{})==0 && empty.work.words[10]==123);
 Fixture spikes;spikes.w(0x31b0,3);assert(spikes.check()==8 && spikes.work.words[8]==43 && spikes.work.words[9]==299 && spikes.work.words[10]==1 && spikes.work.words[11]==31 && spikes.work.words[0]==0 && spikes.flags==0x100 && spikes.calls.empty());
 Fixture immunity;immunity.w(0x31b0,3);immunity.w(0x4078,2);assert(immunity.check()==7 && immunity.work.words[8]==44);immunity.b(0x1302,1);immunity.work.init();immunity.flags=0;assert(immunity.check()==0 && immunity.flags==0 && immunity.work.words[0]==1);
 Fixture cold;cold.w(0x31b0,0x800);assert(cold.check()==0 && cold.work.words[8]==48 && cold.work.words[4]==1 && cold.work.words[0]==0 && cold.flags==0x100); // Code48 maps to0, not a guessed counter number.
 Fixture fire;fire.w(0x31b0,0x20);assert(fire.check()==10 && fire.work.words[2]==1 && fire.work.words[11]==34);
 Fixture electric;electric.statuses.insert({0x2000,21});electric.statuses.insert({0x2000,22});electric.b(0x22f0,1);electric.w(0x31b0,0x3200);assert(electric.check()==16 && electric.work.words[8]==53 && electric.work.words[1]==1 && electric.work.words[3]==1 && electric.work.words[6]==1 && electric.work.words[7]==1 && electric.work.words[10]==1 && electric.work.words[0]==1 && electric.work.words[11]==0);
 Fixture burn;burn.w(0x31b0,0x1000);burn.w(0x4074,0x400);burn.statuses.insert({0x1000,8});assert(burn.check()==12 && burn.work.words[8]==49 && burn.work.words[10]==1);assert((burn.calls==std::vector<std::pair<uint32_t,int32_t>>{{0x1000,8},{0x2000,6},{0x2000,21},{0x2000,22}}));
 std::cout<<"Original counter priority, bypass/status calls, accumulated gates and source result table passed\n";
}
