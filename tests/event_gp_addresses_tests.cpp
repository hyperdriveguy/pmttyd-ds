#include "event_gp_addresses.hpp"
#include "event_time_publication.hpp"
#include "event_operands.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    EventGlobals first{0,60,0},second{1,50,123};EventGpAddresses map;constexpr uint32_t base=0x81000000,other=0x82000000;
    assert(map.registerGp(base,first) && map.registerGp(other,second) && !map.registerGp(base,first) && !map.registerGp(0x80415fd0,first));
    assert(map.writeWord(0x80415fd0,base) && map.currentGlobals()==&first);assert(map.writeWord(base+0x14,0xffffffff) && first.battleContext==-1);
    assert(map.writeByte(base+7,50) && first.refreshRate==50);first.timestamp=0x123456789abcdef0;uint32_t value;assert(map.readWord(base+0x38,value) && value==0x12345678 && map.readWord(base+0x3c,value) && value==0x9abcdef0);
    assert(map.writeWord(base+0x3c,0x11223344) && first.timestamp==0x1234567811223344);
    // GP backing preserves all other bytes rather than guessing meanings for
    // pouch/window/input/clock banks. The three semantic views remain coherent.
    for(uint32_t i=0;i<EventGpAddresses::size;++i)assert(map.writeByte(base+i,uint8_t(i*7+1)));
    for(uint32_t i=0;i<EventGpAddresses::size;++i) {uint8_t v;assert(map.readByte(base+i,v) && v==uint8_t(i*7+1));}
    EventOperandServices operands;operands.user=&map;operands.globalAddress=[](void* p) {return static_cast<EventGpAddresses*>(p)->currentAddress();};
    operands.readWord=[](void* p,uint32_t a) {uint32_t v;assert(static_cast<EventGpAddresses*>(p)->readWord(a,v));return v;};operands.writeWord=[](void* p,uint32_t a,uint32_t v) {assert(static_cast<EventGpAddresses*>(p)->writeWord(a,v));};
    operands.readByte=[](void* p,uint32_t a) {uint8_t v;assert(static_cast<EventGpAddresses*>(p)->readByte(a,v));return v;};operands.writeByte=[](void* p,uint32_t a,uint8_t v) {assert(static_cast<EventGpAddresses*>(p)->writeByte(a,v));};
    writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,0x12345678,operands);assert(map.readWord(base+0x174,value) && value==0x12345678);
    writeSavedSwitch(SavedSwitchFamily::GlobalByte,1,0xabcd,operands);assert(readSavedSwitch(SavedSwitchFamily::GlobalByte,1,operands)==0xcd);
    assert(map.writeWord(0x80415fd0,other) && map.currentGlobals()==&second);writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,99,operands);assert(map.readWord(other+0x174,value) && value==99 && map.readWord(base+0x174,value) && value==0x12345678);
    std::vector<uint8_t> bytes;assert(map.unregisterGp(other,&bytes) && bytes.size()==EventGpAddresses::size && bytes[0x177]==99 && map.currentAddress()==other && map.currentGlobals()==nullptr);
    EventGlobals replacement;assert(map.registerGp(other,replacement) && map.currentGlobals()==&replacement);
    // Current root may contain an unregistered address;it is preserved as a
    // word and cannot silently select another registered GP object.
    assert(map.writeByte(0x80415fd3,1) && map.currentAddress()==other+1 && map.currentGlobals()==nullptr);
    std::cout<<"event GP address tests passed\n";
}
