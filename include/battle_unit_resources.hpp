#pragma once
#include "battle_damage.hpp"

namespace ttyd {
// Semantic BattleWork records, not original disk-save records.
struct PartnerBattleSnapshot {
    float value=0;
    std::array<uint8_t,30> statuses{};
    std::array<uint8_t,2> retainedGap{};
    uint32_t flags=0;
    uint16_t field28=0,field2a=0;
};
struct UnitResourceServices {
    void* user=nullptr;
    // Original pouch byte records use big endian halfwords. Storage must
    // cover pouch offsets through0x91. Called separately by import and revision.
    uint8_t* (*pouch)(void*)=nullptr;
    std::array<PartnerBattleSnapshot,7>* snapshots=nullptr;
};
int32_t battlePartyIndex(int32_t kind);
void initializeStatusChangeInfo(BattleStatusUnit&);
void clearUnitStatus(BattleStatusUnit&);
void reviseUnitResources(DamageUnit&,const UnitResourceServices&);
void importUnitResources(DamageUnit&,const UnitResourceServices&);
void exportUnitResources(const DamageUnit&,const UnitResourceServices&);
}
