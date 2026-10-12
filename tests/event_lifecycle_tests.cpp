#include "event_lifecycle.hpp"
#include "event_dispatch.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t command(int n,int op) {return uint32_t(uint16_t(n))<<16|uint32_t(op);}
struct Fixture {
    EventGlobals gp;std::array<EventEntry,8> entries{},battle{};
    EventContext field{8,entries.data(),0},other{8,battle.data(),0};
    EventRuntimeGlobals globals{8,100,.25f,0};EventSchedule schedule;
    std::map<uint32_t,uint32_t> memory{{0x1000,command(1,3)},{0x1004,8},{0x1008,command(0,1)},{0x100c,0}};
    std::vector<int> clears;int captures=0,runtimeReads=0;bool observeRestart=false;
    Fixture() {for(int i=0;i<8;++i) {entries[i].flags=1;entries[i].id=100+i;entries[i].locals[0]=i;}}
    EventCreationServices services() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.captures;return &f.gp;};
        s.manager.context=[](void* p,bool battle) {auto& f=*static_cast<Fixture*>(p);return battle?&f.other:&f.field;};
        s.runtime=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.runtimeReads;
            if(f.observeRestart && f.runtimeReads==1)assert(f.entries[0].timescale==1 && f.entries[0].credit==0 && f.entries[0].lifetime==55);
            if(!f.observeRestart)for(int i=0;i<8;++i)if(f.entries[i].flags==0 && f.entries[i].id==0) {
                bool seen=false;for(auto slot:f.clears)if(slot==i)seen=true;
                if(!seen)f.clears.push_back(i);
            }
            return &f.globals;
        };
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);assert(f.memory.count(address));return f.memory[address];};
        s.stopAll=[](void* p,uint32_t mask) {auto& f=*static_cast<Fixture*>(p);for(auto& e:f.entries)if((e.flags&1)&&(e.type&mask))e.flags|=2;};return s;
    }
};
}
int main() {
    Fixture inactive;inactive.entries[0].flags=0x92;inactive.entries[0].locals[0]=77;inactive.entries[0].waitingChild=&inactive.entries[1];
    deleteEvent(inactive.entries[0],inactive.services());assert(inactive.entries[0].flags==0x92 && inactive.entries[0].locals[0]==77 && inactive.entries[1].flags==1 && inactive.globals.allocationCount==8 && inactive.runtimeReads==0 && inactive.captures==1);
    Fixture tree;auto& root=tree.entries[0];auto& child=tree.entries[3];auto& grandchild=tree.entries[7];
    root.waitingChild=&child;root.flags|=0x10;child.waitingParent=&root;child.waitingChild=&grandchild;child.flags|=0x10;grandchild.waitingParent=&child;
    tree.entries[1].brotherOwner=&root;tree.entries[6].brotherOwner=&tree.entries[1];
    tree.entries[2].brotherOwner=&root; // Ascending live scan,regardless of priority/ID.
    tree.entries[4].locals[0]=444; // Independent inline-style entry: no owner link.
    deleteEvent(root,tree.services());
    assert(tree.clears==std::vector<int>({7,3,6,1,2,0}) && tree.globals.allocationCount==2);
    assert(tree.entries[4].flags==1 && tree.entries[4].locals[0]==444 && tree.entries[5].flags==1);
    Fixture copy;auto& parent=copy.entries[0];auto& c=copy.entries[1];auto& gc=copy.entries[2];
    parent.flags=0x93;parent.waitingChild=&copy.entries[7];parent.userWork=55;parent.owner=66;parent.timescale=3;parent.id=100;
    c.waitingParent=&parent;c.waitingChild=&gc;c.flags|=0x10;gc.waitingParent=&c;gc.locals.fill(987);gc.localFlags={1,2,3};gc.messageFields={7,6,5,4,3,2,1};
    deleteEvent(c,copy.services());
    assert(parent.flags==0x83 && !parent.waitingChild && parent.locals[15]==987 && parent.localFlags[2]==3 && parent.messageFields[0]==7);
    assert(parent.userWork==55 && parent.owner==66 && parent.timescale==3 && parent.id==100 && copy.entries[7].flags==1);
    Fixture brother;brother.entries[1].brotherOwner=&brother.entries[0];brother.entries[1].locals[0]=999;
    deleteEvent(brother.entries[1],brother.services());assert(brother.entries[0].locals[0]==0);
    Fixture inactiveParent;inactiveParent.entries[0].flags=0x10;inactiveParent.entries[1].waitingParent=&inactiveParent.entries[0];
    deleteEvent(inactiveParent.entries[1],inactiveParent.services());assert(inactiveParent.entries[0].flags==0 && inactiveParent.entries[0].locals[0]==1);
    Fixture ids;ids.entries[1].id=ids.entries[3].id=0xffffffff;ids.entries[5].id=0xffffffff;ids.entries[5].flags=0x80;
    ids.schedule.count=1;ids.schedule.ids[0]=0xffffffff;ids.schedule.slots[0]=1;
    deleteEventId(0xffffffff,ids.services());assert(ids.entries[1].id==0 && ids.entries[3].id==0 && ids.entries[5].id==0xffffffff && ids.globals.allocationCount==6);
    assert(ids.schedule.ids[0]==0xffffffff && ids.schedule.count==1); // Stale schedule is not edited by deletion.
    Fixture underflow;underflow.globals.allocationCount=0;deleteEvent(underflow.entries[0],underflow.services());assert(underflow.globals.allocationCount==0xffffffff);
    Fixture restart;auto& e=restart.entries[0];e.restartScript=0x1000;e.nextCommand=0x9999;e.currentCommand=0x8888;e.opcode=91;e.initialized=3;
    e.arguments=0x7777;e.argumentCount=7;e.cachedFunction=456;e.waitCount=12;e.waitStart=34;e.lifetime=55;e.credit=4;e.timescale=8;
    e.flags=0x11;e.waitingChild=&restart.entries[1];e.waitingParent=&restart.entries[2];e.brotherOwner=&restart.entries[3];
    e.locals[0]=-229998464;e.userWork=99;e.messageFields[0]=88;e.field150=77;e.field170=66;e.labelIds.fill(0);e.labelAddresses.fill(999);
    const auto allocation=restart.globals.allocationCount,id=restart.globals.nextId;restart.observeRestart=true;
    assert(&restartEvent(e,restart.services())==&e);
    assert(e.nextCommand==0x1000 && e.currentCommand==0x1000 && e.opcode==0 && e.timescale==.25f && e.credit==0 && e.lifetime==0);
    assert(e.initialized==3 && e.arguments==0x7777 && e.argumentCount==7 && e.cachedFunction==456 && e.waitCount==12 && e.waitStart==34);
    assert(e.flags==0x11 && e.waitingChild==&restart.entries[1] && e.waitingParent==&restart.entries[2] && e.brotherOwner==&restart.entries[3]);
    assert(e.locals[0]==-229998464 && e.userWork==99 && e.messageFields[0]==88 && e.field150==77 && e.field170==66);
    assert(e.labelIds[0]==8 && e.labelIds[1]==255 && e.labelAddresses[0]==0x1008 && e.labelAddresses[1]==0);
    assert(restart.globals.allocationCount==allocation && restart.globals.nextId==id && restart.schedule.count==0);
    Fixture stopped;stopped.globals.systemLevel=1;stopped.entries[0].restartScript=0x1000;stopped.entries[1].type=1;
    restartEvent(stopped.entries[0],stopped.services());assert(stopped.entries[1].flags==3);
    // Restart leaves the old marker until the next fetch,which initializes wait.
    EventDispatchServices dispatch;dispatch.user=&restart;
    restart.memory[0x1000]=command(1,9);restart.memory[0x1004]=1;
    dispatch.word=[](void* p,uint32_t address) {return static_cast<Fixture*>(p)->memory.at(address);};
    dispatch.resolve=[](void*,EventEntry&,int32_t value)->int32_t {return value;};
    assert(dispatchEvent(e,dispatch)==0 && e.waitCount==0 && e.initialized==1 && e.arguments==0x1004 && e.argumentCount==1);
    for(uint8_t parentPriority:{uint8_t(0),uint8_t(2)}) {
        Fixture resumed;resumed.gp.timestamp=100;
        auto& p=resumed.entries[0];auto& child=resumed.entries[1];
        p.flags=0x11;p.priority=parentPriority;p.timescale=1;p.waitingChild=&child;
        child.priority=1;child.timescale=1;child.waitingParent=&p;child.locals[0]=333;
        std::vector<uint32_t> calls;
        struct Pass {Fixture* fixture;std::vector<uint32_t>* calls;} pass{&resumed,&calls};
        EventManagerServices manager;manager.user=&pass;
        manager.globals=[](void* user) {return &static_cast<Pass*>(user)->fixture->gp;};
        manager.context=[](void* user,bool) {return &static_cast<Pass*>(user)->fixture->field;};
        manager.clock=[](void*)->uint32_t {return 4000;};
        manager.dispatch=[](void* user,EventEntry& event)->int32_t {
            auto& pass=*static_cast<Pass*>(user);pass.calls->push_back(event.id);
            if(event.id==101) {deleteEvent(event,pass.fixture->services());return -1;}
            assert(event.id==100 && event.locals[0]==333);return 0;
        };
        runEventManager(resumed.schedule,manager);
        if(parentPriority==0)assert(calls==std::vector<uint32_t>({101,100}));
        else {
            assert(calls==std::vector<uint32_t>({101}) && p.lifetime==0);
            resumed.gp.timestamp=200;runEventManager(resumed.schedule,manager);
            assert(calls==std::vector<uint32_t>({101,100}) && p.lifetime==100);
        }
    }
    std::cout<<"event lifecycle tests passed\n";
}
