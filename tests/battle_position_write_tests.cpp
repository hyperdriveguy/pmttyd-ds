#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float v) {uint32_t w;std::memcpy(&w,&v,4);return w;}
struct Fixture {
    EventEntry e;std::map<uint32_t,uint32_t> words;std::vector<int32_t> resolved;std::vector<uint32_t> reads,writes,battles;
    int32_t output=0,value=0;std::function<void(int32_t)> onResolve;std::function<void()> onLookup;
    Fixture() {e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=3;words[0x1008]=0xf1194d80;words[0x100c]=uint32_t(-30000000);words[0x8041eb90]=0x2000;words[0x403c]=bits(10.5f);words[0x4040]=bits(20.5f);words[0x4044]=bits(30.5f);}
    BattleUnitEventServices services() {
        BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;};
        s.values.resolve=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(raw);if(f.onResolve)f.onResolve(raw);return raw==-30000000?-250000000:raw;};
        s.translate=[](void*,EventEntry&,int32_t raw) {assert(raw==-2);return 7;};
        s.unit=[](void* p,uint32_t b,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(id==7);f.battles.push_back(b);if(f.onLookup)f.onLookup();return 0x4000u;};
        s.values.store=[](void* p,EventEntry&,int32_t out,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.output=out;f.value=value;};return s;
    }
};
int main() {
    Fixture pos;pos.onResolve=[&](int32_t raw) {if(raw==-30000000) {pos.words[0x8041eb90]=0x5000;pos.e.arguments=0x9000;}};pos.onLookup=[&] {pos.words[0x4040]=bits(99.5f);};
    assert(packedSetUnitPositionEvent(pos.e,pos.services())==2 && pos.battles==std::vector<uint32_t>{0x5000} && pos.resolved==std::vector<int32_t>({-2,3,-250000000,-30000000}) && pos.words[0x403c]==bits(3) && pos.words[0x4040]==bits(99.5f) && pos.words[0x4044]==bits(-250000000.f)); // Raw sentinel preserves Y;variable with same resolved value writes Z.
    assert(pos.reads==std::vector<uint32_t>({0x1000,0x1004,0x1008,0x100c,0x8041eb90,0x403c,0x4040,0x4044}) && pos.writes==std::vector<uint32_t>({0x403c,0x4040,0x4044}));
    Fixture integer;integer.words[0x1004]=12;integer.words[0x1008]=13;integer.words[0x100c]=14;assert(packedSetUnitPositionEvent(integer.e,integer.services())==2 && integer.words[0x403c]==bits(12) && integer.reads.back()==0x4044); // Old visible position read even without sentinels.
    Fixture body;body.words[0x4014]=0x5000;body.words[0x51ac]=2;body.words[0x5000]=0x6000;body.words[0x61ac]=1;body.words[0x6004]=0x7000;body.words[0x7000]=23;
    assert(packedBodyPartId(0,body.services().memory)==-1 && body.reads.empty());assert(packedBodyPartId(0x4000,body.services().memory)==23 && body.reads==std::vector<uint32_t>({0x4014,0x51ac,0x5000,0x61ac,0x6004,0x7000}));
    body.words[0x4014]=0;assert(packedBodyPartId(0x4000,body.services().memory)==-1);body.words[0x4014]=0x5000;body.words[0x51ac]=1;body.words[0x5004]=0x7000;body.words[0x7000]=0xfffffffe;
    body.words[0x1004]=uint32_t(-29999999);body.onResolve=[&](int32_t) {body.words[0x8041eb90]=0x9000;};body.onLookup=[&] {body.words[0x1004]=123;};assert(battleGetBodyIdEvent(body.e,body.services())==2 && body.battles==std::vector<uint32_t>{0x2000} && body.output==-29999999 && body.value==-2);
    EventNativePlatform platform;platform.truncate=[](void*,float v) {return int32_t(v);};EventNativeRuntime r(0x81000000,platform);std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.owner=7;e.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(16)) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x86000000,std::vector<uint8_t>(0x1b0)) && r.memory.registerRam(0x86001000,std::vector<uint8_t>(4)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x8400003c,0x85000000));
    EventCallbackRegistry registry;assert(bindBattleSetPosCallback(registry,r) && bindBattleGetBodyIdCallback(registry,r));int32_t result;uint32_t word;
    assert(r.memory.writeWord(e.arguments,uint32_t(-2)) && r.memory.writeWord(e.arguments+4,0xf24a8880) && r.memory.writeWord(e.arguments+8,0xf1194d80) && r.memory.writeWord(e.arguments+12,9) && r.memory.writeWord(0x85000040,bits(7.25f)));
    assert(registry.invoke(e,0x80112fcc,1,result) && result==2 && r.memory.readWord(0x8500003c,word) && word==bits(3) && r.memory.readWord(0x85000040,word) && word==bits(7.25f) && r.memory.readWord(0x85000044,word) && word==bits(9)); // Encoded3.5 resolves through GetValue to integer3,not GetFloat3.5.
    assert(r.memory.writeWord(e.arguments+4,uint32_t(-30000000)) && r.memory.writeWord(0x85000014,0x86000000) && r.memory.writeWord(0x860001ac,1) && r.memory.writeWord(0x86000004,0x86001000) && r.memory.writeWord(0x86001000,19));assert(registry.invoke(e,0x8010ead8,0,result) && result==2 && e.locals[0]==19);
    std::cout<<"SetPos raw/integer sentinel and body-definition query tests passed\n";
}
