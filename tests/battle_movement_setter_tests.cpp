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
    EventEntry e;std::map<uint32_t,uint32_t> words;std::vector<int> calls;std::vector<uint32_t> reads;
    int32_t id=7;float resolved=3.5f;uint32_t lookupBattle=0;int32_t lookupId=0,floatToken=0;
    std::function<void()> onResolve,onTranslate,onFloat,onLookup;
    Fixture() {e.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=0xf24a8e80;words[0x8041eb90]=0x2000;words[0x4170]=0xabcdef01;words[0x4174]=0x12345678;}
    BattleUnitEventServices services() {
        BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(5);f.words[a]=v;};
        s.values.resolve=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);assert(raw==-2);f.calls.push_back(1);if(f.onResolve)f.onResolve();return raw;};
        s.translate=[](void* p,EventEntry&,int32_t) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(2);if(f.onTranslate)f.onTranslate();return f.id;};
        s.values.resolveFloat=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);f.floatToken=raw;if(f.onFloat)f.onFloat();return f.resolved;};
        s.unit=[](void* p,uint32_t b,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);f.lookupBattle=b;f.lookupId=id;if(f.onLookup)f.onLookup();return id==-1?0u:0x4000u;};return s;
    }
};
int main(int argc,char** argv) {
    for(bool acceleration:{false,true}) {
        Fixture f;f.onResolve=[&] {f.e.arguments=0x9000;};f.onTranslate=[&] {f.words[0x1004]=0xf24a7c80;f.words[0x8041eb90]=0x3000;};f.onFloat=[&] {f.words[0x8041eb90]=0x5000;};f.onLookup=[&] {f.resolved=99;f.words[0x1004]=0;};
        assert(packedMovementSetterEvent(f.e,acceleration,f.services())==2 && uint32_t(f.floatToken)==0xf24a7c80 && f.lookupBattle==0x5000 && f.lookupId==7 && f.calls==std::vector<int>({1,2,3,4,5}) && f.reads==std::vector<uint32_t>({0x1000,0x1004,0x8041eb90}));
        assert(f.words[acceleration?0x4174:0x4170]==bits(3.5f) && f.words[acceleration?0x4170:0x4174]==(acceleration?0xabcdef01u:0x12345678u));
    }
    Fixture unchecked;unchecked.id=-1;unchecked.resolved=-2;assert(packedMovementSetterEvent(unchecked.e,false,unchecked.services())==2 && unchecked.lookupId==-1 && unchecked.words[0x170]==bits(-2) && unchecked.calls==std::vector<int>({1,2,3,4,5})); // Forward raw null-unit store;no HP-style early-out.
    struct Token {uint32_t address,function,raw,expected;};
    const Token tokens[]{{0x80355f40,0x8010d860,0xf24a8e80,0x40a00000},{0x8035607c,0x8010d860,0xf24a8680,0x40400000},{0x80356b6c,0x8010d860,0xf24a9280,0x40c00000},{0x80356688,0x8010d770,0xf24a7d4c,0x3f330000},{0x803566fc,0x8010d770,0xf24a7ce6,0x3f198000},{0x8035690c,0x8010d770,0xf24a7d4c,0x3f330000},{0x80356ab8,0x8010d770,0xf24a7ce6,0x3f198000},{0x80363438,0x8010d770,0xf24a7c80,0x3f000000}};
    EventNativeRuntime r(0x81000000,{});std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.owner=7;e.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(8)) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x100)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x178)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x8400003c,0x85000000));
    EventScriptAssets scripts;if(argc==2)assert(scripts.load(argv[1]) && r.memory.importScripts(scripts));
    EventCallbackRegistry registry;assert(bindBattleSetMoveSpeedCallback(registry,r) && bindBattleSetFallAccelCallback(registry,r));int32_t result;unsigned original=0;
    for(auto token:tokens) {
        const bool acceleration=token.function==0x8010d770;uint32_t field=acceleration?0x85000174:0x85000170,other=acceleration?0x85000170:0x85000174;
        e.arguments=0x80300000;
        if(argc==2 && token.address!=0x80363438) {e.arguments=token.address+8;uint32_t raw;assert(r.memory.readWord(token.address,raw) && raw==0x0003005b && r.memory.readWord(token.address+4,raw) && raw==token.function && r.memory.readWord(e.arguments,raw) && raw==0xfffffffe && r.memory.readWord(e.arguments+4,raw) && raw==token.raw);++original;}
        else assert(r.memory.writeWord(e.arguments,uint32_t(-2)) && r.memory.writeWord(e.arguments+4,token.raw));
        assert(r.memory.writeWord(other,0x76543210) && registry.invoke(e,token.function,0,result) && result==2);uint32_t stored;assert(r.memory.readWord(field,stored) && stored==token.expected && r.memory.readWord(other,stored) && stored==0x76543210);
        assert(registry.invoke(e,token.function,1,result) && result==2); // No first-call gate.
    }
    if(argc==2)assert(original==7);
    e.arguments=0x80300000;assert(r.memory.writeWord(e.arguments,uint32_t(-2)) && r.memory.writeWord(e.arguments+4,0));assert(registry.invoke(e,0x8010d860,1,result) && r.memory.readWord(0x85000170,original) && original==0);
    std::cout<<"movement setter order/fields and original token tests passed\n";
}
