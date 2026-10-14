#include "event_memory_opcodes.hpp"
#include <cassert>
#include <cstring>

// Memory opcodes74/75 install UW/UF,76 allocates UW words;77..80 AND/OR
// (resolved/raw masks),81/82 ms<->60Hz conversion;83..86 absolute
// integer/IEEE-float access;87..90 indirect variable access. Handler2 completes.
// Absolute addresses are emulated tokens; they must be mapped by services.
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t word(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void write(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
void half(const EventMemoryServices& s,uint32_t a,uint16_t v) {assert(s.storeHalf);s.storeHalf(s.user,a,v);}
void byte(const EventMemoryServices& s,uint32_t a,uint8_t v) {assert(s.storeByte);s.storeByte(s.user,a,v);}
int32_t raw(const EventDispatchServices& s,uint32_t a) {assert(s.word);return signedWord(s.word(s.user,a));}
int32_t integer(const EventDispatchServices& s,EventEntry& e,int32_t op) {assert(s.resolve);return s.resolve(s.user,e,op);}
float floating(const EventDispatchServices& s,EventEntry& e,int32_t op) {assert(s.resolveFloat);return s.resolveFloat(s.user,e,op);}
void store(const EventDispatchServices& s,EventEntry& e,int32_t d,int32_t v) {assert(s.store);s.store(s.user,e,d,v);}
void storeFloat(const EventDispatchServices& s,EventEntry& e,int32_t d,float v) {assert(s.storeFloat);s.storeFloat(s.user,e,d,v);}
float ieee(uint32_t v) {float f;static_assert(sizeof(f)==sizeof(v),"binary32 required");std::memcpy(&f,&v,sizeof(f));return f;}
uint32_t ieee(float f) {uint32_t v;std::memcpy(&v,&f,sizeof(v));return v;}
}
// Source memset: replicated low8 fill byte (0x01010101), align to4, then
// write32-byte chunks as8 words. Preserve provider store sizes and order.
uint32_t eventFillMemory(uint32_t address,int32_t fill,uint32_t n,const EventMemoryServices& s) {
    const uint32_t original=address;const uint8_t value=static_cast<uint8_t>(fill);
    if(n>=32) {
        const unsigned leading=(0u-address)&3;for(unsigned i=0;i<leading;++i) {byte(s,address++,value);--n;}
        const uint32_t pattern=uint32_t(value)*0x01010101u;
        while(n>=32) {for(unsigned i=0;i<8;++i) {write(s,address,pattern);address+=4;}n-=32;}
        while(n>=4) {write(s,address,pattern);address+=4;n-=4;}
    }
    while(n!=0) {byte(s,address++,value);--n;}
    return original;
}
// Source cache line32 bytes. Include leading misalignment via address&31;
// +31/>>5 rounds up to lines. Wrapped arithmetic is intentional.
void eventFlushMemory(uint32_t address,uint32_t length,const EventMemoryServices& s) {
    if(length==0)return;
    uint32_t count=(length+(address&31)+31)>>5;assert(s.flushBlock && s.systemCall);
    // Original enters its CTR loop even if the wrapped count is zero.
    do {s.flushBlock(s.user,address);address+=32;--count;}while(count!=0);
    s.systemCall(s.user);
}
// Map allocator:32-byte alignment/header, header+0=next,+4=size,+8=used
// halfword. Best-fit scan, then split and clear/flush payload. This is distinct
// from the SDK doubly-linked heap format in event_heap_allocation.cpp.
uint32_t eventMapAllocate(uint32_t root,uint32_t bytes,const EventMemoryServices& s) {
    const uint32_t payload=(bytes+31)&0xffffffe0u;if(payload==0)return 0;
    const uint32_t span=payload+32;uint32_t best=0,bestSize=0,bestNext=0,h=root;
    assert(s.half);
    for(;;) {
        if(s.half(s.user,h+8)==0) {
            const uint32_t size=word(s,h+4);
            if(size>=span && (bestSize==0 || bestSize>size)) {best=h;bestSize=size;bestNext=word(s,h);}
        }
        h=word(s,h);if(h==0)break;
    }
    if(bestSize==0)return 0;
    write(s,best,best+span);const uint32_t address=best+32,remainder=bestSize-span;
    write(s,best+4,payload);half(s,best+8,1);
    const uint32_t next=word(s,best);write(s,next,bestNext);write(s,next+4,remainder);half(s,next+8,0);
    eventFillMemory(address,0,payload,s);eventFlushMemory(address,payload,s);return address;
}
std::optional<int32_t> eventMemoryOpcode(EventEntry& e,uint8_t op,const EventMemoryOpcodeServices& services) {
    if(op<74 || op>90)return {};
    const auto& s=services.values;const auto& m=services.memory;const uint32_t args=e.arguments;
    if(op==74 || op==75) {const uint32_t value=static_cast<uint32_t>(integer(s,e,raw(s,args)));if(op==74)e.userWork=value;else e.userFlags=value;return 2;}
    // 0x8041e808 holds primary map-pool root; requested count is4-byte UW words.
    if(op==76) {
        const int32_t count=integer(s,e,raw(s,args)),d=raw(s,args+4);const uint32_t root=word(m,0x8041e808);
        e.userWork=eventMapAllocate(root,4u*static_cast<uint32_t>(count),m);store(s,e,d,signedWord(e.userWork));return 2;
    }
    if(op<=82) {
        const int32_t d=raw(s,args);int32_t rhs=raw(s,args+4);
        if(op!=78 && op!=80)rhs=integer(s,e,rhs);
        if(op<=80) {
            const uint32_t lhs=static_cast<uint32_t>(integer(s,e,d)),r=static_cast<uint32_t>(rhs);
            store(s,e,d,signedWord(op<=78?lhs&r:lhs|r));
        } else {
            // Conversion constants60 ticks/sec and1000 ms/sec. Wrapped32-bit
            // product precedes signed truncating division; no saturation.
            const int32_t product=signedWord(static_cast<uint32_t>(rhs)*(op==81?60u:1000u));
            store(s,e,d,product/(op==81?1000:60));
        }
        return 2;
    }
    if(op==83 || op==84) {
        if(op==83) {const int32_t value=integer(s,e,raw(s,args));const uint32_t address=static_cast<uint32_t>(raw(s,args+4));write(m,address,static_cast<uint32_t>(value));}
        else {const float value=floating(s,e,raw(s,args));const uint32_t address=static_cast<uint32_t>(raw(s,args+4));write(m,address,ieee(value));}
    } else if(op==85 || op==86) {
        const uint32_t address=static_cast<uint32_t>(raw(s,args+4));const int32_t d=raw(s,args);const uint32_t value=word(m,address);
        if(op==85)store(s,e,d,signedWord(value));else storeFloat(s,e,d,ieee(value));
    } else if(op==87 || op==88) {
        const int32_t d=integer(s,e,raw(s,args));
        if(op==87)store(s,e,d,integer(s,e,raw(s,args+4)));else storeFloat(s,e,d,floating(s,e,raw(s,args+4)));
    } else {
        const int32_t q=integer(s,e,raw(s,args));
        if(op==89) {const int32_t value=integer(s,e,q),d=raw(s,args+4);store(s,e,d,value);}
        else {const float value=floating(s,e,q);const int32_t d=raw(s,args+4);storeFloat(s,e,d,value);}
    }
    return 2;
}
}
