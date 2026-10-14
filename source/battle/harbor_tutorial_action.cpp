#include "battle_action_events.hpp"
#include "battle_action_manager.hpp"
#include "battle_packed_pad.hpp"
#include "harbor_tutorial_packed.hpp"
namespace ttyd {
bool HarborTutorialPacked::actionManager(uint64_t time) {
  if (actionManagedAt == time)
    return true;
  // Missing platform sample does not fabricate an empty pad update or advance
  // source callbacks. Render retries also do not consume another simulation
  // tick.
  actionDisplays.reinitialize();
  if (!physicalCurrent || sampledAt != time)
    return true;
  actionManagedAt = time;
  BattleActionManagerServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = this;
  s.pad = [](void *self, uint32_t address, int32_t channel) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    if (channel != 0 || address != Battle + 0x1d1c)
      return false;
    auto pad = readPackedBattlePad(address, nativeBattleActorServices(p.vm));
    pad.update(p.sampledPhysical);
    writePackedBattlePad(address, pad, nativeBattleActorServices(p.vm));
    return true;
  };
  s.main = [](void *self, uint32_t callback,
              uint32_t b) -> std::optional<int32_t> {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    if (callback != 0x8012aae0)
      return std::nullopt;
    TimingACommand c;
    c.state = int32_t(p.word(b + 0x1c9c));
    c.finishCountdown = int32_t(p.word(b + 0x1f54));
    c.flags = p.word(b + 0x1f50);
    c.classification = int32_t(p.word(b + 0x1ce8));
    for (unsigned i = 0; i < c.scratch.size(); ++i)
      c.scratch[i] = p.word(b + 0x1f20 + 4 * i);
    const auto result = c.tick();
    p.word(b + 0x1c9c, uint32_t(c.state));
    p.word(b + 0x1f54, uint32_t(c.finishCountdown));
    p.word(b + 0x1f50, c.flags);
    p.word(b + 0x1ce8, uint32_t(c.classification));
    for (unsigned i = 0; i < c.scratch.size(); ++i)
      p.word(b + 0x1f20 + 4 * i, c.scratch[i]);
    // State1002's four clears belong to TimingA Main; the outer manager only
    // clears Main on zero and freshly rereads Display afterwards.
    if (!c.callbacksLive)
      for (const auto offset : {0x1ca0u, 0x1ca8u, 0x1ca4u, 0x1cacu})
        p.word(b + offset, 0);
    return result;
  };
  s.display = [](void *self, int32_t camera, int32_t drawClass,
                 uint32_t callback, uint32_t argument, float priority) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    return p.actionDisplays.enter(uint32_t(camera), drawClass, callback,
                                  argument,
                                  priority) == DisplayAdmission::Registered;
  };
  return packedBattleActionCommandManager(Battle, s);
}

bool HarborTutorialPacked::actionEvent(EventEntry &e, uint32_t function,
                                       int32_t &result) {
  BattleActionEventServices s;
  s.memory = nativeBattleActorServices(vm);
  s.values = nativeBattleUnitEventServices(vm).values;
  s.user = this;
  s.evaluate = [](void *self, uint32_t b,
                  uint32_t callback) -> std::optional<int32_t> {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    if (callback != 0x8012a8c4)
      return std::nullopt;
    TimingAContext context;
    context.difficulty = p.byte(b + 0x1cbd);
    context.setupOptions = p.word(b + 0x1c94);
    const auto actor = p.word(b + 0x1c90);
    context.actorFlags = p.word(actor + 0x27c);
    context.automatic = p.byte(actor + 0x307);
    context.battleFlags = p.word(b + 0xef4);
    if (context.difficulty > 6)
      return std::nullopt;
    const bool needsPad =
        !((context.setupOptions & 1) && (context.actorFlags & 0x10)) &&
        !context.automatic && !(context.battleFlags & 0x01000000);
    if (needsPad && !p.physicalCurrent)
      return std::nullopt; // Native input-provider frontier; missing is not
                           // no-input.
    TimingACommand c;
    c.flags = p.word(b + 0x1f50);
    c.count = int32_t(p.word(b + 0x1cb4));
    c.result = p.word(b + 0x1cb8);
    c.classification = int32_t(p.word(b + 0x1ce8));
    const auto general =
        readPackedBattlePad(b + 0xf2c, nativeBattleActorServices(p.vm));
    BattleTriggerHistory history;
    history.entries = general.triggers.entries;
    c.evaluate(history, context, p.random);
    p.word(b + 0x1cb8, c.result);
    p.word(b + 0x1cb4, uint32_t(c.count));
    p.word(b + 0x1ce8, uint32_t(c.classification));
    return int32_t(c.result);
  };
  s.remove = [](void *self, uint32_t b, uint32_t callback) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    if (callback != 0x8012a7c0)
      return false;
    p.word(b + 0x1c9c, 1000);
    return true;
  };
  if (function == 0x80107660 || function == 0x80107548) {
    result = packedBattleReadActionEvent(e, function == 0x80107548, s);
    return true;
  }
  const auto completion = function == 0x80107708
                              ? packedBattleResultActionEvent(e, s)
                              : packedBattleStopActionEvent(e, s);
  if (!completion)
    return false;
  result = *completion;
  return true;
}
} // namespace ttyd
