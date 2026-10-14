#include "battle_audience_sound.hpp"
#include <cassert>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& s){return currentBattleAddress(s)+0x2858;}
uint32_t word(const BattleActorServices& s,uint32_t a){return s.memory.word(s.memory.user,a);}
void word(const BattleActorServices& s,uint32_t a,uint32_t v){s.memory.storeWord(s.memory.user,a,v);}
void byte(const BattleActorServices& s,uint32_t a,uint8_t v){s.memory.storeByte(s.memory.user,a,v);}
}
uint32_t battleAudienceSoundRecord(int32_t slot,const BattleActorServices& s) {return base(s)+0x133fc+uint32_t(slot)*0x24;}
int32_t battleAudienceHalfCount(bool right,const BattleActorServices& s) {
    (void)base(s); // Source RL_Sub's initial base query precedes per-position getters.
    int32_t count=0;
    for(unsigned row=0;row<10;++row)for(unsigned column=0;column<10;++column) {
        const auto record=base(s)+0x1bc+0x134*(20*row+(right?10:0)+column);
        const auto flags=word(s,record);
        if(!(flags&1) || (flags&0x102))continue;
        const auto state=s.byte(s.user,record+0x19);
        if(state!=0xc && state!=0xd && state!=0x11)++count;
    }
    return count;
}
bool stopBattleAudienceSound(int32_t slot,const BattleAudienceSoundServices& s) {
    const auto record=battleAudienceSoundRecord(slot,s.actors);
    if(!(word(s.actors,record)&1))return true;
    if(!s.stop || !s.stop(s.user,int32_t(word(s.actors,record+4))))return false;
    word(s.actors,record,word(s.actors,record)&~1u); // Reload after actual backend request; preserve every other field/bit.
    return true;
}
bool clapBattleAudienceSound(int32_t lifetime,int32_t fade,const BattleAudienceSoundServices& s) {
    (void)base(s.actors);
    const auto record=battleAudienceSoundRecord(1,s.actors);
    const auto right=battleAudienceHalfCount(true,s.actors),left=battleAudienceHalfCount(false,s.actors);
    if(!stopBattleAudienceSound(1,s))return false;
    const auto total=right+left;uint32_t key=0;bool assigned=total>=1;
    if(assigned) {
        const uint8_t category=total<50?1:total<100?2:total<150?3:4;
        constexpr uint32_t keys[]{0x802fa5c4,0x802fa5dc,0x802fa5f4,0x802fa60c};
        key=keys[category-1];byte(s.actors,record+8,category);
    }
    if(!s.play)return false;
    const auto handle=s.play(s.user,key,assigned,right,left);if(!handle)return false;
    word(s.actors,record+4,uint32_t(*handle));
    if(*handle==-1)return true; // Failed request does not install lifetime/volume/active state.
    word(s.actors,record,word(s.actors,record)|1);
    word(s.actors,record+0xc,uint32_t(lifetime)+uint32_t(fade));word(s.actors,record+0x10,uint32_t(fade));
    if(!s.volume)return false;
    const auto volume=s.volume(s.user,*handle);if(!volume)return false;
    byte(s.actors,record+0x1d,*volume);byte(s.actors,record+0x1c,*volume);
    word(s.actors,record+0x18,0);word(s.actors,record+0x14,0);byte(s.actors,record+0x1e,100);
    return true;
}
void setBattleAudienceVolume(int32_t slot,int32_t target,int32_t duration,const BattleActorServices& s) {
    const auto record=battleAudienceSoundRecord(slot,s);
    byte(s,record+0x1f,s.byte(s.user,record+0x1e));byte(s,record+0x20,uint8_t(target));
    word(s,record+0x14,0);word(s,record+0x18,uint32_t(duration));word(s,record+0x10,0);
}
bool noiseAlwaysBattleAudienceSound(const BattleAudienceSoundServices& s) {
    const auto a=base(s.actors),record=battleAudienceSoundRecord(2,s.actors);
    const auto right=battleAudienceHalfCount(true,s.actors),left=battleAudienceHalfCount(false,s.actors),total=right+left;
    if(word(s.actors,record)&1)return true;
    uint32_t key=0;const bool assigned=total>0;
    if(!total)byte(s.actors,record+8,0);
    else {
        const uint8_t category=total<50?1:total<100?2:total<150?3:4;
        constexpr uint32_t keys[]{0x802fa574,0x802fa588,0x802fa59c,0x802fa5b0};
        key=keys[category-1];byte(s.actors,record+8,category);
    }
    if(!s.play)return false;
    const auto handle=s.play(s.user,key,assigned,right,left);if(!handle)return false;
    word(s.actors,record+4,uint32_t(*handle));if(*handle==-1)return true;
    word(s.actors,record,word(s.actors,record)|1);word(s.actors,record+0xc,0xffffffff);word(s.actors,record+0x10,0);
    if(!s.volume)return false;
    const auto volume=s.volume(s.user,*handle);if(!volume)return false;
    byte(s.actors,record+0x1d,*volume);byte(s.actors,record+0x1c,*volume);
    word(s.actors,record+0x18,0);word(s.actors,record+0x14,0);byte(s.actors,record+0x1e,100);
    word(s.actors,a,word(s.actors,a)|0x2000);return true;
}
void setAllBattleAudienceVolumes(int32_t target,int32_t duration,const BattleActorServices& s) {
    for(int32_t slot=0;slot<24;++slot)setBattleAudienceVolume(slot,target,duration,s);
}
}
