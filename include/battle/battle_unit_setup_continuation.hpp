#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>

namespace ttyd {
struct BattleUnitSetupRequest {
    uint32_t function = 0, callsite = 0;
    std::array<uint32_t, 3> integers{};
    std::array<float, 3> position{};
};
struct BattleUnitSetupOutput {
    uint32_t value = 0;
    std::array<float, 3> position{};
};
struct BattleUnitSetupContinuationServices {
    BattleActorServices actors;
    std::shared_ptr<const void> lifetime;
    void* user = nullptr;
    bool (*current)(void*) = nullptr;
    std::optional<BattleUnitSetupOutput> (*call)(void*, const BattleUnitSetupRequest&) = nullptr;
};
class BattleUnitSetupCursor {
public:
    BattleUnitSetupCursor();
    ~BattleUnitSetupCursor();
    BattleUnitSetupCursor(const BattleUnitSetupCursor&) = delete;
    BattleUnitSetupCursor& operator=(const BattleUnitSetupCursor&) = delete;
    bool active() const;
    uint32_t pending() const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_ = false;
    friend bool resumePackedBattleUnitSetup(uint32_t, const BattleUnitSetupContinuationServices&,
                                           BattleUnitSetupCursor&);
};
// Complete original800f8aac parent. true means original return1; null child
// results retain this invocation. UnitEntry/equipment/import own their partial
// effects independently. All guest memory used by this parent must be admitted.
bool resumePackedBattleUnitSetup(uint32_t battle, const BattleUnitSetupContinuationServices&,
                                BattleUnitSetupCursor&);
}
