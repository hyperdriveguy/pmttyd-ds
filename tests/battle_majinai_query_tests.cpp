#include "battle_majinai_query.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Operation {int kind;uint32_t address,value;bool operator==(const Operation& o) const {return kind==o.kind && address==o.address && value==o.value;}};
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;std::vector<Operation> operations;
    EventEntry event;std::function<void()> onOutput;std::function<void(uint32_t)> onByteStore;
    Fixture() {event.arguments=0x1000;words[0x1000]=uint32_t(-30000000);words[0x8041eb90]=0x2000;words[0x8041eb00]=0x20000;bytes[0x205ba]=1;bytes[0x205b8]=1;}
    BattleMajinaiQueryServices services() {
        BattleMajinaiQueryServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);auto v=f.words[a];f.operations.push_back({0,a,v});return v;};
        s.byte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);auto v=f.bytes[a];f.operations.push_back({1,a,v});return v;};
        s.memory.storeByte=[](void* p,uint32_t a,uint8_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({2,a,v});f.bytes[a]=v;if(f.onByteStore)f.onByteStore(a);};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({3,a,v});f.words[a]=v;};
        s.values.store=[](void* p,EventEntry& e,int32_t token,int32_t value) {auto& f=*static_cast<Fixture*>(p);assert(token==-30000000);f.operations.push_back({4,uint32_t(token),uint32_t(value)});e.locals[0]=value;if(f.onOutput)f.onOutput();};return s;
    }
};
int main() {
    for(uint8_t mode:{0,2,255}) {
        Fixture f;f.bytes[0x205ba]=mode;f.bytes[0x1aff9]=9;
        assert(battleMajinaiPowerQuery(f.event,f.services())==2 && f.event.locals[0]==0 && f.bytes[0x1aff9]==9 && f.bytes[0x205b8]==1 && f.bytes[0x205ba]==mode);
        assert(f.operations.size()==5 && f.operations[0].address==0x8041eb90 && f.operations[1].address==0x1000 && f.operations[2].address==0x8041eb00 && f.operations.back().kind==4);
    }
    for(uint8_t count:{0,1,2,255}) {
        Fixture f;f.bytes[0x205b8]=count;f.words[0x2ef4]=0x100;
        assert(battleMajinaiPowerQuery(f.event,f.services())==2 && f.event.locals[0]==1 && f.bytes[0x1aff9]==1 && f.bytes[0x205b8]==uint8_t(count-1) && f.bytes[0x205b9]==255 && f.bytes[0x205ba]==0);
        assert(f.words[0x2ef4]==(count==1?0x00400100u:0x100u));
        const auto& ops=f.operations;assert(ops[4].kind==4 && ops[5]==Operation({2,0x1aff9,1}) && ops[8]==Operation({2,0x205b8,uint8_t(count-1)}) && ops[9].kind==1 && ops[10]==Operation({2,0x205b9,255}) && ops[11]==Operation({2,0x205ba,0}));
    }
    Fixture alias;alias.onOutput=[&] {alias.words[0x8041eb90]=0x4000;alias.words[0x8041eb00]=0x30000;alias.bytes[0x305b8]=1;alias.bytes[0x305ba]=1;};
    alias.onByteStore=[&](uint32_t a) {if(a==0x305ba) {alias.words[0x8041eb90]=0x5000;alias.words[0x8041eb00]=0x40000;alias.bytes[0x405b8]=0;}};
    assert(battleMajinaiPowerQuery(alias.event,alias.services())==2 && alias.bytes[0x1aff9]==1 && alias.bytes[0x1cff9]==0 && alias.bytes[0x205b8]==1 && alias.bytes[0x305b8]==0 && alias.words[0x5ef4]==0x00400000 && alias.words[0x2ef4]==0);
    Fixture reread;reread.onByteStore=[&](uint32_t a) {if(a==0x205b8)reread.bytes[a]=7;};battleMajinaiDone(reread.services());assert(reread.operations[3]==Operation({1,0x205b8,7}) && reread.bytes[0x205b9]==255 && reread.bytes[0x205ba]==0);
    std::cout<<"Majinai query consumption and captured-owner tests passed\n";
}
