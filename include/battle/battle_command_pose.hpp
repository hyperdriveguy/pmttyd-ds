#pragma once
#include "battle_actor.hpp"
#include <optional>
namespace ttyd {
struct BattleCommandPoseServices {
  BattleActorServices memory;
  void *user = nullptr;
  std::optional<bool> (*status)(void *, uint32_t unit, int32_t id) = nullptr;
  // Original strcpy provider into part+1c4. False means unavailable before
  // copy.
  bool (*copyName)(void *, uint32_t part, uint32_t sourceName) = nullptr;
};
int32_t packedBattleBodyPartId(uint32_t unit, const EventMemoryServices &);
std::optional<int32_t>
packedBattleStatusPoseType(uint32_t unit, const BattleCommandPoseServices &);
void packedBattlePoseAnime(uint32_t part, const EventMemoryServices &);
bool packedBattleSetAnimationType(uint32_t part, int32_t type,
                                  const BattleCommandPoseServices &);
bool packedBattleCommandPose(uint32_t unit, const BattleCommandPoseServices &);
} // namespace ttyd
