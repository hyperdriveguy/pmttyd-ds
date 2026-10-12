#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float value) {uint32_t word;std::memcpy(&word,&value,4);return word;}
struct Fixture {
    std::map<uint32_t,uint8_t> bytes;std::vector<uint32_t> reads,writes,keys,handles,offs;
    std::vector<std::array<float,3>> triggered,positioned;std::function<void(uint32_t)> onStore,onRead;std::function<void()> onTrigger,onOff;
    uint32_t result=7;
    uint8_t b(uint32_t a) {auto i=bytes.find(a);return i==bytes.end()?0:i->second;}
    uint32_t w(uint32_t a) {return uint32_t(b(a))<<24|uint32_t(b(a+1))<<16|uint32_t(b(a+2))<<8|b(a+3);}
    uint16_t h(uint32_t a) {return uint16_t(uint16_t(b(a))<<8|b(a+1));}
    void w(uint32_t a,uint32_t v) {bytes[a]=v>>24;bytes[a+1]=v>>16;bytes[a+2]=v>>8;bytes[a+3]=v;}
    void h(uint32_t a,uint16_t v) {bytes[a]=v>>8;bytes[a+1]=v;}
    Fixture() {for(unsigned i=0;i<3;++i)w(0x103c+4*i,bits(float(i+1)));w(0x2018,0xffffffff);for(unsigned i=8;i<12;++i)bytes[0x2000+i]=0xab;}
    BattleSoundPlaybackServices playback() {
        BattleSoundPlaybackServices s;s.user=this;
        s.trigger=[](void* p,uint32_t unit,uint32_t key,int32_t raw,int32_t zero,const std::array<float,3>& xyz)->uint32_t {auto& f=*static_cast<Fixture*>(p);assert(unit==0x1000 && raw==-250000000 && zero==0);f.keys.push_back(key);f.triggered.push_back(xyz);if(f.onTrigger)f.onTrigger();return f.result;};
        s.position=[](void* p,uint32_t handle,const std::array<float,3>& xyz) {auto& f=*static_cast<Fixture*>(p);f.handles.push_back(handle);f.positioned.push_back(xyz);};
        s.off=[](void* p,uint32_t handle) {auto& f=*static_cast<Fixture*>(p);f.offs.push_back(handle);if(f.onOff)f.onOff();};return s;
    }
    BattlePackedMovementSoundServices services() {
        BattlePackedMovementSoundServices s;s.playback=playback();auto& m=s.position.memory.memory;m.user=this;
        m.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(f.onRead)f.onRead(a);return f.w(a);};m.half=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(f.onRead)f.onRead(a);return f.h(a);};
        m.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.w(a,v);if(f.onStore)f.onStore(a);};m.storeHalf=[](void* p,uint32_t a,uint16_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.h(a,v);if(f.onStore)f.onStore(a);};return s;
    }
    void run(uint32_t part=0) {controlPackedMovementSound(0x1000,0x2000,part,{10,20,30},services());}
};
int main() {
    for(int type:{0,1,2,3}) {Fixture f;const uint32_t offset=type==0?8:type==1?0x18:type==2?0:0x28;f.w(0x4000+offset,42);f.w(0x4004+offset,43);for(unsigned i=0;i<3;++i)f.h(0x4000+offset+8+2*i,uint16_t(0xfff0+i));loadPackedUnitSoundMode(type,-1,0x4000,0x2000,f.services().position.memory.memory);assert(f.w(0x2000)==42 && f.w(0x2004)==43 && f.h(0x200c)==(type==2?0:0xfff0) && f.h(0x2012)==f.h(0x200c) && f.h(0x2014)==0 && f.w(0x2018)==0xffffffff && f.bytes[0x2008]==0xab);assert(f.writes==std::vector<uint32_t>({0x2014,0x2018,0x2000,0x2004,0x200c,0x200e,0x2010,0x2012}));}
    Fixture retain;retain.w(0x2000,42);retain.w(0x2004,43);retain.h(0x200c,0xfff0);retain.h(0x200e,4);retain.h(0x2010,5);loadPackedUnitSoundMode(-999,-2,0xdeadbeef,0x2000,retain.services().position.memory.memory);assert(retain.w(0x2000)==42 && retain.h(0x200e)==4 && retain.h(0x2012)==0xfff0 && retain.reads==std::vector<uint32_t>({0x200c}) && retain.writes==std::vector<uint32_t>({0x2014,0x2018,0x2012}));
    for(int mode:{0,1,-3}) {Fixture f;f.w(0x2000,42);loadPackedUnitSoundMode(0,mode,0xdeadbeef,0x2000,f.services().position.memory.memory);assert(f.w(0x2000)==0 && f.h(0x2012)==0 && f.bytes[0x2008]==0xab && f.reads==std::vector<uint32_t>({0x200c}));}
    for(int type:{-1,4}) {Fixture f;loadPackedUnitSoundMode(type,-1,0xdeadbeef,0x2000,f.services().position.memory.memory);assert(f.w(0x2000)==0 && f.reads==std::vector<uint32_t>({0x200c}));}
    Fixture alias;alias.w(0x1ffc,42);alias.w(0x2000,99);loadPackedUnitSoundMode(2,-1,0x1ffc,0x2000,alias.services().position.memory.memory);assert(alias.w(0x2000)==42 && alias.w(0x2004)==42); // First store changes second source word.
    Fixture self;self.w(0x2008,0x11111111);self.w(0x200c,0x22222222);self.h(0x2010,0x3333);self.h(0x2012,0x4444);self.h(0x2014,0x5555);loadPackedUnitSoundMode(0,-1,0x2000,0x2000,self.services().position.memory.memory);assert(self.w(0x2000)==0x11111111 && self.w(0x2004)==0x22222222 && self.h(0x200c)==0x3333 && self.h(0x200e)==0x4444 && self.h(0x2010)==0 && self.h(0x2012)==0x3333);
    Fixture empty;const auto original=empty.bytes;empty.run();assert(empty.bytes==original && empty.writes.empty() && empty.keys.empty() && empty.reads==std::vector<uint32_t>({0x802ee494,0x802ee498,0x802ee49c,0x2000,0x2004}));
    Fixture retained;retained.w(0x2000,42);retained.onTrigger=[&] {retained.w(0x103c,bits(11));};retained.run();assert(retained.keys==std::vector<uint32_t>({42}) && retained.w(0x2018)==7 && retained.triggered[0]==std::array<float,3>({10,20,30}) && retained.positioned[0]==std::array<float,3>({11,2,3}));retained.run();assert(retained.keys.size()==1 && retained.handles==std::vector<uint32_t>({7,7}));
    Fixture input;input.w(0x2000,42);std::array<float,3> supplied{10,20,30};input.onRead=[&](uint32_t a) {if(a==0x802ee494)supplied[0]=999;};controlPackedMovementSound(0x1000,0x2000,0,supplied,input.services());assert(input.triggered[0]==std::array<float,3>({10,20,30})); // Incoming register values captured before template memory reads.
    Fixture failed;failed.result=0xffffffff;failed.w(0x2000,42);failed.run();failed.run();assert(failed.keys.size()==2 && failed.handles==std::vector<uint32_t>({0xffffffff,0xffffffff}));
    Fixture part;part.w(0x2000,42);part.w(0x31ac,0x10000000);part.w(0x34ec,0xdeadbeef);for(unsigned i=0;i<3;++i)part.w(0x3018+4*i,bits(float(70+i)));part.run(0x3000);assert(part.positioned[0]==std::array<float,3>({70,71,72}));
    Fixture periodic;periodic.w(0x2000,10);periodic.w(0x2004,20);periodic.h(0x200e,2);periodic.h(0x2010,0);periodic.w(0x2018,123);periodic.run();assert(periodic.keys==std::vector<uint32_t>({10}) && periodic.h(0x2012)==2 && periodic.h(0x2014)==1 && periodic.w(0x2018)==123 && periodic.handles.empty());periodic.run();periodic.run();assert(periodic.keys.size()==1);periodic.run();assert(periodic.keys==std::vector<uint32_t>({10,20}) && periodic.h(0x2014)==2);
    Fixture disabled;disabled.w(0x2000,10);disabled.w(0x2004,20);disabled.h(0x2012,0xffff);const auto noChange=disabled.bytes;disabled.run();assert(disabled.bytes==noChange && disabled.keys.empty());
    Fixture silent;silent.w(0x2004,20);silent.h(0x200e,3);silent.run();assert(silent.keys.empty() && silent.h(0x2014)==1 && silent.h(0x2012)==3); // First0/secondnonzero still updates alternating counters.
    Fixture wrap;wrap.w(0x2000,10);wrap.w(0x2004,20);wrap.h(0x2014,0x7fff);wrap.run();assert(wrap.h(0x2014)==0x8000 && wrap.keys==std::vector<uint32_t>({20}));
    Fixture fresh;fresh.w(0x2000,10);fresh.w(0x2004,20);fresh.onStore=[&](uint32_t a) {if(a==0x2014)fresh.w(0x2000,99);};fresh.run();assert(fresh.keys==std::vector<uint32_t>({99})); // Selected key reread after counter writes.
    retained.onOff=[&] {assert(retained.w(0x2018)==7);retained.w(0x2018,99);};endPackedMovementSound(0x2000,retained.services());assert(retained.offs==std::vector<uint32_t>({7}) && retained.w(0x2018)==0xffffffff);endPackedMovementSound(0x2000,retained.services());assert(retained.offs.size()==1);
    Fixture unit;unit.w(0x118c,42);unit.w(0x11a4,0xffffffff);controlPackedUnitMovementSound(0x1000,unit.services());assert(unit.triggered[0]==std::array<float,3>({1,2,3}) && unit.positioned[0]==std::array<float,3>({1,2,3}));assert(unit.reads[0]==0x103c && unit.reads[1]==0x1040 && unit.reads[2]==0x1044);endPackedUnitMovementSound(0x1000,unit.services());assert(unit.offs==std::vector<uint32_t>({7}));
    EventNativeRuntime runtime(0x81000000,{});assert(runtime.memory.registerRam(0x1000,std::vector<uint8_t>(0x200)) && runtime.memory.registerRam(0x4000,std::vector<uint8_t>(0x36)) && runtime.memory.registerRam(0x802ee494,std::vector<uint8_t>(12)));assert(runtime.memory.writeWord(0x4008,42) && runtime.memory.writeWord(0x400c,0) && runtime.memory.writeHalf(0x4010,2));auto services=nativeBattleMovementSoundServices(runtime,unit.playback());loadPackedUnitSoundMode(0,-1,0x4000,0x118c,runtime.memoryBinding.services());unit.keys.clear();unit.handles.clear();controlPackedUnitMovementSound(0x1000,services);assert(unit.keys==std::vector<uint32_t>({42}) && unit.handles==std::vector<uint32_t>({7}));endPackedUnitMovementSound(0x1000,services);
    std::cout<<"packed sound loader/counter/retained playback/native record tests passed\n";
}
