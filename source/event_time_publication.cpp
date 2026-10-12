#include "event_time_publication.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
uint32_t read(const EventMemoryServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
void write(const EventMemoryServices& s,uint32_t a,uint32_t v) {assert(s.storeWord);s.storeWord(s.user,a,v);}
uint32_t gp(const EventMemoryServices& s) {return read(s,0x80415fd0);}
void store64(const EventMemoryServices& s,uint32_t a,uint64_t v) {write(s,a+4,static_cast<uint32_t>(v));write(s,a,static_cast<uint32_t>(v>>32));}
void addLiveDelta(const EventMemoryServices& s,uint32_t g,uint32_t bank) {
    const uint32_t low=read(s,g+bank+4),deltaLow=read(s,g+0x2c),high=read(s,g+bank),deltaHigh=read(s,g+0x28);
    store64(s,g+bank,((uint64_t(high)<<32)|low)+((uint64_t(deltaHigh)<<32)|deltaLow));
}
}
uint64_t readEventRawTime(const EventTimeServices& s) {
    assert(s.upper && s.lower);
    for(;;) {const uint32_t high=s.upper(s.user),low=s.lower(s.user),again=s.upper(s.user);if(high==again)return (uint64_t(high)<<32)|low;}
}
int32_t eventWindowFreeze(const EventMemoryServices& s) {
    const uint32_t window=read(s,0x8041ec30);if(window==0)return 0;
    assert(s.half);if(s.half(s.user,window)&0x1000)return 0;
    const int32_t state=sw(read(s,window+0x20));return state!=0 && state<200?1:0;
}
void publishEventRetraceTime(uint32_t argument,const EventTimeServices& services) {
    const auto& s=services.memory;const uint64_t sample=readEventRawTime(services);uint32_t g=gp(s);
    const uint32_t previousLow=read(s,g+0x34),previousHigh=read(s,g+0x30);
    store64(s,g+0x28,sample-((uint64_t(previousHigh)<<32)|previousLow));
    g=gp(s);addLiveDelta(s,g,0x20);
    const uint32_t clock=read(s,0x800000f8);g=gp(s);
    const uint64_t limit=uint64_t(clock>>2)*0x157529ffu;
    const uint32_t totalHigh=read(s,g+0x20),totalLow=read(s,g+0x24);const uint64_t total=(uint64_t(totalHigh)<<32)|totalLow;
    if((total^0x8000000000000000ULL)>(limit^0x8000000000000000ULL))store64(s,g+0x20,limit);
    g=gp(s);
    if(read(s,g+0x10)==0 && eventWindowFreeze(s)==0) {
        constexpr uint32_t banks[]{0x60,0x58,0x50,0x48},masks[]{8,4,2,1};
        for(unsigned i=0;i<4;++i) {g=gp(s);if((read(s,g+0x18)&masks[i])==0)addLiveDelta(s,g,banks[i]);}
        g=gp(s);addLiveDelta(s,g,0x38);
        g=gp(s);if(read(s,g+0x14)==0)addLiveDelta(s,g,0x40);
    }
    g=gp(s);store64(s,g+0x30,sample);
    g=gp(s);write(s,g+0x1c,argument);
}
}
