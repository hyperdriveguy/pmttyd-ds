#include "event_memory_bootstrap.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t read(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void write(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
void primaryMap(const EventMemoryServices& s) {
    const uint32_t start=read(s,0x8041e834),end=read(s,0x8041e820);
    const int32_t handle=sw(read(s,0x8041e848));const uint32_t length=end-start-32;
    write(s,0x8041e818,length);const uint32_t root=eventSdkAllocate(handle,length,s);
    if(root!=0) {eventFillMemory(root,0,length,s);eventFlushMemory(root,length,s);}
    const uint32_t liveLength=read(s,0x8041e818);write(s,0x8041e808,root);
    write(s,root,0);write(s,root+4,liveLength-32);assert(s.storeHalf);s.storeHalf(s.user,root+8,0);
}
}
uint32_t eventSdkInitializeAllocator(uint32_t start,uint32_t end,int32_t count,const EventMemoryServices& s) {
    const uint32_t metadata=static_cast<uint32_t>(count)*12;
    write(s,0x8041f070,start);write(s,0x8041f074,static_cast<uint32_t>(count));
    uint32_t i=0,offset=0;
    while(sw(i)<sw(read(s,0x8041f074))) {
        const uint32_t d=read(s,0x8041f070)+offset;++i;write(s,d,0xffffffffu);offset+=12;write(s,d+8,0);write(s,d+4,0);
    }
    const uint32_t base=read(s,0x8041f070);write(s,0x8041f07c,end&0xffffffe0u);
    const uint32_t usable=(base+metadata+31)&0xffffffe0u;write(s,0x8041e428,0xffffffffu);write(s,0x8041f078,usable);return usable;
}
int32_t eventSdkCreateHeap(uint32_t start,uint32_t end,const EventMemoryServices& s) {
    const int32_t count=sw(read(s,0x8041f074));uint32_t d=read(s,0x8041f070);
    start=(start+31)&0xffffffe0u;end&=0xffffffe0u;
    for(int32_t i=0;i<count;++i,d+=12)if(sw(read(s,d))<0) {
        write(s,d,end-start);write(s,start,0);write(s,start+4,0);write(s,start+8,read(s,d));write(s,d+4,start);write(s,d+8,0);return i;
    }
    return -1;
}
void eventSdkDestroyHeap(int32_t handle,const EventMemoryServices& s) {
    const uint32_t d=read(s,0x8041f070)+12u*static_cast<uint32_t>(handle);write(s,d,0xffffffffu);write(s,d+8,0);write(s,d+4,0);
}
void clearEventMemoryPool(int32_t index,const EventMemoryServices& s) {
    const uint32_t offset=4u*static_cast<uint32_t>(index);
    eventSdkDestroyHeap(sw(read(s,0x8041e844+offset)),s);
    const uint32_t start=read(s,0x8041e830+offset),end=read(s,0x8041e81c+offset);
    eventSdkCreateHeap(start,end,s); // Original ignores the replacement handle.
    if(index==1)primaryMap(s);
}
void initializeEventMemoryPools(const EventMemoryServices& s) {
    const uint32_t low=read(s,0x8041e430),high=read(s,0x8041f080);
    uint32_t p=eventSdkInitializeAllocator(low,high,5,s);write(s,0x8041e430,p);
    p=(p+31)&0xffffffe0u;const uint32_t end=high&0xffffffe0u;
    for(uint32_t i=0;i<5;++i)if(read(s,0x804151b8+8*i)==1) {
        const uint32_t span=read(s,0x804151bc+8*i)<<10;write(s,0x8041e830+4*i,p);write(s,0x8041e81c+4*i,p+span);p+=span;
    }
    const uint32_t remaining=end-p;
    for(uint32_t i=0;i<5;++i)if(read(s,0x804151b8+8*i)==0) {
        const int32_t percent=sw(read(s,0x804151bc+8*i));
        const uint64_t product=uint64_t(remaining)*static_cast<uint64_t>(static_cast<int64_t>(percent));
        const uint32_t span=static_cast<uint32_t>(product/100)&0xffffffe0u;
        write(s,0x8041e830+4*i,p);write(s,0x8041e81c+4*i,p+span);p+=span;
    }
    for(uint32_t i=0;i<5;++i) {
        const uint32_t start=read(s,0x8041e830+4*i),end=read(s,0x8041e81c+4*i);
        write(s,0x8041e844+4*i,static_cast<uint32_t>(eventSdkCreateHeap(start,end,s)));
    }
    write(s,0x8041e430,end);
    for(int32_t i=0;i<5;++i)clearEventMemoryPool(i,s);
}
}
