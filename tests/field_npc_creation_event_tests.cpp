#include "field_npc_creation_event.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Entry=0x800536e4,Tribe=0x8004e5c0,A=0x2000,D=0x3000;
struct Fixture {
 EventEntry event;std::map<uint32_t,uint32_t> memory;
 std::vector<std::pair<uint32_t,uint32_t>> copied,stored;std::vector<uint32_t> values,modes,models;
 unsigned entries=0,animations=0;bool holdValue=false,holdAsync=false,holdEntry=false,holdAnimation=false,mutateCopy=false;uint32_t asyncResult=1,descriptor=D;
 Fixture(){event.id=17;event.arguments=0x100;memory={{0x100,0x500},{0x104,0x600},{0x80415fd0,0x8000},{0x8014,0},{A+0x104,9},{D+0x28,0x42800000},{D+0x2c,0x42000000},{D+0x10,0x5000},{D+0x14,0x6000},{D+8,0x7000},{D+0x40,10},{D+0x44,11},{D+0x4c,12},{D+0x50,13},{D+0x48,14},{0x8041fd84,0}};}
 FieldNpcCreationEventServices services(){
  FieldNpcCreationEventServices s;s.user=this;
  s.word=[](void* p,uint32_t a,uint32_t& v){auto& f=*static_cast<Fixture*>(p);auto i=f.memory.find(a);if(i==f.memory.end())return false;v=i->second;return true;};
  s.half=[](void* p,uint32_t a,uint16_t& v){auto& f=*static_cast<Fixture*>(p);auto i=f.memory.find(a);if(i==f.memory.end())return false;v=uint16_t(i->second);return true;};
  s.value=[](void* p,EventEntry&,int32_t raw,int32_t& v){auto& f=*static_cast<Fixture*>(p);if(f.holdValue)return false;f.values.push_back(uint32_t(raw));v=raw;return true;};
  s.resolve=[](void*,EventEntry&,uint32_t name)->std::optional<uint32_t>{assert(name==0x500);return A;};
  s.tribe=[](void* p,uint32_t name)->std::optional<uint32_t>{assert(name==0x600);return static_cast<Fixture*>(p)->descriptor;};
  s.async=[](void* p,uint32_t name,uint32_t mode)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if(f.holdAsync)return {};f.models.push_back(name);f.modes.push_back(mode);return f.asyncResult;};
  s.enter=[](void* p,uint32_t name,uint32_t model)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);assert(name==0x500 && model==f.models.back());if(f.holdEntry)return {};++f.entries;return 0;};
  s.copy=[](void* p,uint32_t to,uint32_t from){auto& f=*static_cast<Fixture*>(p);f.copied.push_back({to,from});if(f.mutateCopy && to==A+0x4c){f.memory[A+0x28]=0x4000;f.memory[D+0x14]=0x9000;}return true;};
  s.animation=[](void* p,uint32_t handle,uint32_t name,uint32_t force){auto& f=*static_cast<Fixture*>(p);assert(handle==9 && name==A+0x2c && force==1);if(f.holdAnimation)return false;++f.animations;return true;};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.memory[a]=v;f.stored.push_back({a,v});if(a==A+0x1e0)f.memory[0x8041fd84]=0x12345678;};
  s.storeHalf=[](void* p,uint32_t a,uint16_t v){auto& f=*static_cast<Fixture*>(p);f.memory[a]=v;f.stored.push_back({a,v});};return s;
 }
};
void entry(){
 Fixture f;auto s=f.services();FieldNpcCreationEventCursor c;f.holdAsync=true;assert(!fieldNpcCreationEvent(Entry,f.event,s,c));assert(f.values.size()==2);f.memory[0x104]=0x700;f.memory[0x8014]=7;assert(!fieldNpcCreationEvent(Entry,f.event,s,c) && f.values.size()==2);f.holdAsync=false;f.asyncResult=0;assert(fieldNpcCreationEvent(Entry,f.event,s,c)==0 && !c.phase && f.entries==0 && f.models.back()==0x600 && f.modes.back()==0);
 assert(fieldNpcCreationEvent(Entry,f.event,s,c)==0 && f.models.back()==0x700 && f.modes.back()==1 && f.values.size()==4);f.asyncResult=0xffffffff;f.holdEntry=true;assert(!fieldNpcCreationEvent(Entry,f.event,s,c));const auto polls=f.modes.size();f.memory[0x104]=0x800;assert(!fieldNpcCreationEvent(Entry,f.event,s,c) && f.modes.size()==polls);f.holdEntry=false;assert(fieldNpcCreationEvent(Entry,f.event,s,c)==2 && f.entries==1 && !c.phase);
 Fixture held;auto h=held.services();FieldNpcCreationEventCursor d;held.holdValue=true;assert(!fieldNpcCreationEvent(Entry,held.event,h,d));held.memory[0x100]=0x999;held.holdValue=false;assert(fieldNpcCreationEvent(Entry,held.event,h,d)==2 && held.values.front()==0x500);
}
void tribe(){
 Fixture f;auto s=f.services();FieldNpcCreationEventCursor c;f.mutateCopy=true;f.holdAnimation=true;assert(!fieldNpcCreationEvent(Tribe,f.event,s,c));assert(f.memory[A+0x28]==0x4000 && f.memory[A+0x14c]==0x42800000 && f.copied==std::vector<std::pair<uint32_t,uint32_t>>({{A+0x4c,0x5000},{A+0x6c,0x9000},{A+0x2c,0x7000}}));const auto stores=f.stored;f.memory[A+0x104]=99;f.memory[D+0x4c]=77;assert(!fieldNpcCreationEvent(Tribe,f.event,s,c) && f.stored==stores && f.copied.size()==3);f.holdAnimation=false;assert(fieldNpcCreationEvent(Tribe,f.event,s,c)==1 && f.animations==1 && !c.phase);assert(f.memory[A+0x1e0]==10 && f.memory[A+0x1ec]==77 && f.memory[A+0x1dc]==0 && f.memory[A+0x1d8]==1 && f.memory[A+0x1e8]==14);
 Fixture noInitial;noInitial.memory[D+8]=0;auto n=noInitial.services();FieldNpcCreationEventCursor d;assert(fieldNpcCreationEvent(Tribe,noInitial.event,n,d)==1 && !noInitial.animations && noInitial.copied.size()==2);
 Fixture missing;missing.descriptor=0;auto m=missing.services();FieldNpcCreationEventCursor e;assert(!fieldNpcCreationEvent(Tribe,missing.event,m,e) && missing.memory[A+0x28]==0 && e.phase==13);
}
}
int main(){entry();tribe();}
