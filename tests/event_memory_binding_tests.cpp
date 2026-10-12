#include "event_memory_binding.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_memory_release.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
struct Platform {uint32_t flushes=0,barriers=0,last=0;};
int main() {
    EventEntryAddresses entries;EventContextAddresses contexts(entries);
    EventRuntimeGlobals runtime{0,1,1,0};EventSchedule schedule;
    EventRuntimeAddresses globals(runtime,schedule);EventGpAddresses gp;
    EventAddressSpace memory(entries,contexts,globals,gp);Platform platform;
    EventMemoryBinding binding{memory,&platform,
        [](void* p,uint32_t a) {auto& state=*static_cast<Platform*>(p);++state.flushes;state.last=a;},
        [](void* p) {++static_cast<Platform*>(p)->barriers;}};
    auto services=binding.services();
    // Authored port-provider arena/address policy;original pool config is the
    // recovered static US table. This test does not claim original arena size.
    constexpr uint32_t base=0x82000000,span=12*1024*1024;
    assert(memory.registerRam(base,std::vector<uint8_t>(span,0xa5)));
    assert(memory.registerRam(0x804151b8,std::vector<uint8_t>(40)));
    assert(memory.registerRam(0x8041e808,std::vector<uint8_t>(0x50)));
    assert(memory.registerRam(0x8041e428,std::vector<uint8_t>(12)));
    assert(memory.registerRam(0x8041f070,std::vector<uint8_t>(20)));
    services.storeWord(services.user,0x8041e430,base);services.storeWord(services.user,0x8041f080,base+span);
    const uint32_t values[]{7300,3472,224,128,100};
    for(unsigned i=0;i<5;++i) {services.storeWord(services.user,0x804151b8+i*8,i==4?0:1);services.storeWord(services.user,0x804151bc+i*8,values[i]);}
    initializeEventMemoryPools(services);
    auto word=[&](uint32_t a) {return services.word(services.user,a);};
    assert(word(0x8041f070)==base && word(0x8041f078)==base+64 && word(0x8041e430)==base+span);
    uint32_t p=base+64;
    for(unsigned i=0;i<4;++i) {assert(word(0x8041e830+i*4)==p && word(0x8041e844+i*4)==i);p+=values[i]*1024;assert(word(0x8041e81c+i*4)==p);}
    assert(word(0x8041e840)==p && word(0x8041e82c)==base+span && word(0x8041e854)==4);
    const uint32_t primary=word(0x8041e808);assert(primary==word(0x8041e834)+32 && word(primary+4)==3472*1024-64);
    assert(platform.flushes==(3472*1024-32)/32 && platform.barriers==1);
    // Original context-sized SDK allocations use this SAME arena and descriptor
    // state,not an independent host allocator or the map allocator.
    const uint32_t first=eventHeapAllocate(0,0x1b000,services),second=eventHeapAllocate(0,0x1b000,services);
    assert(first==base+96 && second==first+0x1b020 && word(first)==0 && word(first+0x1affc)==0);
    assert(word(base+8)==second-32 && word(second-32+4)==first-32);
    services.storeWord(services.user,first,0x11223344);services.storeHalf(services.user,first+1,0xaabb);assert(word(first)==0x11aabb44 && services.half(services.user,first+1)==0xaabb);
    eventHeapFree(0,first,services);assert(eventHeapAllocate(0,0x1b000,services)==first && word(first)==0); // Reuse clears payload via wrapper.
    const uint32_t small=eventMapAllocate(primary,64,services);assert(small==primary+32);services.storeByte(services.user,small+1,0x91);assert(word(small)==0x00910000);eventMapFree(primary,small,services);
    allocateEventBattleMapPool(services);const uint32_t battle=word(0x8041e80c);assert(battle==primary+32 && word(battle+4)==0x64000-32);
    const uint32_t child=eventMapAllocate(battle,32,services);assert(child==battle+32);eventMapFree(battle,child,services);freeEventBattleMapPool(services);
    assert(word(0x8041e80c)==0 && word(primary)==0 && word(primary+4)==3472*1024-64);
    clearEventMemoryPool(0,services);assert(eventHeapAllocate(0,0x1b000,services)==first);
    std::cout<<"shared-storage event heap/bootstrap/battle-pool tests passed\n";
}
