#pragma once
#include "battle_packed_part.hpp"
namespace ttyd {
struct BattleSoundPlaybackServices {
    void* user=nullptr;
    uint32_t (*trigger)(void*,uint32_t unit,uint32_t key,int32_t rawSentinel,int32_t zero,const std::array<float,3>&)=nullptr;
    void (*position)(void*,uint32_t handle,const std::array<float,3>&)=nullptr;
    void (*off)(void*,uint32_t handle)=nullptr;
};
struct BattlePackedMovementSoundServices {
    BattlePartPositionServices position;
    BattleSoundPlaybackServices playback;
};
void loadPackedUnitSoundMode(int32_t type,int32_t mode,uint32_t source,uint32_t destination,const EventMemoryServices&);
void controlPackedMovementSound(uint32_t unit,uint32_t record,uint32_t part,const std::array<float,3>& supplied,const BattlePackedMovementSoundServices&);
void endPackedMovementSound(uint32_t record,const BattlePackedMovementSoundServices&);
void controlPackedUnitMovementSound(uint32_t unit,const BattlePackedMovementSoundServices&);
void endPackedUnitMovementSound(uint32_t unit,const BattlePackedMovementSoundServices&);
}
