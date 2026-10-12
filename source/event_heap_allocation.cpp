#include "event_heap_allocation.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t read(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void write(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
}
uint32_t eventSdkAllocate(int32_t handle,uint32_t bytes,const EventMemoryServices& s) {
    const uint32_t d=read(s,0x8041f070)+12u*static_cast<uint32_t>(handle);
    const uint32_t span=(bytes+63)&0xffffffe0u;uint32_t n=read(s,d+4);
    while(n!=0) {if(signedWord(span)<=signedWord(read(s,n+8)))break;n=read(s,n+4);}
    if(n==0)return 0;
    const uint32_t remainder=read(s,n+8)-span;
    if(remainder<64) {
        const uint32_t next=read(s,n+4);uint32_t head=read(s,d+4);
        if(next!=0)write(s,next,read(s,n));
        const uint32_t previous=read(s,n);
        if(previous==0)head=read(s,n+4);else write(s,previous+4,read(s,n+4));
        write(s,d+4,head);
    } else {
        write(s,n+8,span);const uint32_t j=n+span;write(s,j+8,remainder);
        write(s,j,read(s,n));write(s,j+4,read(s,n+4));
        const uint32_t next=read(s,j+4);if(next!=0)write(s,next,j);
        const uint32_t previous=read(s,j);if(previous!=0)write(s,previous+4,j);else write(s,d+4,j);
    }
    const uint32_t allocated=read(s,d+8);write(s,n+4,allocated);write(s,n,0);
    if(allocated!=0)write(s,allocated,n);
    write(s,d+8,n);return n+32;
}
uint32_t eventHeapAllocate(int32_t heapIndex,uint32_t bytes,const EventMemoryServices& s) {
    const int32_t handle=signedWord(read(s,0x8041e844+4u*static_cast<uint32_t>(heapIndex)));
    const uint32_t address=eventSdkAllocate(handle,bytes,s);
    if(address!=0) {eventFillMemory(address,0,bytes,s);eventFlushMemory(address,bytes,s);}
    return address;
}
}
