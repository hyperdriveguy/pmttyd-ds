#include "battle_event_callbacks.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventEntry e;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;std::vector<uint32_t> reads;
    std::vector<std::pair<int32_t,int32_t>> outputs;std::function<void(uint32_t)> onWord,onByte;std::function<void()> onOutput;
    uint32_t* targetSlot=nullptr;bool customPair=false;
    Fixture() {e.arguments=0x1000;words[0x8041eb90]=0x2000;words[0x4738]=0x3000;words[0x3008]=1;words[0x1000]=uint32_t(-29999997);words[0x1004]=uint32_t(-29999996);unit(0,0x4000,222,0);unit(1,0x5000,40,1);unit(2,0x6000,225,0);words[0x5000]=77;words[0x5014]=0x7000;words[0x71ac]=1;words[0x7004]=0x8000;words[0x8000]=19;}
    void unit(unsigned slot,uint32_t u,int kind,uint8_t side) {words[0x2020+slot*4]=u;words[u+8]=uint32_t(kind);bytes[u+0xc]=side;}
    BattleActorServices actors() {
        BattleActorServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.words[a];if(f.onWord)f.onWord(a);return value;};
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.bytes[a];if(f.onByte)f.onByte(a);return value;};return s;
    }
    BattleFirstAttackServices services() {
        BattleFirstAttackServices s;s.user=this;s.memory=actors();s.values.user=this;
        s.pair=[](void* p,uint32_t b,uint32_t* attacker,uint32_t* target) {auto& f=*static_cast<Fixture*>(p);f.targetSlot=target;if(f.customPair) {*attacker=0;*target=0x5000;return 0xdeadbeefu;}return packedFirstAttackPair(b,attacker,target,f.actors());};
        s.values.store=[](void* p,EventEntry&,int32_t output,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back({output,value});if(f.onOutput)f.onOutput();};return s;
    }
};
int main() {
    for(int32_t mode:{-2147483647-1,-1,0,1,6,7,8,9,2147483647}) {Fixture f;f.words[0x3008]=uint32_t(mode);uint32_t attacker,target;const auto raw=packedFirstAttackPair(0x2000,&attacker,&target,f.actors());if(mode>=1 && mode<=6)assert(attacker==0x4000 && target==0x5000 && raw==0x4000);else if(mode==7 || mode==8)assert(attacker==0x6000 && target==0x5000 && raw==0x6000);else assert(attacker==0x5000 && target==0x4000 && raw==0x4000);}
    Fixture alias;uint32_t shared;assert(packedFirstAttackPair(0x2000,&shared,&shared,alias.actors())==0x4000 && shared==0x5000);
    for(bool nonnull:{false,true}) {Fixture miss;miss.bytes[0x500c]=2;if(nonnull)miss.unit(63,0x9000,99,5);uint32_t attacker,target;packedFirstAttackPair(0x2000,&attacker,&target,miss.actors());assert(attacker==0x4000 && target==(nonnull?0x9000u:0u));}
    Fixture mutate;mutate.words[0x3008]=7;mutate.unit(4,0x9000,222,0);unsigned sideReads=0;mutate.onByte=[&](uint32_t a) {if(a==0x400c && ++sideReads==2)mutate.words[0x4008]=223;};uint32_t attacker,target;assert(packedFirstAttackPair(0x2000,&attacker,&target,mutate.actors())==0x9000 && attacker==0x9000 && target==0x5000); // Predicate rereads returned Mario kind;not unconditional Party.
    Fixture absentAttacker;absentAttacker.words[0x2020]=0;assert(packedFirstAttackTargetEvent(absentAttacker.e,absentAttacker.services())==2 && absentAttacker.outputs==std::vector<std::pair<int32_t,int32_t>>({{-29999997,77},{-29999996,19}}));
    Fixture stack;stack.customPair=true;stack.words[0x9000]=99;stack.onWord=[&](uint32_t a) {if(a==0x71ac)*stack.targetSlot=0x9000;};stack.onOutput=[&] {stack.words[0x8000]=42;stack.words[0x1004]=123;stack.e.arguments=0xa000;};assert(packedFirstAttackTargetEvent(stack.e,stack.services())==2 && stack.outputs==std::vector<std::pair<int32_t,int32_t>>({{-29999997,99},{-29999996,19}})); // Target slot reread after old target's body traversal;part/output retained.
    Fixture nullTarget;nullTarget.words[0x3008]=0;nullTarget.words[0x2020]=0;nullTarget.words[0]=0x12345678;assert(packedFirstAttackTargetEvent(nullTarget.e,nullTarget.services())==2 && nullTarget.outputs[0].second==0x12345678 && nullTarget.outputs[1].second==-1); // Unchecked target00 read,not safe(-1,-1).
    Fixture capture;capture.onWord=[&](uint32_t a) {if(a==0x8041eb90) {capture.e.arguments=0xb000;capture.words[a]=0xc000;}};assert(packedFirstAttackTargetEvent(capture.e,capture.services())==2 && capture.outputs[0].second==77 && capture.reads[1]==0x1000); // A retained before B capture;pair retains earlier B.
    EventNativeRuntime r(0x81000000,{});std::array<EventEntry,1> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(8)) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x2740)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x20)) && r.memory.registerRam(0x86000000,std::vector<uint8_t>(0x1b0)) && r.memory.registerRam(0x86001000,std::vector<uint8_t>(4)) && r.memory.registerRam(0x87000000,std::vector<uint8_t>(12)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
    assert(r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x84000024,0x85000000) && r.memory.writeByte(0x8500000c,1) && r.memory.writeWord(0x85000000,77) && r.memory.writeWord(0x85000014,0x86000000) && r.memory.writeWord(0x860001ac,1) && r.memory.writeWord(0x86000004,0x86001000) && r.memory.writeWord(0x86001000,19) && r.memory.writeWord(0x84002738,0x87000000) && r.memory.writeWord(0x87000008,1));
    assert(r.memory.writeWord(e.arguments,uint32_t(-29999997)) && r.memory.writeWord(e.arguments+4,uint32_t(-29999996)));EventCallbackRegistry registry;assert(bindBattleFirstAttackTargetCallback(registry,r));int32_t result;assert(registry.invoke(e,0x8010b418,1,result) && result==2 && e.locals[3]==77 && e.locals[4]==19); // Valid target despite absent attacker/Mario in mode1.
    std::cout<<"first-attack pair/getter/event native composition tests passed\n";
}
