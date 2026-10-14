#include "event_dispatch.hpp"
#include <cassert>

// Source dispatcher result protocol and opcode table: docs/code/event-vm.md.
// Handler results differ from the three codes returned to the manager.
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
// Command header: signed high16 argument count, low8 dispatched opcode.
// Sign extension is intentional; byte argumentCount retains only low8 bits.
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?static_cast<int32_t>(v):static_cast<int32_t>(v)-0x10000;}
uint32_t read(const EventDispatchServices& s,uint32_t address) {assert(s.word);return s.word(s.user,address);}
int32_t resolve(EventEntry& e,const EventDispatchServices& s,uint32_t address) {
    const int32_t raw=signedWord(read(s,address));assert(s.resolve);return s.resolve(s.user,e,raw);
}
}
// First invocation both captures AND decrements. Exact zero continues now;
// count1 completes/yields now. Negative counts wrap; do not clamp them to zero.
int32_t eventWaitFrame(EventEntry& e,const EventDispatchServices& s) {
    if(e.initialized==0) {e.waitCount=resolve(e,s,e.arguments);e.initialized=1;}
    if(e.waitCount==0)return 2;
    e.waitCount=signedWord(static_cast<uint32_t>(e.waitCount)-1);
    return e.waitCount==0?1:0;
}
int32_t eventWaitMilliseconds(EventEntry& e,const EventDispatchServices& s) {
    const uint64_t now=e.lifetime;
    if(e.initialized==0) {e.waitCount=resolve(e,s,e.arguments);e.waitStart=now;e.initialized=1;}
    if(e.waitCount==0)return 2;
    assert(s.clock);const uint32_t k=eventClockRate(s.clock(s.user));assert(k!=0);
    // Original WAIT_MSEC uses only low32 lifetime subtraction, sign-extends it
    // to64 bits, then divides UNSIGNED. This is not a general elapsed64 timer.
    const int32_t elapsed=signedWord(static_cast<uint32_t>(now)-static_cast<uint32_t>(e.waitStart));
    const uint64_t unsignedElapsed=static_cast<uint64_t>(static_cast<int64_t>(elapsed));
    const uint32_t q=static_cast<uint32_t>(unsignedElapsed/k);
    const uint32_t n=static_cast<uint32_t>(e.waitCount);
    // Reproduce original subc/adde result: signed quotient high-bit contribution
    // plus requested-count sign bit plus unsigned comparison carry.
    return (q&0x80000000?-1:0)+static_cast<int32_t>(n>>31)+(q>=n?1:0);
}
int32_t dispatchEvent(EventEntry& e,const EventDispatchServices& s) {
    for(;;) {
        int32_t result=2;
        if(e.opcode<=118) {
            switch(e.opcode) {
            case 0: {
                const uint32_t next=e.nextCommand;e.currentCommand=next;
                const uint32_t command=read(s,next);const int32_t count=signedHalf(command>>16);
                e.opcode=static_cast<uint8_t>(command);e.argumentCount=static_cast<uint8_t>(count);
                e.arguments=next+4;e.nextCommand=e.arguments+static_cast<uint32_t>(4*count);
                e.initialized=0;result=3;break;
            }
            case 2:assert(s.remove);s.remove(s.user,e);result=255;break;
            case 9:result=eventWaitFrame(e,s);break;
            case 10:result=eventWaitMilliseconds(e,s);break;
            // USER_FUNC: first poll resolves/caches the function and removes its
            // operand from the callback argument list; resumed polls keep that list.
            case 91: {
                const bool first=e.initialized==0;
                if(first) {
                    e.cachedFunction=static_cast<uint32_t>(resolve(e,s,e.arguments));
                    e.arguments+=4;e.argumentCount=static_cast<uint8_t>(e.argumentCount-1);e.initialized=1;
                }
                assert(s.call);result=s.call(s.user,e,e.cachedFunction,first?1:0);break;
            }
            default:assert(s.handler);result=s.handler(s.user,e,e.opcode);break;
            }
        }
        // Handler3=redispatch unchanged,255=entry end,negative=whole-pass abort,
        // 0=yield retaining opcode,1=clear/yield,2=clear/continue in this call.
        // Other nonnegative codes repeat without clearing (original protocol).
        if(result==3)continue;
        if(result==255)return -1;
        if(result<0)return 1;
        if(result==0)return 0;
        if(result==1) {e.opcode=0;return 0;}
        if(result==2)e.opcode=0;
    }
}
}
