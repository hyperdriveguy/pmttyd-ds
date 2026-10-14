#pragma once
#include "battle_actor.hpp"
#include "display_queue.hpp"
namespace ttyd {
struct BattleCommandPalette {
    int8_t state=0;std::array<uint8_t,4> rgba{{255,192,192,207}};
    void step();
};
struct BattleFooterRecord {uint32_t camera=0;int32_t drawClass=0;uint32_t callback=0,borrowed=0;float priority=0;};
struct BattleFooterCursor {uint8_t phase=0,index=0;bool active=false,hidden=false,window=false;std::optional<BattleFooterRecord> pending;};
bool registerBattleCommandDisplay(uint32_t,BattleCommandPalette&,DisplayQueue&,BattleFooterCursor&,const BattleActorServices&);
struct BattleAudienceWindowServices {void* user=nullptr;std::optional<bool> (*window)(void*)=nullptr;};
bool registerBattleAudienceDisplay(uint32_t flags,const BattleAudienceWindowServices&,DisplayQueue&,BattleFooterCursor&);
bool registerBattleBreakSlotDisplay(DisplayQueue&,BattleFooterCursor&);
struct BattleAudienceAnimationServices {
    BattleActorServices actors;void* user=nullptr;
    std::optional<uint32_t> (*scriptWord)(void*,uint32_t)=nullptr;
};
struct BattleAudienceAnimationCursor {
    uint32_t first=0,index=0,script=0,scan=0,label=0,destination=0,nesting=0;
    uint8_t phase=0;bool active=false;
};
// Exactly200 retained actor records, stride134hex; one opcode per admitted actor.
// Missing script reads retain the source continuation, including prior stores.
bool processPackedBattleAudienceAnimations(uint32_t,const BattleAudienceAnimationServices&,BattleAudienceAnimationCursor&);
}
