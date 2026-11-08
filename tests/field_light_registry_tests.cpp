#include "field_light_registry.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
using namespace FieldLightRegistrySource;
constexpr uint32_t Bank0=0x85000000,Bank1=Bank0+Capacity*RecordBytes;
constexpr uint32_t InputName=0x1000,InputPosition=0x1100,InputVector=0x1200,InputColor=0x1300,FormatScratch=0x1400;
uint32_t bits(float f){uint32_t b=0;std::memcpy(&b,&f,4);return b;}
struct Fixture {
 std::map<uint32_t,uint8_t> memory;
 std::vector<std::tuple<unsigned,uint32_t,uint32_t>> trace;
 unsigned calls=0,hold=0,allocations=0;int32_t selected=0;bool nullFirst=false;
 std::function<void(uint32_t,uint32_t)> afterStore;
 std::function<void()> afterFormat;
 explicit Fixture(const char* root){
  EventScriptAssets data;assert(data.load((std::string(root)+"/field-light-registry.pmevt").c_str()));
  for(const auto& segment:data.segments())for(uint32_t i=0;i<segment.size;++i){uint8_t b=0;assert(data.readByte(segment.address+i,b));memory[segment.address+i]=b;}
  for(uint32_t i=0;i<4*RecordBytes;++i)memory[Presets+i]=0xa5;
  text(InputName,"lamp");for(unsigned n=0;n<3;++n){put(InputPosition+4*n,bits(float(n+1)));put(InputVector+4*n,bits(float(n+4)));}put(InputColor,0x123456ff);
 }
 void put(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)memory[a+i]=uint8_t(v>>(24-8*i));}
 uint32_t word(uint32_t a) const{uint32_t v=0;for(unsigned i=0;i<4;++i)v=(v<<8)|memory.at(a+i);return v;}
 uint16_t half(uint32_t a) const{return uint16_t(memory.at(a))<<8|memory.at(a+1);}
 void text(uint32_t a,const std::string& name){for(unsigned i=0;i<=name.size();++i)memory[a+i]=i==name.size()?0:uint8_t(name[i]);}
 std::string text(uint32_t a) const{std::string out;for(unsigned n=0;n<256;++n){const auto b=memory.at(a+n);if(!b)return out;out.push_back(char(b));}assert(false);return {};}
 bool admit(unsigned kind,uint32_t a=0,uint32_t b=0){if(++calls==hold)return false;trace.emplace_back(kind,a,b);return true;}
 FieldLightEntryArguments arguments() const{return {InputName,1,InputPosition,InputVector,InputColor,0,2,{bits(0),bits(1)},FormatScratch};}
 FieldLightRegistryServices services(){
  FieldLightRegistryServices s;s.user=this;
  s.group=[](void* p,int32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit(1))return false;out=f.selected;return true;};
  s.word=[](void* p,uint32_t a,uint32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit(2,a))return false;for(unsigned i=0;i<4;++i)if(!f.memory.count(a+i))return false;out=f.word(a);return true;};
  s.half=[](void* p,uint32_t a,uint16_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit(3,a))return false;out=f.half(a);return true;};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);if(!f.admit(4,a,v))return false;f.put(a,v);if(f.afterStore)f.afterStore(a,v);return true;};
  s.storeHalf=[](void* p,uint32_t a,uint16_t v){auto& f=*static_cast<Fixture*>(p);if(!f.admit(5,a,v))return false;f.memory[a]=uint8_t(v>>8);f.memory[a+1]=uint8_t(v);return true;};
  s.allocate=[](void* p,uint32_t heap,uint32_t size,uint32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit(6,heap,size))return false;assert(heap==0 && size==Capacity*RecordBytes);const auto index=f.allocations++;out=f.nullFirst && index==0?0:Bank0+index*size;if(out)for(uint32_t i=0;i<size;++i)f.memory[out+i]=0;return true;};
  s.clear=[](void* p,uint32_t a,uint32_t size){auto& f=*static_cast<Fixture*>(p);if(!f.admit(7,a,size))return false;for(uint32_t i=0;i<size;++i)f.memory[a+i]=0;return true;};
  s.copy=[](void* p,uint32_t to,uint32_t from){auto& f=*static_cast<Fixture*>(p);if(!f.admit(8,to,from))return false;f.text(to,f.text(from));return true;};
  s.compare=[](void* p,uint32_t a,uint32_t b,int32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit(9,a,b))return false;out=f.text(a).compare(f.text(b));return true;};
  s.format=[](void* p,uint32_t to,uint32_t format,uint32_t name,int32_t suffix){auto& f=*static_cast<Fixture*>(p);if(!f.admit(10,to,name))return false;assert(f.text(format)=="%s_%04d");const auto prefix=f.text(name);char tail[32]{};std::snprintf(tail,sizeof(tail),"_%04d",int(suffix));f.text(to,prefix+tail);if(f.afterFormat)f.afterFormat();return true;};
  s.scale=[](void* p,uint32_t scale,std::array<uint32_t,12>& matrix){auto& f=*static_cast<Fixture*>(p);if(!f.admit(11,scale))return false;assert(scale==bits(3));matrix={};for(unsigned n=0;n<3;++n)matrix[5*n]=scale;return true;};
  s.multiply=[](void* p,const std::array<uint32_t,12>& matrix,uint32_t from,uint32_t to){auto& f=*static_cast<Fixture*>(p);if(!f.admit(12,from,to))return false;assert(matrix[0]==bits(3));for(unsigned n=0;n<3;++n){float v=0;const auto raw=f.word(from+4*n);std::memcpy(&v,&raw,4);f.put(to+4*n,bits(v*3));}return true;};
  return s;
 }
 void initialize(){auto s=services();FieldLightRegistryCursor c;assert(initializeFieldLights(s,c));}
 void clearTrace(){trace.clear();calls=0;}
};
}
void registrySource(const char* root){
 Fixture baseline(root);auto s=baseline.services();FieldLightRegistryCursor c;
 assert(initializeFieldLights(s,c) && c.operation==FieldLightOperation::Idle);
 assert(baseline.word(Work)==Capacity && baseline.word(Work+4)==Bank0 && baseline.word(Work+8)==Capacity && baseline.word(Work+12)==Bank1);
 for(unsigned n=0;n<4;++n){assert(baseline.half(Presets+n*RecordBytes)==0x1040);assert(baseline.word(Presets+n*RecordBytes+Reference)==0xa5a5a5a5);}
 assert(baseline.word(Presets+RecordBytes+Position)==bits(-600) && baseline.word(Presets+RecordBytes+Position+4)==bits(300));
 const auto initCalls=baseline.calls;
 for(unsigned hold=1;hold<=initCalls;++hold){Fixture f(root);f.hold=hold;auto world=f.services();FieldLightRegistryCursor cursor;assert(!initializeFieldLights(world,cursor));assert(initializeFieldLights(world,cursor));assert(f.trace==baseline.trace && f.memory==baseline.memory && f.allocations==2);}
 Fixture nullAllocation(root);nullAllocation.nullFirst=true;auto nullable=nullAllocation.services();assert(initializeFieldLights(nullable,c));assert(nullAllocation.word(Work+4)==0 && nullAllocation.word(Work+12)==Bank1 && nullAllocation.half(Presets)==0x1040);
 baseline.clearTrace();auto args=baseline.arguments();assert(enterFieldLight(args,s,c)==0);const auto entryCalls=baseline.calls;
 assert(baseline.text(Bank0+Name)=="lamp" && baseline.half(Bank0)==0x1081);
 assert(baseline.word(Bank0+Position)==bits(1) && baseline.word(Bank0+Reference)==bits(1) && baseline.word(Bank0+Rgba)==0x123456ff);
 for(unsigned hold=1;hold<=entryCalls;++hold){Fixture f(root);f.initialize();f.clearTrace();f.hold=hold;auto world=f.services();FieldLightRegistryCursor cursor;assert(!enterFieldLight(args,world,cursor));auto changed=args;changed.type=0;assert(enterFieldLight(changed,world,cursor)==0);assert(f.trace==baseline.trace && f.memory==baseline.memory);}
 // Cumulative duplicate alias: x_0000 already exists, so the second suffix
 // appends to the prior candidate instead of starting from original x again.
 Fixture duplicate(root);duplicate.initialize();auto world=duplicate.services();auto a=duplicate.arguments();duplicate.text(InputName,"x");assert(enterFieldLight(a,world,c)==0);duplicate.text(InputName,"x_0000");assert(enterFieldLight(a,world,c)==1);duplicate.text(InputName,"x");assert(enterFieldLight(a,world,c)==2);assert(duplicate.text(Bank0+2*RecordBytes+Name)=="x_0000_0001");
 // Fresh count/array after formatting, including the source hard32 decision
 // after a smaller capacity exhausted without finding the name.
 Fixture swapped(root);swapped.initialize();swapped.put(Work,16);swapped.afterFormat=[&]{swapped.put(Work,Capacity);swapped.put(Work+4,Bank1);};auto swapServices=swapped.services();assert(enterFieldLight(swapped.arguments(),swapServices,c)==0);assert(swapped.text(Bank1+Name)=="lamp_0000" && swapped.half(Bank0)==0);
 // COLOR capture precedes destination POSITION writes; reference XYZ reloads
 // after current XYZ publication. Complete core preserves real alias effects.
 Fixture mutableInput(root);mutableInput.initialize();auto mutableServices=mutableInput.services();auto alias=mutableInput.arguments();alias.color=Bank0+Position;mutableInput.afterStore=[&](uint32_t address,uint32_t){if(address==Bank0+Position+8)for(unsigned n=0;n<3;++n)mutableInput.put(InputPosition+4*n,bits(float(n+10)));};assert(enterFieldLight(alias,mutableServices,c)==0);assert(mutableInput.word(Bank0+Rgba)==0 && mutableInput.word(Bank0+Position)==bits(1) && mutableInput.word(Bank0+Reference)==bits(10));
 // Complete source does not gracefully reject a full array. Fake guest memory
 // safely records the actual one-past clear/write; native owner rejects it.
 Fixture full(root);full.initialize();for(unsigned n=0;n<Capacity;++n){full.memory[Bank0+n*RecordBytes]=0;full.memory[Bank0+n*RecordBytes+1]=1;full.text(Bank0+n*RecordBytes+Name,"busy"+std::to_string(n));}auto fullServices=full.services();assert(enterFieldLight(full.arguments(),fullServices,c)==int32_t(Capacity));assert(full.text(Bank1+Name)=="lamp" && full.half(Bank1)==0x1081);
 for(int32_t group:{0,1,-1,2}){Fixture f(root);f.initialize();f.selected=group;auto w=f.services();assert(enterFieldLight(f.arguments(),w,c)==0);const auto bank=group==1?Bank1:Bank0;assert(findFieldLight(InputName,w,c)==bank);assert(resetFieldLights(w,c) && f.half(bank)==0 && f.word(Work)==Capacity && f.word(Work+4)==Bank0 && f.half(Presets)==0x1040);assert(findFieldLight(InputName,w,c)==0);}
}
int main(){registrySource("romfs");}
