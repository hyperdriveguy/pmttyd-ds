#include "battle_majinai_query.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleMajinaiQueryServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
uint8_t byte(const BattleMajinaiQueryServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
void storeByte(const BattleMajinaiQueryServices& s,uint32_t a,uint8_t v) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,a,v);}
}
int32_t battleMajinaiCheck(const BattleMajinaiQueryServices& s) {
    const uint32_t p=word(s,0x8041eb00),b=word(s,0x8041eb90);
    if(byte(s,p+0x5b8)==0)return 0;
    const uint8_t countdown=byte(s,p+0x5b9);
    if(countdown>=128) {
        storeByte(s,p+0x5ba,0);assert(s.irand);const int32_t random=s.irand(s.user,5);
        storeByte(s,p+0x5b9,uint8_t(uint32_t(random)+6));
        storeByte(s,p+0x5b9,uint8_t(byte(s,p+0x5b9)-1));return 0;
    }
    const uint8_t mode=byte(s,p+0x5ba);if(mode!=0)return mode;
    storeByte(s,p+0x5b9,uint8_t(countdown-1));const uint8_t after=byte(s,p+0x5b9);
    if(after>0 && after<128)return 0;
    storeByte(s,p+0x5b9,0);
    const uint32_t descriptor=word(s,b+0x2738),setup=word(s,descriptor+0xc),flags=word(s,setup+4);
    assert(s.irand);const int32_t random=s.irand(s.user,100);
    uint8_t selected;
    if(flags&0x20)selected=random<45?1:random<90?2:3;
    else selected=random<30?1:random<60?2:random<80?3:4;
    storeByte(s,p+0x5ba,selected);return byte(s,p+0x5ba);
}
void initializePouchMajinai(int32_t count,const BattleMajinaiQueryServices& s) {
    const uint32_t p=word(s,0x8041eb00);if(int32_t(byte(s,p+0x5b8))>=count)return;
    storeByte(s,p+0x5b8,uint8_t(count));
    storeByte(s,word(s,0x8041eb00)+0x5b9,0xff);
    storeByte(s,word(s,0x8041eb00)+0x5ba,0);
}
int32_t pouchMajinaiSetEvent(EventEntry& e,const BattleMajinaiQueryServices& s) {
    const uint32_t args=e.arguments,raw=word(s,args);
    const int32_t input=raw<=0x7fffffff?int32_t(raw):int32_t(int64_t(raw)-0x100000000LL);
    assert(s.values.resolve);initializePouchMajinai(s.values.resolve(s.values.user,e,input),s);return 2;
}
void battleMajinaiDone(const BattleMajinaiQueryServices& s) {
    const uint32_t p=word(s,0x8041eb00);
    storeByte(s,p+0x5b8,uint8_t(byte(s,p+0x5b8)-1));
    const auto remaining=byte(s,p+0x5b8);(void)remaining; // Original reread/branch,both branches have identical stores.
    storeByte(s,p+0x5b9,0xff);storeByte(s,p+0x5ba,0);
}
void battleMajinaiEndCheck(const BattleMajinaiQueryServices& s) {
    const uint32_t b=word(s,0x8041eb90),p=word(s,0x8041eb00);
    if(byte(s,p+0x5b8)==0) {assert(s.memory.storeWord);s.memory.storeWord(s.memory.user,b+0xef4,word(s,b+0xef4)|0x00400000);}
}
int32_t battleMajinaiPowerQuery(EventEntry& e,const BattleMajinaiQueryServices& s) {
    const uint32_t args=e.arguments,b=word(s,0x8041eb90),output=word(s,args),p=word(s,0x8041eb00);
    const int32_t destination=output<=0x7fffffff?int32_t(output):int32_t(int64_t(output)-0x100000000LL);
    assert(s.values.store);
    if(byte(s,p+0x5ba)!=1) {s.values.store(s.values.user,e,destination,0);return 2;}
    s.values.store(s.values.user,e,destination,1);
    storeByte(s,b+0x18ff9,1);battleMajinaiDone(s);battleMajinaiEndCheck(s);return 2;
}
}
