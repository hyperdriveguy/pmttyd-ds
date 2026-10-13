#include "battle_packed_hit_event.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};EventEntry event;
 std::vector<uint32_t> removed;unsigned created=0;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x2000)));w(0x1000,2);event.id=47;for(unsigned off:{0x2b4u,0x290u,0x29cu,0x2a8u})w(0x1000+off,off);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}
 uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 BattleUnitEventServices services(){auto s=nativeBattleUnitEventServices(vm);s.user=this;s.removeEventId=[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);assert(f.w(0x1000+id)==id);f.removed.push_back(id);};s.createWait=[](void* p,uint32_t script,int32_t priority,uint32_t flags)->EventEntry&{auto& f=*static_cast<Fixture*>(p);assert(script==0x7654 && priority==10 && flags==0x20 && f.removed.size()==4);++f.created;return f.event;};return s;}
};
int main(){
 Fixture missing;assert(packedBattleRunHitEvent(0x1000,0x100|40,missing.services())==0 && missing.removed.empty());missing.w(0x12b0,0x7654);assert(packedBattleRunHitEvent(0x1000,40,missing.services())==0 && missing.removed.empty()); // Data46 guard precedes script/cancellation.
 Fixture direct;assert(packedBattleRunHitEventDirect(0x1000,0x123,0,direct.services())==0 && direct.created==0);assert((direct.removed==std::vector<uint32_t>{0x2b4,0x290,0x29c,0x2a8}));assert(direct.w(0x1278)==0);
 Fixture hit;hit.w(0x1018,0x2000);hit.w(0x2000,46);hit.w(0x2004,0);hit.w(0x12b0,0x7654);assert(packedBattleRunHitEvent(0x1000,0x728,hit.services())==47 && hit.created==1 && hit.event.owner==2);assert(hit.w(0x1278)==0x728 && hit.w(0x12b4)==47 && hit.w(0x1290)==0 && hit.w(0x129c)==0 && hit.w(0x12a8)==0); // Presence46 is sufficient, even zero data value.
 std::cout<<"Source hit event data guard, ordered cancellation, NULL direct script and original root ownership passed\n";
}
