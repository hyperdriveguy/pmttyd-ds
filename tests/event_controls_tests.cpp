#include "event_controls.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    EventGlobals gp;std::array<EventEntry,8> entries{},battle{};
    EventContext field{8,entries.data(),0},other{8,battle.data(),0};
    EventRuntimeGlobals runtime{0,0,.5f,0};
    int globalsReads=0;std::vector<bool> contexts;std::vector<uint8_t> parentFlags;
    bool switchContext=false;
    EventCreationServices services() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.globalsReads;return &f.gp;};
        s.manager.context=[](void* p,bool battle) {auto& f=*static_cast<Fixture*>(p);f.contexts.push_back(battle);f.parentFlags.push_back(f.entries[0].flags);
            if(f.switchContext && f.contexts.size()==2)f.gp.battleContext=1;
            return battle?&f.other:&f.field;
        };
        s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};return s;
    }
    void active(int index,uint8_t type=1) {entries[index].flags=1;entries[index].type=type;entries[index].id=100+index;}
};
int main() {
    Fixture chain;for(int i=0;i<3;++i)chain.active(i);
    chain.entries[0].waitingChild=&chain.entries[1];chain.entries[1].waitingChild=&chain.entries[2];
    stopEvent(chain.entries[0],1,chain.services());
    assert(chain.globalsReads==1 && chain.contexts==std::vector<bool>({false,false,false}));
    for(int i=0;i<3;++i)assert(chain.entries[i].flags==3);
    for(auto flag:chain.parentFlags)assert(flag==1); // Root mutation follows descendants.
    startEvent(chain.entries[0],1,chain.services());for(int i=0;i<3;++i)assert(chain.entries[i].flags==1);
    Fixture subtree;subtree.active(0,2);subtree.entries[0].waitingChild=&subtree.entries[1];
    subtree.entries[1].flags=0x80;subtree.entries[1].type=1;subtree.active(2);subtree.entries[2].brotherOwner=&subtree.entries[0];
    subtree.entries[3].flags=0x80;subtree.entries[3].type=1;subtree.entries[3].brotherOwner=&subtree.entries[0];
    stopEvent(subtree.entries[0],1,subtree.services());
    assert(subtree.entries[0].flags==1 && subtree.entries[1].flags==0x82 && subtree.entries[2].flags==3 && subtree.entries[3].flags==0x80);
    Fixture high;high.active(0,255);high.active(1,1);high.entries[0].waitingChild=&high.entries[1];
    stopEvent(high.entries[0],0x100,high.services());assert(high.entries[0].flags==1 && high.entries[1].flags==1);
    Fixture preserve;preserve.active(0);auto& e=preserve.entries[0];e.flags=0x93;e.lifetime=123;e.credit=2;e.waitCount=9;e.cachedFunction=888;e.locals[0]=44;
    startEvent(e,1,preserve.services());assert(e.flags==0x91 && e.lifetime==123 && e.credit==2 && e.waitCount==9 && e.cachedFunction==888 && e.locals[0]==44);
    stopEvent(e,1,preserve.services());stopEvent(e,1,preserve.services());startEvent(e,1,preserve.services());assert(e.flags==0x91);
    Fixture repeated;repeated.active(0);repeated.active(1);repeated.entries[1].brotherOwner=&repeated.entries[0];
    stopAllEvents(1,repeated.services());assert(repeated.entries[0].flags==3 && repeated.entries[1].flags==3 && repeated.globalsReads==4);
    Fixture excluded;excluded.active(0);excluded.active(1);excluded.entries[0].waitingChild=&excluded.entries[1];
    stopOtherEvents(&excluded.entries[1],1,excluded.services());assert(excluded.entries[1].flags==3); // Direct exclusion,transitive visit still reaches it.
    startOtherEvents(&excluded.entries[1],1,excluded.services());assert(excluded.entries[1].flags==1);
    Fixture ids;ids.active(0,1);ids.active(1,0x10);ids.active(2,2);ids.entries[0].id=ids.entries[1].id=ids.entries[2].id=7;
    ids.entries[3].id=7;ids.entries[3].flags=0x80;
    stopEventId(7,ids.services());assert(ids.entries[0].flags==3 && ids.entries[1].flags==1 && ids.entries[2].flags==3 && ids.entries[3].flags==0x80);
    assert(checkEventId(7,ids.services())==1 && checkEventId(8,ids.services())==0);
    ids.entries[0].flags=0x93;assert(checkEventId(7,ids.services())==1);startEventId(7,ids.services());assert(ids.entries[0].flags==0x91 && ids.entries[2].flags==1);
    ids.entries[0].id=0xffffffff;assert(checkEventId(0xffffffff,ids.services())==1);
    Fixture capture;for(int i=0;i<4;++i)capture.active(i);
    capture.entries[0].waitingChild=&capture.entries[1];capture.entries[1].waitingChild=&capture.entries[2];capture.entries[2].waitingChild=&capture.entries[3];capture.switchContext=true;
    stopEvent(capture.entries[0],1,capture.services());
    assert(capture.contexts==std::vector<bool>({false,false,false,true}) && capture.globalsReads==2);
    for(int i=0;i<4;++i)assert(capture.entries[i].flags==3);
    Fixture setters;setters.active(0);auto& changed=setters.entries[0];changed.flags=3;changed.credit=2;changed.timescale=4;
    setEventPriority(changed,-1);setEventType(changed,0x110);setEventSpeed(changed,3,setters.services());
    assert(changed.priority==255 && changed.type==0x10 && changed.flags==3 && changed.timescale==1.5f && changed.credit==2);
    // Stop inside a manager visit does not suppress its remaining credits.
    Fixture inside;inside.active(0);inside.entries[0].timescale=2;inside.gp.timestamp=100;EventSchedule schedule;int calls=0;
    struct Pass {Fixture* fixture;int* calls;} pass{&inside,&calls};
    EventManagerServices manager;manager.user=&pass;
    manager.globals=[](void* p) {return &static_cast<Pass*>(p)->fixture->gp;};
    manager.context=[](void* p,bool) {return &static_cast<Pass*>(p)->fixture->field;};
    manager.clock=[](void*)->uint32_t {return 4000;};
    manager.dispatch=[](void* p,EventEntry& event)->int32_t {auto& pass=*static_cast<Pass*>(p);++*pass.calls;stopEvent(event,1,pass.fixture->services());return 0;};
    runEventManager(schedule,manager);assert(calls==2 && inside.entries[0].lifetime==100 && inside.entries[0].flags==3);
    inside.gp.timestamp=200;runEventManager(schedule,manager);assert(calls==2 && inside.entries[0].lifetime==100);
    startEvent(inside.entries[0],1,inside.services());inside.gp.timestamp=300;runEventManager(schedule,manager);assert(calls==4 && inside.entries[0].lifetime==200);
    std::cout<<"event controls tests passed\n";
}
