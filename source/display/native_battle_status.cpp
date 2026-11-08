#include "native_battle_status.hpp"
#include "battle_unit_resources.hpp"
#include "event_native_runtime.hpp"
#include "source_status_values.hpp"
#include <algorithm>
namespace ttyd {
namespace {
using Status = NativeBattleStatusSnapshot;
// Original status-window partner table8041767c, indexed by source partner ID.
constexpr uint16_t PartnerPortraits[]{428, 428, 429, 430, 431, 438, 439, 440};
constexpr uint32_t PouchPointer = 0x8041eb00;
constexpr uint32_t PouchHp = 0x70, PouchMaxHp = 0x72;
constexpr uint32_t PouchFp = 0x74, PouchMaxFp = 0x76;
constexpr uint32_t PouchCoins = 0x78, PouchAp = 0x7a;
constexpr uint32_t PouchMaxApGetterBits = 0x7c, PouchStarPoints = 0x96;
constexpr uint32_t PouchAcquiredStarMask = 0x8c;
constexpr uint32_t PartyRowBytes = 14, YoshiPartnerId = 4;
constexpr uint16_t YoshiFirstPortrait = 431, YoshiLastPortrait = 437;
constexpr uint32_t YoshiColorShift = 13;
constexpr int OrdinaryYoshiColorCount = 6;
constexpr uint32_t BattleUnits = 0x20, UnitSlotCount = 64;
constexpr uint32_t UnitKind = 8, UnitAlliance = 0xc, UnitFlags = 0x1c;
constexpr uint32_t ExcludedPartyFlag = 8, MarioKind = 222;
constexpr uint32_t FirstPartnerKind = 224, PartnerKindEnd = 231;
constexpr uint32_t StarSlots = 8, ApPerStar = 100;
constexpr uint32_t PartialFillSteps = 15, MaxPartialAp = ApPerStar - 1;
constexpr uint16_t FirstPartialIcon = 456, FirstFullStarIcon = 447;
constexpr uint16_t EmptyStarIcon = 455;
constexpr int StarPitch = 32, FullStarYOffset = 12;
uint32_t sign(uint16_t n) {
  return n < 32768 ? n : uint32_t(int32_t(n) - 65536);
}

} // namespace
struct NativeBattleStatusReader::State {
  EventNativeRuntime &vm;
  std::shared_ptr<const void> life;
  void *user;
  bool (*current)(void *);
  bool running = false, invalid = false;
  SourceStatusValuesCursor cursor;
  SourceStatusValueServices services;
  SourceStatusValueOutput output;
  std::array<std::optional<uint16_t>, Status::ValueCount> cells;
  uint32_t phase = 0, pending = 0;
  // Each child retains actual captures across missing reads, rather than
  // restarting a unit registry scan or rereading an earlier pouch pointer.
  uint32_t child = 0, childPhase = 0, base = 0, index = 0, unit = 0, kind = 0;
  uint32_t gaugeIndex = 0, capacity = 0, ap = 0, full = 0;
  NativeBattleStatusSnapshot result;
  bool valid() {
    if (invalid)
      return false;
    if (!current || !current(user))
      invalid = true;
    return !invalid;
  }
  std::optional<uint32_t> word(uint32_t a) {
    if (!valid())
      return {};
    uint32_t v;
    if (!vm.memory.readWord(a, v) || !valid())
      return {};
    return v;
  }
  std::optional<uint16_t> half(uint32_t a) {
    if (!valid())
      return {};
    uint16_t v;
    if (!vm.memory.readHalf(a, v) || !valid())
      return {};
    return v;
  }
  std::optional<uint8_t> byte(uint32_t a) {
    if (!valid())
      return {};
    uint8_t v;
    if (!vm.memory.readByte(a, v) || !valid())
      return {};
    return v;
  }
  std::optional<uint32_t> done(uint32_t v) {
    child = childPhase = index = 0;
    return v;
  }
  std::optional<uint32_t> call(const SourceStatusValueCall &q) {
    if (!valid())
      return {};
    if (!child) {
      child = q.function;
      base = q.arguments[0];
      childPhase = index = 0;
    }
    if (child != q.function)
      return {};
    pending = child;
    if (child == 0x800f8444) {
      if (q.count != 1)
        return {};
      return done(uint32_t(battlePartyIndex(int32_t(base))));
    }
    if (child == 0x800f87a8 || child == 0x800f8728) {
      if (q.count != 1)
        return {};
      const bool party = child == 0x800f8728;
      for (;;) {
        if (index == UnitSlotCount)
          return done(0);
        if (childPhase == 0) {
          auto v = word(base + BattleUnits + 4 * index);
          if (!v)
            return {};
          unit = *v;
          if (!unit) {
            ++index;
            continue;
          }
          childPhase = party ? 1 : 2;
        }
        if (childPhase == 1) {
          auto v = word(unit + UnitFlags);
          if (!v)
            return {};
          if (*v & ExcludedPartyFlag) {
            ++index;
            childPhase = 0;
            continue;
          }
          childPhase = 2;
        }
        if (childPhase == 2) {
          auto v = word(unit + UnitKind);
          if (!v)
            return {};
          kind = *v;
          if (party ? (kind < FirstPartnerKind || kind >= PartnerKindEnd) : kind != MarioKind) {
            ++index;
            childPhase = 0;
            continue;
          }
          childPhase = 3;
        }
        auto b = byte(unit + UnitAlliance);
        if (!b)
          return {};
        if (!*b)
          return done(unit);
        ++index;
        childPhase = 0;
      }
    }
    uint32_t offset = 0;
    switch (child) {
    case 0x800d3d30:
      offset = PouchHp;
      break;
    case 0x800d3d24:
      offset = PouchMaxHp;
      break;
    case 0x800d3c98:
      offset = PouchFp;
      break;
    case 0x800d3c8c:
      offset = PouchMaxFp;
      break;
    case 0x800d3c44:
      offset = PouchAp;
      break;
    case 0x800d3bb8:
      offset = PouchMaxApGetterBits;
      break;
    case 0x800d3424:
      offset = PouchStarPoints;
      break;
    case 0x800d3e64:
      offset = PouchCoins;
      break;
    case 0x800d5eb0:
      break;
    default:
      return {};
    }
    if (q.count)
      return {};
    if (!childPhase) {
      auto p = word(PouchPointer);
      if (!p)
        return {};
      base = *p;
      childPhase = 1;
    }
    if (child == 0x800d5eb0)
      return done(base);
    auto v = half(base + offset);
    return v ? done(sign(*v)) : std::nullopt;
  }
  State(EventNativeRuntime &v, std::shared_ptr<const void> l, void *u,
        bool (*c)(void *))
      : vm(v), life(std::move(l)), user(u), current(c) {
    services.user = this;
    services.lifetime = life;
    services.current = [](void *p) { return static_cast<State *>(p)->valid(); };
    services.word = [](void *p, uint32_t a) {
      return static_cast<State *>(p)->word(a);
    };
    services.half = [](void *p, uint32_t a) {
      return static_cast<State *>(p)->half(a);
    };
    services.byte = [](void *p, uint32_t a) {
      return static_cast<State *>(p)->byte(a);
    };
    services.call = [](void *p, const SourceStatusValueCall &q) {
      return static_cast<State *>(p)->call(q);
    };
    output.user = this;
    output.lifetime = life;
    output.half = [](void *p, uint32_t o) -> std::optional<uint16_t> {
      auto &s = *static_cast<State *>(p);
      return s.valid() && o % 2 == 0 && o < sizeof(int16_t) * Status::ValueCount ? s.cells[o / 2] : std::nullopt;
    };
    output.storeHalf = [](void *p, uint32_t o, uint16_t n) {
      auto &s = *static_cast<State *>(p);
      if (!s.valid() || o % 2 || o >= sizeof(int16_t) * Status::ValueCount)
        return false;
      s.cells[o / 2] = n;
      return true;
    };
  }
  std::optional<NativeBattleStatusSnapshot> resume() {
    if (running || !life)
      return {};
    running = true;
    struct Guard {
      bool &b;
      ~Guard() { b = false; }
    } g{running};
    if (!valid())
      return {};
    if (phase == 0) {
      if (!resumeSourceStatusValues(output, services, cursor)) {
        pending = cursor.pending();
        return {};
      }
      for (unsigned i = 0; i < Status::ValueCount; ++i) {
        if (!cells[i])
          return {};
        result.values[i] = int16_t(*cells[i]);
      }
      phase = 1;
    }
    if (phase == 1) {
      const auto id = result.values[Status::PartnerId];
      if (id > 0 && id < int(std::size(PartnerPortraits))) {
        if (id == YoshiPartnerId) {
          auto p = word(PouchPointer);
          if (!p) {
            pending = 0x800d2e44;
            return {};
          }
          base = *p;
          phase = 2;
        } else {
          result.partnerPortrait = PartnerPortraits[id];
          phase = 3;
        }
      } else
        phase = 3;
    }
    if (phase == 2) {
      auto f = half(base + YoshiPartnerId * PartyRowBytes);
      if (!f) {
        pending = 0x800d2e44;
        return {};
      }
      const auto color = *f >> YoshiColorShift;
      result.partnerPortrait = uint16_t(color < OrdinaryYoshiColorCount ? YoshiFirstPortrait + color : YoshiLastPortrait);
      phase = 3;
    }
    // Native display adaptation of gaugeDisp: retain all eight fresh pouch
    // captures for capacity, then its partial-fill selection and icon loop.
    for (;;) {
      if (phase == 3) {
        if (gaugeIndex == StarSlots) {
          ap = uint32_t(
              std::max(0, std::min(int(result.values[Status::Ap]), int(capacity))));
          full = ap / ApPerStar;
          const auto remainder = ap % ApPerStar;
          const auto partial =
              remainder ? std::max<uint32_t>(1, remainder * PartialFillSteps / MaxPartialAp) : 0u;
          if (partial)
            result.gauge[result.gaugeCount++] = {uint16_t(FirstPartialIcon + partial),
                                                 int16_t(full * StarPitch), 0};
          gaugeIndex = 0;
          phase = 5;
          continue;
        }
        auto p = word(PouchPointer);
        if (!p) {
          pending = 0x800d5eb0;
          return {};
        }
        base = *p;
        phase = 4;
      }
      if (phase == 4) {
        auto f = half(base + PouchAcquiredStarMask);
        if (!f) {
          pending = 0x8013d48c;
          return {};
        }
        if (*f & (1u << gaugeIndex))
          capacity += ApPerStar;
        ++gaugeIndex;
        phase = 3;
        continue;
      }
      if (phase == 5) {
        if (gaugeIndex == StarSlots)
          return publish();
        auto p = word(PouchPointer);
        if (!p) {
          pending = 0x800d5eb0;
          return {};
        }
        base = *p;
        phase = 6;
      }
      if (phase == 6) {
        auto f = half(base + PouchAcquiredStarMask);
        if (!f) {
          pending = 0x8013d5b0;
          return {};
        }
        if (!(*f & (1u << gaugeIndex)))
          return publish();
        result.gauge[result.gaugeCount++] = {
            uint16_t(gaugeIndex < full ? FirstFullStarIcon + gaugeIndex : EmptyStarIcon),
            int16_t(gaugeIndex * StarPitch), FullStarYOffset};
        ++gaugeIndex;
        phase = 5;
        continue;
      }
      return {};
    }
  }
  std::optional<NativeBattleStatusSnapshot> publish() {
    auto out = result;
    phase = pending = gaugeIndex = capacity = 0;
    result = {};
    cells = {};
    return out;
  }
};
NativeBattleStatusReader::NativeBattleStatusReader(
    EventNativeRuntime &v, std::shared_ptr<const void> l, void *u,
    bool (*c)(void *))
    : state_(std::make_unique<State>(v, std::move(l), u, c)) {}
NativeBattleStatusReader::~NativeBattleStatusReader() = default;
std::optional<NativeBattleStatusSnapshot> NativeBattleStatusReader::snapshot() {
  return state_->resume();
}
uint32_t NativeBattleStatusReader::pending() const { return state_->pending; }
} // namespace ttyd
