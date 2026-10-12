#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
using namespace ttyd;
uint32_t bits(float value) {uint32_t word;std::memcpy(&word,&value,4);return word;}
void putHalf(BattleCandidateBuffer& b,unsigned o,int v) {b.bytes[o]=uint16_t(v)>>8;b.bytes[o+1]=uint16_t(v);}
void putWord(BattleCandidateBuffer& b,unsigned o,uint32_t v) {b.bytes[o]=v>>24;b.bytes[o+1]=v>>16;b.bytes[o+2]=v>>8;b.bytes[o+3]=v;}
struct Fixture {
    BattleCandidateBuffer buffer;std::vector<int32_t> outputs,conversions;int randomCalls=0;int32_t draw=0,bound=0;
    std::function<void(bool,int32_t)> onOutput;std::function<void()> onTruncate;
    static EventNativePlatform platform(Fixture* f) {
        EventNativePlatform p;p.user=f;
        p.truncate=[](void* p,float v) {auto& f=*static_cast<Fixture*>(p);const int32_t result=int32_t(v);f.conversions.push_back(result);if(f.onTruncate)f.onTruncate();return result;};
        p.signedWordFloat=[](void*,int32_t v) {return float(v);};p.multiplyFloat=[](void*,float a,float b) {return a*b;};p.addFloat=[](void*,float a,float b) {return a+b;};p.subtractFloat=[](void*,float a,float b) {return a-b;};p.divideFloat=[](void*,float a,float b) {return a/b;};p.fmaFloat=[](void*,float a,float b,float c) {return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c) {return -std::fma(a,b,-c);};p.irand=[](void* p,int32_t bound) {auto& f=*static_cast<Fixture*>(p);++f.randomCalls;f.bound=bound;return f.draw;};return p;
    }
    EventNativeRuntime runtime{0x81000000,platform(this)};
    Fixture() {
        for(uint32_t a:{0x8000u,0x10000u,0x11000u,0x12000u,0x13000u,0x19000u})assert(runtime.memory.registerRam(a,std::vector<uint8_t>(0x400)));
        assert(runtime.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && runtime.memory.writeWord(0x8041eb90,0x8000));
        assert(runtime.memory.writeWord(0x803c,0x10000) && runtime.memory.writeWord(0x8048,0x11000) && runtime.memory.writeWord(0x804c,0x12000));
        for(uint32_t u:{0x10000u,0x11000u,0x12000u}) {assert(runtime.memory.writeWord(u+8,222) && runtime.memory.writeWord(u+0x10,0x19000) && runtime.memory.writeHalf(u+0x108,10) && runtime.memory.writeHalf(u+0x10c,5) && runtime.memory.writeHalf(u+0x10e,20) && runtime.memory.writeHalf(u+0x112,0) && runtime.memory.writeByte(u+0xc,u==0x10000?0:1));}
        assert(runtime.memory.writeByte(0x1900d,5));
        assert(runtime.memory.registerRam(0x804221f0,std::vector<uint8_t>(28)));const float constants[]={1.5f,.1f,.4f,.5f,2,1,.05f};for(unsigned i=0;i<7;++i)assert(runtime.memory.writeWord(0x804221f0+4*i,bits(constants[i])));
        assert(runtime.memory.registerRam(0x803108a8,std::vector<uint8_t>(40*300)));
        putWord(buffer,0xab8,7);buffer.bytes[0xa6c]=1;buffer.bytes[0xa6d]=3;putHalf(buffer,4+36*3,10);putHalf(buffer,6+36*3,103);putHalf(buffer,4+36*2,11);putHalf(buffer,6+36*2,102);
    }
    CandidateChoiceOutputs sink() {return {this,[](void* p,bool u,int32_t v) {auto& f=*static_cast<Fixture*>(p);f.outputs.push_back(v);if(f.onOutput)f.onOutput(u,v);}};}
    void run(uint32_t flags=0) {choosePackedBattleCandidate(privateBattleCandidateRecords(buffer),flags,nativeBattleChoiceServices(runtime),sink());}
    void two() {buffer.bytes[0xa6c]=2;buffer.bytes[0xa6e]=2;}
    int32_t chosen() {return outputs[outputs.size()-2];}
    void weapon(int32_t item) {putWord(buffer,0,0x13000);assert(runtime.memory.writeWord(0x13008,uint32_t(item)));}
};
int main() {
    for(uint8_t n:{uint8_t(0),uint8_t(255)}) {Fixture f;f.buffer.bytes[0xa6c]=n;f.run(0xffffffff);assert(f.outputs==std::vector<int32_t>({-1,0}) && f.conversions.empty() && f.randomCalls==0);}
    Fixture mode4;mode4.two();mode4.run(4);assert(mode4.conversions==std::vector<int32_t>({99,50}) && mode4.chosen()==10 && mode4.randomCalls==0);
    Fixture mode8;mode8.two();mode8.run(8);assert(mode8.conversions==std::vector<int32_t>({55,90}) && mode8.chosen()==11);
    for(auto pair:std::vector<std::pair<uint32_t,int32_t>>{{1,165},{4,55},{8,55},{0x20,165},{0x100,165},{0x200,165},{0x400,137},{0x800,137},{0x1000,165}}) {Fixture f;f.run(pair.first);assert(f.conversions==std::vector<int32_t>({pair.second}));}
    Fixture partner;assert(partner.runtime.memory.writeWord(0x11008,225));partner.run(2);assert(partner.conversions==std::vector<int32_t>({165}));
    Fixture same;assert(same.runtime.memory.writeByte(0x1100c,0));same.run(0x10);assert(same.conversions==std::vector<int32_t>({165}));
    Fixture combined;combined.run(0x1f3f);assert(combined.conversions==std::vector<int32_t>({165,82,41,61,91,136,170,212,318}));
    Fixture bonus;bonus.two();bonus.onOutput=[&](bool unit,int32_t v) {if(unit && v==10)bonus.buffer.bytes[0xa6d]=2;};bonus.run();assert(bonus.chosen()==11); // Bonus permutation is reread AFTER initial outputs.
    Fixture liveCount;liveCount.two();liveCount.onTruncate=[&] {liveCount.buffer.bytes[0xa6c]=1;};liveCount.run(4);assert(liveCount.conversions==std::vector<int32_t>({99}));
    Fixture alias;alias.onOutput=[&](bool unit,int32_t v) {if(unit && v==10)putHalf(alias.buffer,6+36*3,99);};alias.run();assert(alias.outputs==std::vector<int32_t>({-1,0,10,99,10,99}));
    for(auto pair:std::vector<std::pair<int,int>>{{146,11},{147,15},{143,14},{134,18},{135,6},{136,7},{137,5}}) {Fixture f;f.two();f.weapon(pair.first);auto s=nativeBattleChoiceServices(f.runtime);struct State {int expected,calls=0;} state{pair.second};s.user=&state;s.status=[](void* p,uint32_t,int32_t id) {auto& state=*static_cast<State*>(p);assert(id==state.expected);++state.calls;return 1;};choosePackedBattleCandidate(privateBattleCandidateRecords(f.buffer),0,s,f.sink());assert(state.calls==2 && f.chosen()==11);}
    Fixture cannot;cannot.two();cannot.weapon(142);assert(cannot.runtime.memory.writeWord(0x11138,1) && cannot.runtime.memory.writeWord(0x12138,1));cannot.run();assert(cannot.chosen()==11 && cannot.conversions.empty());
    Fixture status153;status153.weapon(153);auto s153=nativeBattleChoiceServices(status153.runtime);std::vector<int> statuses;s153.user=&statuses;s153.status=[](void* p,uint32_t,int32_t id) {static_cast<std::vector<int>*>(p)->push_back(id);return id==3?1:0;};choosePackedBattleCandidate(privateBattleCandidateRecords(status153.buffer),0,s153,status153.sink());assert(statuses==std::vector<int>({1,2,3}));
    Fixture fp;fp.weapon(200);assert(fp.runtime.memory.writeWord(0x11104,0x20000) && fp.runtime.memory.writeByte(0x803108a8+40*200+0x1e,1));fp.run();assert(fp.conversions==std::vector<int32_t>({3})); // Flag1 continues through FP deficit FMA.
    Fixture repeated;repeated.weapon(200);assert(repeated.runtime.memory.writeByte(0x803108a8+40*200+0x1e,1));auto repeatedServices=nativeBattleChoiceServices(repeated.runtime);struct Queries {std::vector<int> calls;} queries;repeatedServices.user=&queries;repeatedServices.maxFp=[](void* p,uint32_t)->int32_t {auto& q=*static_cast<Queries*>(p);q.calls.push_back(1);return q.calls.size()==1?20:40;};repeatedServices.fp=[](void* p,uint32_t)->int32_t {auto& q=*static_cast<Queries*>(p);q.calls.push_back(2);return q.calls.size()==2?0:1;};choosePackedBattleCandidate(privateBattleCandidateRecords(repeated.buffer),0,repeatedServices,repeated.sink());assert(queries.calls==std::vector<int>({1,2,1,2}) && repeated.conversions==std::vector<int32_t>({539}));
    Fixture full;full.two();full.weapon(200);assert(full.runtime.memory.writeHalf(0x11112,20) && full.runtime.memory.writeByte(0x803108a8+40*200+0x1e,1) && full.runtime.memory.writeByte(0x803108a8+40*200+0x1d,1));auto fullServices=nativeBattleChoiceServices(full.runtime);fullServices.user=&full;fullServices.fp=[](void* p,uint32_t)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(f.runtime.memory.writeWord(0x13008,201));return 20;};fullServices.maxFp=[](void*,uint32_t)->int32_t {return 20;};choosePackedBattleCandidate(privateBattleCandidateRecords(full.buffer),0,fullServices,full.sink());assert(full.conversions.empty() && full.chosen()==11); // Fresh weapon item201 with zero1d/1f overrides first row1;old item200 would preserve110 and win.
    Fixture owner;assert(owner.runtime.memory.writeWord(0x11008,225) && owner.runtime.memory.writeHalf(0x11112,99) && owner.runtime.memory.writeHalf(0x10112,7));assert(packedUnitFp(0x11000,false,nativeBattleActorServices(owner.runtime))==7 && packedUnitFp(0x11000,true,nativeBattleActorServices(owner.runtime))==20 && packedCanAct(0,nativeBattleActorServices(owner.runtime))==1);
    for(int draw:{0,100,101,211}) {Fixture f;f.two();f.draw=draw;f.run(0x80000000);assert(f.bound==210 && f.randomCalls==1 && f.chosen()==(draw==0?0:draw==100?11:draw==101?10:515));}
    Fixture raw;assert(raw.runtime.memory.registerRam(0x15000,std::vector<uint8_t>(raw.buffer.bytes.begin(),raw.buffer.bytes.end())));BattleCandidateRecords records{nativeBattleActorServices(raw.runtime),0x15000};choosePackedBattleCandidate(records,4,nativeBattleChoiceServices(raw.runtime),raw.sink());assert(raw.chosen()==10 && raw.conversions==std::vector<int32_t>({55}));
    Fixture core;core.two();BattleTakeoffBackend backend;backend.runtime=&core.runtime;const auto binding=nativeBattleTakeoffCoreServices(backend);int32_t unit,part;binding.choose(binding.user,core.buffer,4,unit,part);assert(unit==10 && part==103 && core.conversions==std::vector<int32_t>({99,50}) && core.randomCalls==0);
    std::cout<<"packed candidate choice modifiers/native sources/default provider tests passed\n";
}
