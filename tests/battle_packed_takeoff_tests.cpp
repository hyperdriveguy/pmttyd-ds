#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float value) {uint32_t word;std::memcpy(&word,&value,4);return word;}
struct Fixture {
    EventEntry e;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads;std::vector<int32_t> resolved,translated;std::vector<std::pair<uint32_t,int32_t>> lookups;
    std::vector<std::pair<int32_t,int32_t>> outputs;
    int32_t sampled=0,chosen=9,part=999;uint8_t count=1;int samples=0,choices=0;
    std::function<void()> onResolve,onSample,onChoose,onOutput;
    std::vector<BattleCandidateBuffer*> buffers;
    Fixture() {
        e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=8;words[0x1008]=999;
        for(unsigned i=0;i<3;++i)words[0x100c+4*i]=uint32_t(-30000000+int32_t(i));
        words[0x8041eb90]=0x2000;bytes[0x2012]=1;words[0x804226a8]=bits(1);
        words[0x4040]=bits(888);words[0x4044]=bits(999);words[0x4030]=bits(1);words[0x4034]=bits(2.9f);words[0x4038]=bits(3);words[0x403c]=bits(10);
        for(uint32_t u:{0x4000u,0x6000u,0x8000u}) {words[0x2000+0x20+4*((u-0x4000)/0x2000+7)]=u;words[u+0x114]=bits(1);bytes[u+0xc]=u==0x4000?0:1;}
        words[0x6014]=0x7000;words[0x8014]=0x9000;
        for(uint32_t p:{0x7000u,0x9000u}) {words[p+0x1ac]=0x10000000;words[p+0x4ec]=0xdeadbeef;}
        words[0x7018]=bits(100.9f);words[0x701c]=bits(99);words[0x7020]=bits(5.9f);
        words[0x9018]=bits(30.9f);words[0x901c]=bits(123);words[0x9020]=bits(777);
    }
    BattleTakeoffCoreServices core() {
        BattleTakeoffCoreServices s;s.user=this;
        s.sample=[](void* p,BattleCandidateBuffer& buffer,uint32_t weapon,int32_t actor,int32_t belong,uint32_t flags,uint32_t attr,int32_t bias) {
            auto& f=*static_cast<Fixture*>(p);assert(weapon==0 && actor==7 && belong==1 && flags==0x01001240 && attr==0x01000000 && bias==-1);++f.samples;f.buffers.push_back(&buffer);buffer.bytes[0xa6c]=f.count;if(f.onSample)f.onSample();return f.sampled;
        };
        s.choose=[](void* p,BattleCandidateBuffer& buffer,uint32_t mode,int32_t& unit,int32_t& part) {auto& f=*static_cast<Fixture*>(p);assert(mode==4 && f.buffers.back()==&buffer);++f.choices;unit=f.chosen;part=f.part;if(f.onChoose)f.onChoose();return -999;};return s;
    }
    BattlePackedTakeoffServices services() {
        BattlePackedTakeoffServices s;auto& p=s.position;p.unit.user=this;p.unit.values.user=this;p.memory.user=this;p.memory.memory.user=this;p.numericUser=this;
        p.memory.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};p.unit.memory=p.memory.memory;
        p.memory.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes[a];};
        p.unit.values.resolve=[](void* p,EventEntry&,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(v);if(f.onResolve)f.onResolve();return v;};
        p.unit.translate=[](void* p,EventEntry&,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.translated.push_back(v);return v==-2?7:v;};
        p.unit.unit=[](void* p,uint32_t b,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.lookups.push_back({b,id});assert(b==0x2000);return f.words[b+0x20+4*uint32_t(id)];};
        p.unit.truncateFloat=[](void*,float v) {return int32_t(v);};p.unit.values.store=[](void* p,EventEntry&,int32_t d,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back({d,v});if(f.onOutput)f.onOutput();};
        p.fma=[](void*,float a,float b,float c) {return std::fma(a,b,c);};p.multiply=[](void*,float a,float b) {return a*b;};p.add=[](void*,float a,float b) {return a+b;};s.subtract=[](void*,float a,float b) {return a-b;};s.core=core();return s;
    }
    void run() {assert(packedTakeoffPositionEvent(e,services())==2 && samples==1 && choices==1);}
};
int main() {
    Fixture base;base.run();assert(base.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000000,60},{-29999999,2},{-29999998,5}}));assert(base.resolved==std::vector<int32_t>({-2,8,999}) && base.translated==std::vector<int32_t>({-2,8,9}));assert(base.lookups==std::vector<std::pair<uint32_t,int32_t>>({{0x2000,7},{0x2000,8},{0x2000,9}}));assert(base.reads[0]==0x8041eb90 && base.reads[1]==0x1000);
    Fixture shared;for(unsigned i=0;i<6;++i)shared.words[0x7170+4*i]=bits(float(i+1));float x,ignored;packedHitPosition(0x6000,0x7000,{&x,&ignored,&ignored},shared.services().position);assert(std::fabs(x-105.9f)<0.0001f && std::fabs(ignored-21.9f)<0.0001f); // Private Y/Z alias accumulates both axes across both layers.
    Fixture reverse;reverse.sampled=1;reverse.run();assert(reverse.outputs[0].second==0);
    Fixture same;same.chosen=8;same.run();assert(same.outputs[0].second==160);
    for(uint8_t count:{uint8_t(0),uint8_t(128),uint8_t(255)}) {Fixture empty;empty.count=count;empty.sampled=1;empty.run();assert(empty.outputs[0].second==160 && empty.translated.size()==2);} // Choice always called;signed count gates candidate branch/Q.
    Fixture tie;tie.words[0x7018]=bits(100);tie.words[0x9018]=bits(-80);tie.run();assert(tie.outputs[0].second==130);
    Fixture nan;nan.words[0x9018]=bits(std::numeric_limits<float>::quiet_NaN());nan.run();assert(nan.outputs[0].second==130);
    Fixture captured;captured.onResolve=[&] {captured.words[0x8041eb90]=0x3000;captured.bytes[0x3012]=255;};captured.onSample=[&] {captured.bytes[0x2012]=255;};captured.onOutput=[&] {captured.words[0x1010]=123;captured.words[0x4034]=bits(999);};captured.run();assert(captured.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000000,0},{-29999999,2},{-29999998,5}})); // Unit lookups/offset side use captured work;hit layers still query fresh work.
    Fixture aliases;aliases.words[0x100c]=0xf1194d80;aliases.words[0x1010]=aliases.words[0x1014]=77;aliases.run();assert(aliases.outputs==std::vector<std::pair<int32_t,int32_t>>({{-250000000,60},{77,2},{77,5}})); // No raw-sentinel skip.
    Fixture high;for(unsigned b:{0u,1u,2u,255u,128u}) {high.bytes[0x400c]=uint8_t(b);assert(packedEnemyBelong(0x4000,high.services().position.memory)==(b==0?1:b==1?0:b<128?int(b):int(b)-256));}
    // A nested provider invocation receives its own private candidate storage.
    Fixture outer,inner;outer.onSample=[&] {inner.run();assert(outer.buffers.back()!=inner.buffers.back());};outer.run();assert(outer.outputs==inner.outputs);
    EventNativePlatform platform;platform.truncate=[](void*,float v) {return int32_t(v);};platform.fmaFloat=[](void*,float a,float b,float c) {return std::fma(a,b,c);};platform.multiplyFloat=[](void*,float a,float b) {return a*b;};platform.addFloat=[](void*,float a,float b) {return a+b;};platform.subtractFloat=[](void*,float a,float b) {return a-b;};EventNativeRuntime runtime(0x81000000,platform);std::array<EventEntry,1> entries{};assert(runtime.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.arguments=0x1000;e.owner=7;
    Fixture native;for(uint32_t a:{0x1000u,0x2000u,0x4000u,0x6000u,0x8000u})assert(runtime.memory.registerRam(a,std::vector<uint8_t>(a==0x1000?24:0x200)));for(uint32_t a:{0x7000u,0x9000u})assert(runtime.memory.registerRam(a,std::vector<uint8_t>(0x500)));assert(runtime.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && runtime.memory.registerRam(0x804226a8,std::vector<uint8_t>(4)));for(const auto& v:native.words)assert(runtime.memory.writeWord(v.first,v.second));for(const auto& v:native.bytes)assert(runtime.memory.writeByte(v.first,v.second));
    auto services=nativeBattleTakeoffServices(runtime,native.core());EventCallbackRegistry registry;assert(bindBattleTakeoffCallback(registry,services));int32_t result;assert(registry.invoke(e,0x80109054,1,result) && result==2 && e.locals[0]==60 && e.locals[1]==2 && e.locals[2]==5);
    std::cout<<"packed takeoff native capture/provider/coordinate tests passed\n";
}
