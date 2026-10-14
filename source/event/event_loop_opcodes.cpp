#include "event_loop_opcodes.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t signedByte(uint8_t v) {return v<128?v:int32_t(v)-256;}
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?int32_t(v):int32_t(v)-0x10000;}
uint32_t word(const EventDispatchServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t resolve(const EventDispatchServices& s,EventEntry& e,int32_t op) {assert(s.resolve);return s.resolve(s.user,e,op);}
unsigned stackIndex(uint8_t b) {const int32_t i=signedByte(b);assert(i>=0 && i<8);return unsigned(i);}
void pop(EventEntry& e) {e.loopIndex=static_cast<uint8_t>(uint32_t(e.loopIndex)-1);}
}
std::optional<int32_t> eventLoopOpcode(EventEntry& e,uint8_t op,const EventDispatchServices& s) {
    if(op<4 || op>8)return {};
    if(op==4) {
        const int32_t target=resolve(s,e,signedWord(word(s,e.arguments)));
        uint32_t next=e.arguments;
        if(target<-270000000)next=static_cast<uint32_t>(target);
        else for(unsigned i=0;i<16;++i)if(signedByte(e.labelIds[i])==target) {next=e.labelAddresses[i];break;}
        e.nextCommand=next;return 2;
    }
    if(op==5) {
        const uint32_t args=e.arguments;e.loopIndex=static_cast<uint8_t>(uint32_t(e.loopIndex)+1);
        const unsigned i=stackIndex(e.loopIndex);e.loopBodies[i]=args+4;e.loopCounts[i]=signedWord(word(s,args));return 2;
    }
    if(op==6) {
        const unsigned i=stackIndex(e.loopIndex);const int32_t raw=e.loopCounts[i];
        if(raw==0) {e.nextCommand=e.loopBodies[i];return 2;}
        int32_t n;
        if(raw>=-10000000) {n=signedWord(static_cast<uint32_t>(raw)-1);e.loopCounts[i]=n;}
        else {
            const int32_t v=resolve(s,e,raw);n=signedWord(static_cast<uint32_t>(v)-1);
            assert(s.store);s.store(s.user,e,raw,n);
        }
        if(n!=0)e.nextCommand=e.loopBodies[i];else pop(e);
        return 2;
    }
    uint32_t p=e.nextCommand;int32_t depth=0;
    for(;;) {
        const uint32_t header=p,w=word(s,p),opcode=w&0xffff;
        const uint32_t after=header+4+static_cast<uint32_t>(4*signedHalf(w>>16));
        if(opcode==5)depth=signedWord(static_cast<uint32_t>(depth)+1);
        else if(opcode==6 || opcode==1) {
            depth=signedWord(static_cast<uint32_t>(depth)-1);
            if(depth<0) {e.nextCommand=op==7?after:header;if(op==7)pop(e);return 2;}
        }
        p=after;
    }
}
}
