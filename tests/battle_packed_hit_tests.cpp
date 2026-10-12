#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float value) {uint32_t word;std::memcpy(&word,&value,4);return word;}
float real(uint32_t word) {float value;std::memcpy(&value,&word,4);return value;}
struct Fixture {
    EventEntry e;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads,writes;std::vector<int> math;
    std::vector<std::pair<int32_t,int32_t>> outputs;
    std::function<void()> onLookup,onStore,onMultiply,onOutput;
    Fixture() {
        e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=999;
        for(unsigned i=0;i<3;++i)words[0x1008+4*i]=uint32_t(-30000000+int32_t(i));
        words[0x8041eb90]=0x2000;words[0x203c]=0x4000;words[0x4014]=0x5000;
        words[0x54ec]=0x6000;bytes[0x400c]=1;bytes[0x2012]=1;bytes[0x6189]=255;
        words[0x4114]=bits(2);words[0x6114]=bits(100);words[0x804226a8]=bits(1);
        for(unsigned i=0;i<3;++i) {words[0x603c+4*i]=bits(float(10+10*i));words[0x5024+4*i]=bits(float(2+i));words[0x5018+4*i]=bits(float(100+100*i));}
        for(unsigned i=0;i<6;++i)words[0x5170+4*i]=bits(float(i+1));
    }
    BattlePartPositionServices services() {
        BattlePartPositionServices s;s.unit.user=this;s.unit.values.user=this;s.memory.user=this;s.memory.memory.user=this;s.numericUser=this;
        s.memory.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;if(f.onStore)f.onStore();};
        s.memory.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes[a];};s.unit.memory=s.memory.memory;
        s.unit.values.resolve=[](void*,EventEntry&,int32_t v) {return v;};s.unit.translate=[](void*,EventEntry&,int32_t v) {assert(v==-2);return 7;};
        s.unit.unit=[](void* p,uint32_t b,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(b==0x2000 && id==7);if(f.onLookup)f.onLookup();return 0x4000u;};
        s.unit.truncateFloat=[](void*,float v) {return int32_t(v);};s.unit.values.store=[](void* p,EventEntry&,int32_t d,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back({d,v});if(f.onOutput)f.onOutput();};
        s.multiply=[](void* p,float a,float b) {auto& f=*static_cast<Fixture*>(p);f.math.push_back(3);if(f.onMultiply)f.onMultiply();return a*b;};
        s.fma=[](void* p,float a,float b,float c) {static_cast<Fixture*>(p)->math.push_back(1);return std::fma(a,b,c);};
        s.add=[](void* p,float a,float b) {static_cast<Fixture*>(p)->math.push_back(2);return a+b;};return s;
    }
};
int main() {
    Fixture distinct;const auto xyz=packedHitPosition(0x4000,0x5000,distinct.services());
    assert(xyz==std::array<float,3>({18,37,52})); // Relative world 8/23/34 uses owner189=-1;hit factor=+1 and explicit unit scale=2.
    assert(distinct.math==std::vector<int>({1,2,2,3,1,3,1,1,3,1,3,1,1}));
    std::vector<uint32_t> expected{0x400c,0x8041eb90,0x2012,0x51ac,0x54ec,0x603c,0x6040,0x6044,0x5024,0x5028,0x502c,0x6189,0x5170,0x4114,0x804226a8,0x5174,0x4114,0x5178,0x4114,0x517c,0x4114,0x5180,0x4114,0x5184,0x4114};assert(distinct.reads==expected);
    Fixture negative;negative.bytes[0x400c]=255;negative.bytes[0x2002]=255;assert(packedHitPosition(0x4000,0x5000,negative.services())==std::array<float,3>({-2,37,52})); // Signed belong -1 selects work-8+0xa without local bounds repair.
    Fixture alias;alias.words[0x51ac]=0x10000000;writePackedHitPosition(0x4000,0x5000,{0xa000,0xa000,0xa000},alias.services());assert(real(alias.words[0xa000])==342); // World's last Z=300,then all six ordered additions share one output.
    Fixture scale;scale.words[0x51ac]=0x10000000;unsigned stores=0;scale.onStore=[&] {++stores;if(stores==4) {scale.words[0x4114]=bits(3);scale.bytes[0x2012]=255;scale.words[0x804226a8]=bits(99);}};
    writePackedHitPosition(0x4000,0x5000,{0xa000,0xa004,0xa008},scale.services());assert(real(scale.words[0xa000])==114 && real(scale.words[0xa004])==221 && real(scale.words[0xa008])==327); // Fresh scale for later axes;captured sign/Y constant.
    Fixture overlap;overlap.words[0x51ac]=0x10000000;writePackedHitPosition(0x4000,0x5000,{0x5174,0x5178,0x517c},overlap.services());assert(real(overlap.words[0x5174])==2318 && real(overlap.words[0x5178])==414 && real(overlap.words[0x517c])==1120); // World stores overwrite offsets,then subsequent hit stores affect later triplet reads.
    Fixture rounded;rounded.words[0x51ac]=0x10000000;rounded.words[0x4114]=bits(1-std::ldexp(1.0f,-23));rounded.words[0x5018]=rounded.words[0x501c]=rounded.words[0x5020]=bits(-1);rounded.words[0x5170]=rounded.words[0x5174]=rounded.words[0x5178]=bits(1+std::ldexp(1.0f,-23));for(unsigned i=3;i<6;++i)rounded.words[0x5170+4*i]=bits(0);const auto r=packedHitPosition(0x4000,0x5000,rounded.services());assert(r[0]==0 && r[1]==0 && r[2]==-std::ldexp(1.0f,-46)); // Z fused, X/Y rounded multiplication.
    Fixture event;event.onLookup=[&] {event.words[0x8041eb90]=0x3000;event.words[0x303c]=0x9000;event.words[0x9014]=0x5000;event.bytes[0x3012]=255;};event.words[0x1010]=0xf1194d80;event.onOutput=[&] {event.words[0x100c]=123;event.words[0x4114]=bits(999);};assert(packedHitPositionEvent(event.e,event.services())==2 && event.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000000,-2},{-29999999,37}})); // Initial unit retained,part lookup fresh,hit side table fresh,outputs/coordinates retained.
    EventNativePlatform platform;platform.truncate=[](void*,float v) {return int32_t(v);};platform.fmaFloat=[](void*,float a,float b,float c) {return std::fma(a,b,c);};platform.addFloat=[](void*,float a,float b) {return a+b;};platform.multiplyFloat=[](void*,float a,float b) {return a*b;};EventNativeRuntime runtime(0x81000000,platform);std::array<EventEntry,1> entries{};assert(runtime.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.owner=7;e.arguments=0x1000;
    Fixture native;assert(runtime.memory.registerRam(0x1000,std::vector<uint8_t>(20)) && runtime.memory.registerRam(0x2000,std::vector<uint8_t>(0x100)) && runtime.memory.registerRam(0x4000,std::vector<uint8_t>(0x190)) && runtime.memory.registerRam(0x5000,std::vector<uint8_t>(0x500)) && runtime.memory.registerRam(0x6000,std::vector<uint8_t>(0x190)) && runtime.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && runtime.memory.registerRam(0x804226a8,std::vector<uint8_t>(4)));for(const auto& v:native.words)assert(runtime.memory.writeWord(v.first,v.second));for(const auto& v:native.bytes)assert(runtime.memory.writeByte(v.first,v.second));
    EventCallbackRegistry registry;assert(bindBattleGetHitPosCallback(registry,runtime));int32_t result;assert(registry.invoke(e,0x801120a8,0,result) && result==2 && e.locals[0]==18 && e.locals[1]==37 && e.locals[2]==52);
    std::cout<<"packed hit-position ordering/event tests passed\n";
}
