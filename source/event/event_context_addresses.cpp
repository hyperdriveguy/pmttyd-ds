#include "event_context_addresses.hpp"

// Packed context is0xa0 bytes:0=capacity,4..83=32 work words,84..8f=3
// flag words,90=entry-array token,94=raw field,98/9c=timestamp high/low.
// Big-endian lane shifts24-8*(offset&3) keep byte aliases live.2^32 bounds
// reject address wrap; unregistering entry arrays preserves unresolved tokens.
namespace ttyd {
bool EventContextAddresses::mayMapRange(uint32_t base,uint64_t size) const {
    if(!size)return false;
    for(const auto& r:regions_)if(uint64_t(base)<uint64_t(r.base)+0xa0 && uint64_t(r.base)<uint64_t(base)+size)return true;
    return false;
}
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
}
EventContextAddresses::Position EventContextAddresses::find(uint32_t a) const {
    for(const auto& r:regions_)if(a>=r.base && uint64_t(a)<uint64_t(r.base)+0xa0)return {r.context,a-r.base};
    return {};
}
bool EventContextAddresses::registerContext(uint32_t base,EventContext& c) {
    if(!base || uint64_t(base)+0xa0>0x100000000ULL)return false;
    for(const auto& r:regions_)if(r.context==&c || (uint64_t(base)<uint64_t(r.base)+0xa0 && uint64_t(r.base)<uint64_t(base)+0xa0))return false;
    regions_.push_back({base,&c});return true;
}
bool EventContextAddresses::readField(EventContext& c,uint32_t off,uint32_t& v) const {
    if(off==0)v=static_cast<uint32_t>(c.capacity);
    else if(off<0x84)v=static_cast<uint32_t>(c.work[(off-4)/4]);
    else if(off<0x90)v=c.flags[(off-0x84)/4];
    else if(off==0x90) {if(c.entries)return entries_.addressOf(c.entries,v);v=c.entriesUnresolved?c.unresolvedEntriesWord:0;}
    else if(off==0x94)v=c.field94;
    else if(off==0x98)v=static_cast<uint32_t>(c.previousTimestamp>>32);
    else if(off==0x9c)v=static_cast<uint32_t>(c.previousTimestamp);
    else return false;
    return true;
}
bool EventContextAddresses::writeField(EventContext& c,uint32_t off,uint32_t v) {
    if(off==0)c.capacity=sw(v);
    else if(off<0x84)c.work[(off-4)/4]=sw(v);
    else if(off<0x90)c.flags[(off-0x84)/4]=v;
    else if(off==0x90) {c.entries=entries_.entryAt(v);c.unresolvedEntriesWord=v;c.entriesUnresolved=v!=0 && !c.entries;}
    else if(off==0x94)c.field94=v;
    else if(off==0x98)c.previousTimestamp=(uint64_t(v)<<32)|(c.previousTimestamp&0xffffffffu);
    else if(off==0x9c)c.previousTimestamp=(c.previousTimestamp&0xffffffff00000000ULL)|v;
    else return false;
    return true;
}
bool EventContextAddresses::readByte(uint32_t a,uint8_t& v) const {
    auto p=find(a);if(!p.context)return false;uint32_t word;if(!readField(*p.context,p.offset&~3u,word))return false;v=uint8_t(word>>(24-8*(p.offset&3)));return true;
}
bool EventContextAddresses::writeByte(uint32_t a,uint8_t v) {
    auto p=find(a);if(!p.context)return false;uint32_t word;if(!readField(*p.context,p.offset&~3u,word))return false;const unsigned shift=24-8*(p.offset&3);
    return writeField(*p.context,p.offset&~3u,(word&~(0xffu<<shift))|(uint32_t(v)<<shift));
}
bool EventContextAddresses::readWord(uint32_t a,uint32_t& v) const {
    if(uint64_t(a)+4>0x100000000ULL)return false;
    auto p=find(a);if(p.context && (p.offset&3)==0)return readField(*p.context,p.offset,v);
    uint32_t word=0;for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(a+i,b))return false;word=word<<8|b;}v=word;return true;
}
bool EventContextAddresses::writeWord(uint32_t a,uint32_t v) {
    if(uint64_t(a)+4>0x100000000ULL)return false;
    auto p=find(a);if(p.context && (p.offset&3)==0)return writeField(*p.context,p.offset,v);
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(a+i,b))return false;}
    for(unsigned i=0;i<4;++i)if(!writeByte(a+i,uint8_t(v>>(24-8*i))))return false;
    return true;
}
void EventContextAddresses::resolveEntryArrays() {
    for(const auto& r:regions_)if(r.context->entriesUnresolved) {
        if(r.context->entries) {uint32_t address;if(entries_.addressOf(r.context->entries,address))r.context->entriesUnresolved=false;}
        else if(auto* p=entries_.entryAt(r.context->unresolvedEntriesWord)) {r.context->entries=p;r.context->entriesUnresolved=false;}
    }
}
void EventContextAddresses::detachEntryArrays(uint32_t base,uint32_t count) {
    const uint64_t end=uint64_t(base)+uint64_t(count)*EventEntryAddresses::stride;
    for(const auto& r:regions_)if(r.context->entries) {uint32_t address;if(entries_.addressOf(r.context->entries,address) && address>=base && uint64_t(address)<end) {
        r.context->unresolvedEntriesWord=address;r.context->entriesUnresolved=true;r.context->entries=nullptr;
    }}
}
}
