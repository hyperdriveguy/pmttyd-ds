#include "battle_event_callbacks.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;std::map<uint32_t,uint16_t> halves;
    std::vector<uint32_t> reads;std::vector<int> calls;std::vector<int32_t> bounds;std::vector<uint32_t> cleared;
    std::function<void()> onRandom;std::function<void()> onJudge;std::function<void(uint32_t)> onWordStore;
    Fixture() {words[0x8041eb90]=0x2000;}
    BattleTurnFirstServices services() {
        BattleTurnFirstServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.half=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->halves[a];};
        s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v) {static_cast<Fixture*>(p)->halves[a]=v;};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.words[a]=v;f.cleared.push_back(a);if(f.onWordStore)f.onWordStore(a);};
        s.byte=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->bytes[a];};s.memory.storeByte=[](void* p,uint32_t a,uint8_t v) {static_cast<Fixture*>(p)->bytes[a]=v;};
        s.judgeTurnRule=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);if(f.onJudge)f.onJudge();};
        s.fortune=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(2);assert(f.cleared.empty());return 255;};
        s.resetMove=[](void* p,uint32_t u) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);assert(f.bytes[u+0x24]==0);};
        s.irand=[](void* p,int32_t bound) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);f.bounds.push_back(bound);if(f.onRandom)f.onRandom();return 5;};
        s.softDelete=[](void* p,uint32_t effect) {auto& f=*static_cast<Fixture*>(p);assert(effect==0x9000);f.calls.push_back(5);f.words[0x8041eb90]=0x3000;};return s;
    }
};
int main() {
    for(uint16_t old:{uint16_t(0),uint16_t(1),uint16_t(32767),uint16_t(65535)}) {Fixture f;f.halves[0x1000]=old;f.bytes[0x17f57]=255;battleTurnFirstProcess(0x1000,f.services());assert(f.halves[0x1000]==uint16_t(old+1) && f.bytes[0x17f57]==255);assert(f.calls==std::vector<int>(old==1?std::initializer_list<int>{1,2}:std::initializer_list<int>{1}));assert(f.cleared==std::vector<uint32_t>({0x19ff4,0x19ff0}));}
    Fixture full;full.onJudge=[&] {full.halves[0x1000]=1;};full.words[0x1020]=0x4000;full.bytes[0x4024]=255;full.words[0x4010]=0x5000;full.bytes[0x5089]=0xfd;full.bytes[0x5088]=10;full.words[0x4138]=0x01000080;full.words[0x1a2b8]=1;full.words[0x1a2bc]=0x9000;full.bytes[0x17f57]=99;
    full.onRandom=[&] {full.words[0x4010]=0x6000;full.bytes[0x5089]=5;full.bytes[0x5088]=0xfb;};battleTurnFirstProcess(0x1000,full.services());
    assert(full.calls==std::vector<int>({1,2,3,4,5}) && full.bounds==std::vector<int32_t>{-3} && full.words[0x4028]==0xfffffffe && full.words[0x4138]==0x80 && full.bytes[0x17f57]==100);
    assert(full.words[0x1a2b8]==0 && full.words[0x1a2bc]==0 && full.words[0x2a2bc]==0 && full.words[0x8041eb90]==0x3000);
    assert(battleUnitStatusFlags(0,0xffffffff,full.services())==0 && battleUnitStatusFlags(0x4000,0x80,full.services())==0x80);
    Fixture zero;zero.words[0x1a2bc]=0x9000;assert(battleFogEndCheck(zero.services())==1 && zero.words[0x1a2bc]==0x9000 && zero.calls.empty() && zero.reads==std::vector<uint32_t>({0x8041eb90,0x1a2b8}));
    for(uint32_t timer:{2u,1u,0xffffffffu,0x80000000u}) {Fixture f;f.words[0x1a2b8]=timer;const int result=battleFogEndCheck(f.services());assert(f.words[0x1a2b8]==timer-1 && result==((timer==2 || timer==0x80000000)?0:1));}
    Fixture reload;reload.words[0x1a2b8]=2;reload.onWordStore=[&](uint32_t a) {if(a==0x1a2b8)reload.words[a]=0;};assert(battleFogEndCheck(reload.services())==1);
    // Native integration uses actual fortune producer on current global battle;
    // unit registry/record writes still retain explicit B supplied by the caller.
    EventNativeRuntime r(0x81000000,{});assert(r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x19000)) && r.memory.registerRam(0x87000000,std::vector<uint8_t>(0x19000)) && r.memory.registerRam(0x86000000,std::vector<uint8_t>(0x5bc)) && r.memory.registerRam(0x8041eb00,std::vector<uint8_t>(4)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
    assert(r.memory.writeWord(0x8041eb00,0x86000000) && r.memory.writeWord(0x8041eb90,0x87000000) && r.memory.writeHalf(0x84000000,1));
    assert(r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x300)) && r.memory.registerRam(0x85001000,std::vector<uint8_t>(0x8c)) && r.memory.writeWord(0x84000024,0x85000000) && r.memory.writeWord(0x85000010,0x85001000));
    assert(r.memory.writeByte(0x85000024,3) && r.memory.writeByte(0x85000130,1) && r.memory.writeByte(0x85001088,5) && r.memory.writeWord(0x85000138,0x01000080));
    r.platform.irand=[](void*,int32_t bound) {assert(bound==0);return 0;};
    BattleTurnFirstBackend backend{&r,&r,[](void*) {},nullptr,nullptr};battleTurnFirstProcess(0x84000000,nativeBattleTurnFirstServices(backend));uint16_t turn;uint8_t count;assert(r.memory.readHalf(0x84000000,turn) && turn==2 && r.memory.readByte(0x84016f57,count) && count==1);
    uint8_t moves;uint32_t priority,flags;assert(r.memory.readByte(0x85000022,moves) && moves==1 && r.memory.readByte(0x85000023,moves) && moves==1 && r.memory.readByte(0x85000024,moves) && moves==4 && r.memory.readWord(0x85000028,priority) && priority==5 && r.memory.readWord(0x85000138,flags) && flags==0x80);
    std::cout<<"turn-first-process ordering and fog expiry tests passed\n";
}
