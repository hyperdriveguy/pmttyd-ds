#include "battle_unit_exp.hpp"
#include "event_manager.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, PouchGlobal = 0x8041eb00;
constexpr uint32_t UnitRegistry = 0x20, UnitKind = 0x10, UnitLevel = 0x0d,
                   PouchLevel = 0x8a;
constexpr uint32_t BattleInformation = 0x2738, InformationSetup = 0x0c,
                   SetupGroup = 0x18;
constexpr uint32_t KindBaseExp = 0x0f, BattleStoredExp = 0x0f00,
                   SignedBias = 0x802f00b8;
constexpr uint32_t OrdinaryFactor = 0x804226a8, Type3Factor = 0x804226b4,
                   Type4Factor = 0x804226b8, OtherFactor = 0x804226bc,
                   BonusMultiplier = 0x804226c0;
constexpr uint32_t Word = 0, Byte = 1, Half = 2, Double = 3, Value = 4,
                   Translate = 5, Subtract = 6, Multiply = 7, Truncate = 8;
struct Guard {
  bool &v;
  explicit Guard(bool &b) : v(b) { v = true; }
  ~Guard() { v = false; }
};
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
float real(uint64_t bits) {
  uint32_t w = uint32_t(bits);
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
double wide(uint64_t w) {
  double f;
  std::memcpy(&f, &w, 8);
  return f;
}
bool same(const BattleUnitExpServices &a, const BattleUnitExpServices &b) {
  return a.user == b.user && a.lifetime == b.lifetime &&
         !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) && a.current == b.current &&
         a.word == b.word && a.byte == b.byte && a.half == b.half &&
         a.doubleBits == b.doubleBits && a.value == b.value &&
         a.translate == b.translate &&
         a.subtractDoubleFloat == b.subtractDoubleFloat &&
         a.multiply == b.multiply && a.truncate == b.truncate &&
         a.store == b.store && a.writeWord == b.writeWord;
}
} // namespace
std::optional<int32_t> resumeBattleGetExpEvent(EventEntry &e,
                                               const BattleUnitExpServices &s,
                                               BattleUnitExpCursor &c) {
  if (c.invoking_ || !s.lifetime || !s.current || !(e.flags & 1) ||
      e.argumentCount < 2)
    return {};
  Guard guard(c.invoking_);
  if (c.event_ && (c.mode_ != 0 || c.event_ != &e || c.id_ != e.id ||
                   c.command_ != e.currentCommand || c.args_ != e.arguments ||
                   !same(s, c.services_)))
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
  if (!c.prepared_) {
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
      c.pending_ = kind <= Double      ? uint32_t(a)
                   : kind == Value     ? 0x80031260
                   : kind == Translate ? 0x801253ec
                   : kind == Subtract  ? 0x80126dcc
                   : kind == Multiply  ? 0x80126dd4
                                       : 0x80126ddc;
      if (!s.current(s.user))
        return false;
      std::optional<uint64_t> result;
      if (kind == Word && s.word) {
        auto q = s.word(s.user, uint32_t(a));
        if (q)
          result = *q;
      } else if (kind == Byte && s.byte) {
        auto q = s.byte(s.user, uint32_t(a));
        if (q)
          result = *q;
      } else if (kind == Half && s.half) {
        auto q = s.half(s.user, uint32_t(a));
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
      } else if (kind == Multiply && s.multiply) {
        auto q = s.multiply(s.user, real(a), real(b));
        if (q)
          result = bits(*q);
      } else if (kind == Truncate && s.truncate) {
        auto q = s.truncate(s.user, real(a));
        if (q)
          result = uint32_t(*q);
      }
      if (!result)
        return false;
      c.operations_.push_back({kind, args, *result});
      ++cursor;
      out = *result;
      return true;
    };
    auto load = [&](uint32_t kind, uint32_t address, uint64_t &v) {
      return op(kind, address, 0, v);
    };
    uint64_t raw, selector, destination, id, battle, unit = 0;
    if (!load(Word, c.args_, raw) || !op(Value, raw, 0, selector) ||
        !load(Word, c.args_ + 4, destination) ||
        !op(Translate, selector, 0, id) || !load(Word, BattleGlobal, battle))
      return {};
    if (int32_t(id) >= 0 && int32_t(id) < 64)
      if (!load(Word, uint32_t(battle) + UnitRegistry + 4 * uint32_t(id), unit))
        return {};
    // BtlUnit_GetExp reloads globalB before pouchGetPtr, including unit0.
    uint64_t innerBattle, pouch, player, enemy, information;
    if (!load(Word, BattleGlobal, innerBattle) ||
        !load(Word, PouchGlobal, pouch) ||
        !load(Half, uint32_t(pouch) + PouchLevel, player) ||
        !load(Byte, uint32_t(unit) + UnitLevel, enemy) ||
        !load(Word, uint32_t(innerBattle) + BattleInformation, information))
      return {};
    uint32_t result = 0;
    if (int16_t(player) <= int32_t(enemy)) {
      uint64_t setup, group, type, factor, bias, kind, multiplier, converted,
          base, first, second, bonus;
      if (!load(Word, uint32_t(information) + InformationSetup, setup) ||
          !load(Word, uint32_t(setup) + SetupGroup, group) ||
          !load(Word, uint32_t(group), type))
        return {};
      const int32_t signedType = int32_t(type);
      const uint32_t factorAddress = signedType >= 0 && signedType <= 2
                                         ? OrdinaryFactor
                                     : signedType == 3 ? Type3Factor
                                     : signedType == 4 ? Type4Factor
                                                       : OtherFactor;
      if (!load(Word, factorAddress, factor) ||
          !load(Double, SignedBias, bias) ||
          !load(Word, uint32_t(unit) + UnitKind, kind) ||
          !load(Word, BonusMultiplier, multiplier))
        return {};
      const uint32_t delta =
          uint32_t(enemy) - uint32_t(int32_t(int16_t(player)));
      const uint64_t convertedBits =
          (uint64_t(0x43300000) << 32) | (delta ^ 0x80000000u);
      if (!op(Subtract, convertedBits, bias, converted) ||
          !load(Byte, uint32_t(kind) + KindBaseExp, base) ||
          !op(Multiply, factor, converted, first) ||
          !op(Multiply, first, multiplier, second) ||
          !op(Truncate, second, 0, bonus))
        return {};
      result = uint32_t(bonus) + uint32_t(base);
    }
    c.destination_ = int32_t(destination);
    c.result_ = result;
    c.prepared_ = true;
  }
  c.pending_ = 0x80030cfc;
  if (!s.current(s.user) || !s.store ||
      !s.store(s.user, e, c.destination_, int32_t(c.result_)))
    return {};
  c.complete_ = true;
  c.pending_ = 0;
  return 2;
}
std::optional<int32_t> resumeBattleStoreExpEvent(EventEntry &e,
                                                 const BattleUnitExpServices &s,
                                                 BattleUnitExpCursor &c) {
  if (c.invoking_ || !s.lifetime || !s.current || !(e.flags & 1) ||
      e.argumentCount < 1)
    return {};
  Guard guard(c.invoking_);
  if (c.event_ && (c.mode_ != 1 || c.event_ != &e || c.id_ != e.id ||
                   c.command_ != e.currentCommand || c.args_ != e.arguments ||
                   !same(s, c.services_)))
    return {};
  if (!s.current(s.user))
    return {};
  if (!c.event_) {
    c.event_ = &e;
    c.id_ = e.id;
    c.command_ = e.currentCommand;
    c.args_ = e.arguments;
    c.services_ = s;
    c.mode_ = 1;
  }
  if (c.complete_)
    return 2;
  if (!c.prepared_) {
    size_t cursor = 0;
    auto load = [&](uint32_t kind, uint32_t at, uint64_t &out) {
      const std::array<uint64_t, 2> args = {at, 0};
      if (cursor < c.operations_.size()) {
        const auto &q = c.operations_[cursor++];
        if (q.kind != kind || q.arguments != args)
          return false;
        out = q.result;
        return true;
      }
      c.pending_ = kind == Word ? at : 0x80031260;
      if (!s.current(s.user))
        return false;
      std::optional<uint64_t> result;
      if (kind == Word && s.word) {
        auto q = s.word(s.user, at);
        if (q)
          result = *q;
      } else if (kind == Value && s.value) {
        auto q = s.value(s.user, e, int32_t(at));
        if (q)
          result = uint32_t(*q);
      }
      if (!result)
        return false;
      c.operations_.push_back({kind, args, *result});
      ++cursor;
      out = *result;
      return true;
    };
    uint64_t raw, amount, battle, old;
    if (!load(Word, c.args_, raw) || !load(Value, uint32_t(raw), amount) ||
        !load(Word, BattleGlobal, battle) ||
        !load(Word, uint32_t(battle) + BattleStoredExp, old))
      return {};
    c.address_ = uint32_t(battle) + BattleStoredExp;
    c.result_ = uint32_t(old) + uint32_t(amount);
    c.prepared_ = true;
  }
  c.pending_ = 0x800f7c04;
  if (!s.current(s.user) || !s.writeWord ||
      !s.writeWord(s.user, c.address_, c.result_))
    return {};
  c.complete_ = true;
  c.pending_ = 0;
  return 2;
}
} // namespace ttyd
