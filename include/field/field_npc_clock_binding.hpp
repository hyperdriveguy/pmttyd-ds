#pragma once
#include "field_npc_clock.hpp"
#include "field_npc_registry_owner.hpp"
namespace ttyd {
class EventNativeRuntime;
struct FieldNpcClockNumbers {
  void *user = nullptr;
  bool (*divide)(void *, uint64_t, uint32_t, uint64_t &) = nullptr;
  bool (*convert)(void *, uint64_t, float &) = nullptr;
};
// One actual initialized registry/facade/VM. No actor copies, frame clock or
// per-actor allocation. Source caller must run Main's preceding gates first.
class FieldNpcClockBinding {
public:
  static std::unique_ptr<FieldNpcClockBinding>
  bind(std::shared_ptr<FieldNpcRegistryOwner>, FieldNpcRegistryBinding &,
       EventNativeRuntime &, const FieldNpcClockNumbers &);
  bool advance(uint32_t actor, FieldNpcClockCursor &);

private:
  FieldNpcClockBinding(std::shared_ptr<FieldNpcRegistryOwner>,
                       EventNativeRuntime &, const FieldNpcClockNumbers &);
  std::shared_ptr<FieldNpcRegistryOwner> owner_;
  EventNativeRuntime &vm_;
  FieldNpcClockNumbers numbers_;
};
} // namespace ttyd
