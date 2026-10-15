#include "field_npc_actor_event.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Flag=0x800500b0,Position=0x80053158,Get=0x80052e98,Yaw=0x80050988,LR=0x8005088c,A=0x1000;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
struct Fixture {
 EventEntry event;FieldNpcActorEventCursor cursor;std::map<uint32_t,uint32_t> words;
 std::vector<uint32_t> reads,stores;std::vector<int32_t> destinations;
 unsigned values=0,reals=0,resolves=0;bool available=true,cameraReady=true,outputReady=true,mutate=false,needsActor=false;
 float camera=0;uint32_t name=0;std::array<float,3> outputs{};
 Fixture(){event.id=9;event.arguments=0x200;words[0x8041fde4]=bits(100);words[0x8041fde0]=bits(5);for(unsigned n=0;n<3;++n)words[0x802c1470+4*n]=0;}
 void args(std::initializer_list<int32_t> a){unsigned n=0;for(auto v:a)words[0x200+4*n++]=uint32_t(v);}
 FieldNpcActorEventServices services(){FieldNpcActorEventServices s;s.user=this;
  s.word=[](void* p,uint32_t a,uint32_t& v){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto i=f.words.find(a);if(i==f.words.end())return false;v=i->second;return true;};
  s.value=[](void* p,EventEntry&,int32_t raw,int32_t& v){++static_cast<Fixture*>(p)->values;v=raw;return true;};
  s.real=[](void* p,EventEntry&,int32_t raw,float& v){++static_cast<Fixture*>(p)->reals;v=float(raw);return true;};
  s.resolve=[](void* p,EventEntry&,uint32_t name,bool dereference)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.resolves;f.name=name;f.needsActor=dereference;if(!f.available)return {};return A;};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.stores.push_back(a);f.words[a]=v;if(f.mutate && a==A+0xa0)f.words[A+0x9c]=bits(22);if(f.mutate && a==A+0x90)f.words[A+0xa0]=bits(33);};
  s.setReal=[](void* p,EventEntry&,int32_t d,float v){auto& f=*static_cast<Fixture*>(p);if(!f.outputReady)return false;const auto n=f.destinations.size();assert(n<3);f.destinations.push_back(d);f.outputs[n]=v;if(f.mutate && n==0){f.words[A+0x90]=bits(44);f.words[0x208]=555;}return true;};
  s.cameraHeading=[](void* p,uint32_t i)->std::optional<float>{assert(i==4);auto& f=*static_cast<Fixture*>(p);if(!f.cameraReady)return {};return f.camera;};return s;
 }
 int run(uint32_t fn,bool first=true){return fieldNpcActorEvent(fn,event,first,cursor,services());}
};
void storesAndOrder(){
 Fixture f;f.args({1,99,0x100});f.words[A]=3;f.words[A+4]=123;assert(f.run(Flag)==2 && f.words[A]==0x103 && f.words[A+4]==123 && f.name==99 && f.values==3);
 f.args({0,99,1});assert(f.run(Flag)==2 && f.words[A]==0x102);
 f.words.erase(A);f.stores.clear();f.args({2,99,1});assert(f.run(Flag)==2 && f.stores.empty() && !f.needsActor);
 f.args({99,1,2,3});f.mutate=true;f.reads.clear();f.stores.clear();assert(f.run(Position)==2);
 assert((f.stores==std::vector<uint32_t>{A+0x98,A+0x9c,A+0xa0,A+0x8c,A+0x90,A+0x94}));
 assert(f.words[A+0x8c]==bits(1) && f.words[A+0x90]==bits(22) && f.words[A+0x94]==bits(33));
 auto at=std::find(f.reads.begin(),f.reads.end(),A+0x98);assert(at!=f.reads.end() && at+1!=f.reads.end() && *(at+1)==A+0x9c);
 f.args({99,-725});f.stores.clear();assert(f.run(Yaw)==2 && f.words[A+0x144]==bits(-725) && f.stores.size()==1);
}
void holdsAndOutputs(){
 Fixture f;f.args({99,1,2,3});f.available=false;assert(f.run(Position)==0 && f.stores.empty());const auto evaluated=f.reals;f.words[0x204]=999;f.event.arguments=0x300;f.available=true;assert(f.run(Position,false)==2 && f.reals==evaluated && f.words[A+0x8c]==bits(1));
 f.event.arguments=0x200;f.args({99,-30000000,-50000000,-19000000});f.mutate=true;f.outputReady=false;assert(f.run(Get)==0 && f.destinations.empty());f.words[A+0x8c]=bits(999);f.outputReady=true;assert(f.run(Get,false)==2);
 assert((f.destinations==std::vector<int32_t>{-30000000,-50000000,-19000000}) && f.outputs[0]==1 && f.outputs[1]==44 && f.outputs[2]==3);
}
void cameraMath(){
 Fixture f;f.args({99,0});f.cameraReady=false;assert(f.run(LR)==0 && f.stores.empty());const auto eval=f.reals,lookups=f.resolves;f.cameraReady=true;assert(f.run(LR,false)==2 && f.words[A+0x144]==bits(-90) && f.reals==eval && f.resolves==lookups);
 f.args({99,1});assert(f.run(LR)==2 && f.words[A+0x144]==bits(90));
 f.camera=359.99f;assert(f.run(LR)==2 && f.words[A+0x144]==bits(90));
 f.camera=123.45f;f.args({99,0});assert(f.run(LR)==2 && f.words[A+0x144]==bits(33));
 f.camera=-0.05f;assert(f.run(LR)==2 && f.words[A+0x144]==bits(-90));
 // Retained output belongs to one event/function; a different identity holds.
 f.cameraReady=false;assert(f.run(LR)==0);++f.event.id;f.cameraReady=true;assert(f.run(LR,false)==0);
}
}
int main(){storesAndOrder();holdsAndOutputs();cameraMath();}
