#include "battle_stage_work.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
// Zero storage first, then preserve the original explicit writes and order.
// Two byte-output groups start at 0xae8/0xb08 (defaults 0/140), initial words1
// at 0xae4/0xb04. Source floor/vector constants occupy0xb24..0xb38.
void initializePackedBattleStageWorkStorage(uint32_t battle,const EventMemoryServices& m) {
    assert(m.word && m.storeWord && m.storeByte);
    const auto w=battle+0x163fc;
    eventFillMemory(w,0,0xb3c,m);
    m.storeWord(m.user,w,0);
    for(unsigned i=0;i<12;++i) {
        const auto e=w+4+0xe8*i;
        m.storeWord(m.user,e,0);m.storeWord(m.user,e+0xe4,0);
    }
    m.storeWord(m.user,w+0xae4,1);
    for(const auto o:{0xae9u,0xae8u,0xaeau,0xaecu})m.storeByte(m.user,w+o,0);
    m.storeWord(m.user,w+0xaf0,0);m.storeWord(m.user,w+0xaf8,0);
    m.storeWord(m.user,w+0xb04,1);
    m.storeByte(m.user,w+0xb09,140);m.storeByte(m.user,w+0xb08,140);
    m.storeByte(m.user,w+0xb0a,0);m.storeByte(m.user,w+0xb0c,0);
    m.storeWord(m.user,w+0xb10,0);m.storeWord(m.user,w+0xb18,0);
    m.storeWord(m.user,w+0xb24,m.word(m.user,0x80422b3c));
    m.storeWord(m.user,w+0xb28,m.word(m.user,0x80422b40));
    m.storeWord(m.user,w+0xb2c,m.word(m.user,0x80422b44));
    m.storeWord(m.user,w+0xb30,m.word(m.user,w+0xb24));
    m.storeWord(m.user,w+0xb34,m.word(m.user,0x80422b48));
    m.storeWord(m.user,w+0xb38,m.word(m.user,0x80422b40));
}
namespace {
int32_t signedWord(uint32_t v){int32_t s;std::memcpy(&s,&v,4);return s;}
}
// Capture stage owner BEFORE spotlight main, then visit all twelve entries.
// Active mask 1 requires the real child; inactive scratch is cleared only after
// free acknowledges. Spotlight/entry failure does not undo earlier work.
bool packedBattleStageWorkMain(const BattleStageWorkServices& s) {
    const auto& m=s.actors.memory;
    assert(m.word && m.storeWord && m.storeByte && s.actors.byte);
    const auto w=currentBattleAddress(s.actors)+0x163fc;
    if(!s.spotlightMain || !s.spotlightMain(s.user))return false;
    for(unsigned i=0;i<12;++i) {
        const auto e=w+4+0xe8*i;
        if(m.word(m.user,e)&1) {
            if(!s.activeEntry || !s.activeEntry(s.user,e))return false;
        } else {
            const auto allocation=m.word(m.user,e+0xe4);
            if(allocation) {
                if(!s.freeScratch || !s.freeScratch(s.user,allocation))return false;
                m.storeWord(m.user,e+0xe4,0);
            }
        }
    }
    // Four source byte interpolators: count/start/end/mode/duration/output
    // offsets below are relative to captured stage work. Signed positive count
    // decrements BEFORE interpolation; duration and remaining are then reloaded.
    // Mode is signed low8, endpoints unsigned bytes, output wraps to low8 after
    // real truncation. Counts track main visits, not measured render frames.
    struct Counter {uint32_t count,start,end,mode,duration,output;};
    constexpr Counter counters[]={{0xaf0,0xaea,0xaec,0xb00,0xaf8,0xae8},
        {0xaf4,0xaeb,0xaed,0xb01,0xafc,0xae9},
        {0xb10,0xb0a,0xb0c,0xb20,0xb18,0xb08},
        {0xb14,0xb0b,0xb0d,0xb21,0xb1c,0xb09}};
    auto byte=[&](uint32_t o){return s.actors.byte(s.actors.user,w+o);};
    for(const auto& c:counters) {
        const auto old=m.word(m.user,w+c.count);
        if(signedWord(old)<=0)continue;
        m.storeWord(m.user,w+c.count,old-1);
        const float start=byte(c.start),end=byte(c.end);
        const auto modeByte=byte(c.mode);
        const int32_t mode=modeByte<128?modeByte:int32_t(modeByte)-256;
        const auto duration=m.word(m.user,w+c.duration);
        const auto remaining=m.word(m.user,w+c.count);
        if(!s.interpolate)return false;
        const auto value=s.interpolate(s.user,mode,signedWord(duration-remaining),signedWord(duration),start,end);
        if(!value || !s.truncate)return false;
        const auto integer=s.truncate(s.user,*value);if(!integer)return false;
        m.storeByte(m.user,w+c.output,uint8_t(uint32_t(*integer)));
    }
    return true;
}
}
