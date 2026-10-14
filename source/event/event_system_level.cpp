#include "event_system_level.hpp"
#include <cassert>

// Requested scalar level0..4 at0x8041e940 differs from live GP+0x18
// low-nibble inhibit bits0/1/3/7/15. Event type stop masks are1/2/0x10/0xef.
// GP pointer token is stored at0x80415fd0; reloads preserve source alias order.
namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t read(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void write(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
uint32_t gp(const EventMemoryServices& s) {return read(s,0x80415fd0);}
}
int32_t getEventSystemLevel(const EventMemoryServices& s) {const uint32_t g=gp(s);return sw(read(s,g+0x18));}
int32_t getRequestedEventSystemLevel(const EventMemoryServices& s) {return sw(read(s,0x8041e940));}
void setEventSystemLevel(int32_t request,const EventMemoryServices& s,const EventCreationServices& events) {
    if(request<0 || request>4)return;
    write(s,0x8041e940,static_cast<uint32_t>(request));uint32_t g=gp(s);
    if(request==0) {write(s,g+0x18,read(s,g+0x18)&0xfffffff0u);startAllEvents(0xef,events);return;}
    if(request==4)write(s,g+0x18,read(s,g+0x18)|15);
    else {
        const uint32_t mask=request==1?0xfffffff1u:request==2?0xfffffff3u:0xfffffff7u;
        write(s,g+0x18,read(s,g+0x18)&mask);
        g=gp(s);write(s,g+0x18,read(s,g+0x18)|(request==1?1u:request==2?3u:7u));
    }
    const uint32_t type=request==1?1u:request==2?2u:request==3?0x10u:0xefu;
    stopAllEvents(type,events);
}
}
