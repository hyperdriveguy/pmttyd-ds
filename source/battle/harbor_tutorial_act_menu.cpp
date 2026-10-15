#include "battle_act_menu.hpp"
#include "battle_act_ui.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_packed_status.hpp"
#include "battle_weapon_menu_main.hpp"
#include "event_heap_allocation.hpp"
#include "event_memory_release.hpp"
#include "harbor_tutorial_packed.hpp"
namespace ttyd {
bool HarborTutorialPacked::classUiRelease(uint32_t address) {
  // The legacy weapon UI owns a fixed projection, not an SDK allocation.
  if (address == MenuRam + 0xc00)
    return true;
  if (!classUiAllocations.erase(address))
    return false;
  eventSdkFree(0, address, vm.memoryBinding.services());
  return true;
}
bool HarborTutorialPacked::menuAudienceItemOn(uint32_t output){
 packedBattleAudienceItemOn({output,0,0,0,0},nativeBattleActorServices(vm));
 return true;
}
std::optional<int32_t>
HarborTutorialPacked::actMenu(HarborTutorial &scene, void *messageUser,
                              uint32_t (*message)(void *, uint32_t), bool setup,
                              uint32_t flag) {
  struct Context {
    HarborTutorialPacked *p;
    HarborTutorial *scene;
    void *messageUser;
    uint32_t (*message)(void *, uint32_t);
  };
  Context c{this, &scene, messageUser, message};
  using F = BattleActService;
  BattleActMenuServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = &c;
  s.privateCursor = MenuRam + 0x1c;
  s.request = [](void *self,
                 const BattleActRequest &r) -> std::optional<uint32_t> {
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
    case F::Mario:
      return battleMarioUnit(a[0], nativeBattleActorServices(p.vm));
    case F::Party:
      return battlePartyUnit(a[0], nativeBattleActorServices(p.vm));
    case F::Status:
      return checkPackedBattleStatus(a[0], int32_t(a[1]),
                                     nativeBattleActorServices(p.vm))
                 ? 1
                 : 0;
    case F::Cursor: {
      uint32_t at = 0;
      if (findPackedCommandCursor(a[0], a[1], at,
                                  nativeBattlePackedSelectionServices(p.vm)))
        p.word(a[2], at);
      return 0;
    }
    case F::ClassTable:
      return p.buildClassMenu(*c.scene, c.messageUser, c.message)
                 ? std::optional<uint32_t>(p.word(Battle + 0x171c + 0x498 + 8))
                 : missing();
    case F::Init:
    case F::End: {
      BattleActUiServices ui;
      ui.memory = nativeBattleActorServices(p.vm);
      ui.user = &p;
      ui.release = [](void *self, uint32_t at) {
        return static_cast<HarborTutorialPacked *>(self)->classUiRelease(at);
      };
      ui.allocate = [](void *self, uint32_t size) -> std::optional<uint32_t> {
        auto &p = *static_cast<HarborTutorialPacked *>(self);
        const auto at =
            eventSdkAllocate(0, size, p.vm.memoryBinding.services());
        if (!at)
          return std::nullopt;
        p.classUiAllocations.insert(at);
        return at;
      };
      const bool ready =
          r.function == F::Init
              ? initializePackedBattleActUi(a[0], a[1], a[2], a[3], ui)
              : endPackedBattleActUi(a[0], ui);
      if (!ready)
        return missing();
      c.scene->classMenuOpen = r.function == F::Init;
      return 0;
    }
    case F::Position: {
      BattleActMenuServices position;
      position.memory = nativeBattleActorServices(p.vm);
      position.user = &c;
      position.request =
          [](void *self, const BattleActRequest &r) -> std::optional<uint32_t> {
        auto &c = *static_cast<Context *>(self);
        auto &p = *c.p;
        const auto &a = r.arguments;
        if (r.function == F::Mario)
          return battleMarioUnit(a[0], nativeBattleActorServices(p.vm));
        if (r.function == F::Party)
          return battlePartyUnit(a[0], nativeBattleActorServices(p.vm));
        if (r.function == F::Status)
          return checkPackedBattleStatus(a[0], int32_t(a[1]),
                                         nativeBattleActorServices(p.vm))
                     ? 1
                     : 0;
        return std::nullopt;
      };
      const auto result = checkPackedBattleChangePosition(a[0], position);
      return result ? std::optional<uint32_t>(uint32_t(*result)) : missing();
    }
    case F::Allowed:
      return packedBattleMenuKeyAllowed(a[0], nativeBattleActorServices(p.vm))
                 ? 1
                 : 0;
    case F::Trigger:
      return p.word(Battle + 0xf44) & a[0];
    case F::Repeat:
      return p.word(Battle + 0xfbc) & a[0];
    case F::Now:
      return p.word(Battle + 0x1034) & a[0];
    case F::Sound:
      c.scene->weaponMenuSounds.push_back(a[0]);
      return 0;
    case F::WeaponTable:
      return p.buildWeaponMenu(a[1], *c.scene, c.messageUser, c.message)
                 ? std::optional<uint32_t>(0)
                 : missing();
    case F::TargetCount: {
      const auto count = p.countWeaponTargets(a[0], a[1], a[2]);
      return count ? std::optional<uint32_t>(uint32_t(*count)) : missing();
    }
    case F::CameraMode:
      setBattleCameraMode(Battle, int32_t(a[0]), int32_t(a[1]), p.camera);
      return 0;
    case F::CameraSpeed:
      setBattleCameraSpeed(Battle, int32_t(a[0]), int32_t(a[1]), p.camera);
      return 0;
    case F::Audience: {
      const auto result = p.attackAudienceMenu(*c.scene);
      return result ? std::optional<uint32_t>(*result ? 1 : 0) : missing();
    }
    case F::Display:
      return 0; // Complete original ActSelect Main is blr.
    }
    return missing();
  };
  const auto result = setup ? (setupPackedBattleActMenu(Battle, flag, s)
                                   ? std::optional<int32_t>(0)
                                   : std::nullopt)
                            : mainPackedBattleActMenu(Battle, s);
  captureClassRows(scene);
  return result;
}
} // namespace ttyd
