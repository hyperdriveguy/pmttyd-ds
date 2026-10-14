#pragma once
#include "battle_actor.hpp"
namespace ttyd {
struct PackedBattleDisplayEntryServices {
 BattleActorServices memory;void* user=nullptr;
 int32_t (*poseEntry)(void*,uint32_t originalBank,int32_t context)=nullptr;
};
// Complete source searchPoseTbl80103cd0 and btlDispEntAnime801011fc.
// PoseEntry must own actual original-bank resource/pose allocation.
uint32_t searchPackedBattlePoseTable(uint32_t table,int32_t type,const EventMemoryServices&);
void initializePackedBattleDisplay(uint32_t unit,const PackedBattleDisplayEntryServices&);
}
