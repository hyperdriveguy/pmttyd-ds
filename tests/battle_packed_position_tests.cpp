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
struct Fixture {
    EventEntry e;std::map<uint32_t,uint32_t> words;std::vector<uint32_t> reads,writes;
    std::vector<std::pair<int32_t,int32_t>> outputs;std::vector<float> converted;
    std::function<void()> onLookup,onConvert,onOutput;
    Fixture() {e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=uint32_t(-30000000);words[0x1008]=uint32_t(-29999999);words[0x100c]=uint32_t(-29999998);words[0x8041eb90]=0x2000;for(uint32_t offset:{0x30,0x3c}) {words[0x4000+offset]=bits(3.9f);words[0x4004+offset]=bits(-4.9f);words[0x4008+offset]=bits(2.25f);}}
    BattleUnitEventServices services() {
        BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;};
        s.values.resolve=[](void*,EventEntry&,int32_t raw) {assert(raw==-2);return raw;};s.translate=[](void*,EventEntry&,int32_t selector) {assert(selector==-2);return 7;};
        s.unit=[](void* p,uint32_t b,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(b==0x2000 && id==7);if(f.onLookup)f.onLookup();return 0x4000u;};
        s.truncateFloat=[](void* p,float value) {auto& f=*static_cast<Fixture*>(p);f.converted.push_back(value);if(f.onConvert)f.onConvert();assert(std::isfinite(value));return int32_t(value);};
        s.values.store=[](void* p,EventEntry&,int32_t out,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back({out,value});if(f.onOutput)f.onOutput();};return s;
    }
};
int main() {
    for(bool home:{false,true}) {
        Fixture leaf;const uint32_t offset=home?0x30:0x3c;
        copyPackedUnitPosition(0x4000,home,{0x4000+offset+4,0x4000+offset+8,0x9000},leaf.services().memory);
        assert(leaf.words[0x9000]==bits(3.9f)); // Ordered overlapping copies propagate first coordinate.
        assert(leaf.reads==std::vector<uint32_t>({0x4000+offset,0x4000+offset+4,0x4000+offset+8}));
        Fixture event;event.onLookup=[&] {event.words[0x1008]=uint32_t(-29999996);};event.onOutput=[&] {event.words[0x4040]=bits(999);event.words[0x4034]=bits(999);event.words[0x100c]=123;};
        assert(packedUnitPositionEvent(event.e,home,event.services())==2 && event.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000000,3},{-29999996,-4},{-29999998,2}}));
        assert(event.converted==std::vector<float>({3.9f,-4.9f,2.25f})); // All coordinates captured before first output mutates source.
    }
    Fixture sentinel;sentinel.words[0x1008]=0xf1194d80;assert(packedUnitPositionEvent(sentinel.e,false,sentinel.services())==2 && sentinel.converted==std::vector<float>({3.9f,2.25f}) && sentinel.outputs.size()==2);
    Fixture homeSentinel;homeSentinel.words[0x1008]=0xf1194d80;assert(packedUnitPositionEvent(homeSentinel.e,true,homeSentinel.services())==2 && homeSentinel.converted.size()==3 && uint32_t(homeSentinel.outputs[1].first)==0xf1194d80);
    Fixture allSkip;for(uint32_t a:{0x1004,0x1008,0x100c})allSkip.words[a]=0xf1194d80;auto skip=allSkip.services();skip.truncateFloat=nullptr;assert(packedUnitPositionEvent(allSkip.e,false,skip)==2 && allSkip.outputs.empty());
    struct Platform {unsigned conversions=0;};Platform p;EventNativePlatform platform;platform.user=&p;platform.truncate=[](void* p,float value) {++static_cast<Platform*>(p)->conversions;assert(std::isfinite(value));return int32_t(value);};
    EventNativeRuntime r(0x81000000,platform);std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.owner=7;e.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(16)) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x8400003c,0x85000000));
    assert(r.memory.writeWord(e.arguments,uint32_t(-2)));for(unsigned i=0;i<3;++i) {assert(r.memory.writeWord(e.arguments+4+4*i,uint32_t(-30000000+int32_t(i))) && r.memory.writeWord(0x85000030+4*i,bits(10.5f+i)) && r.memory.writeWord(0x8500003c+4*i,bits(-10.5f-i)));}
    EventCallbackRegistry registry;assert(bindBattleGetPosCallback(registry,r) && bindBattleGetHomePosCallback(registry,r));int32_t result;
    assert(registry.invoke(e,0x80113620,1,result) && result==2 && e.locals[0]==-10 && e.locals[1]==-11 && e.locals[2]==-12 && p.conversions==3);
    assert(registry.invoke(e,0x801124f8,0,result) && result==2 && e.locals[0]==10 && e.locals[1]==11 && e.locals[2]==12 && p.conversions==6);
    std::cout<<"packed position leaf/event capture and sentinel tests passed\n";
}
