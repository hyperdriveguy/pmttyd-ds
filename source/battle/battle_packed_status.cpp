#include "battle_packed_status.hpp"
#include <cassert>

namespace ttyd {
namespace {
int32_t sb(uint8_t v) {return v<128?int32_t(v):int32_t(v)-256;}
uint8_t byte(const BattleActorServices& s,uint32_t a) {assert(s.byte);return s.byte(s.user,a);}
}
StatusValue getPackedBattleStatus(uint32_t unit,int32_t rawId,const BattleActorServices& s) {
    static constexpr uint32_t offsets[]{0x12a,0x118,0x11a,0x11b,0x11c,0x11e,0x11f,0x120,0x121,0x122,0x123,0x123,0x125,0x125,0x127,0x127,0x129,0x12b,0x12c,0x12f,0x130,0x12d,0x12e,0x131,0x133,0,0,0x135};
    const int32_t id=sb(uint8_t(rawId));if(id<0 || id>27 || offsets[id]==0)return {};
    const uint32_t offset=offsets[id];
    if(id>=10 && id<=15) {
        const uint8_t strength=byte(s,unit+offset+1);const int32_t signedStrength=sb(strength);
        if((id%2==0 && signedStrength<=0) || (id%2!=0 && signedStrength>=0))return {};
        return {byte(s,unit+offset),strength};
    }
    const uint8_t duration=byte(s,unit+offset);
    if(id==4 || id==23 || id==24)return {duration,byte(s,unit+offset+1)};
    return {duration,duration};
}
void writePackedBattleStatus(uint32_t unit,int32_t id,uint32_t durationOutput,uint32_t strengthOutput,const BattleActorServices& s) {
    const auto value=getPackedBattleStatus(unit,id,s);
    if(durationOutput) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,durationOutput,value.duration);}
    if(strengthOutput) {assert(s.memory.storeByte);s.memory.storeByte(s.memory.user,strengthOutput,value.strength);}
}
bool checkPackedBattleStatus(uint32_t unit,int32_t id,const BattleActorServices& s) {
    if(!unit)return false;
    if(getPackedBattleStatus(unit,id,s).duration!=0)return true;
    return sb(uint8_t(id))==6 && byte(s,unit+0x2f7)!=0;
}
}
