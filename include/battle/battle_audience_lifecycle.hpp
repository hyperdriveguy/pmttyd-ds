#pragma once
#include <cstdint>
namespace ttyd {
// Native metadata observations AFTER actual source lifecycle writes. They are
// synchronous/nonblocking; no observer may mutate guest memory or call source
// services. related carries the captured holder record for ItemReady only.
// Birth means source active storage was published, Ready means required
// source initialization/link stores completed. Reserved is an occupied paired
// neighbor, not an independently born actor. Hooks observe, never advance work.
enum class BattleAudienceLifecycleEvent : uint8_t {
  Reset,
  ActorBirth,
  ActorReady,
  ActorReserved,
  ActorClear,
  ItemBirth,
  ItemReady,
  ItemClear
};
struct BattleAudienceLifecycleHook {
  void *user = nullptr;
  void (*event)(void *, BattleAudienceLifecycleEvent, uint32_t record,
                uint32_t related) = nullptr;
};
inline void
observeBattleAudienceLifecycle(const BattleAudienceLifecycleHook &hook,
                               BattleAudienceLifecycleEvent event,
                               uint32_t record, uint32_t related = 0) {
  if (hook.event)
    hook.event(hook.user, event, record, related);
}
} // namespace ttyd
