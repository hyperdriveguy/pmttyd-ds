#include "battle_event_callbacks.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventEntry event;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint16_t> halves;
    std::vector<uint32_t> reads,battles;std::vector<int32_t> resolved,ids;
    int32_t output=0,value=0,translated=7,statusResult=0,statusId=0;unsigned stores=0;
    std::function<void(int32_t)> onResolve;std::function<void()> onTranslate,onLookup,onHalf;
    Fixture() {event.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x1004]=uint32_t(-30000000);words[0x1008]=uint32_t(-29999999);words[0x8041eb90]=0x2000;halves[0x410c]=0xfffd;halves[0x4108]=0x7fff;words[0x4214]=0x87654321;}
    BattleUnitEventServices services() {
        BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.words[a];};
        s.memory.half=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.halves[a];if(f.onHalf)f.onHalf();return value;};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {static_cast<Fixture*>(p)->words[a]=v;};
        s.values.resolve=[](void* p,EventEntry&,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(raw);if(f.onResolve)f.onResolve(raw);return raw;};
        s.values.store=[](void* p,EventEntry&,int32_t output,int32_t value) {auto& f=*static_cast<Fixture*>(p);++f.stores;f.output=output;f.value=value;};
        s.translate=[](void* p,EventEntry&,int32_t input) {auto& f=*static_cast<Fixture*>(p);assert(input==-2);if(f.onTranslate)f.onTranslate();return f.translated;};
        s.unit=[](void* p,uint32_t battle,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.battles.push_back(battle);f.ids.push_back(id);if(f.onLookup)f.onLookup();return id==-1?0u:0x4000u;};s.checkStatus=[](void* p,uint32_t unit,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(unit==0x4000 || unit==0);f.statusId=id;return f.statusResult;};return s;
    }
};
int main() {
    for(bool maximum:{false,true}) {Fixture f;f.onTranslate=[&] {f.words[0x8041eb90]=0x3000;f.event.arguments=0x9000;};f.onHalf=[&] {f.words[0x1004]=uint32_t(-29999998);};assert(battleUnitHpEvent(f.event,maximum,f.services())==2 && f.battles==std::vector<uint32_t>{0x2000} && f.output==-29999998 && f.value==(maximum?32767:-3) && f.resolved==std::vector<int32_t>{-2});}
    Fixture noHp;noHp.translated=-1;assert(battleUnitHpEvent(noHp.event,false,noHp.services())==2 && noHp.stores==0 && noHp.battles.empty() && noHp.reads==std::vector<uint32_t>({0x8041eb90,0x1000}));
    Fixture get;get.words[0x1004]=uint32_t(-1);get.onResolve=[&](int32_t raw) {if(raw==-1) {get.words[0x8041eb90]=0x3000;get.words[0x1008]=uint32_t(-29999997);}};get.onLookup=[&] {get.words[0x1008]=uint32_t(-29999996);};assert(battleGetUnitWorkEvent(get.event,get.services())==2 && get.battles==std::vector<uint32_t>{0x3000} && get.output==-29999997 && uint32_t(get.value)==0x87654321 && get.reads==std::vector<uint32_t>({0x1000,0x1004,0x1008,0x8041eb90,0x4214}));
    Fixture set;set.words[0x1004]=uint32_t(-1);set.words[0x1008]=99;set.onResolve=[&](int32_t raw) {if(raw==99)set.words[0x8041eb90]=0x5000;};assert(battleSetUnitWorkEvent(set.event,set.services())==2 && set.words[0x4214]==99 && set.stores==0 && set.battles==std::vector<uint32_t>{0x5000});
    Fixture noGuard;noGuard.translated=-1;noGuard.words[0x1004]=0;noGuard.words[0x218]=7;assert(battleGetUnitWorkEvent(noGuard.event,noGuard.services())==2 && noGuard.value==7 && noGuard.ids==std::vector<int32_t>{-1}); // Original zero-pointer dereference forwarded to provider.
    Fixture status;status.words[0x1004]=0x1ff;status.statusResult=-7;assert(battleCheckUnitStatusEvent(status.event,status.services())==2 && status.statusId==-1 && status.value==1 && status.resolved==std::vector<int32_t>{-2});
    Fixture data;data.words[0x1008]=7;data.onLookup=[&] {data.words[0x1004]=uint32_t(-29999997);};data.onResolve=[&](int32_t key) {if(key==7) {data.words[0x4018]=0x6000;data.words[0x6000]=7;data.words[0x6004]=0;data.words[0x1004]=uint32_t(-29999996);}};
    assert(battleCheckUnitDataEvent(data.event,data.services())==2 && data.value==1 && data.output==-29999997 && data.reads.back()==0x6000);
    assert(!battleUnitHasData(0x4000,0,data.services().memory));data.words[0x6000]=0;assert(!battleUnitHasData(0x4000,7,data.services().memory));data.words[0x4018]=0;assert(!battleUnitHasData(0x4000,7,data.services().memory));
    // Native registry adapters operate on actual event and unit RAM aliases.
    EventNativeRuntime r(0x81000000,{});std::array<EventEntry,1> entries{};
    assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),1) && r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x120)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x300)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x84000020+7*4,0x85000000) && r.memory.registerRam(0x80300000,std::vector<uint8_t>(16)));
    EventCallbackRegistry registry;assert(bindBattleGetHpCallback(registry,r) && bindBattleGetMaxHpCallback(registry,r) && bindBattleGetUnitWorkCallback(registry,r) && bindBattleSetUnitWorkCallback(registry,r));
    auto& e=entries[0];e.owner=7;e.arguments=0x80300000;int32_t result;
    assert(r.memory.writeWord(e.arguments,uint32_t(-2)) && r.memory.writeWord(e.arguments+4,uint32_t(-30000000)) && r.memory.writeHalf(0x8500010c,0x8000) && r.memory.writeHalf(0x85000108,321));
    assert(registry.invoke(e,0x801101c4,1,result) && result==2 && e.locals[0]==-32768 && registry.invoke(e,0x801100cc,0,result) && e.locals[0]==321);
    assert(r.memory.writeWord(e.arguments+4,uint32_t(-1)) && r.memory.writeWord(e.arguments+8,123));assert(registry.invoke(e,0x8010f7a4,1,result) && result==2);uint32_t stored;assert(r.memory.readWord(0x85000214,stored) && stored==123);
    assert(r.memory.writeWord(e.arguments+8,uint32_t(-29999999)) && registry.invoke(e,0x8010f828,0,result) && result==2 && e.locals[1]==123);
    assert(bindBattleCheckStatusCallback(registry,r) && bindBattleCheckDataCallback(registry,r));
    assert(r.memory.writeWord(e.arguments+4,0x106) && r.memory.writeWord(e.arguments+8,uint32_t(-29999998)) && r.memory.writeByte(0x850002f7,1));
    assert(registry.invoke(e,0x80105f0c,0,result) && result==2 && e.locals[2]==1);
    assert(r.memory.registerRam(0x85001000,std::vector<uint8_t>(16)) && r.memory.writeWord(0x85000018,0x85001000) && r.memory.writeWord(0x85001000,7) && r.memory.writeWord(e.arguments+4,uint32_t(-29999997)) && r.memory.writeWord(e.arguments+8,7));
    assert(registry.invoke(e,0x80109f88,1,result) && result==2 && e.locals[3]==1);
    std::cout<<"native HP/unit-work callback capture and alias tests passed\n";
}
