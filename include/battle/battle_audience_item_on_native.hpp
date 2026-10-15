#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
namespace ttyd {
// Actual original signatures: index only,or index plus XYZ. Private native
// output cells are disjoint; this admits no arbitrary guest-output alias domain.
enum class BattleAudiencePrivateOutputs { Index, IndexAndPosition };
struct BattleAudienceNativeItemOn {
  int32_t index=-1;
  // Original private XYZ words, present only on a match with four outputs.
  std::optional<std::array<uint32_t,3>> position;
};
// Whole GetItemOn for the two signatures actually used by the X checker and
// party footer. Outputs are disjoint native locals. Original cache writes and
// the four-output signature's subsequent cache reads remain source operations.
// Caller owns the actual memory allocation throughout this synchronous query.
BattleAudienceNativeItemOn nativeBattleAudienceItemOn(
    const BattleActorServices&,BattleAudiencePrivateOutputs);
}
