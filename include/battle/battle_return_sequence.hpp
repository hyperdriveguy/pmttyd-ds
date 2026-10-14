#pragma once
#include "battle_actor.hpp"

namespace ttyd {
struct BattleReturnSequenceServices {
    BattleActorServices actors;
    void* user=nullptr;
    void (*setSequence)(void*,int32_t sequence,int32_t parameter,uint32_t data)=nullptr;
    // State5's camera/sound-location tail remains caller-owned. State6 has none.
    void (*tail)(void*)=nullptr;
};
// Only documented seq_battleMain states5/6; other states return false without
// mutations. sequenceWork holds source words+4(state),+10(counter).
bool packedBattleReturnSequence(uint32_t sequenceWork,const BattleReturnSequenceServices&);
struct FieldSequenceFadeServices {
    EventMemoryServices memory;
    void* user=nullptr;
    void (*fade)(void*,uint32_t type,uint32_t milliseconds,uint32_t rgba)=nullptr;
    void (*event)(void*,uint32_t script,int32_t type,int32_t priority,uint32_t flags)=nullptr;
};
// seq_gameMain800073a4 state0 only. Does not enter state1 in the same pass.
bool packedFieldSequenceIncomingFade(uint32_t sequenceWork,const FieldSequenceFadeServices&);
}
