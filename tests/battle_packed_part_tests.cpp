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
    std::vector<uint32_t> reads,writes;std::vector<std::pair<int32_t,int32_t>> outputs;std::vector<int> math;
    std::function<void()> onLookup,onOutput,onMath;
    Fixture() {e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=9;words[0x1008]=uint32_t(-30000000);words[0x100c]=uint32_t(-29999999);words[0x1010]=uint32_t(-29999998);words[0x8041eb90]=0x2000;words[0x203c]=0x4000;words[0x4014]=0x5000;words[0x54ec]=0x6000;bytes[0x6189]=255;for(unsigned i=0;i<3;++i) {words[0x603c+i*4]=bits(float(10+10*i));words[0x5024+i*4]=bits(float(2+i));words[0x5018+i*4]=bits(float(100+100*i));}}
    BattlePartPositionServices services() {
        BattlePartPositionServices s;s.unit.user=this;s.unit.values.user=this;s.memory.user=this;s.memory.memory.user=this;s.numericUser=this;
        s.memory.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;};
        s.memory.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.bytes[a];};s.unit.memory=s.memory.memory;
        s.unit.values.resolve=[](void*,EventEntry&,int32_t v) {return v;};s.unit.translate=[](void*,EventEntry&,int32_t v) {assert(v==-2);return 7;};
        s.unit.unit=[](void* p,uint32_t b,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(b==0x2000 && id==7);if(f.onLookup)f.onLookup();return 0u;}; // Unused first lookup may differ from actual part owner.
        s.unit.truncateFloat=[](void*,float value) {return int32_t(value);};s.unit.values.store=[](void* p,EventEntry&,int32_t dest,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back({dest,value});if(f.onOutput)f.onOutput();};
        s.fma=[](void* p,float a,float b,float c) {auto& f=*static_cast<Fixture*>(p);f.math.push_back(1);if(f.onMath)f.onMath();return std::fma(a,b,c);};s.add=[](void* p,float a,float b) {auto& f=*static_cast<Fixture*>(p);f.math.push_back(2);return a+b;};return s;
    }
};
int main() {
    Fixture single;assert(packedUnitPart(0x4000,-1,single.services().memory.memory)==0x5000 && single.reads==std::vector<uint32_t>{0x4014});single.reads.clear();assert(packedUnitPart(0x4000,999,single.services().memory.memory)==0x5000 && single.reads==std::vector<uint32_t>({0x4014,0x5000}));
    Fixture multi;multi.words[0x5000]=0x7000;multi.words[0x5004]=0x8000;multi.words[0x8000]=1;multi.words[0x7004]=0x9000;multi.words[0x9000]=2;assert(packedUnitPart(0x4000,2,multi.services().memory.memory)==0x7000 && packedUnitPart(0x4000,99,multi.services().memory.memory)==0);
    Fixture absolute;absolute.words[0x51ac]=0x10000000;absolute.words[0x54ec]=0xdeadbeef;writePackedPartWorldPosition(0x5000,{0x501c,0x5020,0xa000},absolute.services());assert(real(absolute.words[0xa000])==100 && absolute.reads==std::vector<uint32_t>({0x51ac,0x54ec,0x5018,0x501c,0x5020}) && absolute.math.empty()); // Owner is read but not queried;ordered source/output aliases.
    Fixture relative;relative.words[0x6114]=bits(100);relative.onMath=[&] {relative.words[0x6040]=bits(999);relative.words[0x5028]=bits(888);};auto coords=packedPartWorldPosition(0x5000,relative.services());assert(coords==std::array<float,3>({8,23,34}) && relative.math==std::vector<int>({1,2,2})); // Separate facing189=-1;unit scale114 not used.
    Fixture event;event.onLookup=[&] {event.words[0x8041eb90]=0x3000;event.words[0x303c]=0x9000;event.words[0x9014]=0x5000;};event.words[0x1010]=0xf1194d80;event.onOutput=[&] {event.words[0x6040]=bits(999);event.words[0x100c]=123;};assert(packedPartPositionEvent(event.e,event.services())==2 && event.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000000,8},{-29999999,23}}));
    EventNativePlatform platform;platform.truncate=[](void*,float value) {return int32_t(value);};platform.fmaFloat=[](void*,float a,float b,float c) {return std::fma(a,b,c);};platform.addFloat=[](void*,float a,float b) {return a+b;};EventNativeRuntime r(0x81000000,platform);std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.owner=7;e.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(20)) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x190)) && r.memory.registerRam(0x86000000,std::vector<uint8_t>(0x500)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x8400003c,0x85000000) && r.memory.writeWord(0x85000014,0x86000000) && r.memory.writeWord(0x860004ec,0x85000000) && r.memory.writeByte(0x85000189,255));
    assert(r.memory.writeWord(e.arguments,uint32_t(-2)) && r.memory.writeWord(e.arguments+4,999));for(unsigned i=0;i<3;++i)assert(r.memory.writeWord(e.arguments+8+4*i,uint32_t(-30000000+int32_t(i))) && r.memory.writeWord(0x8500003c+4*i,bits(float(10+10*i))) && r.memory.writeWord(0x86000024+4*i,bits(float(2+i))));
    EventCallbackRegistry registry;assert(bindBattleGetPartsPosCallback(registry,r));int32_t result;assert(registry.invoke(e,0x80112d98,1,result) && result==2 && e.locals[0]==8 && e.locals[1]==23 && e.locals[2]==34);
    std::cout<<"packed part lookup/world/event position tests passed\n";
}
