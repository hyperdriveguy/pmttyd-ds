#include "battle_action_events.hpp"
#include "battle_packed_pad.hpp"
#include "harbor_tutorial_packed.hpp"
namespace ttyd {
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
