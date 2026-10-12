#include "event_control_opcodes.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t command(int n,int op) {return uint32_t(uint16_t(n))<<16|uint32_t(op);}
struct Fixture {
    EventGlobals gp;std::array<EventEntry,8> entries{};EventContext context{8,entries.data(),0};
    EventRuntimeGlobals globals{1,100,1,0};EventSchedule schedule;
    std::map<uint32_t,uint32_t> memory;
    std::vector<int> order;std::vector<int32_t> destinations,outputs;std::vector<uint32_t> functions;
    bool mutate=false;int32_t resolved=0;float floatValue=3;
    Fixture() {
        auto& e=entries[0];e.flags=1;e.id=1;e.owner=7;e.type=2;e.priority=10;e.arguments=0x3000;
        e.locals[0]=42;e.userWork=100;e.userFlags=200;e.field170=170;e.field150=150;e.messageFields.fill(9);
        memory[0x3000]=0x1000;memory[0x3004]=uint32_t(-30000000);
        memory[0x1000]=command(0,2);memory[0x1004]=command(0,1);memory[0x1008]=0;
    }
    EventCreationServices creation() {
        EventCreationServices s;s.user=this;s.manager.user=this;
        s.manager.globals=[](void* p) {return &static_cast<Fixture*>(p)->gp;};
        s.manager.context=[](void* p,bool) {return &static_cast<Fixture*>(p)->context;};
        s.runtime=[](void* p) {return &static_cast<Fixture*>(p)->globals;};
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(1);return f.memory.at(address);};
        s.stopAll=[](void* p,uint32_t) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);
            if(f.mutate) {f.entries[0].owner=77;f.entries[0].locals[0]=88;f.entries[0].type=8;f.entries[0].field150=151;f.entries[0].messageFields.fill(99);}
        };return s;
    }
    EventDispatchServices values() {
        EventDispatchServices s;s.user=this;
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(1);return f.memory.at(address);};
        s.resolve=[](void* p,EventEntry&,int32_t value)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(2);return f.resolved?f.resolved:value;};
        s.resolveFloat=[](void* p,EventEntry&,int32_t) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(3);return f.floatValue;};
        s.store=[](void* p,EventEntry& e,int32_t raw,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(5);f.destinations.push_back(raw);f.outputs.push_back(value);if(raw==-30000000)e.locals[0]=value;};
        s.remove=[](void* p,EventEntry& e) {auto& f=*static_cast<Fixture*>(p);deleteEvent(e,f.creation());};
        s.call=[](void* p,EventEntry& e,uint32_t function,int32_t first)->int32_t {
            auto& f=*static_cast<Fixture*>(p);assert(first==1);f.functions.push_back(function);
            if(function==0x44440001)e.locals[0]=333;else {assert(function==0x44440000 && e.locals[0]==333);}
            return 2;
        };
        s.handler=[](void* p,EventEntry& e,uint8_t op)->int32_t {auto& f=*static_cast<Fixture*>(p);auto result=eventControlOpcode(e,op,f.services());assert(result);return *result;};return s;
    }
    EventControlOpcodeServices services() {return {&schedule,creation(),values()};}
    int run(uint8_t op) {auto result=eventControlOpcode(entries[0],op,services());assert(result);return *result;}
    void block(int end) {
        entries[0].nextCommand=0x1000;memory[0x1000]=command(0,end);memory[0x1004]=command(0,1);memory[0x1008]=0;
        memory[0x3000]=uint32_t(-30000000);
    }
};
}
int main() {
    for(int opcode:{92,93}) {
        Fixture run;run.schedule.managerActive=true;run.globals.systemLevel=1;run.mutate=true;
        assert(run.run(opcode)==2);auto& newEntry=run.entries[1];
        assert(newEntry.flags==1 && !newEntry.waitingParent && !newEntry.brotherOwner && run.schedule.count==0);
        assert(newEntry.owner==77 && newEntry.locals[0]==88 && newEntry.userWork==100 && newEntry.messageFields[0]==0 && newEntry.field150==0 && newEntry.type==2);
        if(opcode==93)assert(run.entries[0].locals[0]==100 && run.outputs==std::vector<int32_t>({100}) && newEntry.locals[0]==88);
        else assert(run.outputs.empty());
    }
    for(int opcode:{108,111}) {
        Fixture block;block.block(opcode==108?109:112);block.schedule.managerActive=true;
        assert(block.run(opcode)==2 && block.schedule.count==1 && block.entries[0].nextCommand==0x1004);
        assert(block.entries[1].locals[0]==42 && block.entries[0].locals[0]==100 && block.entries[1].field150==0);
        assert(block.entries[1].brotherOwner==(opcode==111?&block.entries[0]:nullptr));
        assert(block.entries[1].messageFields[0]==(opcode==108?9u:0u));
    }
    Fixture plain;plain.block(112);assert(plain.run(110)==2 && plain.entries[1].field150==150);
    Fixture inlinePlain;inlinePlain.block(109);assert(inlinePlain.run(107)==2 && !inlinePlain.entries[1].brotherOwner);
    Fixture child;child.schedule.managerActive=true;child.entries[0].opcode=94;
    assert(child.run(94)==255 && child.entries[0].opcode==0 && child.entries[0].flags==0x11 && child.entries[0].waitingChild==&child.entries[1] && child.schedule.count==1);
    Fixture deleted;deleted.entries[1].flags=1;deleted.entries[1].id=9;deleted.globals.allocationCount=2;deleted.memory[0x3000]=9;
    assert(deleted.run(95)==2 && deleted.entries[1].flags==0 && deleted.entries[0].flags==1);
    for(int opcode:{109,112}) {Fixture end;assert(end.run(opcode)==(opcode==109?255:0) && end.entries[0].flags==0);}
    Fixture restart;restart.memory[0x3000]=0x1000;restart.entries[0].restartScript=0x9999;restart.entries[0].credit=4;
    assert(restart.run(96)==2 && restart.entries[0].restartScript==0x1000 && restart.entries[0].nextCommand==0x1000 && restart.entries[0].credit==0);
    Fixture setters;setters.memory[0x3000]=0xffffffff;assert(setters.run(97)==2 && setters.entries[0].priority==255);
    setters.order.clear();setters.globals.speed=.5f;setters.entries[0].credit=9;
    assert(setters.run(98)==2 && setters.entries[0].timescale==1.5f && setters.entries[0].credit==9 && setters.order==std::vector<int>({1,3}));
    setters.memory[0x3000]=0x110;assert(setters.run(99)==2 && setters.entries[0].type==0x10);
    for(int opcode:{100,102,104}) {
        Fixture stop;stop.entries[0].type=1;stop.entries[1].flags=1;stop.entries[1].type=1;stop.entries[1].id=8;
        stop.memory[0x3000]=opcode==104?8:1;assert(stop.run(opcode)==2 && stop.entries[1].flags==3);
        if(opcode==100)assert(stop.entries[0].flags==3);else assert(stop.entries[0].flags==1);
        assert(stop.run(opcode+1)==2 && stop.entries[1].flags==1);
    }
    Fixture check;check.memory[0x3000]=1;check.memory[0x3004]=uint32_t(-250000000);
    assert(check.run(106)==2 && check.outputs==std::vector<int32_t>({1}) && check.destinations[0]==-250000000);
    assert(!eventControlOpcode(check.entries[0],53,check.services()));
    // Real manager/dispatcher/control94 creates and joins child without deleting parent.
    Fixture flow;flow.entries[0].nextCommand=0x1000;flow.entries[0].timescale=1;flow.entries[0].priority=0;
    flow.memory[0x1000]=command(1,94);flow.memory[0x1004]=0x2000;
    flow.memory[0x1008]=command(1,91);flow.memory[0x100c]=0x44440000;flow.memory[0x1010]=command(0,2);flow.memory[0x1014]=command(0,1);flow.memory[0x1018]=0;
    flow.memory[0x2000]=command(1,91);flow.memory[0x2004]=0x44440001;flow.memory[0x2008]=command(0,2);flow.memory[0x200c]=command(0,1);flow.memory[0x2010]=0;
    flow.gp.timestamp=100;auto manager=flow.creation().manager;
    manager.clock=[](void*)->uint32_t {return 4000;};
    manager.dispatch=[](void* p,EventEntry& e)->int32_t {auto& f=*static_cast<Fixture*>(p);return dispatchEvent(e,f.values());};
    runEventManager(flow.schedule,manager);
    assert(flow.entries[0].flags==1 && !flow.entries[0].waitingChild && flow.entries[0].locals[0]==333 && flow.entries[1].flags==0);
    assert(flow.functions==std::vector<uint32_t>({0x44440001}));
    flow.gp.timestamp=200;runEventManager(flow.schedule,manager);
    assert(flow.entries[0].flags==0 && flow.functions==std::vector<uint32_t>({0x44440001,0x44440000}));
    std::cout<<"event control opcode tests passed\n";
}
