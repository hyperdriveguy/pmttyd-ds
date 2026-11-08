#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
// Private source stack words retain their original offsets; no guest SP/address
// is invented. Unknown words remain unknown, including the saved-register area.
struct BattleBreakSlotInitRequest {
    uint32_t function=0, callsite=0;
    uint32_t argument=0, bytes=0;
    std::optional<uint32_t> privateSourceOffset;
    std::array<std::optional<uint32_t>,28> privateWords{};
};
struct BattleBreakSlotInitServices {
    BattleActorServices actors;
    std::shared_ptr<const void> lifetime;
    void* user=nullptr;
    bool (*current)(void*)=nullptr;
    std::optional<uint32_t> (*call)(void*,const BattleBreakSlotInitRequest&)=nullptr;
};
class BattleBreakSlotInitCursor {
public:
    BattleBreakSlotInitCursor();
    ~BattleBreakSlotInitCursor();
    BattleBreakSlotInitCursor(const BattleBreakSlotInitCursor&)=delete;
    BattleBreakSlotInitCursor& operator=(const BattleBreakSlotInitCursor&)=delete;
    bool active() const;
    uint32_t pending() const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_=false;
    friend bool resumePackedBattleBreakSlotInit(const BattleBreakSlotInitServices&,BattleBreakSlotInitCursor&);
};
// Whole original8020507c..80205210. A successful void child returns engaged0;
// nullopt holds its exact request. Admission covers every parent memory access.
bool resumePackedBattleBreakSlotInit(const BattleBreakSlotInitServices&,BattleBreakSlotInitCursor&);
}
