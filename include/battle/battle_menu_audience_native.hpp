#pragma once
#include "battle_actor.hpp"
#include <memory>
#include <optional>
namespace ttyd {
struct BattleMenuAudienceNativeServices {
  BattleActorServices actors;
  void* user=nullptr;
  // Caller freezes the actual VM allocation through synchronous memory/query
  // and SetTarget primitives. A lost lease poisons this invocation until reset.
  bool (*allocationCurrent)(void*)=nullptr;
  std::optional<uint32_t> (*trigger)(void*,uint32_t mask)=nullptr;
  std::optional<uint32_t> (*mario)(void*,uint32_t battle)=nullptr;
  bool (*endActMenu)(void*,uint32_t battle)=nullptr;
};
// One actual physical-X invocation. Pins provider identities and native index
// independently of query cache. Reset abandons metadata,not prior target/cache
// effects. Allocation proof guards groups; caller freezes their synchronous RAM.
class BattleMenuAudienceNativeCursor {
public:
  BattleMenuAudienceNativeCursor();~BattleMenuAudienceNativeCursor();
  BattleMenuAudienceNativeCursor(const BattleMenuAudienceNativeCursor&)=delete;
  BattleMenuAudienceNativeCursor& operator=(const BattleMenuAudienceNativeCursor&)=delete;
  bool active()const;bool stale()const;uint32_t pending()const;bool reset();
private:
  struct State;std::unique_ptr<State> state_;bool invoking_=false;
  friend std::optional<bool> resumeNativeBattleMenuAudience(
      const BattleMenuAudienceNativeServices&,BattleMenuAudienceNativeCursor&);
};
// Original physical-X check8011f524 with its actual GetItemOn call specialized
// to a disjoint native index output and four absent outputs. First eligible
// includes good items, exactly as original X; selected harmful taps are separate.
// Engaged false/true are completed original results; nullopt retains a child.
std::optional<bool> resumeNativeBattleMenuAudience(
    const BattleMenuAudienceNativeServices&,BattleMenuAudienceNativeCursor&);
}
