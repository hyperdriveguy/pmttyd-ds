#include "event_entry_storage.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_memory_release.hpp"
#include "event_interpreter.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
struct Fixture {
    EventEntryAddresses entries;EventContextAddresses contexts{entries};
    EventRuntimeGlobals runtime{0,1,1,0};EventSchedule schedule;EventRuntimeAddresses globals{runtime,schedule};
    EventGlobals state{0,60,123};EventGpAddresses gp;EventContext field,battle;
    EventAddressSpace ram{entries,contexts,globals,gp};unsigned flushes=0,barriers=0;
    EventMemoryBinding binding{ram,this,[](void* p,uint32_t) {++static_cast<Fixture*>(p)->flushes;},[](void* p) {++static_cast<Fixture*>(p)->barriers;}};
    Fixture() {
        assert(contexts.registerContext(0x803d8d30,field) && contexts.registerContext(0x803d8dd0,battle));
        assert(gp.registerGp(0x81000000,state) && ram.writeWord(0x80415fd0,0x81000000));
        assert(ram.registerRam(0x82000000,std::vector<uint8_t>(1024*1024,0xa5)));
        assert(ram.registerRam(0x8041f070,std::vector<uint8_t>(16)) && ram.registerRam(0x8041e428,std::vector<uint8_t>(4)) && ram.registerRam(0x8041e844,std::vector<uint8_t>(4)));
        auto memory=binding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82100000,1,memory);
        assert(eventSdkCreateHeap(start,0x82100000,memory)==0 && ram.writeWord(0x8041e844,0));
    }
    uint32_t read(uint32_t a) {uint32_t v;assert(ram.readWord(a,v));return v;}
    EventManagerServices manager() {
        EventManagerServices s;s.user=this;s.globals=[](void* p) {return static_cast<Fixture*>(p)->gp.currentGlobals();};
        s.context=[](void* p,bool battle) {auto& f=*static_cast<Fixture*>(p);return battle?&f.battle:&f.field;};s.clock=[](void*)->uint32_t {return 243000000;};return s;
    }
};
int main() {
    Fixture f;uint32_t firstBase=0,secondBase=0;
    {
        EventEntryStorage storage(f.entries,f.contexts,f.binding);
        auto init=storage.initializationServices(f.manager(),f.field,f.battle,f.runtime,f.schedule);
        initializeEventContexts(init);firstBase=f.read(0x803d8dc0);secondBase=f.read(0x803d8e60);
        assert(firstBase==0x82000040 && secondBase==firstBase+0x1b020 && f.field.entries!=f.battle.entries);
        assert(f.field.capacity==256 && f.battle.capacity==256 && f.field.previousTimestamp==123 && f.battle.previousTimestamp==123);
        assert(f.flushes==2*(0x1b000/32) && f.barriers==2); // Duplicate Init clear adds no flush.
        assert(f.read(firstBase-24)==0x1b020 && f.read(secondBase-24)==0x1b020 && f.read(0x82000008)==secondBase-32); // SDK headers remain RAM.
        assert(f.ram.writeWord(firstBase+0x9c,0x12345678) && f.field.entries[0].locals[0]==0x12345678);
        f.field.entries[0].waitingChild=&f.battle.entries[1];f.battle.entries[1].waitingParent=&f.field.entries[0];
        f.field.work[0]=42;f.field.flags[0]=43;f.battle.entries[0].flags=1;f.battle.entries[0].id=99;
        f.runtime.allocationCount=2;f.schedule.count=17;f.schedule.managerActive=2;f.state.timestamp=456;
        reinitializeEventContext(init);
        assert(f.field.entries[0].locals[0]==0 && f.field.entries[0].waitingChild==nullptr && f.field.work[0]==42 && f.field.flags[0]==43);
        assert(f.battle.entries[0].id==99 && f.battle.entries[1].waitingParent==&f.field.entries[0]); // No semantic delete/copy-back.
        assert(f.field.previousTimestamp==456 && f.battle.previousTimestamp==123 && f.runtime.allocationCount==0 && f.schedule.count==17 && f.schedule.managerActive==0);
        assert(f.barriers==2);
        f.state.battleContext=-1;f.state.timestamp=789;reinitializeEventContext(init);
        assert(f.battle.entries[0].flags==0 && f.battle.entries[1].waitingParent==nullptr && f.battle.previousTimestamp==789);
        // Repeated Init allocates new arrays without freeing or resetting IDs.
        f.runtime.nextId=77;f.runtime.speed=.5f;initializeEventContexts(init);
        const auto newField=f.read(0x803d8dc0);assert(newField!=firstBase && f.entries.entryAt(firstBase) && f.runtime.nextId==77 && f.runtime.speed==.5f);
        // Real interpreter/constructor uses promoted arrays and shared counters.
        assert(f.ram.registerRam(0x80300000,{0,0,0,2,0,0,0,1,0,0,0,0}));
        EventCreationServices creation;creation.user=&f;creation.manager=f.manager();creation.runtime=[](void* p) {return &static_cast<Fixture*>(p)->runtime;};creation.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->read(a);};
        f.state.battleContext=0;auto& root=createRootEvent(f.schedule,0x80300000,10,0,creation);assert(root.id==77 && f.read(newField+0x15c)==77);
        root.timescale=1;f.state.timestamp+=eventClockRate(243000000);
        EventInterpreterServices interpreter;interpreter.creation=creation;interpreter.schedule=&f.schedule;interpreter.memory=f.binding.services();
        runEventInterpreterManager(interpreter);assert(root.flags==0 && f.runtime.allocationCount==0 && f.read(newField+8)==0);
        f.entries.entryAt(firstBase)->locals[0]=88;
        assert(storage.retire(firstBase) && !f.entries.entryAt(firstBase) && f.read(firstBase+0x9c)==88 && f.read(firstBase-24)==0x1b020);
        eventHeapFree(0,firstBase,f.binding.services());
        // Promotion snapshots AFTER cache callbacks;it cannot assume all bytes
        // are still zero after the wrapper's earlier memset.
        f.binding.flushBlock=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);++f.flushes;if(a==0x82000040)assert(f.ram.writeWord(a+0x9c,66));};
        auto* reused=storage.allocate(0,0x1b000);uint32_t reusedBase;
        assert(f.entries.addressOf(reused,reusedBase) && reusedBase==firstBase && reused[0].locals[0]==66);
        storage.clear(reused,0x1b000);assert(reused[0].locals[0]==0);
    }
    // Provider destruction leaves original bytes/pointer tokens;it never frees
    // SDK blocks or leaves dangling native context/relationship pointers.
    assert(f.field.entriesUnresolved && f.field.entries==nullptr && f.battle.entriesUnresolved && f.battle.entries==nullptr);
    uint32_t word;assert(f.ram.readWord(firstBase,word) && !f.entries.entryAt(firstBase));
    std::vector<uint8_t> untouched{9};assert(!f.ram.extractRam(0xffffffff,2,untouched) && untouched==std::vector<uint8_t>{9});
    assert(f.ram.registerRam(0x90000000,{1,2,3,4,5,6}));std::vector<uint8_t> cut;assert(f.ram.extractRam(0x90000002,2,cut) && cut==std::vector<uint8_t>({3,4}));
    uint8_t v;assert(f.ram.readByte(0x90000000,v) && v==1 && f.ram.readByte(0x90000005,v) && v==6 && !f.ram.readByte(0x90000002,v));assert(f.ram.registerRam(0x90000002,std::move(cut)) && f.read(0x90000001)==0x02030405);
    std::cout<<"native event entry allocation/init/reset/retirement tests passed\n";
}
