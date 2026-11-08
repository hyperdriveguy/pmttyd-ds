#include "battle_action_events.hpp"
#include "battle_action_manager.hpp"
#include "battle_packed_pad.hpp"
#include "field_camera_events.hpp"
#include "harbor_tutorial_packed.hpp"
#include "message_icons.hpp"
namespace ttyd {
// Simulation millisecond/sample identity gates native action work; screen
// redraws are not source command ticks. Only channel 0/private pad B+0x1d1c is
// admitted; general history at B+0xf2c is sampled separately by samplePhysical.
bool HarborTutorialPacked::actionManager(uint64_t time) {
  if (!actionManagerCursor.active()) {
    if (actionManagedAt == time)
      return battleAddress()==actionBattle&&sourceAllocationLifetime(actionBattle)==actionBattleOwner;
    // An admitted sample owns this whole manager call, including any wait.
    // Queue reset belongs only to the next invocation, never a held child.
    actionDisplays.reinitialize();
    actionDisplayReady = false;
    if (!physicalCurrent || sampledAt != time)
      return true;
    actionBattle=battleAddress();actionBattleOwner=sourceAllocationLifetime(actionBattle);
    if(!actionBattleOwner&&actionBattle!=Battle)return false;
    actionPhysical = sampledPhysical;
    actionInvocationAt = time;
  }
  if(battleAddress()!=actionBattle||sourceAllocationLifetime(actionBattle)!=actionBattleOwner)return false;
  BattleActionManagerServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = this;
  s.pad = [](void *self, uint32_t address, int32_t channel) {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    if (channel != 0 || address != p.actionBattle + 0x1d1c)
      return false;
    auto pad = readPackedBattlePad(address, nativeBattleActorServices(p.vm));
    pad.update(p.actionPhysical);
    writePackedBattlePad(address, pad, nativeBattleActorServices(p.vm));
    return true;
  };
  s.main = [](void *self, uint32_t callback,
              uint32_t b) -> std::optional<int32_t> {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    // Only original TimingA Main is bound. State+0x1c9c, countdown+0x1f54,
    // flags+0x1f50, classification+0x1ce8 and scratch+0x1f20 are copied around tick();
    // callbacks+0x1ca0/0x1ca8/0x1ca4/0x1cac are Main/Display/Result/Delete slots.
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
  if (!resumePackedBattleActionCommandManager(actionBattle, s, actionManagerCursor))
    return false;
  actionManagedAt = actionInvocationAt;
  actionDisplayReady = true;
  return true;
}

// Only camera 8/original TimingA Display 0x8012a7cc with native Battle userdata
// is admitted. Icons 108/109 must both exist before callback effects. Icon
// requests feed the scene projection; this helper does not submit GPU work.
bool HarborTutorialPacked::actionDisplay(uint64_t time, HarborTutorial &scene) {
  (void)time;
  if (actionManagerCursor.active()) {
    scene.actionCommandIcons.clear();
    return false;
  }
  if (actionDisplayReady && actionDisplayedAt == actionManagedAt)
    return battleAddress()==actionBattle&&sourceAllocationLifetime(actionBattle)==actionBattleOwner;
  scene.actionCommandIcons.clear();
  if (!actionDisplayReady)
    return true;
  if(battleAddress()!=actionBattle||sourceAllocationLifetime(actionBattle)!=actionBattleOwner)return false;
  // Admission precedes callback effects. Borrowed queue records are consumed
  // once per native simulation sample, never by a repeated screen draw.
  for (uint32_t i = 0; i < actionDisplays.count(); ++i) {
    const auto *r = actionDisplays.ordered(i);
    if (!r || r->camera != 8 || r->callback != 0x8012a7cc ||
        r->userdata != actionBattle || !messageIcon(108) || !messageIcon(109))
      return false;
  }
  actionDisplayedAt = actionManagedAt;
  BattleTimingDisplayServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = &scene;
  s.interpolate = [](void *, int32_t mode, int32_t step, int32_t duration,
                     float from, float to) {
    return cameraInterpolation(mode, step, duration, from, to);
  };
  s.camera = [](void *, uint32_t camera) { return camera == 8; };
  s.icon = [](void *self, const BattleTimingIconRequest &request) {
    static_cast<HarborTutorial *>(self)->actionCommandIcons.push_back(request);
    return true;
  };
  for (uint32_t i = 0; i < actionDisplays.count(); ++i)
    if (!packedBattleTimingADisplay(actionDisplays.ordered(i)->userdata, s))
      return false;
  return true;
}

// Result/Stop invoke only admitted source callback tokens. Read-count/result
// events 0x80107660/0x80107548 complete without running evaluation. Missing physical
// input is unavailable where evaluation needs it, not a fabricated neutral pad.
bool HarborTutorialPacked::actionEvent(EventEntry &e, uint32_t function,
                                       int32_t &result) {
  BattleActionEventServices s;
  s.memory = nativeBattleActorServices(vm);
  s.values = nativeBattleUnitEventServices(vm).values;
  s.user = this;
  s.evaluate = [](void *self, uint32_t b,
                  uint32_t callback) -> std::optional<int32_t> {
    auto &p = *static_cast<HarborTutorialPacked *>(self);
    // TimingA Result: 0x1cbd difficulty 0..6; 0x1c94 options; 0x1c90 actor pointer;
    // actor+0x27c flags/0x307 automatic count; 0xef4 battle flags. Option 1 with actor
    // flag 0x10, automatic mode or battle 0x01000000 can bypass physical-pad admission.
    // Count+0x1cb4/result+0x1cb8/classification+0x1ce8 write back after actual evaluation.
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
    // Original TimingA Delete requests state 1000, not immediate callback
    // removal. Its later Main cleanup owns the actual source clear sequence.
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
