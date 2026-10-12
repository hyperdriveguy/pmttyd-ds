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
    EventEntry e;std::map<uint32_t,uint32_t> words;std::vector<uint32_t> reads;
    std::vector<int32_t> resolved,converted,floatResolved;std::vector<std::pair<int32_t,int32_t>> outputs;
    std::vector<float> distances,speeds;float requestedSpeed=0;int calls=0;
    std::function<void(int32_t)> onConvert;std::function<void()> onFloat,onLookup,onDistance;
    Fixture() {e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=101;words[0x1008]=999;words[0x100c]=0;words[0x1010]=0;words[0x1014]=uint32_t(-30000000);words[0x8041eb90]=0x2000;words[0x403c]=bits(0);words[0x4040]=bits(777);words[0x4044]=bits(0);words[0x4170]=bits(5);words[0x80422280]=bits(0);words[0x8042228c]=bits(1);}
    BattleMoveFrameServices services() {
        BattleMoveFrameServices s;auto& c=s.common;auto& p=c.position;auto& u=p.unit;u.user=this;u.values.user=this;p.numericUser=this;s.user=this;p.memory.memory.user=this;
        p.memory.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};u.memory=p.memory.memory;
        u.values.resolve=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(raw);return raw;};u.translate=[](void*,EventEntry&,int32_t raw) {assert(raw==-2);return 7;};
        u.values.resolveFloat=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.floatResolved.push_back(raw);if(f.onFloat)f.onFloat();return f.requestedSpeed;};
        u.unit=[](void* p,uint32_t b,int32_t id)->uint32_t {auto& f=*static_cast<Fixture*>(p);assert(b==f.words[0x8041eb90] && id==7);if(f.onLookup)f.onLookup();return 0x4000;};
        u.values.store=[](void* p,EventEntry&,int32_t raw,int32_t value) {static_cast<Fixture*>(p)->outputs.push_back({raw,value});};
        c.convert=[](void* p,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.converted.push_back(value);if(f.onConvert)f.onConvert(value);return float(value);};
        c.divide=[](void* p,float a,float b) {static_cast<Fixture*>(p)->speeds.push_back(b);return a/b;};
        u.truncateFloat=[](void*,float v)->int32_t {return std::isfinite(v)?int32_t(v):std::numeric_limits<int32_t>::min();};
        c.negativeMultiplySubtract=[](void*,float a,float b,float c) {return -std::fma(a,b,-c);};
        s.distance=[](void* p,float x,float z,float targetX,float targetZ) {auto& f=*static_cast<Fixture*>(p);++f.calls;const float d=std::hypot(targetX-x,targetZ-z);f.distances.push_back(d);if(f.onDistance)f.onDistance();return d;};return s;
    }
    int32_t run() {assert(packedMoveFrameEvent(e,services())==2);return outputs.empty()?0:outputs.back().second;}
};
int main() {
    Fixture ordinary;const auto before=ordinary.words;assert(ordinary.run()==21 && ordinary.words==before && ordinary.resolved==std::vector<int32_t>({-2,101,999,0}) && ordinary.floatResolved==std::vector<int32_t>({0}) && ordinary.converted==std::vector<int32_t>({101,0,20}));assert(ordinary.reads==std::vector<uint32_t>({0x1000,0x1004,0x1008,0x100c,0x1010,0x1014,0x8041eb90,0x403c,0x4040,0x4044,0x80422280,0x4170,0x80422280}));
    Fixture explicitSpeed;explicitSpeed.requestedSpeed=10;assert(explicitSpeed.run()==11 && explicitSpeed.words[0x4170]==bits(5));
    Fixture exact;exact.words[0x1004]=100;assert(exact.run()==20);
    Fixture zero;zero.words[0x1004]=0;assert(zero.run()==1);
    Fixture negative;negative.requestedSpeed=-5;assert(negative.run()==-19);
    Fixture fallback;fallback.requestedSpeed=-0.0f;fallback.words[0x4170]=bits(-0.0f);assert(fallback.run()==101 && fallback.speeds==std::vector<float>({1}) && fallback.words[0x4170]==bits(-0.0f));
    Fixture nan;nan.requestedSpeed=std::numeric_limits<float>::quiet_NaN();assert(nan.run()==std::numeric_limits<int32_t>::min());for(uint32_t a:nan.reads)assert(a!=0x4170); // Authored exceptional truncate provider;NaN bypasses local speed fallback.
    Fixture sentinel;sentinel.words[0x1014]=0xf1194d80;assert(sentinel.run()==0 && sentinel.calls==1 && sentinel.converted==std::vector<int32_t>({101,0,20}) && sentinel.outputs.empty());
    Fixture prefetch;prefetch.onConvert=[&](int32_t v) {if(prefetch.converted.size()==1)prefetch.words[0x1008]=123;if(prefetch.converted.size()==2)prefetch.words[0x1010]=456;};prefetch.onFloat=[&] {prefetch.words[0x8041eb90]=0x3000;prefetch.words[0x1014]=uint32_t(-29999999);prefetch.e.arguments=0x9000;};prefetch.onLookup=[&] {prefetch.words[0x1014]=77;};assert(prefetch.run()==21 && prefetch.resolved==std::vector<int32_t>({-2,101,999,0}) && prefetch.floatResolved==std::vector<int32_t>({0}) && prefetch.outputs==std::vector<std::pair<int32_t,int32_t>>({{-29999999,21}}));
    Fixture postDistance;postDistance.onDistance=[&] {postDistance.words[0x4170]=bits(10);};assert(postDistance.run()==11); // Unit speed is fetched after the distance request.
    Fixture secondZero;auto zs=secondZero.services();zs.common.position.unit.truncateFloat=[](void* p,float v)->int32_t {auto& f=*static_cast<Fixture*>(p);f.words[0x80422280]=bits(100);return int32_t(v);};assert(packedMoveFrameEvent(secondZero.e,zs)==2 && secondZero.outputs.back().second==21); // Residual comparison zero captured before truncation/conversion.
    Fixture wrap;auto ws=wrap.services();ws.common.position.unit.truncateFloat=[](void*,float)->int32_t {return std::numeric_limits<int32_t>::max();};ws.common.negativeMultiplySubtract=[](void*,float,float,float) {return 1.f;};assert(packedMoveFrameEvent(wrap.e,ws)==2 && wrap.outputs.back().second==std::numeric_limits<int32_t>::min()); // Explicit numeric providers exercise word increment wrap,not a measured conversion domain.
    EventNativePlatform platform;platform.signedWordFloat=[](void*,int32_t v) {return float(v);};platform.divideFloat=[](void*,float a,float b) {return a/b;};platform.truncate=[](void*,float v)->int32_t {return int32_t(v);};platform.negativeMultiplySubtractFloat=[](void*,float a,float b,float c) {return -std::fma(a,b,-c);};platform.distanceFloat=[](void*,float a,float b,float c,float d) {return std::hypot(c-a,d-b);};EventNativeRuntime r(0x81000000,platform);std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.arguments=0x1000;e.owner=7;
    assert(r.memory.registerRam(0x1000,std::vector<uint8_t>(24)) && r.memory.registerRam(0x2000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x4000,std::vector<uint8_t>(0x180)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.registerRam(0x80422280,std::vector<uint8_t>(16)));Fixture native;native.words[0x203c]=0x4000;for(const auto& v:native.words)assert(r.memory.writeWord(v.first,v.second));
    EventCallbackRegistry registry;assert(bindBattleGetMoveFrameCallback(registry,r));int32_t result;assert(registry.invoke(e,0x8010c8c0,1,result) && result==2 && e.locals[0]==21);
    std::cout<<"native GetMoveFrame operand/precision/speed/count tests passed\n";
}
