#include "event_memory_release.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t read(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void write(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
uint16_t used(const EventMemoryServices& s,uint32_t h) {assert(s.half);return s.half(s.user,h+8);}
void unused(const EventMemoryServices& s,uint32_t h) {assert(s.storeHalf);s.storeHalf(s.user,h+8,0);}
}
void eventMapFree(uint32_t root,uint32_t payload,const EventMemoryServices& s) {
    if(payload==0)return;
    uint32_t h=payload-32;if(used(s,h)==0)return;
    uint32_t next=read(s,h),size=read(s,h+4);
    if(next!=0 && used(s,next)==0) {const uint32_t forwardSize=read(s,next+4);next=read(s,next);size+=forwardSize+32;}
    for(;;) {
        const uint32_t candidate=read(s,root);
        if(candidate==h) {if(used(s,root)==0) {const uint32_t previousSize=read(s,root+4);h=root;size+=previousSize+32;}break;}
        if(root>h || candidate==0)break;
        root=candidate;
    }
    write(s,h,next);write(s,h+4,size);unused(s,h);
}
uint32_t eventSdkInsertFree(uint32_t head,uint32_t n,const EventMemoryServices& s) {
    uint32_t c=head,p=0;
    while(c!=0 && n>c) {p=c;c=read(s,c+4);}
    write(s,n+4,c);write(s,n,p);
    if(c!=0) {
        write(s,c,n);const uint32_t size=read(s,n+8);
        if(n+size==c) {write(s,n+8,size+read(s,c+8));c=read(s,c+4);write(s,n+4,c);if(c!=0)write(s,c,n);}
    }
    if(p!=0) {
        write(s,p+4,n);const uint32_t size=read(s,p+8);
        if(p+size==n) {write(s,p+8,size+read(s,n+8));write(s,p+4,c);if(c!=0)write(s,c,p);}
        return head;
    }
    return n;
}
void eventSdkFree(int32_t handle,uint32_t payload,const EventMemoryServices& s) {
    const uint32_t h=payload-32,d=read(s,0x8041f070)+12u*static_cast<uint32_t>(handle),next=read(s,h+4);
    uint32_t head=read(s,d+8);if(next!=0)write(s,next,read(s,h));
    const uint32_t previous=read(s,h);if(previous==0)head=read(s,h+4);else write(s,previous+4,read(s,h+4));
    write(s,d+8,head);const uint32_t freeHead=read(s,d+4);write(s,d+4,eventSdkInsertFree(freeHead,h,s));
}
void eventHeapFree(int32_t index,uint32_t payload,const EventMemoryServices& s) {
    const int32_t handle=sw(read(s,0x8041e844+4u*static_cast<uint32_t>(index)));eventSdkFree(handle,payload,s);
}
void allocateEventBattleMapPool(const EventMemoryServices& s) {
    write(s,0x8041e814,0x64000);const uint32_t root=read(s,0x8041e808);
    const uint32_t payload=eventMapAllocate(root,0x64000,s),length=read(s,0x8041e814);write(s,0x8041e80c,payload);
    write(s,payload,0);write(s,payload+4,length-32);unused(s,payload);
}
void freeEventBattleMapPool(const EventMemoryServices& s) {
    const uint32_t primary=read(s,0x8041e808),battle=read(s,0x8041e80c);eventMapFree(primary,battle,s);write(s,0x8041e80c,0);
}
}
