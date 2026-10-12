#include "event_context_addresses.hpp"
#include "event_operands.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    constexpr uint32_t base=0x803d8d30;std::array<EventEntry,3> array{};EventEntryAddresses entries;assert(entries.registerEntries(0x81000000,array.data(),3));
    EventContext field{3,array.data(),0},battle;EventContextAddresses map(entries);assert(map.registerContext(base,field) && map.registerContext(base+0xa0,battle) && !map.registerContext(base,field));
    uint32_t w;uint8_t byte;assert(map.readWord(base+0x90,w) && w==0x81000000);
    assert(map.writeWord(base+4,0x12345678) && map.writeByte(base+5,0xaa) && uint32_t(field.work[0])==0x12aa5678 && map.readWord(base+4,w) && w==0x12aa5678);
    assert(map.writeWord(base,0xfffffffe) && field.capacity==-2);assert(map.writeWord(base+0x94,0xabcdef01) && field.field94==0xabcdef01);
    assert(map.writeWord(base+0x98,0x12345678) && map.writeWord(base+0x9c,0x9abcdef0) && field.previousTimestamp==0x123456789abcdef0);
    field.previousTimestamp=0xfedcba9876543210;assert(map.readWord(base+0x98,w) && w==0xfedcba98 && map.readWord(base+0x9c,w) && w==0x76543210);
    assert(map.writeWord(base+0x90,0x810001b0) && field.entries==&array[1]); // Array word can select an interior mapped entry.
    map.detachEntryArrays(0x81000000,3);assert(field.entries==nullptr && field.entriesUnresolved && map.readWord(base+0x90,w) && w==0x810001b0);
    std::vector<uint8_t> saved;assert(entries.unregisterEntries(0x81000000,&saved));std::array<EventEntry,3> replacement{};assert(entries.registerEntries(0x81000000,replacement.data(),3));map.resolveEntryArrays();assert(field.entries==&replacement[1] && !field.entriesUnresolved);
    // Complete160-byte context partition,pointer words included,retains raw
    // values while unknown array addresses wait for a mapping.
    for(uint32_t i=0;i<0xa0;++i)assert(map.writeByte(base+0xa0+i,uint8_t(i*19+3)));
    for(uint32_t i=0;i<0xa0;++i)assert(map.readByte(base+0xa0+i,byte) && byte==uint8_t(i*19+3));
    assert(battle.entriesUnresolved && battle.entries==nullptr);
    assert(map.writeWord(base+0xa0+0x90,0x81000000) && battle.entries==replacement.data() && !battle.entriesUnresolved);
    // Actual global namespaces cross neighboring context fields:GW[-1] is
    // capacity;GF bit96 is the entry-array pointer's bit0,not an isolated flag.
    EventOperandServices s;s.user=&map;s.contextAddress=[](void*)->uint32_t {return base;};
    s.readWord=[](void* p,uint32_t a) {uint32_t v;assert(static_cast<EventContextAddresses*>(p)->readWord(a,v));return v;};s.writeWord=[](void* p,uint32_t a,uint32_t v) {assert(static_cast<EventContextAddresses*>(p)->writeWord(a,v));};
    setEventValue(replacement[0],-50000001,7,s);assert(field.capacity==7);
    setEventValue(replacement[0],-90000000+96,1,s);assert(field.entriesUnresolved && field.entries==nullptr && map.readWord(base+0x90,w) && w==0x810001b1);
    assert(map.writeByte(base+0x93,0xb0) && field.entries==&replacement[1] && !field.entriesUnresolved);
    assert(map.writeWord(base+0x9f,0x11223344) && map.readWord(base+0x9f,w) && w==0x11223344 && uint32_t(battle.capacity)>>8==0x223344);
    assert(!map.writeWord(base+0x13f,0xffffffff) && !map.readByte(base-1,byte));
    std::cout<<"event context address tests passed\n";
}
