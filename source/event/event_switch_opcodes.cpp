#include "event_switch_opcodes.hpp"
#include <cassert>

// SWITCH34 resolves selector;35 uses raw selector. Cases36..41 are
// EQ/NE/LT/GT/LE/GE,42 default,43/44 grouped OR/AND,45 flag mask,46 group
// end,47 inclusive range,48 break,49 end. Handler2 continues dispatch.
// Eight stack slots; signed state1=searching,0=done,255=-1,254=-2 group state.
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
int32_t signedByte(uint8_t v) {return v<128?v:int32_t(v)-256;}
int32_t signedHalf(uint32_t v) {v&=0xffff;return v<0x8000?int32_t(v):int32_t(v)-0x10000;}
uint32_t word(const EventDispatchServices& s,uint32_t a) {assert(s.word);return s.word(s.user,a);}
int32_t resolve(const EventDispatchServices& s,EventEntry& e,uint32_t a) {const int32_t op=signedWord(word(s,a));assert(s.resolve);return s.resolve(s.user,e,op);}
unsigned stackIndex(uint8_t v) {const int32_t i=signedByte(v);assert(i>=0 && i<8);return unsigned(i);}
// Source scan nests SWITCH34 (and END1), not raw SWITCH35. Preserve this
// asymmetry; replacing it with a generic structured parser changes script flow.
void scan(EventEntry& e,const EventDispatchServices& s,bool nextCase) {
    uint32_t p=e.nextCommand;int32_t depth=1;
    for(;;) {
        const uint32_t header=p,w=word(s,p),op=w&0xffff;p+=4+static_cast<uint32_t>(4*signedHalf(w>>16));
        if(op==34 || op==1)depth=signedWord(static_cast<uint32_t>(depth)+1);
        else if(op==49) {depth=signedWord(static_cast<uint32_t>(depth)-1);if(depth==0) {e.nextCommand=header;return;}}
        else if(nextCase && depth==1 && op>=36 && op<=47) {e.nextCommand=header;return;}
    }
}
bool comparison(uint8_t op,int32_t value,int32_t candidate) {
    switch(op) {case 36:return value==candidate;case 37:return value!=candidate;case 38:return value<candidate;case 39:return value>candidate;case 40:return value<=candidate;default:return value>=candidate;}
}
}
std::optional<int32_t> eventSwitchOpcode(EventEntry& e,uint8_t op,const EventDispatchServices& s) {
    if(op<34 || op>49)return {};
    const uint32_t args=e.arguments;
    if(op==34 || op==35) {
        int32_t value;uint8_t previous;
        if(op==34) {value=resolve(s,e,args);previous=e.switchIndex;}
        else {previous=e.switchIndex;value=signedWord(word(s,args));}
        e.switchIndex=static_cast<uint8_t>(uint32_t(previous)+1);const unsigned i=stackIndex(e.switchIndex);
        e.switchValues[i]=value;e.switchStates[i]=1;return 2;
    }
    if(op==48) {scan(e,s,false);return 2;}
    const unsigned i=stackIndex(e.switchIndex);
    if(op==49) {e.switchStates[i]=0;e.switchIndex=static_cast<uint8_t>(uint32_t(e.switchIndex)-1);return 2;}
    if(op==42 || op==46) {
        const int32_t state=signedByte(e.switchStates[i]);
        if(op==42) {if(state>0)e.switchStates[i]=0;else scan(e,s,false);}
        else if(state==0)scan(e,s,false);
        else if(state==-1) {e.switchStates[i]=0;scan(e,s,false);}
        else {e.switchStates[i]=1;scan(e,s,true);}
        return 2;
    }
    const int32_t candidate=op==45?signedWord(word(s,args)):resolve(s,e,args);
    int32_t high=0;if(op==47)high=resolve(s,e,args+4);
    const int32_t state=signedByte(e.switchStates[i]),value=e.switchValues[i];
    if(op==43) {
        if(state==0)scan(e,s,false);
        else if(candidate==value)e.switchStates[i]=255;
        else if(state!=-1)scan(e,s,true);
    } else if(op==44) {
        if(state==0)scan(e,s,false);
        else if(state==-2)scan(e,s,true);
        else if(candidate==value)e.switchStates[i]=255;
        else {e.switchStates[i]=254;scan(e,s,true);}
    } else if(state<=0)scan(e,s,false);
    else {
        const bool accepted=op==45?(static_cast<uint32_t>(value)&static_cast<uint32_t>(candidate))!=0:
            op==47?candidate<=value && value<=high:comparison(op,value,candidate);
        if(accepted)e.switchStates[i]=0;else scan(e,s,true);
    }
    return 2;
}
}
