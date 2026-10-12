#include "event_creation.hpp"
#include <cassert>
#include <cmath>
#include <map>
#include <vector>
#include <limits>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t command(int count,int opcode) {return uint32_t(uint16_t(count))<<16|uint32_t(opcode);}
struct Fixture {
    EventGlobals gp;
    std::array<EventEntry,6> field{},battle{};
    EventContext fieldContext{6,field.data(),0},battleContext{6,battle.data(),0};
    EventSchedule schedule;EventRuntimeGlobals runtime{5,100,.5f,0};
    std::map<uint32_t,uint32_t> memory;
    std::vector<uint32_t> reads,masks;int stopCount=-1;uint32_t stopId=999;
    bool switchContext=false,mutateFlags=false,mutateId=false;int chosenSlot=0;
    Fixture() {memory[0x1000]=command(0,1);memory[0x1004]=0xdeadbeef;}
    EventCreationServices services() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->gp;};
        s.manager.context=[](void* p,bool battle) {auto& f=*static_cast<Fixture*>(p);return battle?&f.battleContext:&f.fieldContext;};
        s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);assert(f.memory.count(address));f.reads.push_back(address);
            if(address==0x1000) {
                auto& e=f.field[f.chosenSlot];assert(f.runtime.allocationCount==6 && e.nextCommand==0x1000 && e.credit==0 && e.initialized==0);
                if(f.switchContext)f.gp.battleContext=1;
                if(f.mutateFlags)e.flags=0x20;
                if(f.mutateId)e.id=777;
            }
            return f.memory[address];
        };
        s.stopAll=[](void* p,uint32_t mask) {auto& f=*static_cast<Fixture*>(p);f.masks.push_back(mask);f.stopCount=f.schedule.count;f.stopId=f.runtime.nextId;
            auto& c=f.gp.battleContext?f.battleContext:f.fieldContext;
            for(int i=0;i<c.capacity;++i)if((c.entries[i].flags&1) && (c.entries[i].type&mask))c.entries[i].flags|=2;
        };return s;
    }
    EventEntry& create(uint32_t flags=0,int priority=10,int type=0xef) {return createTypedRootEvent(schedule,0x1000,priority,flags,type,services());}
};
}
int main() {
    Fixture dirty;dirty.field[0]={0,3,1,99,1,0};dirty.chosenSlot=1;
    dirty.field[1].flags=0x92;dirty.field[1].credit=std::numeric_limits<float>::infinity();
    dirty.field[1].locals.fill(123);dirty.field[1].messageFields.fill(9);dirty.field[1].waitingParent=&dirty.field[0];
    dirty.field[1].cachedFunction=0xabcdef;dirty.field[1].waitStart=999;dirty.field[1].userWork=99;
    auto& e=dirty.create(0x120,257,-2);
    assert(&e==&dirty.field[1] && e.flags==0x21 && e.priority==1 && e.type==254 && e.id==100 && dirty.runtime.nextId==101);
    assert(dirty.runtime.allocationCount==6 && e.timescale==.5f && e.credit==0 && !std::signbit(e.credit));
    assert(e.owner==-1 && e.field16c==-1 && e.loopIndex==255 && e.switchIndex==255);
    assert(e.restartScript==0x1000 && e.currentCommand==0x1000 && e.arguments==0 && e.opcode==0 && e.argumentCount==0);
    assert(!e.waitingParent && !e.waitingChild && !e.brotherOwner && e.cachedFunction==0 && e.waitStart==0 && e.userWork==0);
    for(auto value:e.locals)assert(value==0);for(auto value:e.localFlags)assert(value==0);
    for(auto id:e.labelIds)assert(id==255);for(auto addr:e.labelAddresses)assert(addr==0);for(auto value:e.messageFields)assert(value==0);
    assert(dirty.schedule.count==0 && dirty.reads==std::vector<uint32_t>({0x1000,0x1004}));
    Fixture defaultType;auto& root=createRootEvent(defaultType.schedule,0x1000,255,0,defaultType.services());assert(root.type==0xef && root.priority==255);
    Fixture labels;labels.memory.clear();labels.memory[0x1000]=command(0,2);labels.memory[0x1004]=command(1,3);
    labels.memory[0x1008]=0xfe363c80;labels.memory[0x100c]=command(1,3);labels.memory[0x1010]=0x180;
    labels.memory[0x1014]=command(0,1);labels.memory[0x1018]=77;
    auto& labeled=labels.create();assert(labeled.labelIds[0]==128 && labeled.labelIds[1]==128 && labeled.labelIds[2]==255);
    assert(labeled.labelAddresses[0]==0x100c && labeled.labelAddresses[1]==0x1014 && labeled.labelAddresses[2]==0);
    assert(labeled.nextCommand==0x1000 && labels.reads==std::vector<uint32_t>({0x1000,0x1004,0x1004,0x1008,0x100c,0x1010,0x1014,0x1018}));
    Fixture sixteen;sixteen.memory.clear();
    for(unsigned i=0;i<16;++i) {sixteen.memory[0x1000+8*i]=command(1,3);sixteen.memory[0x1004+8*i]=i;}
    sixteen.memory[0x1080]=command(0,1);sixteen.memory[0x1084]=0;
    auto& full=sixteen.create();for(unsigned i=0;i<16;++i)assert(full.labelIds[i]==i && full.labelAddresses[i]==0x1008+8*i);
    Fixture low16;low16.memory[0x1000]=command(1,0x103);low16.memory[0x1004]=5;low16.memory[0x1008]=command(0,1);low16.memory[0x100c]=0;
    assert(low16.create().labelIds[0]==255); // Scanner uses low16,not dispatcher low8.
    for(bool active:{false,true})for(uint32_t flags:{0u,0x20u}) {
        Fixture append;append.schedule.managerActive=active;append.schedule.count=1;append.schedule.ids[0]=500;
        auto& created=append.create(flags,255);const bool added=active && (flags&0x20);
        assert(append.schedule.count==(added?2:1) && append.schedule.ids[0]==500);
        if(added)assert(append.schedule.slots[1]==0 && append.schedule.ids[1]==created.id);
    }
    Fixture late;late.schedule.managerActive=true;late.mutateFlags=true;late.mutateId=true;late.create();
    assert(late.schedule.count==1 && late.schedule.ids[0]==777 && late.field[0].flags==0x20);
    for(int level:{-1,0,1,2,3,4,5}) {
        Fixture checked;checked.runtime.systemLevel=level;checked.create();
        const uint32_t masks[]={0,1,2,0x10,0xef};
        if(level>=1 && level<=4)assert(checked.masks==std::vector<uint32_t>({masks[level]}));else assert(checked.masks.empty());
    }
    Fixture all;all.field[0]={0,1,0,90,1,0};all.field[0].type=1;all.chosenSlot=1;
    all.schedule.managerActive=true;all.schedule.count=1;all.runtime.systemLevel=1;
    all.create(0x20,255,2);assert(all.field[0].flags==3 && all.field[1].flags==0x21 && all.stopCount==2);
    Fixture wrap;wrap.runtime.nextId=0xffffffff;wrap.runtime.systemLevel=1;wrap.schedule.managerActive=true;
    auto& wrapped=wrap.create(0x20);assert(wrapped.id==0xffffffff && wrap.stopId==0 && wrap.runtime.nextId==1 && wrap.stopCount==1);
    Fixture zero;zero.runtime.nextId=0;assert(zero.create().id==0 && zero.runtime.nextId==1);
    Fixture context;context.runtime.systemLevel=1;context.switchContext=true;context.battle[0]={0,1,0,50,1,0};context.battle[0].type=1;
    auto& field=context.create();assert(&field==&context.field[0] && context.field[0].flags==1 && context.battle[0].flags==3);
    Fixture live;live.field[0]={0,1,1,90,1,0};live.chosenSlot=1;live.gp.timestamp=100;
    auto manager=live.services().manager;std::vector<uint32_t> dispatched;bool created=false;
    struct Pass {Fixture* fixture;std::vector<uint32_t>* ids;bool* created;} pass{&live,&dispatched,&created};
    manager.user=&pass;
    manager.globals=[](void* p) {return &static_cast<Pass*>(p)->fixture->gp;};
    manager.context=[](void* p,bool battle) {auto& f=*static_cast<Pass*>(p)->fixture;return battle?&f.battleContext:&f.fieldContext;};
    manager.clock=[](void*)->uint32_t {return 4000;};
    manager.dispatch=[](void* p,EventEntry& entry)->int32_t {
        auto& pass=*static_cast<Pass*>(p);pass.ids->push_back(entry.id);
        if(!*pass.created) {*pass.created=true;pass.fixture->create(0x20,255);}
        return 0;
    };
    runEventManager(live.schedule,manager);
    assert(dispatched==std::vector<uint32_t>({90}) && live.field[1].credit==.5f && live.field[1].lifetime==100);
    live.gp.timestamp=200;runEventManager(live.schedule,manager);
    assert(dispatched==std::vector<uint32_t>({90,100,90}) && live.field[1].credit==0 && live.field[1].lifetime==200);
    std::cout<<"event creation tests passed\n";
}
