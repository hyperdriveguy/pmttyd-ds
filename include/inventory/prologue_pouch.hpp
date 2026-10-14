#pragma once
#include <array>
#include <cstdint>
#include <memory>
namespace ttyd {
// Original packed player storage, retained across native field-room rebuilds.
// Legacy checkpoints omit this block; native v8 preserves its original bytes.
struct ProloguePouch {
    std::array<uint8_t,0x5d4> bytes{};
    int16_t half(unsigned offset) const;
    void half(unsigned offset,int16_t value);
    int32_t keyItemCount(int32_t item) const;
    int32_t removeKeyItem(int32_t item); // Low-level0/1 only; revision/event wrapper is separate.
    int32_t insertKeyItem(int32_t item); // Array branch only, not full pouchGetItem dispatch.
    int32_t addCoins(int32_t amount); // Original low16 store, signed reread, then0..999 clamp.
};
// Pouch portion of original stg0_00_init: initialize, grant6/32hex/9 in source
// order, set100 coins and revise the genuinely fresh unequipped resources.
ProloguePouch initializeProloguePouch(const char* originalYoshiName);
bool loadProloguePouchAssets(const char* root);
std::shared_ptr<ProloguePouch> createProloguePouch();
}
