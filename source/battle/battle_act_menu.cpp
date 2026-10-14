#include "battle_act_menu.hpp"
#include <cassert>
#include <cstring>
#include <initializer_list>
namespace ttyd {
namespace {
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
int32_t byteInteger(uint8_t n) {
  return n < 128 ? int32_t(n) : int32_t(n) - 256;
}
struct Context {
  const BattleActMenuServices &s;
  uint32_t word(uint32_t a) const {
    return s.memory.memory.word(s.memory.memory.user, a);
  }
  void word(uint32_t a, uint32_t v) const {
    s.memory.memory.storeWord(s.memory.memory.user, a, v);
  }
  uint8_t byte(uint32_t a) const { return s.memory.byte(s.memory.user, a); }
  void byte(uint32_t a, uint8_t v) const {
    s.memory.memory.storeByte(s.memory.memory.user, a, v);
  }
  std::optional<uint32_t>
  call(BattleActService f, std::initializer_list<uint32_t> args = {}) const {
    BattleActRequest r{f};
    assert(s.request && args.size() <= 4);
    for (auto a : args)
      r.arguments[r.count++] = a;
    return s.request(s.user, r);
  }
};
} // namespace
void initializePackedBattleActCursor(uint32_t c, const EventMemoryServices &m) {
  m.storeWord(m.user, c, 0);
  m.storeWord(m.user, c + 4, 0);
}
bool setupPackedBattleActMenu(uint32_t b, uint32_t flag,
                              const BattleActMenuServices &s) {
  using F = BattleActService;
  Context c{s};
  const auto m = b + 0x171c;
  if (!c.call(F::ClassTable, {b}) ||
      !c.call(F::Cursor, {m, 14, s.privateCursor}))
    return false;
  if (!c.call(F::Init, {b, c.word(s.privateCursor), m + 8, flag}))
    return false;
  c.word(m + 0x560, 0);
  c.word(m + 4, c.word(m + 4) | 0x01000001);
  if (flag == 0 && c.byte(m + 0x558) == 0)
    for (auto offset : {0x4b0u, 0x4bcu, 0x4c8u, 0x4d4u, 0x4e0u, 0x4ecu, 0x4f8u,
                        0x504u, 0x51cu})
      initializePackedBattleActCursor(m + offset, s.memory.memory);
  return true;
}
bool endPackedBattleActMenu(uint32_t b, const BattleActMenuServices &s) {
  Context c{s};
  if (!c.call(BattleActService::End, {b}))
    return false;
  c.word(b + 0x1720, c.word(b + 0x1720) & ~1u);
  return true;
}
std::optional<int32_t>
checkPackedBattleChangePosition(uint32_t b, const BattleActMenuServices &s) {
  using F = BattleActService;
  Context c{s};
  const auto flags = c.word(b + 0xef4);
  if (flags & 0x00800000)
    return 0;
  if (flags & 0x40000000)
    return 0;
  for (auto fn : {F::Mario, F::Party}) {
    const auto value = c.call(fn, {b});
    if (!value)
      return std::nullopt;
    const auto u = *value;
    if (!u)
      return 0;
    if (byteInteger(c.byte(u + 0x20)) != 0)
      return 0;
    const auto moves = byteInteger(c.byte(u + 0x23));
    if (moves <= 0)
      return 0;
    if (byteInteger(c.byte(u + 0x22)) != moves)
      return 0;
    for (auto id : {1u, 2u, 9u, 27u}) {
      const auto active = c.call(F::Status, {u, id});
      if (!active)
        return std::nullopt;
      if (*active)
        return 0;
    }
    if (fn == F::Mario) {
      if (c.word(u + 0x104) & 0x10)
        return 0;
    } else
      return c.byte(u + 0x12b) == 0 ? 1 : 0;
  }
  return 0;
}
std::optional<int32_t> mainPackedBattleActMenu(uint32_t b,
                                               const BattleActMenuServices &s) {
  using F = BattleActService;
  Context c{s};
  const auto m = b + 0x171c;
  const auto value = c.call(F::Unit, {b, c.word(b + 0x420)});
  if (!value)
    return std::nullopt;
  const auto u = *value;
  if (!c.call(F::Cursor, {m, 14, s.privateCursor}))
    return std::nullopt;
  auto cursor = [&] { return c.word(s.privateCursor); };
  auto row = [&] { return m + 20 * c.word(cursor()); };
  auto finish = [&](int32_t result) -> std::optional<int32_t> {
    if (!c.call(F::Display, {b}))
      return std::nullopt;
    return result;
  };
  auto position = [&]() -> std::optional<int32_t> {
    if (!c.call(F::Sound, {2520}) || !endPackedBattleActMenu(b, s))
      return std::nullopt;
    c.word(m + 0x540, 9);
    c.word(m, 29);
    return finish(1);
  };
  auto trigger = c.call(F::Trigger, {0x800});
  if (!trigger)
    return std::nullopt;
  if (*trigger) {
    const auto enabled = c.call(F::Position, {b});
    if (!enabled)
      return std::nullopt;
    if (*enabled)
      return position();
  }
  const auto allowed = c.call(F::Allowed, {b});
  if (!allowed)
    return std::nullopt;
  if (!*allowed)
    return finish(0);
  for (uint32_t j = 0, offset = 0; integer(j) < integer(c.word(cursor() + 8));
       ++j, offset += 20) {
    const auto r = m + offset, t = c.word(r + 8);
    if ((t == 0 && (c.word(b + 0xef4) & 0x200)) ||
        (t == 1 && (c.word(b + 0xef4) & 0x400))) {
      if (!c.call(F::WeaponTable, {b, t}))
        return std::nullopt;
      const auto weapon = c.word(m + 0x80);
      const auto count = c.call(F::TargetCount, {b, u, weapon});
      if (!count)
        return std::nullopt;
      if (integer(*count) <= 0) {
        c.word(r + 0xc, 0);
        if (!c.word(r + 0x18))
          c.word(r + 0x18, t == 0 ? 4 : 5);
      }
    }
  }
  const auto now = c.call(F::Now, {0xffffffff});
  if (!now)
    return std::nullopt;
  if (*now) {
    c.word(m + 0x560, 0);
    if (!c.call(F::CameraMode, {0, 0}))
      return std::nullopt;
  } else {
    const auto timer = c.word(m + 0x560) + 1;
    c.word(m + 0x560, timer);
    if (integer(timer) >= 900 &&
        (!c.call(F::CameraMode, {0, 6}) || !c.call(F::CameraSpeed, {0, 5})))
      return std::nullopt;
  }
  if (c.byte(b + 0x1c74)) {
    const auto t = c.word(row() + 8);
    if ((t == 0 && (c.word(b + 0xef4) & 0x200)) ||
        (t == 1 && (c.word(b + 0xef4) & 0x400))) {
      c.byte(b + 0x1c74, 0);
      c.word(b + 0x1c70, 0);
    }
  }
  trigger = c.call(F::Trigger, {0x100});
  if (!trigger)
    return std::nullopt;
  if (*trigger || c.byte(b + 0x1c74)) {
    if (c.word(row() + 0xc)) {
      if (!c.call(F::Sound, {2520}))
        return std::nullopt;
      c.word(m + 0x540, c.word(row() + 8));
      int32_t result = 0;
      switch (c.word(m + 0x540)) {
      case 0:
      case 1:
      case 4:
      case 6:
      case 7:
      case 8:
        c.word(m, 5);
        result = 1;
        break;
      case 2:
        c.word(m, 5);
        if (byteInteger(c.byte(u + 0x20)) == 0) {
          c.byte(m + 0x550, 1);
          if (c.byte(u + 0x2f3) || c.byte(u + 0x2f4)) {
            c.word(m, 14);
            result = 1;
          }
        }
        break;
      case 5:
        c.word(m, 19);
        result = 1;
        break;
      case 13:
        c.word(m, 29);
        result = 1;
        break;
      default:
        break;
      }
      if (!endPackedBattleActMenu(b, s))
        return std::nullopt;
      return finish(result);
    }
    if (!c.byte(b + 0x1c74) && !c.call(F::Sound, {0x802efe88}))
      return std::nullopt;
    c.byte(b + 0x1c74, 0);
    if (c.word(row() + 0x18)) {
      if (!endPackedBattleActMenu(b, s))
        return std::nullopt;
      c.word(m, 3);
      c.word(m + 0x564, c.word(row() + 0x18));
      return finish(1);
    }
  }
  auto repeat = c.call(F::Repeat, {0x80000});
  if (!repeat)
    return std::nullopt;
  if (*repeat) {
    const auto ptr = cursor(), count = c.word(ptr + 8), index = c.word(ptr);
    if (integer(index) < integer(count - 1)) {
      c.word(ptr, index + 1);
      if (!c.call(F::Sound, {0x802efef4}))
        return std::nullopt;
    } else {
      if (!c.call(F::Sound, {0x802efef4}))
        return std::nullopt;
      c.word(cursor(), 0);
    }
    return finish(0);
  }
  repeat = c.call(F::Repeat, {0x40000});
  if (!repeat)
    return std::nullopt;
  if (*repeat) {
    const auto ptr = cursor(), index = c.word(ptr);
    if (integer(index) > 0) {
      c.word(ptr, index - 1);
      if (!c.call(F::Sound, {0x802efef4}))
        return std::nullopt;
    } else {
      if (!c.call(F::Sound, {0x802efef4}))
        return std::nullopt;
      const auto fresh = cursor();
      c.word(fresh, c.word(fresh + 8) - 1);
    }
    return finish(0);
  }
  const auto audience = c.call(F::Audience);
  if (!audience)
    return std::nullopt;
  if (*audience)
    return finish(0);
  trigger = c.call(F::Trigger, {0x800});
  if (!trigger)
    return std::nullopt;
  if (*trigger) {
    const auto enabled = c.call(F::Position, {b});
    if (!enabled)
      return std::nullopt;
    if (*enabled)
      return position();
  }
  return finish(0);
}
} // namespace ttyd
