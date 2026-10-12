#include "battle_majinai_query.hpp"
#include "battle_timing_a.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Operation {int kind;uint32_t address,value;bool operator==(const Operation& o) const {return kind==o.kind && address==o.address && value==o.value;}};
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;std::vector<Operation> ops;
    int32_t random=0;std::vector<int32_t> bounds;std::function<void()> onRandom;std::function<void(uint32_t,uint8_t)> onStore;
    Fixture() {words[0x8041eb00]=0x20000;words[0x8041eb90]=0x2000;bytes[0x205b8]=2;bytes[0x205b9]=1;words[0x4738]=0x3000;words[0x300c]=0x4000;}
    BattleMajinaiQueryServices services() {
        BattleMajinaiQueryServices s;s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);auto v=f.words[a];f.ops.push_back({0,a,v});return v;};
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);auto v=f.bytes[a];f.ops.push_back({1,a,v});return v;};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.ops.push_back({2,a,v});f.bytes[a]=v;if(f.onStore)f.onStore(a,v);};
        s.irand=[](void* p,int32_t bound) {auto& f=*static_cast<Fixture*>(p);f.bounds.push_back(bound);f.ops.push_back({3,uint32_t(bound),uint32_t(f.random)});if(f.onRandom)f.onRandom();return f.random;};return s;
    }
};
int main() {
    Fixture empty;empty.bytes[0x205b8]=0;empty.bytes[0x205b9]=255;empty.bytes[0x205ba]=9;assert(battleMajinaiCheck(empty.services())==0 && empty.ops.size()==3 && empty.ops[0].address==0x8041eb00 && empty.ops[1].address==0x8041eb90 && empty.bounds.empty() && empty.bytes[0x205ba]==9);
    for(uint8_t countdown:{128,255}) {Fixture f;f.bytes[0x205b9]=countdown;f.bytes[0x205ba]=9;f.random=4;assert(battleMajinaiCheck(f.services())==0 && f.bytes[0x205b9]==9 && f.bytes[0x205ba]==0 && f.bounds==std::vector<int32_t>{5});assert(f.ops[4]==Operation({2,0x205ba,0}) && f.ops[6]==Operation({2,0x205b9,10}) && f.ops[7]==Operation({1,0x205b9,10}) && f.ops[8]==Operation({2,0x205b9,9}));}
    for(uint8_t mode:{1,2,255}) {Fixture f;f.bytes[0x205ba]=mode;assert(battleMajinaiCheck(f.services())==mode && f.bytes[0x205b9]==1 && f.bounds.empty() && f.ops.size()==5);}
    for(uint8_t countdown:{2,127}) {Fixture f;f.bytes[0x205b9]=countdown;assert(battleMajinaiCheck(f.services())==0 && f.bytes[0x205b9]==countdown-1 && f.bounds.empty());}
    struct Threshold {int32_t draw;int clearMode,setMode;};
    const Threshold thresholds[]{{-1,1,1},{29,1,1},{30,2,1},{44,2,1},{45,2,2},{59,2,2},{60,3,2},{79,3,2},{80,4,2},{89,4,2},{90,4,3},{100,4,3}};
    for(uint8_t countdown:{0,1})for(bool setup:{false,true})for(auto row:thresholds) {
        Fixture f;f.bytes[0x205b9]=countdown;f.words[0x4004]=setup?0x20:0;f.random=row.draw;
        const int expected=setup?row.setMode:row.clearMode;
        assert(battleMajinaiCheck(f.services())==expected && f.bytes[0x205ba]==expected && f.bytes[0x205b9]==0 && f.bytes[0x205b8]==2 && f.bounds==std::vector<int32_t>{100});
        assert(f.ops[5]==Operation({2,0x205b9,uint8_t(countdown-1)}) && f.ops[6].kind==1 && f.ops[7]==Operation({2,0x205b9,0}) && f.ops[8].address==0x4738 && f.ops.back().kind==1);
    }
    Fixture aliases;aliases.bytes[0x205b9]=255;aliases.onRandom=[&] {aliases.words[0x8041eb00]=0x30000;};aliases.onStore=[&](uint32_t a,uint8_t v) {if(a==0x205b9 && v==6)aliases.bytes[a]=2;};assert(battleMajinaiCheck(aliases.services())==0 && aliases.bytes[0x205b9]==1 && aliases.bytes[0x305b9]==0);
    Fixture finalRead;finalRead.onStore=[&](uint32_t a,uint8_t) {if(a==0x205ba)finalRead.bytes[a]=255;};assert(battleMajinaiCheck(finalRead.services())==255);
    for(int count:{-1,0,2,256}) {Fixture init;init.bytes[0x205b8]=2;init.bytes[0x205b9]=7;init.bytes[0x205ba]=8;initializePouchMajinai(count,init.services());assert(init.bounds.empty());if(count<=2)assert(init.ops.size()==2 && init.bytes[0x205b9]==7 && init.bytes[0x205ba]==8);else assert(init.bytes[0x205b8]==0 && init.bytes[0x205b9]==255 && init.bytes[0x205ba]==0);}
    Fixture initAlias;initAlias.onStore=[&](uint32_t a,uint8_t) {if(a==0x205b8)initAlias.words[0x8041eb00]=0x30000;if(a==0x305b9)initAlias.words[0x8041eb00]=0x40000;};initializePouchMajinai(3,initAlias.services());assert(initAlias.bytes[0x205b8]==3 && initAlias.bytes[0x305b9]==255 && initAlias.bytes[0x405ba]==0 && initAlias.bytes[0x205b9]==1);
    // Existing original generator,with authored seed:producer advances it only
    // on the chosen draw branch,not on an already selected mode.
    Fixture draw;BattleRandom generator(1);draw.onRandom=[&] {draw.random=generator.bounded(draw.bounds.back());};draw.bytes[0x205b9]=255;assert(battleMajinaiCheck(draw.services())==0 && generator.state!=1);auto saved=generator.state;draw.bytes[0x205b9]=1;draw.bytes[0x205ba]=2;assert(battleMajinaiCheck(draw.services())==2 && generator.state==saved);
    std::cout<<"fortune producer/count initializer tests passed\n";
}
