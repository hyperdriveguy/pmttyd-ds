#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
#include <memory>
#include <optional>

namespace ttyd {
// Original US BattleInit800f8f04 constants, distinct from native arena capacity.
constexpr uint32_t BattleColdWorkBytes = 0x19098;
constexpr uint32_t BattleColdPointerCell = 0x8041eb90;

struct BattleColdInitRequest {
    uint32_t function = 0;
    uint32_t callsite = 0;
    std::array<uint32_t, 3> arguments{};
    unsigned argumentCount = 0;
};
struct BattleColdInitServices {
    EventMemoryServices memory;
    std::shared_ptr<const void> lifetime;
    void* user = nullptr;
    bool (*current)(void*) = nullptr;
    // Engaged zero is a completed original call, including an allocation
    // returning null. nullopt means this exact child still owns pending work.
    std::optional<uint32_t> (*call)(void*, const BattleColdInitRequest&) = nullptr;
};
class BattleColdInitCursor {
public:
    BattleColdInitCursor();
    ~BattleColdInitCursor();
    BattleColdInitCursor(const BattleColdInitCursor&) = delete;
    BattleColdInitCursor& operator=(const BattleColdInitCursor&) = delete;
    bool active() const;
    uint32_t pending() const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_ = false;
    friend bool resumePackedBattleColdInit(uint32_t, const BattleColdInitServices&,
                                           BattleColdInitCursor&);
};
// Whole cold parent800f8f04..800f9038. Does not create units, run btlseqInit,
// or declare any omitted initializer complete. Allocation/memset/initializers
// require real child owners; successful children and stores are never replayed.
bool resumePackedBattleColdInit(uint32_t information, const BattleColdInitServices&,
                               BattleColdInitCursor&);
}
