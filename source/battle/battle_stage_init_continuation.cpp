#include "battle_stage_init_continuation.hpp"
#include "battle_packed_unit_vectors.hpp"
#include "battle_spotlight.hpp"
#include "battle_stage_work.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, PouchGlobal = 0x8041eb00;
constexpr uint32_t ObjectBank = 0x1715c, ObjectStride = 0x7c, ObjectCount = 32;
constexpr uint32_t Template = 0x802f42b8, StageTable = 0x80373e38,
                   SourceZero = 0x80422fd0;
constexpr uint32_t SpotPointer = 0x804175a0, SpotTable = 0x802f37a8,
                   SpotStride = 0x2c, SpotCount = 5;
constexpr uint32_t MapGetWork = 0x8002b438, MapGetObject = 0x8001f878,
                   MapOn = 0x8001edd0, MapOff = 0x8001ecb8;
constexpr uint32_t EntryPriority = 10;
struct Guard {
  bool &v;
  explicit Guard(bool &x) : v(x) { v = true; }
  ~Guard() { v = false; }
};
int32_t signedWord(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
float number(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
uint32_t bits(float v) {
  uint32_t b;
  std::memcpy(&b, &v, 4);
  return b;
}
bool same(const BattleStageSetupServices &a,
          const BattleStageSetupServices &b) {
  const auto &x = a.actors.memory;
  const auto &y = b.actors.memory;
  return a.user == b.user && a.current == b.current && a.call == b.call &&
         a.fma == b.fma && a.subtractToSingle == b.subtractToSingle &&
         a.lifetime == b.lifetime && !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) &&
         a.actors.user == b.actors.user && a.actors.byte == b.actors.byte &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall &&
         a.actors.audienceLifecycle.user == b.actors.audienceLifecycle.user &&
         a.actors.audienceLifecycle.event == b.actors.audienceLifecycle.event;
}
bool admitted(const BattleStageSetupServices &s) {
  return s.lifetime && s.current && s.actors.memory.word &&
         s.actors.memory.half && s.actors.byte && s.actors.memory.storeWord &&
         s.actors.memory.storeHalf && s.actors.memory.storeByte;
}
struct Base {
  BattleStageSetupServices s;
  uint32_t pending = 0;
  bool invalid = false;
  std::optional<BattleStageSetupRequest> held;
  bool current() {
    if (invalid || !s.current(s.user)) {
      invalid = true;
      return false;
    }
    return true;
  }
  uint32_t word(uint32_t a) const {
    return s.actors.memory.word(s.actors.memory.user, a);
  }
  uint16_t half(uint32_t a) const {
    return s.actors.memory.half(s.actors.memory.user, a);
  }
  uint8_t byte(uint32_t a) const { return s.actors.byte(s.actors.user, a); }
  void store(uint32_t a, uint32_t v) const {
    s.actors.memory.storeWord(s.actors.memory.user, a, v);
  }
  void put(uint32_t a, uint8_t v) const {
    s.actors.memory.storeByte(s.actors.memory.user, a, v);
  }
  int32_t rank() const {
    const auto raw = half(word(PouchGlobal) + 0x88);
    return raw < 0x8000 ? int32_t(raw) : int32_t(raw) - 65536;
  }
  void triple(uint32_t from, uint32_t to) const {
    const auto x = word(from), y = word(from + 4);
    store(to, x);
    store(to + 4, y);
    store(to + 8, word(from + 8));
  }
  template <class Maker>
  std::optional<uint32_t> call(uint32_t site, uint32_t fn, Maker make) {
    pending = fn;
    if (!current())
      return {};
    if (!held)
      held = BattleStageSetupRequest{fn, site, make()};
    if (!s.call)
      return {};
    auto v = s.call(s.user, *held);
    if (!current() || !v)
      return {};
    held.reset();
    pending = 0;
    return v;
  }
  std::optional<uint32_t> call(uint32_t site, uint32_t fn) {
    return call(site, fn, [] { return std::array<uint32_t, 3>{}; });
  }
};
enum class ObjectStep {
  Capture,
  RankAnimate,
  Animate,
  RankCache,
  Cache,
  RankFields,
  MapWork,
  RootOff,
  FindName,
  NameOn,
  NameOff,
  EscapeHide,
  Selected,
  SelectedOff,
  SelectedEntry,
  SelectedPos,
  SelectedHome,
  DecorRank,
  Decor,
  DecorOff,
  Wall,
  WallOn,
  Builtin,
  BuiltinOn,
  Events,
  FinalFlags,
  Body,
  Wire,
  Finish
};
} // namespace
struct BattleStageObjectInitCursor::State : Base {
  ObjectStep step = ObjectStep::Capture;
  uint32_t battle = 0, stage = 0, remaining = 0, descriptor = 0, ordinal = 1,
           row = 0, unit = 0, nameIndex = 0, decor = 0, builtin = 0,
           eventIndex = 0, zero = 0;
  int32_t cachedRank = 0;
  std::array<uint32_t, 4> counts{};
  bool interfacePresent = false, hasRankCache = false;
  uint32_t freeRow(bool guarded) const {
    uint32_t last = battle + ObjectBank;
    for (uint32_t i = 0; i < ObjectCount; ++i) {
      last = battle + ObjectBank + ObjectStride * i;
      if (signedWord(word(last)) <= 0)
        return last;
    }
    return guarded ? 0 : last;
  }
  void initializeRow(uint32_t d, uint32_t cache, uint32_t z) {
    store(row + 0x64, d);
    store(row, ordinal);
    store(row + 0x6c, 0xffffffff);
    triple(d + 8, row + 4);
    store(row + 0x24, z);
    store(row + 0x20, z);
    store(row + 0x1c, z);
    for (unsigned i = 0; i < 4; ++i)
      triple(cache + 12 * i, row + 0x34 + 12 * i);
  }
  void nextSelected() {
    ++ordinal;
    --remaining;
    descriptor += 0x18;
    step = ObjectStep::Selected;
  }
  void setPosition(uint32_t function) {
    std::array<float, 3> xyz{number(word(row + 4)), number(word(row + 8)),
                             number(word(row + 0xc))};
    PackedUnitVectorServices v;
    v.memory = s.actors.memory;
    setPackedUnitVector(function, unit, xyz, v);
  }
};
BattleStageObjectInitCursor::BattleStageObjectInitCursor() = default;
BattleStageObjectInitCursor::~BattleStageObjectInitCursor() = default;
bool BattleStageObjectInitCursor::active() const { return bool(state_); }
uint32_t BattleStageObjectInitCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleStageObjectInitCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleStageObjectInit(const BattleStageSetupServices &s,
                                       BattleStageObjectInitCursor &cursor) {
  if (cursor.invoking_ || !admitted(s) ||
      (cursor.state_ && !same(cursor.state_->s, s)))
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleStageObjectInitCursor::State>();
    cursor.state_->s = s;
  }
  auto &c = *cursor.state_;
  if (!c.current())
    return false;
  for (;;)
    switch (c.step) {
    case ObjectStep::Capture: {
      c.battle = c.word(BattleGlobal);
      const auto info = c.word(c.battle + 0x2738), setup = c.word(info + 0xc);
      c.stage = c.word(setup);
      c.remaining = c.word(c.stage + 8);
      c.descriptor = c.word(c.stage + 0xc);
      eventFillMemory(c.battle + ObjectBank, 0, ObjectCount * ObjectStride,
                      s.actors.memory);
      c.step = ObjectStep::RankAnimate;
      break;
    }
    case ObjectStep::RankAnimate: {
      const auto rank = c.rank();
      c.step =
          rank >= 1 && rank < 4 ? ObjectStep::Animate : ObjectStep::RankCache;
      break;
    }
    case ObjectStep::Animate:
      if (!c.call(0x80149c64, 0x80029de4,
                  [] { return std::array<uint32_t, 3>{0x80423038, 1, 0}; }))
        return false;
      c.step = ObjectStep::RankCache;
      break;
    case ObjectStep::RankCache: {
      const auto rank = c.rank();
      c.step = ObjectStep::Cache;
      c.hasRankCache = rank >= 1 && rank < 4;
      if (c.hasRankCache)
        c.cachedRank = c.rank();
      break;
    }
    case ObjectStep::Cache:
      c.store(c.battle + 0x18110,
              c.hasRankCache
                  ? StageTable + 0x1c0 + uint32_t(c.cachedRank) * 0x310
                  : 0);
      c.step = ObjectStep::RankFields;
      break;
    case ObjectStep::RankFields:
      c.store(c.battle + 0x18114,
              StageTable + 0xe00 + uint32_t(c.rank()) * 0x78c);
      c.put(c.battle + 0x182a6, 255);
      c.store(c.battle + 0x182b8, 0);
      c.store(c.battle + 0x182bc, 0);
      c.step = ObjectStep::MapWork;
      break;
    case ObjectStep::MapWork: {
      const auto v = c.call(0x80149cd4, MapGetWork);
      if (!v)
        return false;
      c.unit = *v;
      c.step = ObjectStep::RootOff;
      break;
    }
    case ObjectStep::RootOff:
      if (!c.call(0x80149ce8, MapOff, [&] {
            return std::array<uint32_t, 3>{
                c.word(c.word(c.word(c.unit + 0xa8) + 8)), 2, 0};
          }))
        return false;
      c.step = ObjectStep::FindName;
      break;
    case ObjectStep::FindName: {
      if (c.nameIndex == 4) {
        c.step = ObjectStep::EscapeHide;
        break;
      }
      const auto v = c.call(0x80149cf0 + 0x28 * c.nameIndex, MapGetObject, [&] {
        return std::array<uint32_t, 3>{0x80423028 + 4 * c.nameIndex, 0, 0};
      });
      if (!v)
        return false;
      if (*v)
        c.step = ObjectStep::NameOn;
      else
        ++c.nameIndex;
      break;
    }
    case ObjectStep::NameOn:
      if (!c.call(0x80149d04 + 0x28 * c.nameIndex, MapOn, [&] {
            return std::array<uint32_t, 3>{0x80423028 + 4 * c.nameIndex, 1, 0};
          }))
        return false;
      c.step = ObjectStep::NameOff;
      break;
    case ObjectStep::NameOff:
      if (!c.call(0x80149d10 + 0x28 * c.nameIndex, MapOff, [&] {
            return std::array<uint32_t, 3>{0x80423028 + 4 * c.nameIndex, 2, 0};
          }))
        return false;
      ++c.nameIndex;
      c.step = ObjectStep::FindName;
      break;
    case ObjectStep::EscapeHide:
      if (!c.call(0x80149d94, MapOn, [] {
            return std::array<uint32_t, 3>{Template + 0xe4, 1, 0};
          }))
        return false;
      if (c.descriptor) {
        c.zero = c.word(SourceZero);
        c.step = ObjectStep::Selected;
      } else
        c.step = ObjectStep::DecorRank;
      break;
    case ObjectStep::Selected: {
      if (!c.remaining) {
        c.step = ObjectStep::DecorRank;
        break;
      }
      c.row = c.freeRow(true);
      if (!c.row) {
        c.step = ObjectStep::DecorRank;
        break;
      }
      c.store(c.row + 0x64, c.descriptor);
      c.store(c.row, c.ordinal);
      c.store(c.row + 0x6c, 0xffffffff);
      const auto layer = c.half(c.word(c.row + 0x64) + 6);
      if (layer < 4)
        ++c.counts[layer];
      else if (layer == 6)
        c.interfacePresent = true;
      if (layer < 4 || layer == 6 || layer == 7)
        c.triple(c.descriptor + 8, c.row + 4);
      c.store(c.row + 0x24, c.zero);
      c.store(c.row + 0x20, c.zero);
      c.store(c.row + 0x1c, c.zero);
      for (unsigned i = 0; i < 4; ++i)
        c.triple(Template + 12 * i, c.row + 0x34 + 12 * i);
      c.step = ObjectStep::SelectedOff;
      break;
    }
    case ObjectStep::SelectedOff:
      if (!c.call(0x80149f30, MapOff, [&] {
            return std::array<uint32_t, 3>{c.word(c.word(c.row + 0x64)), 1, 0};
          }))
        return false;
      {
        const auto category = c.half(c.descriptor + 4);
        if (category <= 1) {
          c.decor = category;
          c.step = ObjectStep::SelectedEntry;
        } else
          c.nextSelected();
      }
      break;
    case ObjectStep::SelectedEntry: {
      const auto v = c.call(c.decor ? 0x80149f9c : 0x80149f58, 0x80128fe0, [&] {
        return std::array<uint32_t, 3>{c.decor ? 0x80417d18u : 0x80417ce8u, 0,
                                       0};
      });
      if (!v)
        return false;
      c.unit = *v;
      c.store(c.unit + 0xb30, c.word(c.row));
      c.store(c.row + 0x6c, c.word(c.unit));
      c.step = ObjectStep::SelectedPos;
      break;
    }
    case ObjectStep::SelectedPos:
      c.setPosition(0x80128c6c);
      c.step = ObjectStep::SelectedHome;
      break;
    case ObjectStep::SelectedHome:
      c.setPosition(0x801289a0);
      c.nextSelected();
      break;
    case ObjectStep::DecorRank:
      if (c.rank() > 0) {
        c.zero = c.word(SourceZero);
        c.decor = 0;
        c.step = ObjectStep::Decor;
      } else
        c.step = ObjectStep::Wall;
      break;
    case ObjectStep::Decor:
      if (c.decor == 3) {
        c.step = ObjectStep::Wall;
        break;
      }
      c.row = c.freeRow(true);
      if (!c.row) {
        c.step = ObjectStep::Wall;
        break;
      }
      c.initializeRow(StageTable + 0x2c30 + 0x18 * c.decor, Template + 0x30,
                      c.zero);
      c.step = ObjectStep::DecorOff;
      break;
    case ObjectStep::DecorOff:
      if (!c.call(0x8014a0e4, MapOff, [&] {
            return std::array<uint32_t, 3>{c.word(c.word(c.row + 0x64)), 1, 0};
          }))
        return false;
      ++c.ordinal;
      ++c.decor;
      c.step = ObjectStep::Decor;
      break;
    case ObjectStep::Wall: {
      c.row = c.freeRow(false);
      c.store(c.row + 0x64, 0x80417ca0);
      c.store(c.row, c.ordinal);
      c.store(c.row + 0x6c, 0xffffffff);
      c.triple(0x80417ca8, c.row + 4);
      const auto work = c.word(BattleGlobal) + 0x163fc;
      const auto y = c.word(work + 0xb28), zero = c.word(SourceZero);
      c.store(c.row + 8, y);
      c.store(c.row + 0x24, zero);
      c.store(c.row + 0x20, zero);
      c.store(c.row + 0x1c, zero);
      for (unsigned i = 0; i < 4; ++i)
        c.triple(Template + 0x60 + 12 * i, c.row + 0x34 + 12 * i);
      c.step = ObjectStep::WallOn;
      break;
    }
    case ObjectStep::WallOn:
      if (!c.call(0x8014a1dc, MapOn, [&] {
            return std::array<uint32_t, 3>{c.word(c.word(c.row + 0x64)), 1, 0};
          }))
        return false;
      {
        const auto flags = c.word(c.row + 0x68);
        c.builtin = 0;
        c.zero = c.word(SourceZero);
        ++c.ordinal;
        c.store(c.row + 0x68, flags | 1);
      }
      c.step = ObjectStep::Builtin;
      break;
    case ObjectStep::Builtin: {
      const auto d = 0x80417cb8 + 0x18 * c.builtin;
      if (!c.word(d)) {
        c.step = ObjectStep::Events;
        break;
      }
      c.row = c.freeRow(false);
      c.initializeRow(d, Template + 0x90, c.zero);
      c.step = ObjectStep::BuiltinOn;
      break;
    }
    case ObjectStep::BuiltinOn:
      if (!c.call(0x8014a2d8, MapOn, [&] {
            return std::array<uint32_t, 3>{c.word(c.word(c.row + 0x64)), 1, 0};
          }))
        return false;
      {
        const auto flags = c.word(c.row + 0x68);
        ++c.ordinal;
        ++c.builtin;
        c.store(c.row + 0x68, flags | 1);
      }
      c.step = ObjectStep::Builtin;
      break;
    case ObjectStep::Events: {
      if (c.eventIndex == 5) {
        c.step = ObjectStep::FinalFlags;
        break;
      }
      const auto index = c.eventIndex;
      if (index && c.counts[index - 1] == 0) {
        ++c.eventIndex;
        break;
      }
      if (!c.held) {
        const auto script =
            c.word(c.stage + (index ? 0x19c + 4 * index : 0x190));
        if (!script) {
          ++c.eventIndex;
          break;
        }
        c.unit = script;
      }
      const auto v = c.call(
          index ? 0x8014a328 + 0x2c * (index - 1) : 0x8014a308, 0x800374ac,
          [&] { return std::array<uint32_t, 3>{c.unit, EntryPriority, 0}; });
      if (!v)
        return false;
      if (index)
        c.store(c.battle + 0x180fc + 4 * index, c.word(*v + 0x15c));
      ++c.eventIndex;
      break;
    }
    case ObjectStep::FinalFlags:
      for (unsigned i = 0; i < 3; ++i)
        if (!c.counts[i])
          c.put(c.battle + 0x180f8,
                c.byte(c.battle + 0x180f8) | uint8_t(2u << i));
      for (unsigned i = 0; i < 3; ++i)
        if (c.byte(c.stage + 0x1b1 + i))
          c.put(c.battle + 0x180f8,
                c.byte(c.battle + 0x180f8) | uint8_t(2u << i));
      if (c.interfacePresent)
        c.step = ObjectStep::Finish;
      else {
        c.put(c.battle + 0x180f8, c.byte(c.battle + 0x180f8) | 0x20);
        c.step = ObjectStep::Body;
      }
      break;
    case ObjectStep::Body:
      if (!c.call(0x8014a478, MapOn, [] {
            return std::array<uint32_t, 3>{Template + 0x1cc, 1, 0};
          }))
        return false;
      c.step = ObjectStep::Wire;
      break;
    case ObjectStep::Wire:
      if (!c.call(0x8014a484, MapOn, [] {
            return std::array<uint32_t, 3>{Template + 0x288, 1, 0};
          }))
        return false;
      c.step = ObjectStep::Finish;
      break;
    case ObjectStep::Finish:
      cursor.state_.reset();
      return true;
    }
}
namespace {
enum class SpotStep {
  Count,
  Allocate,
  Clear,
  Entry,
  Convert,
  Fma,
  Store,
  Finish
};
}
struct BattleSpotlightInitCursor::State : Base {
  SpotStep step = SpotStep::Count;
  uint32_t allocationBytes = 0, index = 0, record = 0, y = 0, z = 0;
  float sourceStep = 0, offset = 0, converted = 0, x = 0;
  double bias = 0;
};
BattleSpotlightInitCursor::BattleSpotlightInitCursor() = default;
BattleSpotlightInitCursor::~BattleSpotlightInitCursor() = default;
bool BattleSpotlightInitCursor::active() const { return bool(state_); }
uint32_t BattleSpotlightInitCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleSpotlightInitCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleSpotlightInit(const BattleStageSetupServices &s,
                                     BattleSpotlightInitCursor &cursor) {
  if (cursor.invoking_ || !admitted(s) ||
      (cursor.state_ && !same(cursor.state_->s, s)))
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleSpotlightInitCursor::State>();
    cursor.state_->s = s;
  }
  auto &c = *cursor.state_;
  if (!c.current())
    return false;
  for (;;)
    switch (c.step) {
    case SpotStep::Count:
      c.store(c.word(SpotPointer), SpotCount);
      c.allocationBytes = c.word(c.word(SpotPointer)) * SpotStride;
      c.step = SpotStep::Allocate;
      break;
    case SpotStep::Allocate: {
      const auto a = c.call(0x801380b0, 0x800f8934, [&] {
        return std::array<uint32_t, 3>{c.allocationBytes, 0, 0};
      });
      if (!a)
        return false;
      c.store(c.word(SpotPointer) + 4, *a);
      c.step = SpotStep::Clear;
      break;
    }
    case SpotStep::Clear: {
      const auto w = c.word(SpotPointer), count = c.word(w),
                 bank = c.word(w + 4);
      eventFillMemory(bank, 0, count * SpotStride, s.actors.memory);
      c.sourceStep = number(c.word(0x80422b1c));
      const auto high = c.word(SpotTable + 0x78),
                 low = c.word(SpotTable + 0x7c);
      const auto raw = (uint64_t(high) << 32) | low;
      std::memcpy(&c.bias, &raw, 8);
      c.offset = number(c.word(0x80422b18));
      c.step = SpotStep::Entry;
      break;
    }
    case SpotStep::Entry: {
      if (c.index == SpotCount) {
        c.step = SpotStep::Finish;
        break;
      }
      BattleSpotlightServices entry;
      entry.memory = s.actors.memory;
      c.record = packedBattleSpotlightEntry(entry);
      (void)c.word(SpotTable + 0x30);
      c.y = c.word(SpotTable + 0x34);
      c.z = c.word(SpotTable + 0x38);
      c.step = SpotStep::Convert;
      break;
    }
    case SpotStep::Convert: {
      c.pending = 0x80138114;
      if (!c.current() || !s.subtractToSingle)
        return false;
      constexpr uint32_t ConversionExponent = 0x43300000,
                         ConversionSign = 0x80000000;
      const auto raw =
          (uint64_t(ConversionExponent) << 32) | (c.index ^ ConversionSign);
      double encoded;
      std::memcpy(&encoded, &raw, 8);
      const auto v = s.subtractToSingle(s.user, encoded, c.bias);
      if (!c.current() || !v)
        return false;
      c.converted = *v;
      c.pending = 0;
      c.step = SpotStep::Fma;
      break;
    }
    case SpotStep::Fma: {
      c.pending = 0x8013811c;
      if (!c.current() || !s.fma)
        return false;
      const auto v = s.fma(s.user, c.sourceStep, c.converted, c.offset);
      if (!c.current() || !v)
        return false;
      c.x = *v;
      c.pending = 0;
      c.step = SpotStep::Store;
      break;
    }
    case SpotStep::Store:
      c.store(c.record + 4, bits(c.x));
      c.store(c.record + 8, c.y);
      c.store(c.record + 0xc, c.z);
      c.triple(SpotTable + 0x3c, c.record + 0x10);
      c.triple(SpotTable + 0x48, c.record + 0x1c);
      c.store(c.record + 0x28, c.word(0x80422ad4));
      ++c.index;
      c.step = SpotStep::Entry;
      break;
    case SpotStep::Finish:
      cursor.state_.reset();
      return true;
    }
}
struct BattleStageInitCursor::State : Base {
  uint32_t step = 0;
  BattleStageObjectInitCursor objects;
  BattleSpotlightInitCursor spotlight;
};
BattleStageInitCursor::BattleStageInitCursor() = default;
BattleStageInitCursor::~BattleStageInitCursor() = default;
bool BattleStageInitCursor::active() const { return bool(state_); }
uint32_t BattleStageInitCursor::pending() const {
  return state_ ? (state_->step == 1   ? state_->objects.pending()
                   : state_->step == 2 ? state_->spotlight.pending()
                                       : state_->pending)
                : 0;
}
bool BattleStageInitCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleStageInit(const BattleStageSetupServices &s,
                                 BattleStageInitCursor &cursor) {
  if (cursor.invoking_ || !admitted(s) ||
      (cursor.state_ && !same(cursor.state_->s, s)))
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleStageInitCursor::State>();
    cursor.state_->s = s;
  }
  auto &c = *cursor.state_;
  if (!c.current())
    return false;
  if (c.step == 0) {
    const auto battle = c.word(BattleGlobal);
    initializePackedBattleStageWorkStorage(battle, s.actors.memory);
    c.step = 1;
  }
  if (c.step == 1) {
    if (!resumePackedBattleStageObjectInit(s, c.objects))
      return false;
    if (!c.current())
      return false;
    c.step = 2;
  }
  if (c.step == 2) {
    if (!resumePackedBattleSpotlightInit(s, c.spotlight))
      return false;
    if (!c.current())
      return false;
    c.step = 3;
  }
  cursor.state_.reset();
  return true;
}
} // namespace ttyd
