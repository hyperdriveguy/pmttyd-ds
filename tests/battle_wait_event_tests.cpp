#include "battle_event_callbacks.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventEntry caller,created;std::map<uint32_t,uint32_t> words;std::vector<uint32_t> deleted,scripts,reads,writes;
    std::function<void()> onDelete,onCreate,onUnitId;
    Fixture() {caller.arguments=0x1000;words[0x1000]=uint32_t(-2);words[0x8041eb90]=0x2000;words[0x4000]=7;created.id=11;}
    BattleUnitEventServices services() {
        BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto value=f.words[a];if(a==0x4000 && f.onUnitId)f.onUnitId();return value;};
        s.memory.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=v;};
        s.values.resolve=[](void*,EventEntry&,int32_t raw) {return raw;};s.translate=[](void*,EventEntry&,int32_t id) {assert(id==-2);return 7;};
        s.unit=[](void*,uint32_t b,int32_t id) {assert(b==0x2000 && id==7);return 0x4000u;};
        s.removeEventId=[](void* p,uint32_t id) {auto& f=*static_cast<Fixture*>(p);f.deleted.push_back(id);if(f.onDelete)f.onDelete();};
        s.createWait=[](void* p,uint32_t script,int32_t priority,uint32_t flags)->EventEntry& {auto& f=*static_cast<Fixture*>(p);assert(priority==10 && flags==0);f.scripts.push_back(script);if(f.onCreate)f.onCreate();return f.created;};return s;
    }
};
int main() {
    Fixture absent;absent.words[0x4290]=9;assert(battleStartUnitWaitEvent(absent.caller,absent.services())==2 && absent.deleted.empty() && absent.words[0x4290]==9);assert(runBattleUnitWait(0x4000,absent.services())==0 && absent.deleted==std::vector<uint32_t>{9} && absent.words[0x4290]==0); // Outer script gate skips cancellation;direct helper does not.
    Fixture stop;assert(battleStopUnitWaitEvent(stop.caller,stop.services())==2 && stop.deleted.empty() && stop.writes.empty());stop.words[0x4290]=99;stop.onDelete=[&] {stop.words[0x4290]=43;};assert(battleStopUnitWaitEvent(stop.caller,stop.services())==2 && stop.deleted==std::vector<uint32_t>{99} && stop.words[0x4290]==0);
    Fixture replace;replace.words[0x428c]=0x5000;replace.words[0x4290]=9;replace.onDelete=[&] {replace.words[0x428c]=0x6000;};replace.onCreate=[&] {assert(replace.words[0x4290]==0);replace.words[0x4000]=42;};replace.onUnitId=[&] {replace.created.id=77;};assert(battleStartUnitWaitEvent(replace.caller,replace.services())==2 && replace.deleted==std::vector<uint32_t>{9} && replace.scripts==std::vector<uint32_t>{0x6000} && replace.created.owner==42 && replace.words[0x4290]==77);
    // Real native root construction/deletion,unit identity and flags0 schedule rule.
    EventNativeRuntime r(0x81000000,{});std::array<EventEntry,3> entries{};assert(r.entryAddresses.registerEntries(0x82000000,entries.data(),3));r.field={3,entries.data(),0};r.runtime.allocationCount=1;r.runtime.nextId=1;
    auto& caller=entries[0];caller.flags=1;caller.id=500;caller.owner=7;caller.arguments=0x80300000;
    assert(r.memory.registerRam(0x80300000,std::vector<uint8_t>(4)) && r.memory.writeWord(0x80300000,uint32_t(-2)) && r.memory.registerRam(0x80301000,{0,0,0,2,0,0,0,1,0,0,0,0}));
    assert(r.memory.registerRam(0x84000000,std::vector<uint8_t>(0x1000)) && r.memory.registerRam(0x85000000,std::vector<uint8_t>(0x300)) && r.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)) && r.memory.writeWord(0x8041eb90,0x84000000) && r.memory.writeWord(0x84000020+7*4,0x85000000) && r.memory.writeWord(0x85000000,42) && r.memory.writeWord(0x8500028c,0x80301000));
    EventCallbackRegistry registry;assert(bindBattleStartWaitCallback(registry,r) && bindBattleStopWaitCallback(registry,r) && bindBattleWaitGuardMoveCallback(registry,r));int32_t result;r.schedule.managerActive=1;r.schedule.count=1;
    assert(registry.invoke(caller,0x8010a26c,1,result) && result==2 && entries[1].flags==1 && entries[1].owner==42 && entries[1].priority==10 && r.schedule.count==1 && r.runtime.allocationCount==2);
    uint32_t id;assert(r.memory.readWord(0x85000290,id) && id==1);assert(registry.invoke(caller,0x8010a26c,0,result) && result==2 && entries[1].id==2 && r.runtime.allocationCount==2 && r.schedule.count==1);
    assert(registry.invoke(caller,0x8010a1fc,0,result) && result==2 && entries[1].flags==0 && r.runtime.allocationCount==1 && r.memory.readWord(0x85000290,id) && id==0);
    assert(r.memory.writeWord(0x85000290,999) && registry.invoke(caller,0x8010a1fc,1,result) && result==2 && r.memory.readWord(0x85000290,id) && id==0 && r.runtime.allocationCount==1);
    for(uint32_t flags:{0u,1u,2u,3u,0x80000000u,0xffffffffu}) {assert(r.memory.writeWord(0x84000ef4,flags) && registry.invoke(caller,0x8010a1c8,0,result) && result==((flags&1)?0:2));}
    // Actual dispatcher USER_FUNC remains cached while blocked,then resumes
    // to original deletion once the independently owned guard flag clears.
    assert(r.memory.registerRam(0x80302000,{0,1,0,91,0x80,0x10,0xa1,0xc8,0,0,0,2,0,0,0,1,0,0,0,0}));
    r.platform.user=&registry;r.platform.call=EventCallbackRegistry::dispatch;
    auto& guarded=createRootEvent(r.schedule,0x80302000,10,0,r.creationServices());
    assert(r.memory.writeWord(0x84000ef4,1) && dispatchEventInterpreter(guarded,r.interpreterServices())==0 && guarded.opcode==91 && guarded.initialized==1 && guarded.cachedFunction==0x8010a1c8);
    assert(dispatchEventInterpreter(guarded,r.interpreterServices())==0 && guarded.flags==1);
    assert(r.memory.writeWord(0x84000ef4,0) && dispatchEventInterpreter(guarded,r.interpreterServices())==-1 && guarded.flags==0 && r.runtime.allocationCount==1);
    std::cout<<"wait-event lifecycle/identity/guard polling tests passed\n";
}
