#pragma once
#include "original_sequence.hpp"
#include <optional>
namespace ttyd {
// Integer arguments retain original address tokens. Stack color/vector contents
// have semantic payloads; they are never invented guest stack addresses.
struct BattleSequenceCall {
    uint32_t function=0,result=0,rgba=0;
    std::array<uint32_t,5> args{};
    std::array<float,3> position{};
    float yaw=0;
};
struct BattleSequenceEntryServices {
    void* user=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
    bool (*store)(void*,uint32_t,uint32_t)=nullptr;
    bool (*call)(void*,BattleSequenceCall&)=nullptr;
};
struct BattleSequenceEntryCursor {
    bool active=false,queued=false,getterCaptured=false;
    uint32_t phase=0,pending=0,gp=0,field=0,npc=0,descriptor=0,pouch=0,value=0,flags=0,root=0,state=0,type=0;
    BattleSequenceCall call;
};
bool initializeBattleSequenceEntry(const BattleSequenceEntryServices&,BattleSequenceEntryCursor&);
// Complete seq_battleMain80073b24 parent, including all twelve table routes.
// battle_init800738ac and its transitive resource graph are real child services.
bool updateBattleSequenceEntry(OriginalSequenceWork&,const BattleSequenceEntryServices&,BattleSequenceEntryCursor&);
}
