#include "battle_unit_coin.hpp"
#include "event_manager.hpp"
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, UnitRegistry = 0x20,
                   UnitKind = 0x10;
constexpr uint32_t CoinTrials = 0x10, CoinChance = 0x11, CoinBase = 0x12;
constexpr int32_t UnitCapacity = 64, CoinRandomBound = 100, EventComplete = 2;
struct Guard {
  bool &v;
  explicit Guard(bool &b) : v(b) { v = true; }
  ~Guard() { v = false; }
};
bool same(const BattleUnitCoinServices &a, const BattleUnitCoinServices &b) {
  return a.user == b.user && a.lifetime == b.lifetime &&
         !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) && a.current == b.current &&
         a.word == b.word && a.byte == b.byte && a.value == b.value &&
         a.translate == b.translate && a.random == b.random &&
         a.store == b.store && a.writeWord == b.writeWord;
}
} // namespace
std::optional<int32_t> resumeBattleGetCoinEvent(EventEntry &e,
                                                const BattleUnitCoinServices &s,
                                                BattleUnitCoinCursor &c) {
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
    return EventComplete;
  if (!c.prepared_) {
    size_t read = 0;
    auto load = [&](uint32_t kind, uint32_t address, uint32_t &out) {
      if (read < c.reads_.size()) {
        const auto &r = c.reads_[read++];
        if (r.kind != kind || r.address != address)
          return false;
        out = r.value;
        return true;
      }
      c.pending_ = kind == 0   ? address
                   : kind == 1 ? address
                   : kind == 2 ? 0x80031260
                   : kind == 3 ? 0x801253ec
                               : 0x80005da0;
      if (!s.current(s.user))
        return false;
      std::optional<uint32_t> v;
      if (kind == 0 && s.word)
        v = s.word(s.user, address);
      else if (kind == 1 && s.byte) {
        auto q = s.byte(s.user, address);
        if (q)
          v = *q;
      } else if (kind == 2 && s.value) {
        auto q = s.value(s.user, e, int32_t(address));
        if (q)
          v = uint32_t(*q);
      } else if (kind == 3 && s.translate) {
        auto q = s.translate(s.user, e, int32_t(address));
        if (q)
          v = uint32_t(*q);
      } else if (kind == 4 && s.random) {
        auto q = s.random(s.user, int32_t(address));
        if (q)
          v = uint32_t(*q);
      }
      if (!v)
        return false;
      c.reads_.push_back({kind, address, *v});
      ++read;
      out = *v;
      return true;
    };
    uint32_t selector, resolved, destination, id, battle, unit = 0;
    if (!load(0, c.args_, selector) || !load(2, selector, resolved) ||
        !load(0, c.args_ + 4, destination) || !load(3, resolved, id) ||
        !load(0, BattleGlobal, battle))
      return {};
    if (int32_t(id) >= 0 && int32_t(id) < UnitCapacity)
      if (!load(0, battle + UnitRegistry + 4 * id, unit))
        return {};
    uint32_t coins = 0;
    if (unit) {
      uint32_t kind;
      if (!load(0, unit + UnitKind, kind) || !load(1, kind + CoinBase, coins))
        return {};
      uint32_t iteration = 0;
      for (;;) {
        uint32_t count;
        if (!load(0, unit + UnitKind, kind) ||
            !load(1, kind + CoinTrials, count))
          return {};
        if (iteration >= count)
          break;
        uint32_t random, chance;
        if (!load(4, CoinRandomBound, random) ||
            !load(0, unit + UnitKind, kind) ||
            !load(1, kind + CoinChance, chance))
          return {};
        if (int32_t(random) < int32_t(chance))
          ++coins;
        ++iteration;
      }
    }
    c.destination_ = int32_t(destination);
    c.coins_ = int32_t(coins);
    c.prepared_ = true;
  }
  c.pending_ = 0x80030cfc;
  if (!s.current(s.user) || !s.store ||
      !s.store(s.user, e, c.destination_, c.coins_))
    return {};
  c.complete_ = true;
  c.pending_ = 0;
  return EventComplete;
}
std::optional<int32_t>
resumeBattleStoreCoinEvent(EventEntry &e, const BattleUnitCoinServices &s,
                           BattleUnitCoinCursor &c) {
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
    return EventComplete;
  if (!c.prepared_) {
    size_t read = 0;
    auto load = [&](uint32_t kind, uint32_t at, uint32_t &out) {
      if (read < c.reads_.size()) {
        const auto &r = c.reads_[read++];
        if (r.kind != kind || r.address != at)
          return false;
        out = r.value;
        return true;
      }
      c.pending_ = kind == 0 ? at : 0x80031260;
      if (!s.current(s.user))
        return false;
      std::optional<uint32_t> v;
      if (kind == 0 && s.word)
        v = s.word(s.user, at);
      else if (kind == 2 && s.value) {
        auto q = s.value(s.user, e, int32_t(at));
        if (q)
          v = uint32_t(*q);
      }
      if (!v)
        return false;
      c.reads_.push_back({kind, at, *v});
      ++read;
      out = *v;
      return true;
    };
    constexpr uint32_t BattleInformation = 0x2738, StoredCoins = 0x14;
    uint32_t raw, amount, battle, information, old;
    if (!load(0, c.args_, raw) || !load(2, raw, amount) ||
        !load(0, BattleGlobal, battle) ||
        !load(0, battle + BattleInformation, information) ||
        !load(0, information + StoredCoins, old))
      return {};
    c.address_ = information + StoredCoins;
    c.sum_ = old + amount;
    c.prepared_ = true;
  }
  c.pending_ = 0x800f7c18;
  if (!s.current(s.user) || !s.writeWord ||
      !s.writeWord(s.user, c.address_, c.sum_))
    return {};
  c.complete_ = true;
  c.pending_ = 0;
  return EventComplete;
}

} // namespace ttyd
