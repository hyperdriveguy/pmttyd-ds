#include "key_item_inventory.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t signedHalf(uint16_t value){return value<0x8000?int32_t(value):int32_t(value)-0x10000;}
int32_t operand(const ItemInventoryEventServices& s,uint32_t at) {
    const uint32_t value=s.operands.word(s.operands.user,at);
    return value<=0x7fffffff?int32_t(value):int32_t(int64_t(value)-0x100000000LL);
}
}
// Capture argument base, resolve item, count, then freshly fetch raw output.
// Count/provider callbacks may affect later operands; do not prefetch them.
int32_t checkItemEvent(EventEntry& e,const ItemInventoryEventServices& s) {
    const auto& o=s.operands;assert(o.word && o.resolve && o.store && s.count);
    const uint32_t arguments=e.arguments;
    const int32_t item=o.resolve(o.user,e,operand(s,arguments));
    const int32_t count=s.count(s.user,item);
    o.store(o.user,e,operand(s,arguments+4),count);return 2;
}
// Only low-level1 triggers Mario THEN party revision and output0; other
// results output-1. Read output destination after those source side effects.
int32_t removeItemEvent(EventEntry& e,const ItemInventoryEventServices& s) {
    const auto& o=s.operands;assert(o.word && o.resolve && o.store && s.remove && s.reviseMario && s.reviseParty);
    const uint32_t arguments=e.arguments;
    const int32_t item=o.resolve(o.user,e,operand(s,arguments));
    int32_t result=-1;
    if(s.remove(s.user,item)==1){s.reviseMario(s.user);s.reviseParty(s.user);result=0;}
    o.store(o.user,e,operand(s,arguments+4),result);return 2;
}
int32_t packedKeyItemCount(uint32_t pouch,int32_t item,const EventMemoryServices& m) {
    assert(item>=1 && item<=120 && m.half);int32_t count=0;
    for(unsigned i=0;i<121;++i)if(signedHalf(m.half(m.user,pouch+0xa0+2*i))==item)++count;
    return count;
}
// Remove first matching slot only. Each left-shift store is followed by
// clearing its source slot; do not collapse observable callbacks into memcpy.
int32_t removePackedKeyItem(uint32_t pouch,int32_t item,const EventMemoryServices& m) {
    assert(item>=1 && item<=120 && m.half && m.storeHalf);
    for(unsigned i=0;i<121;++i)if(signedHalf(m.half(m.user,pouch+0xa0+2*i))==item) {
        m.storeHalf(m.user,pouch+0xa0+2*i,0);
        for(unsigned j=i;j<120;++j) {
            m.storeHalf(m.user,pouch+0xa0+2*j,m.half(m.user,pouch+0xa0+2*(j+1)));
            m.storeHalf(m.user,pouch+0xa0+2*(j+1),0);
        }
        return 1;
    }
    return 0;
}
// Search backwards from120 for HIGHEST zero, then shift that prefix back
// to front. It does not choose the first gap or compact/sort the whole array.
int32_t insertPackedKeyItem(uint32_t pouch,int32_t item,const EventMemoryServices& m) {
    assert(item>=1 && item<=120 && m.half && m.storeHalf);
    int slot=120;while(slot>=0 && m.half(m.user,pouch+0xa0+2*unsigned(slot))!=0)--slot;
    if(slot<0)return 0;
    for(;slot>0;--slot)m.storeHalf(m.user,pouch+0xa0+2*unsigned(slot),m.half(m.user,pouch+0xa0+2*unsigned(slot-1)));
    m.storeHalf(m.user,pouch+0xa0,uint16_t(item));return 1;
}
}
