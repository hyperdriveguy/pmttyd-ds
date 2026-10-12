#include "battle_candidate_choice.hpp"
#include "battle_timing_a.hpp"
#include <cassert>
#include <cmath>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
void half(BattleCandidateBuffer& b,unsigned offset,int value) {const uint16_t v=value;b.bytes[offset]=v>>8;b.bytes[offset+1]=v;}
void word(BattleCandidateBuffer& b,unsigned offset,uint32_t value) {b.bytes[offset]=value>>24;b.bytes[offset+1]=value>>16;b.bytes[offset+2]=value>>8;b.bytes[offset+3]=value;}
struct Fixture {
    BattleCandidateBuffer buffer;DamageWeapon weapon;DamageUnit actor,candidate,second;
    std::vector<int32_t> outputs,conversions,statusCalls;std::vector<int> calls;
    std::map<uint32_t,uint8_t> itemData;std::vector<uint32_t> itemReads;
    std::map<int,bool> statuses;int work=1,randomCalls=0;int32_t draw=0,bound=0;
    bool forceDraw=true,forceZero=false,recover=true,mutatePart=false,mutateItem=false;
    int maxCalls=0,fpCalls=0;BattleRandom rng{1};
    Fixture() {
        actor.kind=222;candidate.kind=222;candidate.belong=1;candidate.hp=5;candidate.maxHp=10;
        second=candidate;word(buffer,0xab8,7);buffer.bytes[0xa6c]=1;buffer.bytes[0xa6d]=3;
        half(buffer,4+36*3,10);half(buffer,6+36*3,103);
        half(buffer,4+36*2,11);half(buffer,6+36*2,102);
    }
    CandidateChoiceServices services() {
        CandidateChoiceServices s;s.user=this;
        s.captureWork=[](void* p)->void* {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);return &f.work;};
        s.output=[](void* p,bool unit,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(unit?2:3);f.outputs.push_back(value);
            if(f.mutatePart && unit && value==10)half(f.buffer,6+36*3,99);};
        s.unit=[](void* p,void* work,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work);f.calls.push_back(4);return id==7?&f.actor:id==10?&f.candidate:&f.second;};
        s.truncate=[](void* p,float value)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(std::isfinite(value) && double(value)>=-2147483648.0 && double(value)<2147483648.0);
            const int32_t result=f.forceZero?0:static_cast<int32_t>(value);f.conversions.push_back(result);return result;};
        s.random=[](void* p,int32_t bound)->int32_t {auto& f=*static_cast<Fixture*>(p);++f.randomCalls;f.bound=bound;return f.forceDraw?f.draw:f.rng.bounded(bound);};
        s.status=[](void* p,DamageUnit&,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.statusCalls.push_back(id);return f.statuses[id];};
        s.canRecover=[](void* p,DamageUnit&) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(5);return f.recover;};
        s.danger=[](void*,DamageUnit&)->uint8_t {return 5;};
        s.itemByte=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);f.itemReads.push_back(address);assert(f.itemData.count(address));return f.itemData[address];};
        s.maxFp=[](void* p,DamageUnit&)->int32_t {auto& f=*static_cast<Fixture*>(p);++f.maxCalls;f.calls.push_back(6);return 20;};
        s.fp=[](void* p,DamageUnit& u)->int32_t {auto& f=*static_cast<Fixture*>(p);++f.fpCalls;f.calls.push_back(7);if(f.mutateItem)f.weapon.itemId=201;return u.fp;};
        return s;
    }
    void two() {buffer.bytes[0xa6c]=2;buffer.bytes[0xa6e]=2;}
    void run(uint32_t flags=0) {chooseBattleCandidate(buffer,flags,services());}
    int32_t chosen() const {return outputs[outputs.size()-2];}
    uint32_t item(int id,unsigned offset) {return 0x803108a8u+40u*static_cast<uint32_t>(id)+offset;}
};
}
int main() {
    for(int8_t count:{int8_t(0),int8_t(-1)}) {
        Fixture empty;empty.buffer.bytes[0xa6c]=uint8_t(count);empty.run();
        assert(empty.outputs==std::vector<int32_t>({-1,0}) && empty.calls==std::vector<int>({1,2,3}) && empty.randomCalls==0);
    }
    Fixture ordinary;ordinary.two();const auto untouched=ordinary.buffer.bytes;ordinary.run(4);
    assert(ordinary.chosen()==10 && ordinary.conversions==std::vector<int32_t>({66,50}));
    assert(ordinary.randomCalls==0 && ordinary.rng.state==1 && ordinary.buffer.bytes==untouched);
    Fixture alias;alias.mutatePart=true;alias.run();assert(alias.outputs==std::vector<int32_t>({-1,0,10,99,10,99}));
    for(auto entry:std::map<uint32_t,int32_t>{{1,165},{4,55},{8,55},{0x20,165},{0x100,165},{0x200,165},{0x400,137},{0x800,137},{0x1000,165}}) {
        Fixture f;f.run(entry.first);assert(f.conversions==std::vector<int32_t>({entry.second}));
    }
    Fixture partner;partner.candidate.kind=225;partner.run(2);assert(partner.conversions.back()==165);
    Fixture same;same.candidate.belong=0;same.run(0x10);assert(same.conversions.back()==165);
    Fixture neutral;neutral.candidate.belong=2;neutral.run(0x20);assert(neutral.conversions.empty());
    Fixture ordered;ordered.run(0x1f3f);assert(ordered.conversions==std::vector<int32_t>({165,82,41,61,91,136,170,212,318}));
    Fixture tie;tie.two();tie.buffer.weapon=&tie.weapon;tie.weapon.itemId=146;tie.statuses[11]=true;tie.run();assert(tie.chosen()==11);
    Fixture drawZero;drawZero.two();drawZero.run(0x80000000);assert(drawZero.bound==210 && drawZero.chosen()==0);
    for(int draw:{100,101}) {Fixture f;f.two();f.draw=draw;f.run(0x80000000);assert(f.chosen()==(draw==100?11:10));}
    Fixture beyond;beyond.two();beyond.draw=211;beyond.run(0x80000000);assert(beyond.chosen()==515); // Physical row74 reads metadata.
    Fixture zeroSum;zeroSum.forceZero=true;zeroSum.forceDraw=false;zeroSum.run(0x80000004);
    assert(zeroSum.bound==0 && zeroSum.randomCalls==1 && zeroSum.rng.state==1 && zeroSum.chosen()==0);
    Fixture negative;negative.candidate.hp=100;negative.run(0x400);assert(negative.conversions.back()<0 && negative.chosen()==0);
    Fixture recovery;recovery.buffer.weapon=&recovery.weapon;recovery.weapon.itemId=142;recovery.recover=false;recovery.run();assert(recovery.statusCalls.empty());
    Fixture status;status.buffer.weapon=&status.weapon;status.weapon.itemId=153;status.statuses[3]=true;status.run();assert(status.statusCalls==std::vector<int32_t>({1,2,3}));
    for(auto entry:std::map<int,int>{{146,11},{147,15},{143,14},{134,18},{135,6},{136,7},{137,5}}) {
        Fixture f;f.buffer.weapon=&f.weapon;f.weapon.itemId=entry.first;f.statuses[entry.second]=true;f.run();assert(f.statusCalls==std::vector<int32_t>({entry.second}));
    }
    Fixture fp;fp.buffer.weapon=&fp.weapon;fp.weapon.itemId=200;fp.candidate.attributes=0x20000;
    fp.itemData[fp.item(200,0x1e)]=1;fp.run();
    assert(fp.maxCalls==2 && fp.fpCalls==2 && fp.conversions==std::vector<int32_t>({3})); // Flag overwrite continues into FP modifier.
    Fixture full;full.buffer.weapon=&full.weapon;full.weapon.itemId=200;full.candidate.fp=20;full.mutateItem=true;
    full.itemData[full.item(200,0x1e)]=1;full.itemData[full.item(201,0x1d)]=0;full.itemData[full.item(201,0x1f)]=0;
    full.run();assert(full.maxCalls==1 && full.fpCalls==1 && full.itemReads==std::vector<uint32_t>({full.item(200,0x1e),full.item(201,0x1d),full.item(201,0x1f)}));
    Fixture shortCircuit;shortCircuit.buffer.weapon=&shortCircuit.weapon;shortCircuit.weapon.itemId=200;shortCircuit.candidate.fp=20;
    shortCircuit.itemData[shortCircuit.item(200,0x1e)]=1;shortCircuit.itemData[shortCircuit.item(200,0x1d)]=1;shortCircuit.run();assert(shortCircuit.itemReads.size()==2);
    std::cout<<"candidate choice tests passed\n";
}
