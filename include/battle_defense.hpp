#pragma once
#include "battle_timing_a.hpp"

namespace ttyd {
constexpr uint32_t BattleButtonB = 0x200;
// Semantic state, not the original packed PowerPC structure/ABI.
struct BattlePadSnapshot {
    uint16_t buttons = 0;
    int8_t mainX = 0, mainY = 0, subX = 0, subY = 0;
    uint8_t analogA = 0, analogB = 0;
};
struct BattlePadSample {
    BattlePadSnapshot snapshot;
    // Already sampled producer masks; physical edges/repeat are not derived here.
    uint32_t pressed = 0, repeat = 0, released = 0;
};
struct BattlePadWork {
    BattlePadSnapshot current, previous;
    BattleTriggerHistory triggers, repeats, held;
    std::array<uint32_t, 30> released{};
    uint8_t repeatPhase = 0, repeatCounter = 0;
    void init(); // Original WorkInit leaves the release array untouched.
    void update(const BattlePadSample&);
};

struct DefenseContext {
    int16_t hp = 10;
    uint8_t simplifiers = 0, unsimplifiers = 0, automatic = 0;
    uint8_t weaponGate = 0; // Original second-argument byte+0x13, not inferred eligibility.
};
struct DefenseCommand {
    int16_t overrideCounter = 0;
    int result = 0;
    int evaluate(BattlePadWork& privatePad, const DefenseContext&);
};
}
