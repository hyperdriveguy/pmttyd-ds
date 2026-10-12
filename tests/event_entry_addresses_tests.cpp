#include "event_entry_addresses.hpp"
#include "event_operands.hpp"
#include "event_lifecycle.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main() {
    std::array<EventEntry,3> entries{};EventEntryAddresses map;constexpr uint32_t base=0x80010000;
    assert(map.registerEntries(base,entries.data(),3));assert(!map.registerEntries(base,entries.data(),3));assert(!map.registerEntries(0,entries.data(),3));
    auto& e=entries[0];uint32_t w=77;uint8_t byte=88;assert(!map.readWord(base-1,w) && w==77 && !map.readByte(base-1,byte));
    // Every432-byte lane,including opaque bytes,float bit patterns and unmapped
    // pointer words,round trips without using the host struct layout.
    for(uint32_t i=0;i<0x1b0;++i)assert(map.writeByte(base+i,uint8_t(i*37+11)));
    for(uint32_t i=0;i<0x1b0;++i) {assert(map.readByte(base+i,byte));assert(byte==uint8_t(i*37+11));}
    assert(!map.linksResolved(e));assert(map.writeWord(base+0x6c,0) && map.writeWord(base+0x70,0) && map.writeWord(base+0x74,0) && map.linksResolved(e));
    e={};e.flags=0x11;e.argumentCount=0x22;e.opcode=0x33;e.priority=0x44;e.type=0x55;e.initialized=0x66;e.loopIndex=0x77;e.switchIndex=0x88;
    assert(map.readWord(base+8,w) && w==0x11223344 && map.readWord(base+12,w) && w==0x55667788);
    e.lifetime=0x123456789abcdef0;e.waitStart=0xfedcba9876543210;e.waitCount=-7;e.owner=-2;e.id=0x11223344;e.currentCommand=0x80355cf0;e.restartScript=0x8034f2e4;
    assert(map.readWord(base,w) && w==0x12345678 && map.readWord(base+4,w) && w==0x9abcdef0);
    assert(map.readWord(base+0x78,w) && w==uint32_t(-7) && map.readWord(base+0x7c,w) && w==0xfedcba98 && map.readWord(base+0x80,w) && w==0x76543210);
    assert(map.readWord(base+0x160,w) && w==uint32_t(-2) && map.readWord(base+0x15c,w) && w==0x11223344 && map.readWord(base+0x1a8,w) && w==0x80355cf0 && map.readWord(base+0x1a0,w) && w==0x8034f2e4);
    assert(map.writeWord(base+0x9c,0x12345678));assert(map.writeByte(base+0x9d,0xaa));assert(uint32_t(e.locals[0])==0x12aa5678 && map.readWord(base+0x9c,w) && w==0x12aa5678);
    e.locals[0]=-1;assert(map.readWord(base+0x9c,w) && w==0xffffffff);assert(map.writeWord(base+0x164,0x7fa12345));assert(map.readWord(base+0x164,w) && w==0x7fa12345); // Preserve IEEE payload without arithmetic.
    const uint32_t b=base+0x1b0;assert(map.writeWord(base+0x6c,b));assert(e.waitingParent==&entries[1] && map.linksResolved(e));entries[1].id=123;assert(map.readWord(base+0x6c,w) && w==b);
    e.waitingChild=&entries[2];assert(map.readWord(base+0x70,w) && w==base+2*0x1b0);
    assert(map.writeWord(base+0x6c,0x90000000) && map.readWord(base+0x6c,w) && w==0x90000000 && !map.linksResolved(e));
    EventEntry later;assert(map.registerEntries(0x90000000,&later,1) && e.waitingParent==&later && map.linksResolved(e));
    struct Alias {uint32_t offset;};
    for(uint32_t offset:{0x98u,0xdcu,0xe8u,0xe4u,0x104u,0x108u,0x128u,0x12cu,0x130u,0x150u,0x2cu})assert(map.writeWord(base+offset,0x12345678) && map.readWord(base+offset,w) && w==0x12345678);
    assert(e.cachedFunction==0x12345678 && e.localFlags[0]==0x12345678 && e.loopBodies[0]==0x12345678 && e.loopCounts[0]==0x12345678 && e.switchStates[0]==0x12 && e.switchValues[0]==0x12345678 && e.field150==0x12345678 && e.labelAddresses[0]==0x12345678);
    // Real namespaces cross physical field boundaries;indices are not clamped
    // to their nominal semantic arrays by the address provider.
    EventOperandServices s;s.user=&map;s.contextAddress=[](void*)->uint32_t {return 0;};s.entryAddress=[](void*,EventEntry&)->uint32_t {return base;};
    s.readWord=[](void* p,uint32_t a) {uint32_t v;assert(static_cast<EventEntryAddresses*>(p)->readWord(a,v));return v;};s.writeWord=[](void* p,uint32_t a,uint32_t v) {assert(static_cast<EventEntryAddresses*>(p)->writeWord(a,v));};
    assert(getEventValue(e,-30000001,s)==0x12345678);setEventValue(e,-30000000+16,7,s);assert(e.localFlags[0]==7);
    e.loopBodies[0]=0;setEventValue(e,-70000000+96,1,s);assert(e.loopBodies[0]==1);
    e.userWork=base+0x150;setEventValue(e,-190000000,99,s);assert(e.field150==99); // Shared user address aliases cursor.
    assert(map.writeWord(base+0x178,1) && map.writeWord(base+0x17c,2) && e.messageFields[0]==1 && e.messageFields[1]==2);
    assert(map.writeWord(base+0x184,0x3fc00000) && e.messageFields[3]==0x3fc00000);
    // Word access across adjacent entries uses original stride432,not sizeof.
    assert(map.writeWord(b-2,0x12345678) && map.readWord(b-2,w) && w==0x12345678 && entries[1].lifetime==0x5678000000000000ULL);
    assert(!map.writeWord(base+3*0x1b0-1,0xffffffff));
    EventEntryAddresses edge;EventEntry final;assert(edge.registerEntries(0xfffffe50,&final,1));assert(edge.writeWord(0xfffffffc,0xdeadbeef) && edge.readWord(0xfffffffc,w) && w==0xdeadbeef && !edge.readWord(0xfffffffd,w));
    // Restart preserves opaque bytes;deletion/slot reuse clear them and retain
    // the original address mapping independently of the new event ID.
    e={};e.flags=1;e.id=1;e.restartScript=0x1000;e.rawBytes.fill(0xaa);EventGlobals gp;EventContext context{3,entries.data(),0};EventRuntimeGlobals runtime{1,9,1,0};
    EventCreationServices creation;creation.manager.user=&context;creation.manager.globals=[](void*)->EventGlobals* {static EventGlobals g;return &g;};creation.manager.context=[](void* p,bool) {return static_cast<EventContext*>(p);};creation.user=&runtime;creation.runtime=[](void* p) {return static_cast<EventRuntimeGlobals*>(p);};creation.word=[](void*,uint32_t a) {return a==0x1000?1u:0u;};
    restartEvent(e,creation);for(auto v:e.rawBytes)assert(v==0xaa);deleteEvent(e,creation);for(auto v:e.rawBytes)assert(v==0);
    e.id=55;e.flags=1;uint32_t address;assert(map.addressOf(&e,address) && address==base && map.entryAt(base)==&e && map.entryAt(base+1)==nullptr);
    std::cout<<"event entry address tests passed\n";
}
