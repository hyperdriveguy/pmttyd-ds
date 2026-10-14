#include "field_bero_initial_event.hpp"
#include "field_bero_info.hpp"
#include "field_map_flags.hpp"
#include "field_bero_exec.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
#include <type_traits>
using namespace ttyd;
uint32_t bits(float v){uint32_t out;std::memcpy(&out,&v,4);return out;}
struct Fixture {
 FieldBeroInfo info;FieldMapFlags map;FieldBeroExec exec;FieldBeroInitialEvent event;
 std::map<uintptr_t,uint32_t> memory;std::vector<std::pair<uintptr_t,uint32_t>> writes;
 bool floorReady=true,varyOwners=true;uint16_t motion=25;unsigned marioCalls=0,halves=0,roadResets=0,shiftResets=0;
 Fixture(){info.synchronize(2);map.synchronize(2);assert(info.initialize(map,exec) && info.publishIncoming("w_bero",0));}
 FieldBeroPositionServices geometry() {
  return {this,[](void*,const std::string& name,FieldWallVector& p){assert(name=="w_bero");p={100,5,200};return true;},
   [](void* p,FieldWallVector origin,FieldWallVector direction,float length){auto& f=*static_cast<Fixture*>(p);assert(origin.x==70 && origin.y==5 && origin.z==200 && direction.y==-1 && length==1000000);return FieldBeroFloor{f.floorReady,true,2};},
   [](void*,float){return 1.f;},[](void*,float){return 0.f;},
   [](void*,float a,float b,float c){return -std::fma(a,b,-c);},[](void*,float a,float b,float c){return std::fma(a,b,c);}};
 }
 FieldPositionWorld world(bool camera=true) {
  FieldPositionWorld w;w.user=this;
  w.mario=[](void* p)->uintptr_t{auto& f=*static_cast<Fixture*>(p);++f.marioCalls;return f.varyOwners && f.marioCalls==2?0x2000:f.varyOwners && f.marioCalls==3?0x3000:0x1000;};
  w.currentParty=[](void*){return 0;};w.party=[](void*,int32_t n)->uintptr_t{assert(n==0 || n==1);return n==0?0x6000:0;};
  if(camera)w.camera=[](void*,int32_t n)->uintptr_t{assert(n==4);return 0x4000;};
  w.read=[](void* p,uintptr_t a){return static_cast<Fixture*>(p)->memory.at(a);};
  w.write=[](void* p,uintptr_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);
   if(a==0x22e4){uint32_t x=0;assert(f.event.readGlobal(0x8041e9f8,x) && x==bits(70));}
   f.writes.push_back({a,v});f.memory[a]=v;};
  w.half=[](void* p,uintptr_t a){auto& f=*static_cast<Fixture*>(p);assert(a==0x102e && f.memory.count(f.varyOwners?0x22ec:0x12ec));++f.halves;return f.motion;};
  w.roadReset=[](void* p){auto& f=*static_cast<Fixture*>(p);assert(f.shiftResets==0);++f.roadResets;};
  w.shiftReset=[](void* p){auto& f=*static_cast<Fixture*>(p);assert(f.roadResets==1);++f.shiftResets;};return w;
 }
};
int main() {
 static_assert(!std::is_copy_constructible<FieldBeroInitialEvent>::value);
 static_assert(!std::is_move_constructible<FieldBeroInitialEvent>::value);
 assert(loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs"));
 std::array<int32_t,16> bank;for(unsigned n=0;n<16;++n)bank[n]=100+int32_t(n);
 Fixture f;assert(f.event.begin(f.info,bank,0));uint32_t word=999;assert(!f.event.readGlobal(0x8041e9f8,word) && word==999);
 f.event.tick(17,f.geometry(),f.world());assert(f.event.finished && !f.event.failed && !f.event.pendingFunction && f.event.locals && f.event.position);
 const auto values=*f.event.locals;assert(values[0]==0 && values[1]==0 && values[2]==2 && values[4]==0 && values[5]==6 && uint32_t(values[8])==0xffff0000);
 assert(values[9]==109 && values[13]==113 && values[14]==114 && values[15]==115);
 assert(values[10]==-230000000+70*1024 && values[11]==-230000000+2*1024 && values[12]==-230000000+200*1024);
 assert(f.event.readGlobal(0x8041e9f8,word) && word==bits(70));assert(f.event.readGlobal(0x8041e9ec,word) && word==bits(130));
 assert(!f.event.readGlobal(0x8041e9fc,word) && !f.event.readGlobal(0x8041e9e5,word));
 assert(f.memory.at(0x22e4)==bits(130) && f.memory.at(0x22e8)==bits(39) && f.memory.at(0x22ec)==bits(200));
 assert(f.memory.at(0x32e0)==1 && f.halves==1 && f.marioCalls==5);
 assert(f.memory.at(0x108c)==bits(70) && f.memory.at(0x1090)==bits(2) && f.memory.at(0x1094)==bits(200));
 assert(f.memory.at(0x6058)==bits(70) && f.memory.at(0x605c)==bits(2) && f.memory.at(0x6060)==bits(200));
 assert(f.memory.at(0x4098)==bits(2.01f) && f.memory.at(0x10f0)==bits(2) && f.roadResets==1 && f.shiftResets==1);
 const auto count=f.writes.size();f.event.tick(34,f.geometry(),f.world());assert(f.writes.size()==count);
 Fixture held;held.varyOwners=false;held.motion=0;held.floorReady=false;assert(held.event.begin(held.info,bank,0));
 held.event.tick(17,held.geometry(),held.world());assert(!held.event.finished && !held.event.failed && held.event.pendingFunction==0x80079b74 && held.writes.empty() && !held.event.readGlobal(0x8041e9f8,word));
 const auto id=held.event.eventId;
 // The entry owns its admitted source snapshot, independent of the caller lifetime/state.
 held.info.invalidate();
 held.floorReady=true;held.event.tick(34,held.geometry(),held.world(false));
 assert(held.event.eventId==id && !held.event.finished && held.event.pendingFunction==0x800a785c && held.event.position && !held.event.locals);
 assert(!held.memory.count(0x12e0));const auto before=held.writes.size();
 held.event.tick(51,held.geometry(),held.world());assert(held.event.finished && !held.event.failed && held.halves==1 && held.writes.size()>before && held.roadResets==1);
 held.info.synchronize(2);assert(held.info.initialize(held.map,held.exec));
 assert(held.info.publishIncoming("dokan",0));FieldBeroInitialEvent unsupported;assert(!unsupported.begin(held.info,bank,0) && !unsupported.started);
}
