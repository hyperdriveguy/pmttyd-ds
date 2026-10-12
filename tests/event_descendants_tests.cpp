#include "event_creation.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t command(int count,int opcode) {return uint32_t(uint16_t(count))<<16|uint32_t(opcode);}
struct Fixture {
    EventGlobals gp;
    std::array<EventEntry,6> entries{};
    EventContext context{6,entries.data(),0};EventSchedule schedule;EventRuntimeGlobals globals{1,100,.5f,0};
    std::map<uint32_t,uint32_t> memory;std::vector<uint32_t> observedIds;
    bool mutateSource=false,zeroId=false;int stopCount=0;
    Fixture() {
        auto& p=entries[0];p.flags=1;p.id=99;p.priority=255;p.type=2;p.owner=7;
        p.lifetime=999;p.timescale=4;p.credit=3;p.initialized=3;p.cachedFunction=456;
        p.userWork=0x80001000;p.userFlags=0x80002000;p.field170=170;p.field150=150;
        p.waitCount=33;p.waitStart=44;p.loopIndex=3;p.switchIndex=4;
        p.messageFields={1,2,3,4,5,6,7};p.locals[0]=-230000000+1536;p.localFlags={1,2,3};
        p.labelIds.fill(99);p.labelAddresses.fill(0xdeadbeef);
        memory[0x1000]=command(1,3);memory[0x1004]=8;
        memory[0x1008]=command(0,1);memory[0x100c]=0;
        entries[1].flags=0x92;entries[1].field150=1234;entries[1].messageFields.fill(99);
    }
    EventCreationServices services() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->gp;};
        s.manager.context=[](void* p,bool) {return &static_cast<Fixture*>(p)->context;};
        s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->globals;};
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);assert(f.memory.count(address));return f.memory[address];};
        s.stopAll=[](void* p,uint32_t) {auto& f=*static_cast<Fixture*>(p);++f.stopCount;f.observedIds.push_back(f.globals.nextId);
            assert(f.schedule.count==1 || !f.schedule.managerActive);
            if(f.zeroId)f.globals.nextId=0;
            if(f.mutateSource) {
                auto& p=f.entries[0];p.owner=77;p.type=8;p.userWork=0x80003000;p.userFlags=0x80004000;
                p.field170=171;p.field150=151;p.locals[0]=987;p.localFlags[0]=123;p.messageFields.fill(42);
            }
        };return s;
    }
    void block(int end) {
        entries[0].nextCommand=0x1000;
        memory[0x1008]=command(0,end);memory[0x100c]=command(1,3);memory[0x1010]=9;
        memory[0x1014]=command(0,1);memory[0x1018]=0;
    }
};
}
int main() {
    Fixture child;child.entries[0].waitingChild=&child.entries[4];child.schedule.managerActive=true;
    auto& c=createChildEvent(child.schedule,child.entries[0],0x1000,0,child.services());
    assert(&c==&child.entries[1] && c.waitingParent==&child.entries[0] && !c.waitingChild && !c.brotherOwner);
    assert(child.entries[0].waitingChild==&c && child.entries[0].flags==0x11 && child.entries[4].flags==0);
    assert(c.priority==0 && c.type==2 && c.owner==7 && c.flags==1 && c.id==100 && c.timescale==.5f && c.credit==0);
    assert(c.userWork==child.entries[0].userWork && c.userFlags==child.entries[0].userFlags);
    assert(c.locals==child.entries[0].locals && c.localFlags==child.entries[0].localFlags && c.messageFields==child.entries[0].messageFields);
    assert(c.field170==170 && c.field150==0 && c.lifetime==0 && c.waitCount==0 && c.waitStart==0 && c.initialized==0 && c.cachedFunction==0);
    assert(c.loopIndex==255 && c.switchIndex==255 && c.labelIds[0]==8 && c.labelAddresses[0]==0x1008 && c.labelIds[1]==255);
    assert(child.schedule.count==1 && child.schedule.slots[0]==1 && child.schedule.ids[0]==100);
    c.locals[0]=1;assert(child.entries[0].locals[0]==-229998464); // Copied encoded word,not shared locals.
    Fixture brother;brother.schedule.managerActive=true;
    auto& b=createBrotherEvent(brother.schedule,brother.entries[0],0x1000,0,brother.services());
    assert(!b.waitingParent && !b.waitingChild && b.brotherOwner==&brother.entries[0]);
    assert(b.priority==255 && brother.entries[0].flags==1 && brother.entries[0].waitingChild==nullptr);
    for(auto word:b.messageFields)assert(word==0);
    assert(b.field150==0 && b.locals==brother.entries[0].locals && brother.schedule.count==1);
    Fixture outside;createBrotherEvent(outside.schedule,outside.entries[0],0x1000,0,outside.services());assert(outside.schedule.count==0);
    for(bool isChild:{true,false}) {
        Fixture wrap;wrap.globals.nextId=0xffffffff;wrap.globals.systemLevel=1;wrap.zeroId=true;
        if(isChild)createChildEvent(wrap.schedule,wrap.entries[0],0x1000,0,wrap.services());
        else createBrotherEvent(wrap.schedule,wrap.entries[0],0x1000,0,wrap.services());
        assert(wrap.entries[1].id==0xffffffff);
        assert(wrap.observedIds==std::vector<uint32_t>({isChild?0u:1u}));
        assert(wrap.globals.nextId==(isChild?1u:0u));
    }
    Fixture inherited;inherited.globals.systemLevel=1;inherited.mutateSource=true;
    auto& copy=createChildEvent(inherited.schedule,inherited.entries[0],0x1000,0,inherited.services());
    assert(copy.owner==7 && copy.type==2 && copy.locals[0]==-229998464 && copy.messageFields[0]==1 && inherited.entries[0].locals[0]==987);
    Fixture attached;attached.block(112);attached.globals.systemLevel=1;attached.mutateSource=true;attached.schedule.managerActive=true;
    assert(startAttachedBrotherBlock(attached.schedule,attached.entries[0],attached.services())==2);
    auto& a=attached.entries[1];assert(attached.entries[0].nextCommand==0x100c && a.nextCommand==0x1000 && a.flags==0x61);
    assert(a.brotherOwner==&attached.entries[0] && !a.waitingParent && a.owner==77 && a.type==8 && a.field150==151 && a.field170==171);
    assert(a.userWork==0x80003000 && a.locals[0]==-229998464 && a.messageFields[0]==0);
    assert(a.labelIds[0]==8 && a.labelIds[1]==9); // Constructor scans beyond block end to terminal1.
    Fixture independent;independent.block(109);independent.globals.systemLevel=1;independent.mutateSource=true;independent.schedule.managerActive=true;
    assert(startIndependentInlineBlock(independent.schedule,independent.entries[0],independent.services())==2);
    auto& i=independent.entries[1];assert(independent.entries[0].nextCommand==0x100c && i.flags==0x61);
    assert(!i.waitingParent && !i.waitingChild && !i.brotherOwner && i.owner==77 && i.type==2 && i.field150==0);
    assert(i.locals[0]==987 && i.localFlags[0]==123 && i.messageFields[0]==42 && i.userWork==0x80003000 && independent.schedule.count==1);
    // The wrapper stops at the FIRST matching end marker,without nested scanning.
    Fixture first;first.block(112);first.memory[0x1000]=command(0,110);first.memory[0x1004]=command(0,112);
    startAttachedBrotherBlock(first.schedule,first.entries[0],first.services());assert(first.entries[0].nextCommand==0x1008);
    std::cout<<"event descendant tests passed\n";
}
