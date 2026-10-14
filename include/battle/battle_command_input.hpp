#pragma once
#include "battle_actor.hpp"
#include <array>
#include <optional>
namespace ttyd {
// Original callee identities: scalar results are raw words, not VM completion.
enum class BattleCommandService : uint32_t {
  Unit = 0x800f88cc,
  Cursor = 0x80124cb8,
  PresentItem = 0x80120024,
  Restore = 0x8011f5f0,
  HelpDisplay = 0x8021690c,
  HelpKey = 0x80216954,
  CommandPose = 0x801264cc,
  LightOn = 0x8013899c,
  LightOffFriend = 0x80138990,
  LightOffEnemy = 0x80138988,
  LightOnEnemy = 0x8013898c,
  LightOff = 0x80138998,
  ActSetup = 0x80122794,
  ActMain = 0x8012230c,
  WeaponDecide = 0x80123210,
  WeaponSetup = 0x80122068,
  WeaponMain = 0x80121d0c,
  TargetSetup = 0x80120fe4,
  Entry = 0x800374ac,
  EventLive = 0x80036604,
  MultiCancelSetup = 0x80121c98,
  MultiCancelMain = 0x80121af4,
  MultiSetup = 0x801215b8,
  MultiMain = 0x80121354,
  OperationSetup = 0x80121928,
  OperationMain = 0x8012165c,
  PartySetup = 0x801212bc,
  PartyMain = 0x80121098,
  ProtectSetup = 0x80120990,
  ProtectEnd = 0x80120834,
  Trigger = 0x8011a06c,
  Sound = 0x800d98bc,
  SelectAttack = 0x80120268
};
struct BattleCommandInputRequest {
  BattleCommandService function;
  std::array<uint32_t, 3> arguments{};
  uint8_t count = 0;
};
struct BattleCommandInputServices {
  BattleActorServices memory;
  void *user = nullptr;
  // One native word adapts the same source SP08 output for the entire call.
  uint32_t privateCursor = 0;
  std::optional<uint32_t> (*request)(
      void *, const BattleCommandInputRequest &) = nullptr;
  // Complete source state11; the parent owns progress/re-entry and common tail.
  bool (*target)(void *, uint32_t battle, uint32_t retainedUnit,
                 uint32_t cursorSlot) = nullptr;
};
// Complete thirty-state local owner. Missing providers return nullopt after
// preceding effects. This is not a retry-safe suspended stack or a frame yield.
std::optional<int32_t>
packedBattleCommandInput(uint32_t battle, const BattleCommandInputServices &);
} // namespace ttyd
