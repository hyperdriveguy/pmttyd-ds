#include "field_npc_main_entry_binding.hpp"
#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
#include <cstring>
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
FieldNpcMoveFrontier
FieldNpcMainEntryBinding::advanceAutomaticPose(FieldNpcMainEntryCursor &entry,
                                               FieldNpcMainMoveCursor &movement,
                                               FieldNpcAutoPoseCursor &pose) {
  const auto frontier = advanceMovement(entry, movement);
  if (frontier != FieldNpcMoveFrontier::AutomaticPose)
    return frontier;
  FieldNpcAutoPoseServices s;
  s.memory = services().clock;
  s.doubleWord = [](void *p, uint32_t a) -> std::optional<uint64_t> {
    auto &m = static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory;
    uint32_t hi = 0, lo = 0;
    if (!m.readWord(a, hi) || !m.readWord(a + 4, lo))
      return {};
    return uint64_t(hi) << 32 | lo;
  };
  s.estimate = [](void *p, double q, double &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.estimate && b.native_.estimate(b.native_.user, q, out);
  };
  s.request = [](void *p, const FieldNpcAutoPoseRequest &q) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    if (q.function != 0x80041170 || q.args[2] != 1 || !b.native_.poseClock)
      return false;
    auto pool = b.owner_->poses().sharedPool();
    const auto handle = int32_t(q.args[0]);
    if (!pool->pose(handle))
      return false;
    std::string name;
    if (q.args[1] & 0x80000000u) {
      bool ended = false;
      for (unsigned i = 0; i < 256; ++i) {
        uint8_t ch;
        if (uint64_t(q.args[1]) + i > UINT32_MAX ||
            !b.vm_.memory.readByte(q.args[1] + i, ch))
          return false;
        if (!ch) {
          ended = true;
          break;
        }
        name.push_back(char(ch));
      }
      if (!ended)
        return false;
    }
    BattlePoseClock clock;
    if (!b.native_.poseClock(b.native_.user, clock))
      return false;
    return q.args[1] & 0x80000000u
               ? pool->selectName(handle, name, true, clock)
               : pool->selectIndex(handle, q.args[1], true, clock);
  };
  return advanceFieldNpcMainAutoPose(entry.actor, movement.delta[0],
                                     movement.delta[2], entry.factor, s, pose)
             ? FieldNpcMoveFrontier::Detection
             : FieldNpcMoveFrontier::Pending;
}
FieldNpcDetectionFrontier FieldNpcMainEntryBinding::advanceDetection(
    FieldNpcMainEntryCursor &entry, FieldNpcMainMoveCursor &movement,
    FieldNpcAutoPoseCursor &pose, FieldNpcDetectionCursor &detection) {
  if (advanceAutomaticPose(entry, movement, pose) !=
      FieldNpcMoveFrontier::Detection)
    return FieldNpcDetectionFrontier::Pending;
  FieldNpcDetectionServices s;
  s.norm.memory = services().clock;
  s.norm.doubleWord = [](void *p, uint32_t a) -> std::optional<uint64_t> {
    auto &m = static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory;
    uint32_t hi = 0, lo = 0;
    if (!m.readWord(a, hi) || !m.readWord(a + 4, lo))
      return {};
    return uint64_t(hi) << 32 | lo;
  };
  s.norm.estimate = [](void *p, double q, double &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.estimate && b.native_.estimate(b.native_.user, q, out);
  };
  s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
    uint8_t v;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readByte(a, v))
      return {};
    return v;
  };
  s.request = [](void *p, const FieldNpcDetectionRequest &q,
                 FieldNpcDetectionResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    auto &m = b.vm_.memory;
    if (q.function == 0x80035a28) {
      // Actual original current-context count/array and first live-ID match.
      // Native admission bounds storage, never clamps or fabricates a pointer.
      uint32_t gp = 0, mode = 0, count = 0, array = 0;
      if (!m.readWord(0x80415fd0, gp) || !m.readWord(gp + 0x14, mode))
        return false;
      const auto context = mode ? 0x803d8dd0u : 0x803d8d30u;
      if (!m.readWord(context, count) || !m.readWord(context + 0x90, array))
        return false;
      if (int32_t(count) <= 0) {
        out.word = 0;
        return true;
      }
      auto &owned = mode ? b.vm_.battle : b.vm_.field;
      if (count > 256 || owned.entriesUnresolved || !owned.entries ||
          b.vm_.entryAddresses.entryAt(array) != owned.entries)
        return false;
      for (uint32_t i = 0; i < count; ++i) {
        const uint64_t candidate = uint64_t(array) + i * 0x1b0;
        if (candidate + 0x1b0 > 0x100000000ULL)
          return false;
        uint8_t flags;
        uint32_t id = 0;
        if (!m.readByte(uint32_t(candidate) + 8, flags))
          return false;
        if (flags & 1) {
          if (!m.readWord(uint32_t(candidate) + 0x15c, id))
            return false;
          if (id == q.argument) {
            out.word = uint32_t(candidate);
            return true;
          }
        }
      }
      out.word = 0;
      return true;
    }
    return b.native_.detection &&
           b.native_.detection(b.native_.user, m, q, out);
  };
  return advanceFieldNpcMainDetection(entry.actor, s, detection);
}
FieldNpcDetectionFrontier FieldNpcMainEntryBinding::advanceMarioDetection(
    FieldNpcMainEntryCursor &entry, FieldNpcMainMoveCursor &movement,
    FieldNpcAutoPoseCursor &pose, FieldNpcDetectionCursor &detection,
    FieldNpcMarioCursor &mario) {
  const auto frontier = advanceDetection(entry, movement, pose, detection);
  if (frontier != FieldNpcDetectionFrontier::Mario)
    return frontier;
  FieldNpcMarioServices s;
  s.norm.memory = services().clock;
  s.norm.doubleWord = [](void *p, uint32_t a) -> std::optional<uint64_t> {
    auto &m = static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory;
    uint32_t hi, lo;
    if (!m.readWord(a, hi) || !m.readWord(a + 4, lo))
      return {};
    return uint64_t(hi) << 32 | lo;
  };
  s.norm.estimate = [](void *p, double q, double &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.estimate && b.native_.estimate(b.native_.user, q, out);
  };
  s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
    uint16_t v;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readHalf(a, v))
      return {};
    return v;
  };
  s.pair = [](void *p, uint32_t a) -> std::optional<std::array<uint32_t, 2>> {
    auto &m = static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory;
    std::array<uint32_t, 2> v;
    if (!m.readWord(a, v[0]) || !m.readWord(a + 4, v[1]))
      return {};
    return v;
  };
  s.request = [](void *p, const FieldNpcMarioRequest &q,
                 FieldNpcMarioResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    auto &m = b.vm_.memory;
    if (q.function == 0x8005e0c0) {
      const auto e = b.services();
      return e.request(p, {q.function, {}}, out.word);
    }
    if (q.function == 0x8002e210)
      return m.readWord(0x80415190, out.word);
    if (q.function == 0x801ff028) {
      const auto e = b.services();
      uint32_t mario = 0, work = 0;
      uint8_t mode = 0;
      if (!e.request(p, {0x8005e0c0, {}}, mario) ||
          !m.readByte(mario + 0x3c, mode))
        return false;
      if (mode != 2) {
        out.word = 0;
        return true;
      }
      return m.readWord(mario + 0x298, work) && m.readWord(work + 4, out.word);
    }
    if (q.function == 0x800440f8) {
      FieldNpcDetectionResponse r;
      return b.native_.detection &&
             b.native_.detection(b.native_.user, m, {q.function, q.args[0]}, r);
    }
    return b.native_.marioDetection &&
           b.native_.marioDetection(b.native_.user, m, q, out);
  };
  return advanceFieldNpcMarioDetection(entry.actor, detection.eventClass, s,
                                       mario)
             ? FieldNpcDetectionFrontier::Display
             : FieldNpcDetectionFrontier::Pending;
}
FieldNpcDisplayFrontier FieldNpcMainEntryBinding::advanceDisplay(
    FieldNpcMainEntryCursor &entry, FieldNpcMainMoveCursor &movement,
    FieldNpcAutoPoseCursor &pose, FieldNpcDetectionCursor &detection,
    FieldNpcMarioCursor &mario, FieldNpcDisplayCursor &display) {
  if (advanceMarioDetection(entry, movement, pose, detection, mario) !=
      FieldNpcDetectionFrontier::Display)
    return FieldNpcDisplayFrontier::Pending;
  FieldNpcDisplayServices s;
  s.memory = services().clock;
  s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
    uint8_t v;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readByte(a, v))
      return {};
    return v;
  };
  s.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
    uint16_t v;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readHalf(a, v))
      return {};
    return v;
  };
  s.request = [](void *p, const FieldNpcDisplayRequest &q,
                 FieldNpcDisplayResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    auto &m = b.vm_.memory;
    if (q.function == 0x80040730 || q.function == 0x80040710 ||
        q.function == 0x800406b4) {
      // Source getter is arithmetic: do not add active/resource admission.
      if (q.args[0] >= 256 || !b.poses_.aliases(*b.owner_->poses().sharedPool(), m))
        return false;
      auto a = b.poses_.poseAddress(q.args[0]);
      if (!a)
        return false;
      if (q.function == 0x800406b4)
        return m.writeWord(a + 0xf0, q.color);
      uint32_t v;
      return m.readWord(a + 0xe8, v) &&
             m.writeWord(a + 0xe8, q.function == 0x80040730
                                          ? v | q.args[1] : v & ~q.args[1]);
    }
    return b.native_.display && b.native_.display(b.native_.user, m, q, out);
  };
  return advanceFieldNpcMainDisplay(entry.actor, s, display);
}
FieldNpcDisplayFrontier FieldNpcMainEntryBinding::advanceShadow(
    FieldNpcMainEntryCursor &entry, FieldNpcMainMoveCursor &movement,
    FieldNpcAutoPoseCursor &pose, FieldNpcDetectionCursor &detection,
    FieldNpcMarioCursor &mario, FieldNpcDisplayCursor &display,
    FieldNpcShadowCursor &shadow) {
  const auto frontier = advanceDisplay(entry, movement, pose, detection, mario,
                                       display);
  if (frontier == FieldNpcDisplayFrontier::Pending)
    return frontier;
  FieldNpcShadowServices s;
  s.memory = services().clock;
  s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
    uint8_t v;
    if (!static_cast<FieldNpcMainEntryBinding *>(p)->vm_.memory.readByte(a, v))
      return {};
    return v;
  };
  s.queue = [](void *p, const FieldNpcDisplayRequest &q,
               FieldNpcDisplayResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    return b.native_.display &&
           b.native_.display(b.native_.user, b.vm_.memory, q, out);
  };
  s.request = [](void *p, const FieldNpcShadowRequest &q,
                 FieldNpcShadowResponse &out) {
    auto &b = *static_cast<FieldNpcMainEntryBinding *>(p);
    auto &m = b.vm_.memory;
    if (q.function == 0x80039824) {
      if (q.args[0] >= 256 ||
          !b.poses_.aliases(*b.owner_->poses().sharedPool(), m))
        return false;
      const auto a = b.poses_.poseAddress(q.args[0]);
      uint32_t flags;
      if (!m.contiguousRam(a, 0x170) || !m.readWord(a, flags)) return false;
      // Complete shared alias row is writable and retained for this call.
      // Actual source leaf: flag20 then strided matrix translations.
      if (!m.writeWord(a, flags | 0x20)) return false;
      for (unsigned i = 0; i < 3; ++i) {
        uint32_t w; std::memcpy(&w, &q.point[i], 4);
        if (!m.writeWord(a + 0x130 + i * 0x10, w)) return false;
      }
      return true;
    }
    return b.native_.shadow && b.native_.shadow(b.native_.user, m, q, out);
  };
  return advanceFieldNpcMainShadow(entry.actor, frontier, display.depth, s, shadow)
             ? FieldNpcDisplayFrontier::History
             : FieldNpcDisplayFrontier::Pending;
}
FieldNpcActorsFrontier
FieldNpcMainEntryBinding::advanceActors(FieldNpcMainActorsCursor &c) {
  using Result = FieldNpcActorsFrontier;
  if ((c.binding && c.binding != this) ||
      !poses_.aliases(*owner_->poses().sharedPool(), vm_.memory))
    return Result::Pending;
  c.binding = this;
  for (;;) switch (c.phase) {
  case 0: {
    const auto frontier = advance(c.entry);
    if (frontier == FieldNpcMainFrontier::Pending) return Result::Pending;
    if (frontier == FieldNpcMainFrontier::FieldTail) { c.phase = 4; break; }
    c.phase = 1; break;
  }
  case 1:
    if (advanceShadow(c.entry, c.movement, c.pose, c.detection, c.mario,
                       c.display, c.shadow) != FieldNpcDisplayFrontier::History)
      return Result::Pending;
    c.phase = 2; break;
  case 2:
    if (!advanceFieldNpcMainHistory(c.entry.actor, services().clock, c.history))
      return Result::Pending;
    // Only a completely executed body can release the parent movement frontier.
    c.entry.phase = 78;
    c.phase = 3; break;
  case 3: {
    const auto frontier = advance(c.entry);
    if (frontier == FieldNpcMainFrontier::Pending) return Result::Pending;
    c.movement = {}; c.pose = {}; c.detection = {}; c.mario = {};
    c.display = {}; c.shadow = {}; c.history = {};
    c.phase = frontier == FieldNpcMainFrontier::FieldTail ? 4 : 1;
    break;
  }
  case 4: return Result::FieldTail;
  default: return Result::Pending;
  }
}
} // namespace ttyd
