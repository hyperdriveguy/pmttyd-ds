#include "battle_wait_attack_end_event.hpp"
#include "event_controls.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t AttackerId = 0x420, AttackEvent = 0x2a8;
int32_t signedWord(uint32_t v) {
  return v <= 0x7fffffff ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
} // namespace
int32_t battleWaitAttackEndEvent(const BattleActorServices &a,
                                 const EventCreationServices &events) {
  const auto &m = a.memory;
  assert(m.word);
  const auto battle = currentBattleAddress(a);
  const auto id = signedWord(m.word(m.user, battle + AttackerId));
  const auto unit = battleRegistryUnit(battle, id, a);
  if (!unit)
    return 2;
  const auto event = m.word(m.user, unit + AttackEvent);
  if (!event)
    return 2;
  if (checkEventId(event, events))
    return 0;
  assert(m.storeWord);
  m.storeWord(m.user, unit + AttackEvent, 0);
  return 2;
}
} // namespace ttyd
