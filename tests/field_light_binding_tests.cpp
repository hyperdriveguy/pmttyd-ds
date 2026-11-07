#include "field_light_binding.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t b=0;std::memcpy(&b,&f,4);return b;}
float real(uint32_t b){float f=0;std::memcpy(&f,&b,4);return f;}
struct Fixture {
 std::map<uint32_t,uint32_t> memory;
 EventEntry event;unsigned calls=0,holdAt=0;bool held=false;
 std::vector<uint32_t> values,lookups,sdkCalls;
 Fixture(int32_t mode){
  event.id=19;event.arguments=0x100;
  memory[0x100]=0x1000;memory[0x104]=0x2000;memory[0x108]=uint32_t(mode);
  for(unsigned n=0;n<6;++n)memory[0x802f3748+4*n]=0;
  matrix(0x301c,{1,0,0,10,0,1,0,20,0,0,1,30});
  matrix(0x30ac,{2,0,0,100,0,3,0,200,0,0,4,300});
  for(unsigned n=0;n<3;++n)memory[0x403c+4*n]=bits(float(13+12*n));
  memory[0x4040]=bits(25);memory[0x4044]=bits(37);
 }
 void matrix(uint32_t address,std::array<float,12> values){for(unsigned n=0;n<12;++n)memory[address+4*n]=bits(values[n]);}
 bool admit(){++calls;if(calls==holdAt && !held){held=true;return false;}return true;}
 FieldLightBindServices services(){
  FieldLightBindServices s;s.user=this;
  s.word=[](void* p,uint32_t address,uint32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit())return false;auto i=f.memory.find(address);if(i==f.memory.end())return false;out=i->second;return true;};
  s.value=[](void* p,EventEntry&,int32_t raw,int32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit())return false;f.values.push_back(uint32_t(raw));out=raw;return true;};
  s.lookup=[](void* p,uint32_t fn,uint32_t name,uint32_t& out){auto& f=*static_cast<Fixture*>(p);if(!f.admit())return false;f.lookups.push_back(fn);assert(name==(fn==0x8001bcf8?0x1000:0x2000));out=fn==0x8001bcf8?0x4000:0x3000;return true;};
  s.sdk=[](void* p,FieldLightSdkRequest& q){auto& f=*static_cast<Fixture*>(p);if(!f.admit())return false;f.sdkCalls.push_back(q.function);
   if(q.function==0x8029e6a4){for(unsigned row=0;row<3;++row){float out=0;for(unsigned col=0;col<4;++col){const auto m=q.matrix?f.memory.at(q.matrix+4*(4*row+col)):q.localMatrix[4*row+col];out+=real(m)*(col==3?1.0f:real(q.vector[col]));}q.output[row]=bits(out);}return true;}
   if(q.function==0x8029eba4){for(unsigned n=0;n<3;++n)q.output[n]=bits(real(f.memory.at(q.source+4*n))-real(q.vector[n]));return true;}
   if(q.function==0x8029e230){q.outputMatrix={};for(unsigned n=0;n<3;++n){q.outputMatrix[5*n]=bits(1);q.outputMatrix[4*n+3]=q.vector[n];}return true;}
   assert(q.function==0x8029eb80 && q.destination==0x4024);for(unsigned n=0;n<3;++n)f.memory[q.destination+4*n]=bits(real(q.vector[n])+real(q.other[n]));return true;
  };return s;
 }
 void position(std::array<float,3> expected){for(unsigned n=0;n<3;++n)assert(memory.at(0x4024+4*n)==bits(expected[n]));}
};
}
void sourceLight(){
 for(int32_t mode:{-1,0,1,2}){
  const std::array<float,3> expected=mode==0?std::array<float,3>{116,235,358}:mode==1?std::array<float,3>{113,225,337}:std::array<float,3>{13,25,37};
  Fixture baseline(mode);auto s=baseline.services();FieldLightBindCursor c;
  assert(fieldLightBindEvent(baseline.event,-7,s,c)==0 && !c.active());baseline.position(expected);
  assert(uint32_t(baseline.event.waitCount)==bits(3) && uint32_t(baseline.event.waitStart>>32)==bits(5) && uint32_t(baseline.event.waitStart)==bits(7));
  const auto total=baseline.calls;
  // Hold every reached operand/read/lookup/SDK child. Retrying with first0
  // must retain the original first capture and completed child prefixes.
  for(unsigned hold=1;hold<=total;++hold){Fixture f(mode);f.holdAt=hold;auto world=f.services();FieldLightBindCursor held;assert(!fieldLightBindEvent(f.event,9,world,held));assert(f.held && held.active());assert(fieldLightBindEvent(f.event,0,world,held)==0 && !held.active());f.position(expected);assert(f.values==baseline.values && f.lookups==baseline.lookups && f.sdkCalls==baseline.sdkCalls);}
  baseline.matrix(0x301c,{1,0,0,11,0,1,0,22,0,0,1,33});baseline.memory[0x403c]=bits(999);
  assert(fieldLightBindEvent(baseline.event,0,s,c)==0);auto moved=expected;moved[0]+=1;moved[1]+=2;moved[2]+=3;baseline.position(moved);
  assert(uint32_t(baseline.event.waitCount)==bits(3));
  // A completed poll resolves fresh mode. Unknown signed values skip Mac.
  baseline.memory[0x108]=99;assert(fieldLightBindEvent(baseline.event,0,s,c)==0);baseline.position({14,27,40});
 }
 // Late callback installation preserves the first call before any provider
 // admission. Previously this could skip scratch initialization on retry0.
 for(unsigned missing=0;missing<4;++missing){Fixture f(0);auto complete=f.services();auto unavailable=complete;FieldLightBindCursor c;
  if(missing==0)unavailable.word=nullptr;
  if(missing==1)unavailable.value=nullptr;
  if(missing==2)unavailable.lookup=nullptr;
  if(missing==3)unavailable.sdk=nullptr;
  assert(!fieldLightBindEvent(f.event,5,unavailable,c) && c.phase==FieldLightBindPhase::ReadLightArgument && c.first && f.calls==0);
  auto changed=complete;changed.user=nullptr;assert(!fieldLightBindEvent(f.event,0,changed,c) && f.calls==0);
  assert(fieldLightBindEvent(f.event,0,complete,c)==0);f.position({116,235,358});
 }
 // Arguments are captured only when reached: raw A0 survives value hold;
 // A4/A8 remain fresh until their later source reads.
 Fixture f(0);f.holdAt=2;auto s=f.services();FieldLightBindCursor c;
 assert(!fieldLightBindEvent(f.event,1,s,c));f.memory[0x100]=0xdead;f.memory[0x108]=2;
 assert(fieldLightBindEvent(f.event,0,s,c)==0);f.position({13,25,37});assert(f.values.front()==0x1000 && f.values.back()==2);
}
int main(){sourceLight();}
