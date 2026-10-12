#include "event_context_initialization.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
using namespace ttyd;
namespace {
struct Operation {int kind;bool battle;uint32_t value;bool operator==(const Operation& o) const {return kind==o.kind && battle==o.battle && value==o.value;}};
struct Fixture {
    std::array<EventEntry,256> fieldEntries{},battleEntries{},replacement{};
    EventContext field{256,fieldEntries.data(),0xaaaabbbbccccdddd},battle{256,battleEntries.data(),0x1111222233334444};
    EventGlobals gp{0,60,0x123456789abcdef0},otherGp{1,50,0xaaaaaaaa55555555};
    EventGlobals* current=&gp;
    EventRuntimeGlobals runtime{123,77,.25f,3};
    EventSchedule schedule;
    std::vector<Operation> operations;
    std::vector<EventEntry*> cleared;
    unsigned allocations=0,gpReads=0;
    bool failure=false;
    std::function<void()> onAllocate,onGlobals;
    std::function<void(EventContext&,bool)> onTimestamp;
    Fixture() {
        field.work.fill(42);battle.work.fill(43);field.flags.fill(44);battle.flags.fill(45);field.field94=46;battle.field94=47;
        schedule.count=8;schedule.managerActive=true;schedule.slots.fill(9);schedule.ids.fill(10);
        for(auto* bank:{&fieldEntries,&battleEntries,&replacement})for(auto& e:*bank) {e.flags=0x11;e.id=99;e.locals.fill(100);e.loopCounts.fill(101);e.switchStates.fill(255);e.credit=3;e.userWork=0x7000;}
    }
    EventContextInitializationServices services() {
        EventContextInitializationServices s;s.user=this;s.manager.user=this;s.field=&field;s.battle=&battle;s.runtime=&runtime;s.schedule=&schedule;
        s.manager.globals=[](void* p) {auto& f=*static_cast<Fixture*>(p);++f.gpReads;f.operations.push_back({1,f.allocations==2,0});if(f.onGlobals)f.onGlobals();return f.current;};
        s.allocate=[](void* p,int32_t heap,uint32_t bytes) {auto& f=*static_cast<Fixture*>(p);assert(heap==0 && bytes==0x1b000);++f.allocations;assert((f.allocations==1?f.field:f.battle).capacity==256);f.operations.push_back({2,f.allocations==2,bytes});if(f.onAllocate)f.onAllocate();return f.failure?nullptr:(f.allocations==1?f.fieldEntries.data():f.battleEntries.data());};
        s.timestampHalf=[](void* p,EventContext& c,bool high,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({high?4:3,&c==&f.battle,v});
            c.previousTimestamp=high?(uint64_t(v)<<32)|(c.previousTimestamp&0xffffffffu):(c.previousTimestamp&0xffffffff00000000ULL)|v;
            if(f.onTimestamp)f.onTimestamp(c,high);
        };
        s.clearEntries=[](void* p,EventEntry* entries,uint32_t bytes) {auto& f=*static_cast<Fixture*>(p);f.cleared.push_back(entries);f.operations.push_back({5,entries==f.battleEntries.data(),bytes});
            assert(bytes%0x1b0==0);if(entries)for(uint32_t i=0;i<bytes/0x1b0;++i)entries[i]={}; // Semantic adapter,original byte count.
        };
        s.clearBank=[](void* p,EventContext& c,bool flags) {auto& f=*static_cast<Fixture*>(p);f.operations.push_back({flags?7:6,&c==&f.battle,flags?12u:128u});if(flags)c.flags.fill(0);else c.work.fill(0);};return s;
    }
};
}
int main() {
    Fixture init;init.onAllocate=[&] {init.gp.timestamp+=0x100000001ULL;};initializeEventContexts(init.services());
    assert(init.allocations==2 && init.gpReads==2 && init.field.entries==init.fieldEntries.data() && init.battle.entries==init.battleEntries.data());
    assert(init.field.capacity==256 && init.battle.capacity==256 && init.field.previousTimestamp==0x123456799abcdef1 && init.battle.previousTimestamp==0x1234567a9abcdef2);
    const std::vector<Operation> expected{{2,false,0x1b000},{1,false,0},{3,false,0x9abcdef1},{4,false,0x12345679},{5,false,0x1b000},{6,false,128},{7,false,12},{2,true,0x1b000},{1,true,0},{3,true,0x9abcdef2},{4,true,0x1234567a},{5,true,0x1b000},{6,true,128},{7,true,12}};
    assert(init.operations==expected);
    for(auto* c:{&init.field,&init.battle}) {for(auto v:c->work)assert(v==0);for(auto v:c->flags)assert(v==0);for(int i=0;i<256;++i) {auto& e=c->entries[i];assert(e.flags==0 && e.id==0 && e.userWork==0 && e.locals[0]==0 && e.loopCounts[0]==0 && e.switchStates[0]==0 && e.credit==0);}}
    assert(init.runtime.allocationCount==0 && init.runtime.nextId==77 && init.runtime.speed==.25f && init.runtime.systemLevel==3);
    assert(init.schedule.count==0 && !init.schedule.managerActive && init.schedule.slots[0]==9 && init.schedule.ids[0]==10 && init.field.field94==46 && init.battle.field94==47);
    Fixture retained;retained.onGlobals=[&] {auto& c=retained.allocations==1?retained.field:retained.battle;c.entries=retained.replacement.data();c.capacity=1;};initializeEventContexts(retained.services());
    assert(retained.cleared==std::vector<EventEntry*>({retained.fieldEntries.data(),retained.battleEntries.data()}) && retained.field.entries==retained.replacement.data());
    assert(retained.fieldEntries[0].flags==0 && retained.fieldEntries[1].flags==0x11 && retained.replacement[0].flags==0x11 && retained.operations[4].value==0x1b0);
    Fixture failure;failure.failure=true;initializeEventContexts(failure.services());assert(failure.field.entries==nullptr && failure.battle.entries==nullptr && failure.cleared==std::vector<EventEntry*>({nullptr,nullptr}) && failure.operations[4].value==0x1b000);
    for(int32_t selector:{0,1,-1}) {
        Fixture reset;reset.gp.battleContext=selector;
        auto& selected=selector?reset.battle:reset.field;auto& untouched=selector?reset.field:reset.battle;
        const auto timestamp=untouched.previousTimestamp;auto* old=selected.entries;
        // Reset must bulk-clear without performing destructor copy-back into
        // another bank or allocating/freeing shared user buffers.
        selected.entries[1].waitingParent=&untouched.entries[0];untouched.entries[0].waitingChild=&selected.entries[1];selected.entries[1].locals[0]=1234;
        reinitializeEventContext(reset.services());
        assert(reset.allocations==0 && reset.gpReads==1 && selected.entries==old && selected.capacity==256 && selected.previousTimestamp==reset.gp.timestamp);
        assert(selected.entries[0].flags==0 && selected.entries[1].waitingParent==nullptr && untouched.entries[0].flags==0x11 && untouched.entries[0].locals[0]==100 && untouched.entries[0].waitingChild==old+1);
        assert(untouched.previousTimestamp==timestamp && reset.field.work[0]==42 && reset.battle.work[0]==43 && reset.field.flags[0]==44 && reset.battle.flags[0]==45);
        assert(reset.runtime.allocationCount==0 && reset.runtime.nextId==77 && reset.runtime.speed==.25f && reset.schedule.count==8 && !reset.schedule.managerActive && reset.schedule.slots[0]==9 && reset.schedule.ids[0]==10);
        assert(reset.field.field94==46 && reset.battle.field94==47 && reset.operations.size()==4);
    }
    Fixture capture;capture.gp.battleContext=0;
    capture.onTimestamp=[&](EventContext& c,bool high) {if(!high) {assert(c.previousTimestamp==0xaaaabbbb9abcdef0);capture.current=&capture.otherGp;}else {c.entries=capture.replacement.data();c.capacity=2;}};
    reinitializeEventContext(capture.services());assert(capture.gpReads==1 && capture.field.previousTimestamp==0x123456789abcdef0 && capture.battle.previousTimestamp==0x1111222233334444);
    assert(capture.cleared==std::vector<EventEntry*>({capture.replacement.data()}) && capture.replacement[0].flags==0 && capture.replacement[1].flags==0 && capture.replacement[2].flags==0x11 && capture.fieldEntries[0].flags==0x11);
    std::cout<<"event context initialization tests passed\n";
}
