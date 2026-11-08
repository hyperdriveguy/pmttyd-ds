#include "battle_star_point_offset.hpp"
#include "event_manager.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, UnitRegistry = 0x20,
                   UnitKind = 0x10;
constexpr uint32_t KindOffsetX = 0x90, KindOffsetY = 0x94, KindOffsetZ = 0x98,
                   SignedBias = 0x802eea38;
constexpr uint32_t Word = 0, Value = 1, Translate = 2, Double = 3, Subtract = 4,
                   Add = 5, Truncate = 6, Store = 7;
struct Guard {
  bool &v;
  explicit Guard(bool &b) : v(b) { v = true; }
  ~Guard() { v = false; }
};
float real(uint64_t w) {
  uint32_t bits = uint32_t(w);
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
double wide(uint64_t w) {
  double d;
  std::memcpy(&d, &w, 8);
  return d;
}
uint64_t converted(uint64_t w) {
  return (uint64_t(0x43300000) << 32) | (uint32_t(w) ^ 0x80000000u);
}
bool same(const BattleStarPointOffsetServices &a,
          const BattleStarPointOffsetServices &b) {
  return a.user == b.user && a.lifetime == b.lifetime &&
         !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) && a.current == b.current &&
         a.word == b.word && a.doubleBits == b.doubleBits &&
         a.value == b.value && a.translate == b.translate &&
         a.subtractDoubleFloat == b.subtractDoubleFloat && a.add == b.add &&
         a.truncate == b.truncate && a.store == b.store;
}
} // namespace
std::optional<int32_t>
resumeBattleStarPointOffset(EventEntry &e,
                            const BattleStarPointOffsetServices &s,
                            BattleStarPointOffsetCursor &c) {
  if (c.invoking_ || !s.lifetime || !s.current || !(e.flags & 1) ||
      e.argumentCount < 4)
    return {};
  Guard guard(c.invoking_);
  if (c.event_ &&
      (c.event_ != &e || c.id_ != e.id || c.command_ != e.currentCommand ||
       c.args_ != e.arguments || !same(s, c.services_)))
    return {};
  if (!s.current(s.user))
    return {};
  if (!c.event_) {
    c.event_ = &e;
    c.id_ = e.id;
    c.command_ = e.currentCommand;
    c.args_ = e.arguments;
    c.services_ = s;
  }
  if (c.complete_)
    return 2;
  size_t cursor = 0;
  auto op = [&](uint32_t kind, uint64_t a, uint64_t b, uint64_t &out) {
    const std::array<uint64_t, 2> args = {a, b};
    if (cursor < c.operations_.size()) {
      const auto &q = c.operations_[cursor++];
      if (q.kind != kind || q.arguments != args)
        return false;
      out = q.result;
      return true;
    }
    c.pending_ = kind == Word || kind == Double ? uint32_t(a)
                 : kind == Value                ? 0x80031260
                 : kind == Translate            ? 0x801253ec
                 : kind == Store                ? 0x80030cfc
                                                : 0x80113f88;
    if (!s.current(s.user))
      return false;
    std::optional<uint64_t> result;
    if (kind == Word && s.word) {
      auto q = s.word(s.user, uint32_t(a));
      if (q)
        result = *q;
    } else if (kind == Double && s.doubleBits)
      result = s.doubleBits(s.user, uint32_t(a));
    else if (kind == Value && s.value) {
      auto q = s.value(s.user, e, int32_t(a));
      if (q)
        result = uint32_t(*q);
    } else if (kind == Translate && s.translate) {
      auto q = s.translate(s.user, e, int32_t(a));
      if (q)
        result = uint32_t(*q);
    } else if (kind == Subtract && s.subtractDoubleFloat) {
      auto q = s.subtractDoubleFloat(s.user, wide(a), wide(b));
      if (q)
        result = bits(*q);
    } else if (kind == Add && s.add) {
      auto q = s.add(s.user, real(a), real(b));
      if (q)
        result = bits(*q);
    } else if (kind == Truncate && s.truncate) {
      auto q = s.truncate(s.user, real(a));
      if (q)
        result = uint32_t(*q);
    } else if (kind == Store && s.store &&
               s.store(s.user, e, int32_t(a), int32_t(b)))
      result = 0;
    if (!result)
      return false;
    c.operations_.push_back({kind, args, *result});
    ++cursor;
    out = *result;
    return true;
  };
  auto load = [&](uint32_t kind, uint32_t at, uint64_t &out) {
    return op(kind, at, 0, out);
  };
  uint64_t battle, rawActor, actor, id,
      unit = 0, rawX, rawY, rawZ, x, y, z, biasX, biasY, biasZ, fx, fy, fz,
      kind, dx, dy, dz, sumX, sumY, sumZ, ix, iy, iz, ignored;
  if (!load(Word, BattleGlobal, battle) || !load(Word, c.args_, rawActor) ||
      !op(Value, rawActor, 0, actor) || !op(Translate, actor, 0, id))
    return {};
  if (int32_t(id) >= 0 && int32_t(id) < 64)
    if (!load(Word, uint32_t(battle) + UnitRegistry + 4 * uint32_t(id), unit))
      return {};
  if (!load(Word, c.args_ + 4, rawX) || !op(Value, rawX, 0, x) ||
      !load(Double, SignedBias, biasX) || !load(Word, c.args_ + 8, rawY) ||
      !op(Subtract, converted(x), biasX, fx) || !op(Value, rawY, 0, y) ||
      !load(Double, SignedBias, biasY) || !load(Word, c.args_ + 12, rawZ) ||
      !op(Subtract, converted(y), biasY, fy) || !op(Value, rawZ, 0, z))
    return {};
  if (!load(Word, uint32_t(unit) + UnitKind, kind) ||
      !load(Word, uint32_t(kind) + KindOffsetX, dx) ||
      !load(Word, uint32_t(kind) + KindOffsetY, dy) || !op(Add, fx, dx, sumX) ||
      !load(Double, SignedBias, biasZ) || !op(Add, fy, dy, sumY) ||
      !op(Truncate, sumX, 0, ix) ||
      !load(Word, uint32_t(kind) + KindOffsetZ, dz) ||
      !op(Subtract, converted(z), biasZ, fz) || !op(Add, fz, dz, sumZ))
    return {};
  if (!op(Store, rawX, ix, ignored) || !op(Truncate, sumY, 0, iy) ||
      !op(Store, rawY, iy, ignored) || !op(Truncate, sumZ, 0, iz) ||
      !op(Store, rawZ, iz, ignored))
    return {};
  c.complete_ = true;
  c.pending_ = 0;
  return 2;
}
} // namespace ttyd
