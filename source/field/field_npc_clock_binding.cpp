#include "field_npc_clock_binding.hpp"
#include "event_native_runtime.hpp"
namespace ttyd {
FieldNpcClockBinding::FieldNpcClockBinding(
    std::shared_ptr<FieldNpcRegistryOwner> owner, EventNativeRuntime &vm,
    const FieldNpcClockNumbers &numbers)
    : owner_(std::move(owner)), vm_(vm), numbers_(numbers) {}
// Require the same initialized owner AND exact memory facade; another VM or
// registry cannot silently supply the clock for an actor with the same address.
std::unique_ptr<FieldNpcClockBinding>
FieldNpcClockBinding::bind(std::shared_ptr<FieldNpcRegistryOwner> owner,
                           FieldNpcRegistryBinding &registry,
                           EventNativeRuntime &vm,
                           const FieldNpcClockNumbers &numbers) {
  if (!owner || !owner->initialized() || !numbers.divide || !numbers.convert ||
      registry.owner_ != owner || &registry.memory_ != &vm.memory)
    return {};
  return std::unique_ptr<FieldNpcClockBinding>(
      new FieldNpcClockBinding(std::move(owner), vm, numbers));
}
// No actor copy/copyback or Main gate traversal here. Reads/stores alias the
// VM bytes directly, so another facade sees completed partial publications.
// Callback lambdas have stable identities across cursor resumes.
bool FieldNpcClockBinding::advance(uint32_t actor,
                                   FieldNpcClockCursor &cursor) {
  if (owner_->entryPending() || !owner_->ownsFieldActor(actor))
    return false;
  FieldNpcClockServices s;
  s.user = this;
  s.word = [](void *p, uint32_t address) -> std::optional<uint32_t> {
    auto &self = *static_cast<FieldNpcClockBinding *>(p);
    // The same explicitly installed platform bus owner serves EVT time
    // requests. Do not cache bus at binding or substitute the host/3DS CPU
    // frequency.
    // Original low-memory bus-clock word is served by the installed platform
    // callback on EACH reached source read; every other word uses guest memory.
    if (address == 0x800000f8) {
      if (!self.vm_.platform.clock)
        return {};
      return self.vm_.platform.clock(self.vm_.platform.user);
    }
    uint32_t v = 0;
    if (!self.vm_.memory.readWord(address, v))
      return {};
    return v;
  };
  s.store = [](void *p, uint32_t a, uint32_t v) {
    return static_cast<FieldNpcClockBinding *>(p)->vm_.memory.writeWord(a, v);
  };
  s.divide = [](void *p, uint64_t n, uint32_t d, uint64_t &out) {
    auto &self = *static_cast<FieldNpcClockBinding *>(p);
    return self.numbers_.divide &&
           self.numbers_.divide(self.numbers_.user, n, d, out);
  };
  s.convert = [](void *p, uint64_t n, float &out) {
    auto &self = *static_cast<FieldNpcClockBinding *>(p);
    return self.numbers_.convert &&
           self.numbers_.convert(self.numbers_.user, n, out);
  };
  return updateFieldNpcClock(actor, s, cursor);
}
} // namespace ttyd
