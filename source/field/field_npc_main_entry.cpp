#include "field_npc_main_entry.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
bool same(const FieldNpcMainEntryServices &a,
          const FieldNpcMainEntryServices &b) {
  return a.clock.user == b.clock.user && a.clock.word == b.clock.word &&
         a.clock.store == b.clock.store && a.clock.divide == b.clock.divide &&
         a.clock.convert == b.clock.convert && a.half == b.half &&
         a.storeHalf == b.storeHalf && a.request == b.request &&
         a.bank == b.bank;
}
} // namespace
FieldNpcMainFrontier
advanceFieldNpcMainEntry(const FieldNpcMainEntryServices &s,
                         FieldNpcMainEntryCursor &c) {
  using Result = FieldNpcMainFrontier;
  if (!s.clock.word || !s.clock.store)
    return Result::Pending;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
  }
  if (!same(c.owner, s))
    return Result::Pending;
  auto read = [&](uint32_t a, uint32_t &out) {
    const auto v = s.clock.word(s.clock.user, a);
    if (!v)
      return false;
    out = *v;
    return true;
  };
  auto store = [&](uint32_t a, uint32_t v) {
    return s.clock.store(s.clock.user, a, v);
  };
  auto request = [&] {
    return s.request && s.request(s.clock.user, c.call, c.response);
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (!read(0x80415fd0, c.gp))
        return Result::Pending;
      ++c.phase;
      break;
    case 1:
      if (!read(c.gp + 0x14, c.value))
        return Result::Pending;
      c.bank = 0x8041e8b4 + (c.value ? 0x14 : 0);
      c.call = {0x8005e0c0, {}};
      ++c.phase;
      break;
    case 2:
      if (!request())
        return Result::Pending;
      c.mario = c.response;
      ++c.phase;
      break;
    case 3:
      if (!store(c.mario + 0x1dc, 0))
        return Result::Pending;
      ++c.phase;
      break;
    case 4:
      if (!read(0x8041e8ac, c.mainCount))
        return Result::Pending;
      ++c.phase;
      break;
    case 5:
      if (!read(c.bank + 4, c.count))
        return Result::Pending;
      ++c.mainCount;
      ++c.phase;
      break;
    case 6:
      if (!read(c.bank + 0xc, c.array))
        return Result::Pending;
      c.actor = c.array;
      ++c.phase;
      break;
    case 7:
      if (!s.bank || !s.bank(s.clock.user, c.bank, c.count, c.array))
        return Result::Pending;
      ++c.phase;
      break;
    case 8:
      if (!store(0x8041e8ac, c.mainCount))
        return Result::Pending;
      c.phase = 10;
      break;
    // cmpw index,count is signed. Supported counts cannot wrap the index; count
    // with its sign bit set executes no actors, after the entry publications.
    case 10:
      if ((c.count & 0x80000000u) || c.index >= c.count) {
        c.phase = 90;
        break;
      }
      ++c.phase;
      break;
    case 11:
      if (!read(c.actor, c.flags))
        return Result::Pending;
      c.phase = (c.flags & 3) == 3 ? 12 : 80;
      break;
    case 12:
      if (!read(c.actor + 0x104, c.value))
        return Result::Pending;
      c.call = {0x80039cf8, {c.value, 0}};
      ++c.phase;
      break;
    case 13:
      if (!request())
        return Result::Pending;
      c.pose = c.response;
      ++c.phase;
      break;
    case 14:
      if (!read(c.pose, c.value))
        return Result::Pending;
      c.value = (c.flags & 0x8000) ? c.value | 0x100 : c.value & ~0x100u;
      ++c.phase;
      break;
    case 15:
      if (!store(c.pose, c.value))
        return Result::Pending;
      ++c.phase;
      break;
    case 16:
      if (!read(c.actor, c.value))
        return Result::Pending;
      c.phase = (c.value & 0x100000) ? 19 : 17;
      break;
    case 17:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      ++c.phase;
      break;
    case 18:
      if (!store(c.actor + 0x1d4, c.state | 0x10))
        return Result::Pending;
      ++c.phase;
      break;
    case 19:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      c.phase = (c.state & 0x10000) ? 20 : 21;
      break;
    case 20:
      if (!store(c.actor + 0x1d4, c.state & ~0x10u))
        return Result::Pending;
      ++c.phase;
      break;
    case 21: {
      const auto v =
          s.half ? s.half(s.clock.user, c.actor + 0x320) : std::nullopt;
      if (!v)
        return Result::Pending;
      c.timer = *v;
      c.phase = c.timer ? 22 : 23;
      break;
    }
    case 22:
      if (!s.storeHalf ||
          !s.storeHalf(s.clock.user, c.actor + 0x320, uint16_t(c.timer - 1)))
        return Result::Pending;
      ++c.phase;
      break;
    case 23:
      if (!updateFieldNpcClock(c.actor, s.clock, c.clock))
        return Result::Pending;
      ++c.phase;
      break;
    case 24:
      if (!read(c.actor, c.value))
        return Result::Pending;
      c.phase = (c.value & 2) ? 25 : 78;
      break;
    case 25:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      c.state =
          (c.state & 0x100000) ? c.state | 0x200000 : c.state & ~0x200000u;
      ++c.phase;
      break;
    case 26:
      if (!store(c.actor + 0x1d4, c.state))
        return Result::Pending;
      ++c.phase;
      break;
    case 27:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      ++c.phase;
      break;
    case 28:
      if (!store(c.actor + 0x1d4, c.state & ~0x100000u))
        return Result::Pending;
      ++c.phase;
      break;
    case 29:
      if (!read(0x80415fd0, c.gp))
        return Result::Pending;
      c.call = {0x8026d464, {c.gp + 0x12c, 0x802c1394}};
      ++c.phase;
      break;
    case 30:
      if (!request())
        return Result::Pending;
      c.phase = c.response ? 40 : 31;
      break;
    case 31:
      if (!read(c.actor + 0x90, c.value))
        return Result::Pending;
      c.height = real(c.value);
      ++c.phase;
      break;
    case 32:
      if (!read(0x8041fc88, c.value))
        return Result::Pending;
      c.phase = c.height > real(c.value) ? 40 : 33;
      break;
    case 33:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      ++c.phase;
      break;
    case 34:
      if (!store(c.actor + 0x1d4, c.state | 0x100000))
        return Result::Pending;
      ++c.phase;
      break;
    case 35:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      c.phase = !(c.state & 0x200000) && (c.state & 0x100000) ? 36 : 40;
      break;
    case 36:
      c.call = {0x800d9848, {0x802c139c, c.actor + 0x8c}};
      ++c.phase;
      break;
    case 37:
      if (!request())
        return Result::Pending;
      c.phase = 40;
      break;
    case 40:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      c.phase = (c.state & 0x100000) && !(c.state & 0x10000000) ? 41 : 45;
      break;
    // Original loads100 then.5 before storing the previously captured100 word.
    case 41:
      if (!read(0x8041fcf0, c.value))
        return Result::Pending;
      ++c.phase;
      break;
    case 42:
      if (!read(0x8041fc80, c.response))
        return Result::Pending;
      c.factor = real(c.response);
      ++c.phase;
      break;
    case 43:
      if (!store(c.actor + 0x1d0, c.value))
        return Result::Pending;
      c.phase = 46;
      break;
    case 45:
      if (!read(0x8041fc94, c.value))
        return Result::Pending;
      c.factor = real(c.value);
      ++c.phase;
      break;
    case 46:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      c.phase = (c.state & 0x100000) ? 47 : 60;
      break;
    case 47: {
      const auto v =
          s.half ? s.half(s.clock.user, c.actor + 0x31e) : std::nullopt;
      if (!v)
        return Result::Pending;
      c.timer = *v;
      ++c.phase;
      break;
    }
    case 48:
      if (!s.storeHalf ||
          !s.storeHalf(s.clock.user, c.actor + 0x31e, uint16_t(c.timer | 1)))
        return Result::Pending;
      c.phase = 60;
      break;
    case 60:
      return Result::Movement; // Next source load49e8c; no later work skipped.
    case 78:
      if (!read(c.actor + 0x1d4, c.state))
        return Result::Pending;
      ++c.phase;
      break;
    case 79:
      if (!store(c.actor + 0x1d4, c.state & ~0x10u))
        return Result::Pending;
      ++c.phase;
      break;
    case 80:
      ++c.index;
      c.actor += 0x340;
      c.phase = 10;
      break;
    case 90:
      return Result::FieldTail; // Next source GP/F load4b1d0, not completion.
    default:
      return Result::Pending;
    }
}
} // namespace ttyd
