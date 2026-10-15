#include "animation_initialize.hpp"
#include "animation_tangent.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <map>
#include <vector>
#include <cstdio>
#include <string>
using namespace ttyd;
namespace {
uint32_t floatBits(float v){uint32_t u;std::memcpy(&u,&v,4);return u;}
struct Startup {
 uint32_t work=0x81000000,next=0x82000000,hold=0,holdStore=0,handle=0x83000000,holder=0x84000000,payload=0x85000000;
 std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
 std::vector<BattleSequenceCall> calls;std::vector<std::pair<uint32_t,uint32_t>> writes;
 std::vector<uint32_t> reads;std::vector<double> angles;
 std::function<void(BattleSequenceCall&)> onCall;
 std::function<void(uint32_t,uint32_t)> onStore;
 std::function<void(uint32_t)> onRead;
 bool tangentReady=true,hasVivian=true;
 Startup(){words[0x80415dc0]=work;words[0x8041fbb4]=0x3c8efa33;words[0x802c1070]=0x43300000;words[0x802c1074]=0x80000000;
  words[0x80415db4]=0x80305900;words[handle+0xa0]=holder;words[holder]=payload;words[work+0xf8]=0x12345678;words[work+0xfc]=0xabcdef01;}
 AnimationInitializeServices services(){AnimationInitializeServices s;s.memory.user=this;
  s.memory.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Startup*>(p);f.reads.push_back(a);if(f.onRead)f.onRead(a);auto at=f.words.find(a);return at==f.words.end()?std::nullopt:std::optional<uint32_t>(at->second);};
  s.memory.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Startup*>(p);if(a==f.holdStore)return false;f.words[a]=v;f.writes.push_back({a,v});if(f.onStore)f.onStore(a,v);return true;};
  s.memory.call=[](void* p,BattleSequenceCall& c){auto& f=*static_cast<Startup*>(p);f.calls.push_back(c);if(c.function==f.hold)return false;
   if(c.function==0x800300f0){c.result=f.next;f.next+=c.args[1]+0x100;}
   else if(c.function==0x8018a61c)c.result=c.args[1]==0x802c11d8?(f.hasVivian?f.handle:0):f.handle+0xb0;
   else if(c.function==0x80005634)for(uint32_t i=0;i<c.args[2];++i)f.bytes[c.args[0]+i]=f.bytes.at(c.args[1]+i);
   if(f.onCall)f.onCall(c);
   return true;};
  s.doubleWord=[](void* p,uint32_t a)->std::optional<uint64_t>{auto& f=*static_cast<Startup*>(p);auto hi=f.words.find(a),lo=f.words.find(a+4);if(hi==f.words.end() || lo==f.words.end())return std::nullopt;return uint64_t(hi->second)<<32|lo->second;};
  s.tangentUser=this;s.tangent=[](void* p,double a,double& r){auto& f=*static_cast<Startup*>(p);f.angles.push_back(a);if(!f.tangentReady)return false;return nativeAnimationTangent(nullptr,a,r);};return s;}
 void originalVivian(const char* root){auto path=std::string(root)+"/animation-startup/a/vivian.bin";FILE* f=std::fopen(path.c_str(),"rb");assert(f);for(unsigned i=0;i<0xc10;++i){int b=std::fgetc(f);assert(b>=0);bytes[payload+i]=uint8_t(b);}assert(std::fgetc(f)==EOF);std::fclose(f);}
 unsigned count(uint32_t fn) const{unsigned n=0;for(auto& c:calls)n+=c.function==fn;return n;}
};
}
void runAnimationInitializeFixture(const char* root){
 {Startup f;f.originalVivian(root);AnimationInitializeCursor c;assert(initializeAnimation(f.services(),c));
  assert(f.calls.size()==11 && f.count(0x800300f0)==5 && f.count(0x8018a61c)==2 && f.count(0x80005634)==2 && f.count(0x8018a260)==1 && f.count(0x80189d38)==1);
  const uint32_t sizes[]={1024,768,0x17000,0x3000,0x180000}; // 11 calls: heap5,file2,copy2,free1,archive1.
  unsigned index=0;for(auto& call:f.calls)if(call.function==0x800300f0){assert(call.args[0]==0 && call.args[1]==sizes[index]);++index;}
  assert(f.words[f.work+4]==64 && f.words[f.work+0xc]==64 && f.words[f.work+0x14]==256 && f.words[f.work+0xf0]==1024 && f.words[f.work+0xf4]==0 && f.words[f.work+0xe8]==0);
  assert(f.words[f.work+0xf8]==0x12345678 && f.words[f.work+0xfc]==0xabcdef01 && f.words[f.work+0xd8]==1 && f.words[f.work+0xdc]==1 && f.words[f.work+0x104]==f.words[f.work+0x108] && f.words[f.work+0x100]==f.handle+0xb0);
  assert(f.angles.size()==178 && f.angles[89]==0);
  for(unsigned i=0;i<178;++i)assert(f.words[0x803d9580+4*i]==floatBits(nativeAnimationTangents()[i]));
  for(unsigned i=0;i<0xc00;++i)assert(f.bytes[0x80305900+i]==f.bytes[f.payload+i]);
  for(unsigned i=0;i<16;++i)assert(f.bytes[0x80415da0+i]==f.bytes[f.payload+0xc00+i]);
  const auto oldPool=f.words[f.work];assert(initializeAnimation(f.services(),c) && f.count(0x800300f0)==10 && f.words[f.work]!=oldPool && f.count(0x8018a260)==2 && f.words[f.work+0xf8]==0x12345678);}

 {Startup f;f.hasVivian=false;AnimationInitializeCursor c;assert(initializeAnimation(f.services(),c) && f.count(0x80005634)==0 && f.count(0x8018a260)==0 && f.count(0x80189d38)==1);}
 // Real wait, captured allocation size and fresh W publication after child.
 {Startup f;f.hasVivian=false;f.hold=0x800300f0;AnimationInitializeCursor c;
  assert(!initializeAnimation(f.services(),c) && c.pending==f.hold);auto writes=f.writes.size();f.words[f.work+4]=7;
  assert(!initializeAnimation(f.services(),c) && f.writes.size()==writes && f.calls.back().args[1]==1024);
  uint32_t newer=f.work+0x1000;f.words[0x80415dc0]=newer;f.words[newer+0xc]=2;f.words[newer+0x14]=3;f.words[newer+0xf0]=4;f.hold=0;
  assert(initializeAnimation(f.services(),c) && f.words[newer]==0x82000000 && !f.words.count(f.work));
  assert(f.calls[3].args[1]==24 && f.calls[4].args[1]==0x450 && f.calls[5].args[1]==48);}
 // Scratch's literal null test must survive observable work mutation. Factor
 // and bias are captured once; pending tan never recaptures them or its row.
 {Startup f;f.hasVivian=false;f.tangentReady=false;AnimationInitializeCursor c;
  assert(!initializeAnimation(f.services(),c) && c.pending==0x802724e8 && f.count(0x800300f0)==5);const double angle=c.angle;auto writes=f.writes.size();
  f.words[0x8041fbb4]=floatBits(2.f);f.words[0x802c1074]=0;assert(!initializeAnimation(f.services(),c) && c.angle==angle && f.writes.size()==writes);
  f.tangentReady=true;assert(initializeAnimation(f.services(),c) && f.words[0x803d9580]==floatBits(nativeAnimationTangents()[0]));}
 {Startup f;f.hasVivian=false;f.holdStore=0x803d9580;AnimationInitializeCursor c;
  assert(!initializeAnimation(f.services(),c) && f.angles.size()==1);auto writes=f.writes.size();assert(!initializeAnimation(f.services(),c) && f.angles.size()==1 && f.writes.size()==writes);
  f.holdStore=0;assert(initializeAnimation(f.services(),c) && f.angles.size()==178);}
 {Startup f;f.hasVivian=false;AnimationInitializeCursor c;f.onStore=[&](uint32_t a,uint32_t){if(a==f.work+0x10c)f.words[f.work+0x104]=0x90000000;};
  assert(initializeAnimation(f.services(),c) && f.count(0x800300f0)==4 && f.words[f.work+0x108]==0x90000000);}
 {Startup f;f.hasVivian=false;AnimationInitializeCursor c;f.onCall=[](BattleSequenceCall& call){if(call.function==0x800300f0)call.result=0;};
  assert(initializeAnimation(f.services(),c) && f.words[f.work]==0 && f.words[f.work+0x10]==0 && f.words[f.work+0x104]==0);}
 {Startup f;f.hasVivian=false;AnimationInitializeCursor c;f.words[f.work+0x1104]=0x90000000;
  f.onRead=[&](uint32_t a){if(a==0x8041fbb4)f.words[0x80415dc0]=f.work+0x1000;};
  assert(initializeAnimation(f.services(),c) && f.words[f.work+0x108]==f.words[f.work+0x104] && !f.words.count(f.work+0x1108));}
 {Startup f;f.hasVivian=false;AnimationInitializeCursor c;auto services=f.services();services.doubleWord=nullptr;
  assert(!initializeAnimation(services,c) && f.angles.empty());const auto writes=f.writes.size();assert(!initializeAnimation(services,c) && f.writes.size()==writes);
  assert(initializeAnimation(f.services(),c));}
 {Startup f;f.hasVivian=false;AnimationInitializeCursor c;auto services=f.services();services.tangent=[](void*,double,double& out){out=INFINITY;return true;};
  assert(initializeAnimation(services,c) && f.words[0x803d9580]==0x7f800000 && f.words[0x803d9844]==0x7f800000);}
 // Holder and destination are captured in source order, then holder/payload
 // reloaded for the second copy. A null second file handle is still published.
 {Startup f;f.originalVivian(root);AnimationInitializeCursor c;const auto old=f.payload;const auto newer=f.payload+0x10000;
  for(unsigned i=0;i<16;++i)f.bytes[newer+0xc00+i]=uint8_t(255-i);
  f.onRead=[&](uint32_t a){if(a==0x80415db4)f.words[f.handle+0xa0]=f.holder+4;};f.words[f.holder+4]=newer;
  f.onCall=[&](BattleSequenceCall& call){if(call.function==0x8018a61c && call.args[1]==0x802c11e8){call.result=0;f.words[0x80415dc0]=f.work+0x1000;}};
  assert(initializeAnimation(f.services(),c));for(unsigned i=0;i<16;++i)assert(f.bytes[0x80415da0+i]==uint8_t(255-i));assert(f.bytes[0x80305900]==f.bytes[old] && f.words[f.work+0x1100]==0);}
}
int main(){runAnimationInitializeFixture("romfs");}
