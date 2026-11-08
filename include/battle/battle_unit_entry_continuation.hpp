#pragma once
#include "battle_actor.hpp"
#include "event_creation.hpp"
#include <array>
#include <memory>
#include <optional>

namespace ttyd {
struct BattleUnitEntryRequest {
    uint32_t function = 0, callsite = 0;
    std::array<uint32_t, 3> integers{};
    std::array<float, 3> vector{};
};
struct BattleUnitEntryOutput {
    uint32_t value = 0;
    EventEntry* event = nullptr;
};
struct BattleUnitEntryContinuationServices {
    BattleActorServices actors;
    std::shared_ptr<const void> lifetime;
    void* user = nullptr;
    bool (*current)(void*) = nullptr;
    std::optional<BattleUnitEntryOutput> (*call)(void*, const BattleUnitEntryRequest&) = nullptr;
};
class BattleUnitEntryCursor {
public:
    BattleUnitEntryCursor();
    ~BattleUnitEntryCursor();
    BattleUnitEntryCursor(const BattleUnitEntryCursor&) = delete;
    BattleUnitEntryCursor& operator=(const BattleUnitEntryCursor&) = delete;
    uint32_t pending() const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_ = false;
    friend std::optional<uint32_t> resumePackedBattleUnitEntry(uint32_t, uint32_t,
        const BattleUnitEntryContinuationServices&, BattleUnitEntryCursor&);
};
// Complete source constructor80128fe0. An engaged zero is an original return0;
// nullopt retains the exact child request and all already-published work.
// Guest storage and source table domains must be admitted by the owner.
std::optional<uint32_t> resumePackedBattleUnitEntry(uint32_t battle, uint32_t definition,
    const BattleUnitEntryContinuationServices&, BattleUnitEntryCursor&);
class BattleDisplayEntryCursor {
public:
    BattleDisplayEntryCursor();
    ~BattleDisplayEntryCursor();
    BattleDisplayEntryCursor(const BattleDisplayEntryCursor&) = delete;
    BattleDisplayEntryCursor& operator=(const BattleDisplayEntryCursor&) = delete;
    uint32_t pending() const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_ = false;
    friend bool resumePackedBattleDisplayEntry(uint32_t,
        const BattleUnitEntryContinuationServices&, BattleDisplayEntryCursor&);
};
// Whole btlDispEntAnime801011fc, including independently held search, string
// copy and actual source pose Entry. A previous part's handle is never replayed.
bool resumePackedBattleDisplayEntry(uint32_t unit,
    const BattleUnitEntryContinuationServices&, BattleDisplayEntryCursor&);
}
