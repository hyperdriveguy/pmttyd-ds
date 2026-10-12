#pragma once
#include "battle_timing_a.hpp"

namespace ttyd {
constexpr uint32_t BattleHeldLeft = 0x40000;

// Applies only to weapons already bound to the ordinary Hammer core event.
int ordinaryHammerPeriod(uint32_t originalWeaponAddress);
int attackDifficulty(int base, uint8_t simplifiers, uint8_t unsimplifiers);

struct HammerContext {
    uint32_t held = 0, setupOptions = 0, actorFlags = 0;
    bool automatic = false;
    int difficulty = 3;
};
struct HammerSoundSink {
    void* user = nullptr;
    int32_t (*play)(void*, const char*) = nullptr;
};

// Local US ID3 contract. Sound requests use original keys; voice/macro playback
// stays in platform services. This is not the original attack event or tutorial.
struct HammerCommand {
    int32_t state = 0, count = 0, classification = 0;
    int32_t chargeSteps = 4, period = 25, countdown = 0, stage = 0;
    int32_t stepCount = 0, finishCountdown = 0, specialDelay = 0;
    int32_t suppression = 0, suppressionLatch = 0;
    int16_t forcedCounter = 0;
    uint32_t mode = 0, result = 0;
    bool enteredCharge = false, finalSound = false;
    bool mainLive = true, resultLive = true, displayLive = true, deleteLive = true;
    std::array<uint32_t, 11> scratch{};
    std::array<int32_t, 12> handles{};
    const char* soundRequest = nullptr; // Current invocation's request, if any.

    static int releaseAllowance(int difficulty);
    // Returns -1 without state mutation for unsupported native parameter domains
    // (N outside1..12, negative T or difficulty outside0..6). Retail ordinary N=4.
    int tick(const HammerContext&, BattleRandom&, HammerSoundSink sounds = {});
    bool pending() const { return (result&1)!=0; }
    void stop() { state=1005; } // Original delete callback's single state write.
};
}
