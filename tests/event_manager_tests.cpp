#include "event_manager.hpp"
#include <cassert>
#include <limits>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    EventGlobals global;
    std::array<EventEntry,6> field{},battle{};
    EventContext fieldContext{6,field.data(),0},battleContext{6,battle.data(),0};
    EventSchedule schedule;
    std::vector<uint32_t> calls;
    bool abort=false,terminate=false,append=false,changeIdentity=false,changeCurrent=false,contextSwitch=false;
    int globalsReads=0;uint32_t clock=40500000;
    Fixture() {global.timestamp=100;global.refreshRate=60;}
    EventManagerServices services() {
        EventManagerServices s;s.user=this;
        s.globals=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.globalsReads;return &f.global;};
        s.context=[](void* p,bool battle) {auto& f=*static_cast<Fixture*>(p);return battle?&f.battleContext:&f.fieldContext;};
        s.clock=[](void* p) {auto& f=*static_cast<Fixture*>(p);assert(f.schedule.managerActive);if(f.contextSwitch)f.global.battleContext=1;return f.clock;};
        s.dispatch=[](void* p,EventEntry& e)->int32_t {
            auto& f=*static_cast<Fixture*>(p);assert(f.schedule.managerActive && e.credit<e.timescale+1);f.calls.push_back(e.id);
            if(f.append && f.calls.size()==1) {f.field[3]={0,1,255,104,1,0};appendEventSchedule(f.schedule,3,104);}
            if(f.changeIdentity && f.calls.size()==1)f.field[1].id=200;
            if(f.changeCurrent && f.calls.size()==1) {e.flags=0x92;e.id=999;}
            if(f.abort)return 1;if(f.terminate)return -1;return 0;
        };return s;
    }
    void run() {runEventManager(schedule,services());}
    void three() {field[0]={0,1,1,101,1,0};field[1]={0,1,1,102,1,0};field[2]={0,1,2,103,1,0};}
};
int main() {
    assert(eventClockRate(40500000)==10125);
    assert(eventLifetimeDelta(100,200,40500000,60)==0);
    assert(eventLifetimeDelta(5,std::numeric_limits<uint64_t>::max()-4,40500000,60)==10);
    assert(eventLifetimeDelta(501*10125ULL,0,40500000,60)==16*10125ULL);
    assert(eventLifetimeDelta(500*10125ULL,0,40500000,60)==500*10125ULL);
    Fixture order;order.three();order.run();
    assert(order.calls==std::vector<uint32_t>({103,102,101}));
    assert(order.schedule.slots[0]==2 && order.schedule.slots[1]==1 && order.schedule.slots[2]==0);
    assert(order.schedule.ids[0]==103 && order.schedule.ids[1]==102 && order.schedule.ids[2]==101);
    assert(order.fieldContext.previousTimestamp==100 && !order.schedule.managerActive && order.globalsReads==2);
    for(auto flag:{uint8_t(2),uint8_t(0x10),uint8_t(0x80)}) {
        Fixture skip;skip.field[0]={55,uint8_t(1|flag),255,101,2,3};skip.run();
        assert(skip.schedule.count==1 && skip.calls.empty() && skip.field[0].lifetime==55 && skip.field[0].credit==3);
    }
    Fixture inactive;inactive.field[0]={55,2,255,101,2,3};inactive.run();assert(inactive.schedule.count==0);
    Fixture fractional;fractional.field[0]={0,1,0,1,.5f,0};fractional.run();
    assert(fractional.calls.empty() && fractional.field[0].credit==.5f && fractional.field[0].lifetime==100);
    fractional.global.timestamp=200;fractional.run();
    assert(fractional.calls==std::vector<uint32_t>({1}) && fractional.field[0].credit==0 && fractional.field[0].lifetime==200);
    Fixture credits;credits.field[0]={0,1,0,1,2,.25f};credits.run();
    assert(credits.calls==std::vector<uint32_t>({1,1}) && credits.field[0].credit==.25f && credits.field[0].lifetime==100);
    Fixture noRevalidation;noRevalidation.field[0]={0,1,0,1,2,0};noRevalidation.changeCurrent=true;noRevalidation.run();
    assert(noRevalidation.calls==std::vector<uint32_t>({1,999}) && noRevalidation.field[0].credit==0);
    Fixture stopEntry;stopEntry.three();stopEntry.field[2].timescale=2;stopEntry.terminate=true;stopEntry.run();
    assert(stopEntry.calls==std::vector<uint32_t>({103,102,101}) && stopEntry.field[2].credit==1);
    Fixture stopPass;stopPass.three();stopPass.abort=true;stopPass.run();
    assert(stopPass.calls==std::vector<uint32_t>({103}) && stopPass.field[0].lifetime==0 && !stopPass.schedule.managerActive);
    Fixture changed;changed.three();changed.changeIdentity=true;changed.run();
    assert(changed.calls==std::vector<uint32_t>({103,101}) && changed.field[1].lifetime==0);
    Fixture append;append.three();append.append=true;append.run();
    assert(append.calls==std::vector<uint32_t>({103,102,101,104}) && append.schedule.count==4);
    Fixture battle;battle.global.battleContext=-1;battle.battle[0]={0,1,0,9,1,0};battle.run();
    assert(battle.calls==std::vector<uint32_t>({9}) && battle.battleContext.previousTimestamp==100 && battle.fieldContext.previousTimestamp==0);
    Fixture different;different.field[0]={0,1,0,1,1,0};different.battle[0]={0,1,0,2,1,0};different.contextSwitch=true;different.run();
    assert(different.calls.empty() && different.schedule.ids[0]==2 && different.fieldContext.previousTimestamp==100 && different.battleContext.previousTimestamp==0);
    Fixture wrap;wrap.field[0]={std::numeric_limits<uint64_t>::max()-4,1,0,1,1,0};wrap.global.timestamp=10;wrap.run();assert(wrap.field[0].lifetime==5);
    Fixture large;large.field[0]={0,1,0,1,1,0};large.global.timestamp=501*10125ULL;large.run();
    assert(large.field[0].lifetime==16*10125ULL && large.fieldContext.previousTimestamp==501*10125ULL);
    // Two dispatch credits share one caller-owned sample; manager never samples input.
    Fixture shared;shared.field[0]={0,1,0,1,2,0};shared.run();assert(shared.calls.size()==2 && shared.field[0].lifetime==100);
    std::cout<<"event manager tests passed\n";
}
