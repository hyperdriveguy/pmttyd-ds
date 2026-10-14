#pragma once
#include <array>
#include <cstdint>

namespace ttyd {
constexpr uint32_t BattleButtonA = 0x100;

// General battle trigger history (+0x0f44), not the private AC/defense pad.
// The caller supplies one already sampled press-edge word per simulation tick.
struct BattleTriggerHistory {
    // Thirty newest-first source words; index0 is the current sampled tick.
    std::array<uint32_t, 30> entries{};
    void push(uint32_t triggers);
};

// US rand/irand arithmetic. Seed ownership is deliberately supplied by callers:
// the original new-game/load seed contract has not been recovered.
class BattleRandom {
public:
    explicit BattleRandom(uint32_t seed) : state(seed) {}
    uint32_t state;
    int32_t next();
    int32_t bounded(int32_t bound);
};

struct TimingAContext {
    int difficulty = 3; // Caller must supply the established range 0..6.
    uint32_t setupOptions = 0;
    uint32_t actorFlags = 0;
    uint8_t automatic = 0;
    uint32_t battleFlags = 0;
};

// Local US ID1 callback contract; presentation, script cadence and impact
// alignment belong to callers. See docs/behavior/battle/BATTLE_TIMING_A_COMMAND.md.
struct TimingACommand {
    // Result bit1 success; classification0 unset,1 mash/press failure,2 no
    // press,3 ordinary success. Main return1 keeps callback,0 retires it.
    int32_t state = 0, count = 0, classification = 0, finishCountdown = 0;
    uint32_t result = 0, flags = 0;
    std::array<uint32_t, 11> scratch{};
    bool callbacksLive = true;

    // Invalid difficulty is rejected with -1 at this native API boundary.
    int successWindow(int difficulty) const;
    uint32_t evaluate(const BattleTriggerHistory&, const TimingAContext&, BattleRandom&);
    int tick();
};
}
