#include "battle_action_help.hpp"
#include "battle_command_input.hpp"
#include "battle_packed_sampling.hpp"
#include "battle_target_selection.hpp"
#include "battle_weapon_menu_main.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
namespace ttyd {
std::optional<int32_t>
HarborTutorialPacked::commandInput(HarborTutorial &scene, void *messageUser,
                                   uint32_t (*message)(void *, uint32_t)) {
  struct Context {
    HarborTutorialPacked *p;
    HarborTutorial *scene;
    void *messageUser;
    uint32_t (*message)(void *, uint32_t);
  };
  Context c{this, &scene, messageUser, message};
  using F = BattleCommandService;
  scene.commandInputPending = commandUnbound;
  if (commandUnbound)
    return std::nullopt;
  // Cancel's complete command-pose provider is still missing. Admission keeps
  // the target UI/source work intact rather than executing a partial cancel.
  if (word(Battle + 0x171c) == 11 && (word(Battle + 0xf44) & 0x200)) {
    scene.commandInputPending = uint32_t(F::CommandPose);
    return std::nullopt;
  }
  BattleCommandInputServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = &c;
  s.privateCursor = MenuRam + 0x18;
  s.request =
      [](void *self,
         const BattleCommandInputRequest &r) -> std::optional<uint32_t> {
    auto &c = *static_cast<Context *>(self);
    auto &p = *c.p;
    const auto &a = r.arguments;
    auto missing = [&]() -> std::optional<uint32_t> {
      c.scene->commandInputPending = uint32_t(r.function);
      return std::nullopt;
    };
    switch (r.function) {
    case F::Unit:
      return battleRegistryUnit(a[0], int32_t(a[1]),
                                nativeBattleActorServices(p.vm));
    case F::Cursor: {
      uint32_t cursor = 0;
      if (findPackedCommandCursor(a[0], a[1], cursor,
                                  nativeBattlePackedSelectionServices(p.vm)))
        p.word(a[2], cursor);
      return 0;
    }
    case F::PresentItem: {
      const auto b = p.word(0x8041eb90);
      const auto item = int32_t(p.word(p.word(0x8041eb90) + 0x2858 + 0x137e4));
      if (item >= 339)
        return 0;
      const auto target =
          int32_t(p.word(p.word(0x8041eb90) + 0x2858 + 0x137ec));
      if (target < -1 || target >= 64)
        return 0;
      (void)battleRegistryUnit(b, target, nativeBattleActorServices(p.vm));
      if (item == 0)
        return 0;       // Complete ordinary None-item exit after actual lookup.
      return missing(); // Gift effects/state changes require their original
                        // owner.
    }
    case F::Restore: {
      const auto b = p.word(0x8041eb90);
      if (!p.byte(b + 0x1c74))
        p.word(b + 0x1c70, p.word(b + 0x171c));
      else if (p.word(b + 0x1c70) == p.word(b + 0x171c)) {
        p.word(b + 0x1c70, 0);
        p.byte(b + 0x1c74, 0);
      }
      return 0;
    }
    case F::HelpKey:
      packedBattleActionHelpSetKey(a[0], nativeBattleActorServices(p.vm));
      return 0;
    case F::HelpDisplay:
      packedBattleActionHelpSetDisplay(int32_t(a[0]),
                                       nativeBattleActorServices(p.vm));
      return 0;
    case F::WeaponMain: {
      const auto value = p.inputWeaponMenu(*c.scene, c.messageUser, c.message);
      return value ? std::optional<uint32_t>(uint32_t(*value)) : missing();
    }
    case F::WeaponSetup:
      return p.openWeaponMenu(a[1], *c.scene, c.messageUser, c.message, a[2])
                 ? std::optional<uint32_t>(0)
                 : missing();
    case F::WeaponDecide:
      return p.selectWeapon(a[2], *c.scene, c.messageUser, c.message)
                 ? std::optional<uint32_t>(0)
                 : missing();
    case F::TargetSetup:
      c.scene->weaponTargetMenuOpen = true;
      return 0;
    case F::LightOn:
    case F::LightOnEnemy:
    case F::LightOffEnemy:
    case F::LightOffFriend:
    case F::LightOff:
      return 0; // Original complete leaves are blr.
    case F::Trigger:
      return p.word(Battle + 0xf44) & a[0];
    case F::Sound:
      c.scene->weaponMenuSounds.push_back(a[0]);
      return 0;
    case F::SelectAttack:
      if (a[0] != Units + 0x1000 || a[1] != Battle + 0x171c)
        return missing();
      return p.selectAttackEvent() ? std::optional<uint32_t>(0) : missing();
    default:
      return missing();
    }
  };
  s.target = [](void *self, uint32_t b, uint32_t u, uint32_t cursorSlot) {
    auto &c = *static_cast<Context *>(self);
    auto &p = *c.p;
    BattleTargetSelectionServices t;
    t.memory = nativeBattleActorServices(p.vm);
    t.user = &c;
    t.privateCursor = cursorSlot;
    t.cursor = [](void *self, uint32_t menu, uint32_t type, uint32_t out) {
      auto &p = *static_cast<Context *>(self)->p;
      uint32_t cursor = 0;
      if (findPackedCommandCursor(menu, type, cursor,
                                  nativeBattlePackedSelectionServices(p.vm)))
        p.word(out, cursor);
    };
    t.refresh = [](void *self, uint32_t buffer) {
      auto &p = *static_cast<Context *>(self)->p;
      updatePackedBattleCandidates({nativeBattleActorServices(p.vm), buffer},
                                   nativeBattleSamplingServices(p.vm));
      return true;
    };
    t.repeat = [](void *self, uint32_t mask) {
      return static_cast<Context *>(self)->p->word(Battle + 0xfbc) & mask;
    };
    t.trigger = [](void *self, uint32_t mask) {
      return static_cast<Context *>(self)->p->word(Battle + 0xf44) & mask;
    };
    t.sound = [](void *self, uint32_t key) {
      static_cast<Context *>(self)->scene->weaponMenuSounds.push_back(key);
    };
    t.endTarget = [](void *self, uint32_t) {
      static_cast<Context *>(self)->scene->weaponTargetMenuOpen = false;
      return true;
    };
    t.help = [](void *self, int32_t mode) {
      packedBattleActionHelpSetDisplay(
          mode, nativeBattleActorServices(static_cast<Context *>(self)->p->vm));
    };
    t.commandPose = [](void *self, uint32_t) {
      static_cast<Context *>(self)->scene->commandInputPending =
          uint32_t(F::CommandPose);
      return false;
    };
    t.offEnemyLight = [](void *) {};
    t.offLight = [](void *, int32_t, int32_t) {
    }; // Verified original blr bodies.
    t.pay = [](void *self, uint32_t, uint32_t) {
      static_cast<Context *>(self)->scene->commandInputPending = 0x80126840;
      return false;
    };
    t.audience = [](void *self) -> std::optional<bool> {
      auto &c = *static_cast<Context *>(self);
      BattleMenuAudienceServices a;
      a.memory = nativeBattleActorServices(c.p->vm);
      a.user = &c;
      a.privateItem = MenuRam + 0x14;
      a.trigger = [](void *self, uint32_t mask) {
        return static_cast<Context *>(self)->p->word(Battle + 0xf44) & mask;
      };
      a.mario = [](void *self, uint32_t b) {
        return battleMarioUnit(
            b, nativeBattleActorServices(static_cast<Context *>(self)->p->vm));
      };
      a.itemOn = [](void *self, uint32_t, int32_t, int32_t, int32_t, int32_t) {
        static_cast<Context *>(self)->scene->commandInputPending = 0x801a1520;
        return false;
      };
      return packedBattleMenuAudienceCheck(a);
    };
    return processPackedBattleTargetSelection(b, u, t);
  };
  const auto result = packedBattleCommandInput(Battle, s);
  if (!result)
    commandUnbound =
        scene.commandInputPending ? scene.commandInputPending : 0x801229d8;
  if (result && *result == 1)
    commandFinalized =
        true; // Native source29 admission latch; no root/HP/result synthesis.
  return result;
}
} // namespace ttyd
