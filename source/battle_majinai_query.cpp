#include "battle_majinai_query.hpp"
#include <cassert>

namespace ttyd {
namespace {
uint32_t word(const BattleMajinaiQueryServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
uint8_t byte(const BattleMajinaiQueryServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
void storeByte(const BattleMajinaiQueryServices& s,uint32_t a,uint8_t v) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,a,v);}
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
