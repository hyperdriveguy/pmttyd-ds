#pragma once
#include <array>
#include <cstdint>
#include <memory>
namespace ttyd {
// Original packed player storage, retained across native field-room rebuilds.
// Native legacy checkpoint files do not yet serialize this original block.
struct ProloguePouch {
    std::array<uint8_t,0x5d4> bytes{};
    int16_t half(unsigned offset) const;
    void half(unsigned offset,int16_t value);
};
// Pouch portion of original stg0_00_init: initialize, grant6/32hex/9 in source
// order, set100 coins and revise the genuinely fresh unequipped resources.
ProloguePouch initializeProloguePouch(const char* originalYoshiName);
bool loadProloguePouchAssets(const char* root);
std::shared_ptr<ProloguePouch> createProloguePouch();
}
