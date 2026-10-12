#include "battle_event_callbacks.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventEntry event;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint16_t> halves;std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads;std::vector<std::pair<int32_t,int32_t>> outputs;
    std::function<void(uint32_t)> onByte;std::function<void()> onResolve,onTranslate;std::function<void()> onFirstOutput;
    Fixture() {event.arguments=0x1000;words[0x8041eb90]=0x2000;words[0x2420]=7;words[0x203c]=0x4000;words[0x4008]=222;words[0x1000]=uint32_t(-30000003);words[0x1004]=uint32_t(-30000004);}
    BattlePackedSelectionServices services() {
        BattlePackedSelectionServices s;s.user=this;s.memory.user=this;s.memory.memory.user=this;s.values.user=this;
        s.memory.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.memory.half=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.halves[a];};
        s.memory.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.bytes[a];if(f.onByte)f.onByte(a);return value;};
        s.values.resolve=[](void* p,EventEntry&,int32_t raw) {assert(raw==-2);auto& f=*static_cast<Fixture*>(p);if(f.onResolve)f.onResolve();return raw;};
        s.translate=[](void* p,EventEntry&,int32_t raw) {assert(raw==-2);auto& f=*static_cast<Fixture*>(p);if(f.onTranslate)f.onTranslate();return 7;};
        s.values.store=[](void* p,EventEntry&,int32_t dest,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back({dest,value});if(f.outputs.size()==1 && f.onFirstOutput)f.onFirstOutput();};return s;
    }
};
int main() {
    for(uint8_t count:{0,128,255}) {Fixture f;f.bytes[0x2e94]=count;assert(packedSelectedTargetEvent(f.event,f.services())==2 && f.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000003,-1},{-30000004,-1}}) && f.reads==std::vector<uint32_t>({0x8041eb90,0x2e94,0x1000,0x1004}));}
    Fixture target;target.bytes[0x2e94]=1;target.bytes[0x2edf]=255;target.halves[0x2450]=0xfffe;target.halves[0x2452]=9; // cursor-1 reads count1 as permutation index,row1.
    target.onByte=[&](uint32_t a) {if(a==0x2e94)target.words[0x1004]=uint32_t(-29999996);};target.onFirstOutput=[&] {target.halves[0x2452]=0x8000;target.words[0x1004]=123;target.words[0x8041eb90]=0x5000;};
    assert(packedSelectedTargetEvent(target.event,target.services())==2 && target.outputs==std::vector<std::pair<int32_t,int32_t>>({{-30000003,-2},{-29999996,-32768}}));
    const std::vector<std::pair<uint32_t,uint32_t>> cursors{{0,0x4b0},{1,0x4bc},{2,0x51c},{3,0x528},{4,0x4ec},{5,0x4f8},{6,0x4c8},{7,0x4d4},{8,0x4e0},{10,0x510},{14,0x498}};
    for(auto [type,offset]:cursors) {Fixture f;uint32_t cursor=99;assert(findPackedCommandCursor(0x9000,type,cursor,f.services()) && cursor==0x9000+offset);if(type!=5 && type!=14)assert(f.reads==std::vector<uint32_t>({0x8041eb90,0x2420,0x203c}));}
    for(uint32_t type:{9u,11u,12u,13u,15u,0xffffffffu}) {Fixture f;uint32_t cursor=99;assert(!findPackedCommandCursor(0x9000,type,cursor,f.services()) && cursor==99 && f.reads==std::vector<uint32_t>({0x8041eb90,0x2420,0x203c}));}
    Fixture other;other.words[0x4008]=0xffffffff;uint32_t cursor;assert(findPackedCommandCursor(0x9000,5,cursor,other.services()) && cursor==0x9504 && findPackedCommandCursor(0x9000,14,cursor,other.services()) && cursor==0x94a4);
    Fixture captured;captured.words[0x1000]=uint32_t(-2);captured.words[0x3c5c]=0;captured.words[0x3bcc]=2;captured.words[0x37d4]=0x803538a8;
    captured.onResolve=[&] {captured.words[0x1004]=uint32_t(-29999988);};captured.onTranslate=[&] {captured.words[0x8041eb90]=0x5000;captured.words[0x503c]=0x6000;captured.words[0x6008]=222;captured.words[0x5420]=7;captured.words[0x1004]=123;};
    assert(packedCommandWeaponEvent(captured.event,captured.services())==2 && captured.outputs==std::vector<std::pair<int32_t,int32_t>>({{-29999988,int32_t(0x803538a8u)}})); // Captured menu,current actor and output-before-translation.
    Fixture item;item.words[0x1000]=uint32_t(-2);item.words[0x4008]=223;item.words[0x4308]=0xffffffff;item.words[0x803108a4]=0;assert(packedCommandWeaponEvent(item.event,item.services())==2 && item.outputs[0].second==0); // Wrapped held-item address,zero weapon remains a value.
    Fixture invalid;invalid.words[0x1000]=uint32_t(-2);invalid.words[0x3c5c]=9;assert(!packedCommandWeaponEvent(invalid.event,invalid.services()) && invalid.outputs.empty());
    // Native registry bindings use actual selected rows,menu,cursor and unit RAM.
    EventNativeRuntime r(0x81000000,{});std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.owner=7;e.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(8)) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x2200)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x310)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x8400003c,0x85000000) && r.memory.writeWord(0x85000008,222) && r.memory.writeWord(0x84000420,7));
    EventCallbackRegistry registry;assert(bindBattleSelectedTargetCallback(registry,r) && bindBattleCommandWeaponCallback(registry,r));int32_t result;
    assert(r.memory.writeWord(e.arguments,uint32_t(-30000000)) && r.memory.writeWord(e.arguments+4,uint32_t(-29999999)) && r.memory.writeByte(0x84000e94,1) && r.memory.writeHalf(0x8400042c,9) && r.memory.writeHalf(0x8400042e,2));
    assert(registry.invoke(e,0x8010b540,1,result) && result==2 && e.locals[0]==9 && e.locals[1]==2);
    assert(r.memory.writeWord(e.arguments,uint32_t(-2)) && r.memory.writeWord(e.arguments+4,uint32_t(-29999988)) && r.memory.writeWord(0x84001bcc,2) && r.memory.writeWord(0x840017d4,0x803538a8));
    assert(registry.invoke(e,0x801057e8,0,result) && result==2 && uint32_t(e.locals[12])==0x803538a8);
    std::cout<<"packed selection and command-weapon native callback tests passed\n";
}
