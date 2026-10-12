#include "battle_event_utilities.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
uint32_t word(const BattleEventUtilityServices& s,uint32_t a) {assert(s.memory.word);return s.memory.word(s.memory.user,a);}
}
int32_t battleFlagQueryEvent(EventEntry& e,const BattleEventUtilityServices& s) {
    const uint32_t args=e.arguments,battle=word(s,0x8041eb90);
    const uint32_t mask=word(s,args+4),flags=word(s,battle+0xef4);
    const int32_t output=sw(word(s,args));assert(s.values.store);
    s.values.store(s.values.user,e,output,(flags&mask)!=0?1:0);return 2;
}
int32_t battleFramesToMillisecondsEvent(EventEntry& e,const BattleEventUtilityServices& s) {
    const uint32_t args=e.arguments;const int32_t input=sw(word(s,args));assert(s.values.resolve && s.values.store);
    const int32_t frames=s.values.resolve(s.values.user,e,input);
    const int32_t product=sw(uint32_t(frames)*1000u),output=sw(word(s,args+4));
    s.values.store(s.values.user,e,output,product/60);return 2;
}
}
