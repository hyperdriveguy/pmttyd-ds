#include "battle_data_child.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
    std::array<EventEntry,4> entries{};EventContext context{4,entries.data(),0};EventGlobals gp;
    EventRuntimeGlobals runtime{1,11,1,0};EventSchedule schedule;
    std::map<uint32_t,uint32_t> memory;
    std::vector<int> order;std::vector<uint32_t> reads;
    std::vector<int32_t> resolved;
    uint32_t work=0x2000;int32_t translated=42;
    std::function<void()> onTranslate,onKey,onLookup,onStop;
    Fixture() {
        auto& e=entries[0];e.flags=1;e.id=10;e.owner=7;e.priority=10;e.type=0xef;e.opcode=91;e.initialized=1;e.arguments=0x1000;e.locals[0]=123;e.messageFields[0]=7;e.userWork=0x9000;
        memory[0x1000]=uint32_t(-2);memory[0x1004]=7;memory[0x4018]=0x5000;memory[0x5000]=7;memory[0x5004]=0x7000;memory[0x7000]=1;
    }
    uint32_t read(uint32_t a) {order.push_back(1);reads.push_back(a);return memory[a];}
    EventCreationServices creation() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->gp;};s.manager.context=[](void* p,bool) {return &static_cast<Fixture*>(p)->context;};s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};
        s.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->read(a);};s.stopAll=[](void* p,uint32_t) {auto& f=*static_cast<Fixture*>(p);if(f.onStop)f.onStop();};return s;
    }
    BattleDataChildServices services() {
        BattleDataChildServices s;s.user=this;s.memory.user=this;s.values.user=this;s.schedule=&schedule;s.creation=creation();s.memory.word=s.creation.word;s.values.word=s.creation.word;
        s.values.resolve=[](void* p,EventEntry&,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(2);f.resolved.push_back(value);if(f.resolved.size()==2 && f.onKey)f.onKey();return value;};
        s.translate=[](void* p,EventEntry&,int32_t value) {assert(value==-2);auto& f=*static_cast<Fixture*>(p);f.order.push_back(3);if(f.onTranslate)f.onTranslate();return f.translated;};
        s.currentBattle=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);return f.work;};
        s.unit=[](void* p,uint32_t work,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(work==f.work && id==42);f.order.push_back(5);if(f.onLookup)f.onLookup();return 0x4000u;};return s;
    }
};
}
int main() {
    Fixture f;f.schedule.managerActive=true;f.schedule.count=1;f.schedule.slots[0]=0;f.schedule.ids[0]=10;auto& parent=f.entries[0];
    assert(runBattleDataChild(parent,f.services())==255);auto& child=f.entries[1];
    assert(parent.flags==0x11 && parent.opcode==0 && parent.waitingChild==&child && child.waitingParent==&parent && child.owner==42);
    assert(child.priority==11 && child.type==0xef && child.userWork==0x9000 && child.locals[0]==123 && child.messageFields[0]==7 && f.runtime.allocationCount==2 && f.schedule.count==2 && f.schedule.ids[1]==11);
    assert(std::vector<int>(f.order.begin(),f.order.begin()+10)==std::vector<int>({1,2,3,4,5,1,2,1,1,1}));
    Fixture captured;captured.onTranslate=[&] {captured.work=0x3000;captured.entries[0].arguments=0x9999;captured.memory[0x1004]=8;};
    captured.onLookup=[&] {captured.memory[0x1004]=9;};captured.onKey=[&] {captured.memory[0x4018]=0x6000;captured.memory[0x6000]=9;captured.memory[0x6004]=0x7000;};
    captured.runtime.systemLevel=1;captured.onStop=[&] {captured.entries[0].owner=99;captured.memory[0x4000]=100;};
    assert(runBattleDataChild(captured.entries[0],captured.services())==255 && captured.resolved==std::vector<int32_t>({-2,9}) && captured.entries[1].owner==42 && captured.entries[0].owner==99);
    for(int mode:{0,1,2}) {
        Fixture missing;if(mode==0)missing.memory[0x4018]=0;else if(mode==1)missing.memory[0x5000]=0;else missing.memory[0x5004]=0;
        assert(runBattleDataChild(missing.entries[0],missing.services())==2 && missing.entries[0].opcode==91 && missing.entries[0].flags==1 && missing.runtime.allocationCount==1 && missing.entries[1].flags==0);
    }
    Fixture duplicate;duplicate.memory[0x5004]=0;duplicate.memory[0x5008]=7;duplicate.memory[0x500c]=0x8000;
    assert(battleUnitData(0x4000,7,duplicate.services().memory)==0); // First matching zero value wins.
    assert(battleUnitData(0x4000,0,duplicate.services().memory)==0); // Terminator is tested before key comparison.
    std::cout<<"battle data-child tests passed\n";
}
