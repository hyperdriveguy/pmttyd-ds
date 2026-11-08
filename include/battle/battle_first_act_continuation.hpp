#pragma once
#include <memory>
#include <array>
#include <optional>
#include "battle_actor.hpp"
namespace ttyd {
struct BattleFirstActRequest {
 uint32_t function=0,callsite=0;
 // Original scalar operands. FadeEntry80013368 uses {type,rawDuration,
 // capturedRGBA}: slot2 transports its private source color contents, never
 // a fabricated guest stack pointer. Other calls retain numeric ABI operands.
 std::array<uint32_t,4> integers{};
 // Only800feaf8 writes these source-uninitialized private cells. Provider may
 // retain successful individual writes across its own wait; no guest SP alias.
 std::array<std::optional<uint32_t>,2>*firstAttackUnits=nullptr;
};
struct BattleFirstActContinuationServices {
 BattleActorServices actors;
 std::shared_ptr<const void>lifetime;
 void*user=nullptr;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*call)(void*,const BattleFirstActRequest&)=nullptr;
};
class BattleFirstActCursor {
public:
 BattleFirstActCursor();~BattleFirstActCursor();
 BattleFirstActCursor(const BattleFirstActCursor&)=delete;
 BattleFirstActCursor&operator=(const BattleFirstActCursor&)=delete;
 bool active()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 friend bool resumePackedBattleFirstAct(uint32_t,const BattleFirstActContinuationServices&,BattleFirstActCursor&);
};
// Whole8011e5c0/7f8, including nonzero first-strike entry. Each original
// invocation captures firstAttack/state once; later invocation supplies cadence.
// Engaged0 is an actual scalar return. nullopt retains this exact child prefix.
bool resumePackedBattleFirstAct(uint32_t,const BattleFirstActContinuationServices&,BattleFirstActCursor&);
}
