#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
#include <optional>
namespace ttyd {
// Per-session SBSS history, distinct from each BattleWork/map-heap lifetime.
// Native live memory is published here between battle runtime lifetimes.
struct BattleSpotlightState {
    std::array<uint32_t,3> work{};
    uint32_t entryColor=0; // Separate original SBSS2 WORD804295d4.
};
struct BattleSpotlightServices {
    EventMemoryServices memory;
    void* user=nullptr;
    std::optional<uint32_t> (*allocate)(void*,uint32_t bytes)=nullptr;
    bool (*free)(void*,uint32_t allocation)=nullptr;
    float (*fma)(void*,float,float,float)=nullptr;
    bool (*display)(void*,int32_t camera,int32_t displayClass,uint32_t callback,
        uint32_t argument,float priority)=nullptr;
};
// Entry's source domain requires a free slot in the allocated record bank.
uint32_t packedBattleSpotlightEntry(const BattleSpotlightServices&);
bool initializePackedBattleSpotlight(const BattleSpotlightServices&);
bool packedBattleSpotlightMain(const BattleSpotlightServices&);
bool freePackedBattleSpotlight(const BattleSpotlightServices&);
}
