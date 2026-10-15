#pragma once
#include "battle_act_draw_continuation.hpp"
namespace ttyd {
// Source BattleMenuDisp_ActSelect_Disp80116cb8/20: its passed battle argument
// is unused. DrawMainMenu captures its own current-global battle on first entry.
bool resumePackedBattleClassDisplay(uint32_t battle,
                                    const BattleActDrawServices&,
                                    BattleActDrawCursor&);
struct BattleClassDisplayServices {
    BattleActDrawServices draw;
    void* user=nullptr;
    // Readiness acknowledgement, NOT the source lookup's scalar result.
    // Stack output starts unwritten and is unused after the completed call.
    bool (*cursorLookup)(void*,uint32_t menu,uint32_t type,
                         std::optional<uint32_t>& output)=nullptr;
};
struct BattleClassDisplayCursor {
    bool active=false,lookupComplete=false;
    uint32_t battle=0;
    std::optional<uint32_t> lookupOutput;
    BattleClassDisplayServices owner;
    BattleActDrawCursor draw;
};
// Complete queued BattleCommandDisplay_ActMenuDisp8012210c/40. Camera argument
// is ignored. Retain passed B for lookup(B+171c,14,SP8), acknowledge that call
// once, then run the wrapper's retained DrawMainMenu. No queue insertion or
// invented partial-publication policy is provided. Provider tables/owners must
// remain identical while pending; resuming never replays the completed lookup.
bool resumePackedBattleClassQueuedDisplay(uint32_t camera,uint32_t battle,
                                          const BattleClassDisplayServices&,
                                          BattleClassDisplayCursor&);
}
