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
    std::vector<int32_t> resolved,converted;std::vector<float> truncations;std::vector<std::pair<int32_t,int32_t>> outputs;
    uint32_t destination=uint32_t(-29999994);int32_t requested=36;int conversions=0;bool forceBase=false,nanDistance=false;
    std::function<void(int)> onConvert;std::function<void(int32_t)> onResolve;std::function<void()> onDistance,onSubtract;
    Fixture() {e.arguments=0x1000;e.initialized=77;words[0x1000]=uint32_t(-2);words[0x1004]=100;words[0x1008]=777;words[0x100c]=0;words[0x1010]=destination;words[0x8041eb90]=0x2000;words[0x403c]=bits(0);words[0x4040]=bits(999);words[0x4044]=bits(0);words[0x4170]=bits(5);words[0x804222d0]=bits(.15000000596046448f);words[0x804222d4]=bits(60);}
    BattleMoveFrameServices services() {
        BattleMoveFrameServices s;auto& c=s.common;auto& p=c.position;auto& u=p.unit;u.user=this;u.values.user=this;p.numericUser=this;s.user=this;p.memory.memory.user=this;
        p.memory.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};u.memory=p.memory.memory;
        u.values.resolve=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(raw);if(f.onResolve)f.onResolve(raw);return uint32_t(raw)==f.destination?f.requested:raw;};u.translate=[](void*,EventEntry&,int32_t raw) {assert(raw==-2);return 7;};
        u.unit=[](void* p,uint32_t battle,int32_t id)->uint32_t {auto& f=*static_cast<Fixture*>(p);assert(battle==f.words[0x8041eb90] && id==7);return 0x4000;};u.values.store=[](void* p,EventEntry&,int32_t raw,int32_t v) {static_cast<Fixture*>(p)->outputs.push_back({raw,v});};
        c.convert=[](void* p,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.converted.push_back(v);if(f.onConvert)f.onConvert(++f.conversions);return float(v);};
        c.divide=[](void*,float a,float b) {return a/b;};c.subtract=[](void* p,float a,float b) {auto& f=*static_cast<Fixture*>(p);if(f.onSubtract)f.onSubtract();return a-b;};p.multiply=[](void*,float a,float b) {return a*b;};
        u.truncateFloat=[](void* p,float v)->int32_t {auto& f=*static_cast<Fixture*>(p);f.truncations.push_back(v);if(f.forceBase && f.truncations.size()==1)return std::numeric_limits<int32_t>::max();return int32_t(v);};
        s.distance=[](void* p,float a,float b,float c,float d) {auto& f=*static_cast<Fixture*>(p);assert(a==0 && b==0 && d==0);if(f.onDistance)f.onDistance();return f.nanDistance?std::numeric_limits<float>::quiet_NaN():std::hypot(c-a,d-b);};return s;
    }
    int32_t run() {assert(packedMarioJumpParameterEvent(e,services())==2 && e.initialized==77);return outputs.empty()?std::numeric_limits<int32_t>::min():outputs.back().second;}
};
int main() {
    Fixture ordinary;const auto before=ordinary.words;assert(ordinary.run()==42 && ordinary.words==before && ordinary.resolved==std::vector<int32_t>({-2,100,777,0,-29999994}) && ordinary.converted==std::vector<int32_t>({100,0}) && ordinary.truncations==std::vector<float>({6}));assert(ordinary.reads==std::vector<uint32_t>({0x1000,0x1004,0x1008,0x100c,0x1010,0x8041eb90,0x403c,0x4040,0x4044,0x804222d4,0x804222d0}));
    for(int x:{0,59,60}) {Fixture f;f.words[0x1004]=uint32_t(x);assert(f.run()==36 && f.truncations.empty());}
    Fixture autoCount;autoCount.requested=0;assert(autoCount.run()==26 && autoCount.truncations==std::vector<float>({20,6}));
    Fixture zero;zero.requested=0;zero.words[0x1004]=0;assert(zero.run()==0); // No minimum1 policy.
    Fixture negative;negative.requested=-5;assert(negative.run()==1);Fixture autoNegative;autoNegative.requested=0;autoNegative.words[0x4170]=bits(-5);assert(autoNegative.run()==-14);
    Fixture bounded;bounded.requested=std::numeric_limits<int32_t>::max();assert(bounded.run()==std::numeric_limits<int32_t>::max()); // Wrapped corrected candidate is below nonzero requested bound.
    Fixture wrap;wrap.requested=0;wrap.forceBase=true;assert(wrap.run()==std::numeric_limits<int32_t>::min()+5); // Authored conversion provider;zero requested accepts wrapped candidate.
    Fixture skip;skip.requested=-250000000;skip.words[0x1004]=60;skip.run();assert(skip.outputs.empty());
    Fixture rawSentinel;rawSentinel.destination=0xf1194d80;rawSentinel.requested=-250000000;rawSentinel.words[0x1010]=rawSentinel.destination;assert(rawSentinel.run()==-249999994 && rawSentinel.outputs==std::vector<std::pair<int32_t,int32_t>>({{-250000000,-249999994}})); // Result test,not raw destination skip.
    Fixture nan;nan.nanDistance=true;assert(nan.run()==36 && nan.truncations.empty());
    Fixture afterDistance;afterDistance.requested=0;afterDistance.onDistance=[&] {afterDistance.words[0x4170]=bits(10);};assert(afterDistance.run()==16);
    Fixture coefficient;coefficient.onSubtract=[&] {coefficient.words[0x804222d0]=bits(.5f);};assert(coefficient.run()==56); // Source coefficient load follows rounded subtraction.
    Fixture prefetched;prefetched.onConvert=[&](int n) {if(n==1) {prefetched.words[0x1008]=888;prefetched.e.arguments=0x9000;}if(n==2)prefetched.words[0x1010]=uint32_t(-29999993);};prefetched.onResolve=[&](int32_t v) {if(uint32_t(v)==prefetched.destination)prefetched.words[0x8041eb90]=0x3000;};assert(prefetched.run()==42 && prefetched.resolved==std::vector<int32_t>({-2,100,777,0,-29999994}) && prefetched.outputs==std::vector<std::pair<int32_t,int32_t>>({{-29999994,42}}));
    EventNativePlatform platform;platform.signedWordFloat=[](void*,int32_t v) {return float(v);};platform.divideFloat=[](void*,float a,float b) {return a/b;};platform.subtractFloat=[](void*,float a,float b) {return a-b;};platform.multiplyFloat=[](void*,float a,float b) {return a*b;};platform.truncate=[](void*,float v)->int32_t {return int32_t(v);};platform.distanceFloat=[](void*,float a,float b,float c,float d) {return std::hypot(c-a,d-b);};EventNativeRuntime runtime(0x81000000,platform);std::array<EventEntry,2> entries{};assert(runtime.entryAddresses.registerEntries(0x82000000,entries.data(),entries.size()));auto& e=entries[0];e.owner=7;e.arguments=0x1000;e.locals[6]=36;
    assert(runtime.memory.registerRam(0x1000,std::vector<uint8_t>(20)) && runtime.memory.registerRam(0x2000,std::vector<uint8_t>(0x100)) && runtime.memory.registerRam(0x4000,std::vector<uint8_t>(0x180)) && runtime.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && runtime.memory.registerRam(0x804222d0,std::vector<uint8_t>(8)));Fixture native;native.words[0x203c]=0x4000;for(const auto& v:native.words)assert(runtime.memory.writeWord(v.first,v.second));
    EventCallbackRegistry registry;assert(bindBattleMarioJumpParameterCallback(registry,runtime));int32_t result;assert(registry.invoke(e,0x80109c68,0,result) && result==2 && e.locals[6]==42 && e.initialized==0);
    // Original-shaped brother creation inherits the updated count,not the old36.
    assert(runtime.memory.registerRam(0x80300000,{0,0,0,1,0,0,0,0}));runtime.field.entries=entries.data();runtime.field.capacity=2;e.flags=1;e.id=1;runtime.runtime.nextId=2;runtime.runtime.allocationCount=1;
    auto& brother=createBrotherEvent(runtime.schedule,e,0x80300000,0,runtime.creationServices());assert(brother.locals[6]==42 && e.locals[6]==42); // Source setup/creation services compose;fixture END stream is not story dialogue.
    std::cout<<"native MarioJumpParam count/correction/sentinel/prefetch/inheritance tests passed\n";
}
