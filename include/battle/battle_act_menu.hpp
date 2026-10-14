#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
namespace ttyd {
enum class BattleActService : uint32_t {
  Unit = 0x800f88cc,
  Mario = 0x800f87a8,
  Party = 0x800f8728,
  Status = 0x80127814,
  Cursor = 0x80124cb8,
  ClassTable = 0x80124dc8,
  Init = 0x80116cdc,
  End = 0x80116c78,
  Display = 0x80116cd8,
  Position = 0x8012214c,
  Allowed = 0x80119df8,
  Trigger = 0x8011a06c,
  Repeat = 0x80119f60,
  Now = 0x80119fe0,
  WeaponTable = 0x80123ec0,
  TargetCount = 0x80123d00,
  Sound = 0x800d98bc,
  CameraMode = 0x8012c028,
  CameraSpeed = 0x8012b958,
  Audience = 0x8011f524
};
struct BattleActRequest {
  BattleActService function;
  std::array<uint32_t, 4> arguments{};
  uint8_t count = 0;
};
struct BattleActMenuServices {
  BattleActorServices memory;
  void *user = nullptr;
  uint32_t privateCursor = 0;
  std::optional<uint32_t> (*request)(void *,
                                     const BattleActRequest &) = nullptr;
};
void initializePackedBattleActCursor(uint32_t, const EventMemoryServices &);
bool setupPackedBattleActMenu(uint32_t battle, uint32_t flag,
                              const BattleActMenuServices &);
bool endPackedBattleActMenu(uint32_t battle, const BattleActMenuServices &);
std::optional<int32_t>
checkPackedBattleChangePosition(uint32_t battle, const BattleActMenuServices &);
std::optional<int32_t> mainPackedBattleActMenu(uint32_t battle,
                                               const BattleActMenuServices &);
} // namespace ttyd
