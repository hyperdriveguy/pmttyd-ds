#include "field_npc_main_entry_binding.hpp"
#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
namespace ttyd {
FieldNpcMainEntryBinding::FieldNpcMainEntryBinding(
    std::shared_ptr<FieldNpcRegistryOwner> owner, EventNativeRuntime &vm,
    AnimationPoolMemory &poses, const FieldNpcMainEntryNativeServices &native)
    : owner_(std::move(owner)), vm_(vm), poses_(poses), native_(native) {}
std::unique_ptr<FieldNpcMainEntryBinding> FieldNpcMainEntryBinding::bind(
    std::shared_ptr<FieldNpcRegistryOwner> owner,
    FieldNpcRegistryBinding &registry, EventNativeRuntime &vm,
    AnimationPoolMemory &poses, const FieldNpcMainEntryNativeServices &s) {
  if (!owner || !owner->initialized() || registry.owner_ != owner ||
      &registry.memory_ != &vm.memory ||
      !poses.aliases(*owner->poses().sharedPool(), vm.memory) ||
      !s.numbers.divide || !s.numbers.convert || !s.admitMario)
    return {};
  return std::unique_ptr<FieldNpcMainEntryBinding>(
      new FieldNpcMainEntryBinding(std::move(owner), vm, poses, s));
}
FieldNpcMainEntryServices FieldNpcMainEntryBinding::services() {
  FieldNpcMainEntryServices s;
  s.clock.user = this;
  s.clock.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    if (a == 0x800000f8) {
      if (!b.vm_.platform.clock)
        return {};
      return b.vm_.platform.clock(b.vm_.platform.user);
    }
    uint32_t v = 0;
    if (!b.vm_.memory.readWord(a, v))
      return {};
    return v;
  };
  s.clock.store = [](void *p, uint32_t a, uint32_t v) {
    return static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.writeWord(a,
                                                                            v);
  };
  s.clock.divide = [](void *p, uint64_t n, uint32_t d, uint64_t &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.numbers.divide(b.native_.numbers.user, n, d, out);
  };
  s.clock.convert = [](void *p, uint64_t n, float &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.numbers.convert(b.native_.numbers.user, n, out);
  };
  s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
    uint16_t v = 0;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readHalf(a, v))
      return {};
    return v;
  };
  s.storeHalf = [](void *p, uint32_t a, uint16_t v) {
    return static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.writeHalf(a,
                                                                            v);
  };
  s.bank = [](void *p, uint32_t bank, uint32_t count, uint32_t array) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    if (bank != 0x8041e8b4 && bank != 0x8041e8c8)
      return false;
    if (!count || (count & 0x80000000u))
      return true;
    const bool alt = bank == 0x8041e8c8;
    return count <= (alt ? 16u : 36u) &&
           array == b.owner_->addresses_[alt ? 2 : 0];
  };
  s.request = [](void *p, const FieldNpcMainEntryRequest &q, uint32_t &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    auto &m = b.vm_.memory;
    if (q.function == 0x8005e0c0) {
      uint32_t mario = 0, lane = 0;
      if (!m.readWord(0x8041e900, mario) ||
          uint64_t(mario) + 0x1e0 > 0x100000000ULL ||
          !b.native_.admitMario(b.native_.user, m, mario) ||
          !m.readWord(mario + 0x1dc, lane))
        return false;
      out = mario;
      return true;
    }
    if (q.function == 0x80039cf8) {
      if (q.args[0] >= 256 ||
          !b.poses_.aliases(*b.owner_->poses().sharedPool(), m))
        return false;
      out = b.poses_.poseAddress(q.args[0]);
      return true;
    } // Actual source getter has no active/resource gate.
    if (q.function == 0x8026d464) {
      // Bounded native text admission. Reads are synchronous/no-effects; a
      // false request has not admitted the original compare and publishes no
      // result.
      for (unsigned n = 0; n < 256; ++n) {
        uint8_t a = 0, c = 0;
        if (uint64_t(q.args[0]) + n > UINT32_MAX ||
            uint64_t(q.args[1]) + n > UINT32_MAX ||
            !m.readByte(q.args[0] + n, a) || !m.readByte(q.args[1] + n, c))
          return false;
        if (a != c || !a) {
          out = uint32_t(int32_t(a) - int32_t(c));
          return true;
        }
      }
      return false;
    }
    if (q.function == 0x800d9848) {
      if (!b.native_.sound || !b.native_.sound(b.native_.user, m, q))
        return false;
      out = 0;
      return true;
    }
    return false;
  };
  return s;
}
FieldNpcMainFrontier
FieldNpcMainEntryBinding::advance(FieldNpcMainEntryCursor &c) {
  if (!poses_.aliases(*owner_->poses().sharedPool(), vm_.memory))
    return FieldNpcMainFrontier::Pending;
  return advanceFieldNpcMainEntry(services(), c);
}
FieldNpcMoveFrontier
FieldNpcMainEntryBinding::advanceMovement(FieldNpcMainEntryCursor &entry,
                                          FieldNpcMainMoveCursor &movement) {
  if (advance(entry) != FieldNpcMainFrontier::Movement)
    return FieldNpcMoveFrontier::Pending;
  FieldNpcMainMoveServices s;
  s.memory = services().clock;
  s.pair = [](void *p, uint32_t a) -> std::optional<std::array<uint32_t, 2>> {
    auto &m = static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory;
    std::array<uint32_t, 2> v;
    if (!m.readWord(a, v[0]) || !m.readWord(a + 4, v[1]))
      return {};
    return v;
  };
  s.request = [](void *p, const FieldNpcMainMoveRequest &q,
                 FieldNpcMainMoveResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.movement &&
           b.native_.movement(b.native_.user, b.vm_.memory, q, out);
  };
  return advanceFieldNpcMainMovement(entry.actor, s, movement);
}
} // namespace ttyd
