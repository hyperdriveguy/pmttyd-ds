#include "battle_audience_sound_start.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t address,const BattleActorServices& s){const auto bits=s.memory.word(s.memory.user,address);float value;std::memcpy(&value,&bits,4);return value;}
int32_t integer(uint32_t value){int32_t result;std::memcpy(&result,&value,4);return result;}
}
std::optional<BattleAudienceSoundInfo> packedBattleAudienceSoundInfo1(int32_t right,int32_t left,const BattleAudienceSoundStartServices& s){
    const auto total=integer(uint32_t(right)+uint32_t(left));BattleAudienceSoundInfo info;
    if(total!=0){
        if(!s.truncate)return {};
        const float product=scalar(0x804249c0,s.actors)*float(total);
        const float quotient=product/scalar(0x804249b8,s.actors);
        const float value=scalar(0x804249bc,s.actors)+quotient;
        const auto converted=s.truncate(s.user,value);if(!converted)return {};info.volume=uint8_t(*converted);
    }
    if(left==0 || total==0)info.position[0]=scalar(0x804249c4,s.actors);
    else{
        if(!s.truncate)return {};
        const float product=scalar(0x804249c4,s.actors)*float(integer(uint32_t(right)-uint32_t(left)));
        const auto converted=s.truncate(s.user,product/float(total));if(!converted)return {};
        info.position[0]=float(uint8_t(*converted));
    }
    info.position[1]=scalar(0x80424988,s.actors);info.position[2]=scalar(0x80424988,s.actors);return info;
}
std::array<float,3> packedBattleAudienceSoundInfo2(int32_t slot,const BattleActorServices& s){
    const auto r=currentBattleAddress(s)+0x2858+0x1bc+uint32_t(slot)*0x134;
    return {scalar(r+0x48,s),scalar(r+0x4c,s),scalar(r+0x50,s)};
}
std::optional<int32_t> startPackedBattleAudienceSound1(uint32_t key,int32_t right,int32_t left,const BattleAudienceSoundStartServices& s){
    const auto info=packedBattleAudienceSoundInfo1(right,left,s);if(!info || !s.on)return {};
    const auto handle=s.on(s.user,key,info->position);if(!handle)return {};
    if(!s.volume || !s.volume(s.user,*handle,info->volume))return {};
    return handle; // Original retained handle, including real -1 rejection.
}
std::optional<int32_t> startPackedBattleAudienceSound2(uint32_t key,int32_t slot,const BattleAudienceSoundStartServices& s){
    const auto position=packedBattleAudienceSoundInfo2(slot,s.actors);if(!s.on)return {};
    return s.on(s.user,key,position);
}
}
