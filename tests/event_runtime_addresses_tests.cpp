#include "event_runtime_addresses.hpp"
#include "event_system_level.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    EventRuntimeGlobals globals;EventSchedule schedule;EventRuntimeAddresses map(globals,schedule);uint32_t word=123;uint8_t byte=45;
    assert(!map.readWord(0x80415fd0,word) && word==123 && !map.readByte(0,byte) && byte==45);
    assert(map.writeWord(0x804151e8,0xffffffff) && globals.nextId==0xffffffff);
    assert(map.writeWord(0x804151ec,0x3f000000) && globals.speed==.5f);assert(map.writeWord(0x804151ec,0x7fa12345) && map.readWord(0x804151ec,word) && word==0x7fa12345);
    assert(map.writeWord(0x8041e940,3) && globals.systemLevel==3);assert(map.writeByte(0x8041e941,0xaa) && map.readWord(0x8041e940,word) && word==0x00aa0003);
    assert(map.writeWord(0x8041e858,2) && schedule.managerActive==2 && map.readWord(0x8041e858,word) && word==2); // Preserve full original word,not bool-normalized1.
    assert(map.writeByte(0x8041e858,0xff) && schedule.managerActive==0xff000002);
    assert(map.writeWord(0x8041e85c,0xfffffffe) && schedule.count==-2 && map.writeWord(0x8041e860,17) && globals.allocationCount==17);
    assert(map.writeWord(0x803d8530,0x12345678) && schedule.ids[0]==0x12345678 && map.writeByte(0x803d8531,0xaa) && schedule.ids[0]==0x12aa5678);
    schedule.ids[255]=0x89abcdef;schedule.slots[0]=-7;assert(map.readWord(0x803d892c,word) && word==0x89abcdef && map.readWord(0x803d8930,word) && word==uint32_t(-7));
    assert(map.writeWord(0x803d892f,0x11223344) && map.readWord(0x803d892f,word) && word==0x11223344 && schedule.ids[255]==0x89abcd11 && uint32_t(schedule.slots[0])==0x223344f9);
    assert(!map.writeWord(0x803d8d2f,0xffffffff)); // Context begins next;dispatcher owner composes regions.
    for(unsigned i=0;i<256;++i) {assert(map.writeWord(0x803d8530+4*i,i) && map.writeWord(0x803d8930+4*i,uint32_t(-int32_t(i))));assert(schedule.ids[i]==i && schedule.slots[i]==-int32_t(i));}
    EventMemoryServices memory;memory.user=&map;memory.word=[](void* p,uint32_t a) {uint32_t v;assert(static_cast<EventRuntimeAddresses*>(p)->readWord(a,v));return v;};
    globals.systemLevel=4;assert(getRequestedEventSystemLevel(memory)==4); // Actual requested source shares runtime object.
    std::cout<<"event runtime address tests passed\n";
}
