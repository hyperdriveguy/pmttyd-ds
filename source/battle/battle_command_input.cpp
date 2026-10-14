#include "battle_command_input.hpp"
#include <cassert>
#include <initializer_list>
namespace ttyd {
namespace {
int32_t signedByte(uint8_t v) {
  return v < 128 ? int32_t(v) : int32_t(v) - 256;
}
} // namespace
std::optional<int32_t>
packedBattleCommandInput(uint32_t b, const BattleCommandInputServices &s) {
  const auto &a = s.memory;
  const auto &m = a.memory;
  assert(m.word && m.storeWord && m.storeByte && a.byte && s.request &&
         s.privateCursor);
  using F = BattleCommandService;
  auto word = [&](uint32_t at) { return m.word(m.user, at); };
  auto store = [&](uint32_t at, uint32_t v) { m.storeWord(m.user, at, v); };
  auto byte = [&](uint32_t at) { return a.byte(a.user, at); };
  auto put = [&](uint32_t at, uint8_t v) { m.storeByte(m.user, at, v); };
  auto request = [&](F f, std::initializer_list<uint32_t> args = {}) {
    BattleCommandInputRequest r{f};
    assert(args.size() <= r.arguments.size());
    for (auto v : args)
      r.arguments[r.count++] = v;
    return s.request(s.user, r);
  };
  const auto unit = request(F::Unit, {b, word(b + 0x420)});
  if (!unit)
    return std::nullopt;
  const auto u = *unit, menu = b + 0x171c;
  if (!menu)
    return 1;
  if (!request(F::Cursor, {menu, 14, s.privateCursor}))
    return std::nullopt;
  const auto item = request(F::PresentItem);
  if (!item)
    return std::nullopt;
  if (*item && !request(F::HelpDisplay, {0}))
    return std::nullopt;
  if (!request(F::Restore))
    return std::nullopt;
  bool clearProgress = true;
  int32_t progress = 1;
  for (;;) {
    if (clearProgress)
      progress = 0;
    const auto state = word(menu);
    switch (state) {
    case 0: {
      if (byte(b + 0x1c74)) {
        const auto type = word(menu + 0x540);
        if (type == 11 || type == 12)
          store(menu + 0x540, word(menu + 0x544));
        if (byte(b + 0x1c74) == 2)
          put(b + 0x1c74, 0);
      }
      put(menu + 0x551, 0);
      store(b + 0xef8, word(b + 0xef8) | 1);
      if (!request(F::CommandPose, {u}))
        return std::nullopt;
      const auto mode = signedByte(byte(u + 0x20));
      if (mode >= 5 && mode <= 9) {
        if (!request(F::LightOn) || !request(F::LightOffFriend) ||
            !request(F::LightOffEnemy))
          return std::nullopt;
        store(menu, 5);
      } else {
        if (!request(F::ActSetup, {b, 0}))
          return std::nullopt;
        store(menu, 2);
        if (!request(F::LightOn) || !request(F::LightOffFriend) ||
            !request(F::LightOffEnemy))
          return std::nullopt;
      }
      break;
    }
    case 1:
      if (!request(F::ActSetup, {b, 1}))
        return std::nullopt;
      store(menu, 2);
      break;
    case 2: {
      const auto r = request(F::ActMain, {b});
      if (!r)
        return std::nullopt;
      if (*r)
        progress = 1;
      break;
    }
    case 3:
    case 4:
    case 8:
    case 9:
    case 17:
    case 18:
    case 22:
    case 23: {
      const bool create =
          state == 3 || state == 8 || state == 17 || state == 22;
      const auto wait = create ? state + 1 : state;
      if (create) {
        store(menu, wait);
        const auto e = request(F::Entry, {0x80416dc4, 0, 32});
        if (!e)
          return std::nullopt;
        store(menu + 0x568, word(*e + 0x15c));
      }
      const auto live = request(F::EventLive, {word(menu + 0x568)});
      if (!live)
        return std::nullopt;
      if (!*live) {
        store(menu + 0x568, 0);
        progress = 1;
        store(menu, wait == 4 ? 1 : wait == 9 ? 6 : wait == 18 ? 15 : 20);
      }
      break;
    }
    case 5:
    case 6: {
      const auto type = word(menu + 0x540);
      if ((type == 0 && (word(b + 0xef4) & 0x200)) ||
          (type == 1 && (word(b + 0xef4) & 0x400))) {
        if (!request(F::WeaponDecide, {b, type, 0}) ||
            !request(F::TargetSetup, {b}))
          return std::nullopt;
        store(menu, 10);
        if (!request(F::LightOnEnemy))
          return std::nullopt;
        break;
      }
      uint32_t flag = 0;
      if (state == 5) {
        flag = 1;
        if (type == 2 && (byte(u + 0x2f3) || byte(u + 0x2f4)))
          flag = 0;
      }
      if (!request(F::WeaponSetup, {b, type, flag}) ||
          !request(F::LightOffFriend) || !request(F::LightOffEnemy))
        return std::nullopt;
      store(menu, 7);
      if (!request(F::Restore))
        return std::nullopt;
      [[fallthrough]];
    }
    case 7: {
      const auto r = request(F::WeaponMain, {b, word(menu + 0x540)});
      if (!r)
        return std::nullopt;
      if (*r)
        progress = 1;
      break;
    }
    case 10: {
      const auto type = word(menu + 0x540);
      if (!request(F::Cursor, {menu, type, s.privateCursor}))
        return std::nullopt;
      const auto cursor = word(s.privateCursor), index = word(cursor),
                 weapon = word(menu + 0x80 + 28 * index);
      if (!request(F::HelpKey, {word(weapon + 0x70)}) ||
          !request(F::HelpDisplay, {1}))
        return std::nullopt;
      store(menu, 11);
      break;
    }
    case 11:
      if (!s.target || !s.target(s.user, b, u, s.privateCursor))
        return std::nullopt;
      break;
    case 12:
      store(menu, 13);
      if (!request(F::MultiCancelSetup, {b}) || !request(F::Restore))
        return std::nullopt;
      break;
    case 13: {
      const auto r = request(F::MultiCancelMain, {b});
      if (!r)
        return std::nullopt;
      if (*r)
        progress = 1;
      break;
    }
    case 14:
    case 15:
      if (!request(F::MultiSetup, {b, state == 14 ? 1u : 0u}))
        return std::nullopt;
      store(menu, 16);
      if (!request(F::Restore))
        return std::nullopt;
      [[fallthrough]];
    case 16: {
      const auto r = request(F::MultiMain, {b});
      if (!r)
        return std::nullopt;
      if (*r)
        progress = 1;
      break;
    }
    case 19:
    case 20:
      if (!request(F::OperationSetup, {b, state == 19 ? 1u : 0u}))
        return std::nullopt;
      store(menu, 21);
      if (!request(F::Restore))
        return std::nullopt;
      [[fallthrough]];
    case 21: {
      const auto r = request(F::OperationMain, {b});
      if (!r)
        return std::nullopt;
      if (*r)
        progress = 1;
      break;
    }
    case 24:
    case 25:
      if (!request(F::PartySetup, {b, state == 24 ? 1u : 0u}))
        return std::nullopt;
      store(menu, 26);
      if (!request(F::Restore))
        return std::nullopt;
      [[fallthrough]];
    case 26: {
      const auto r = request(F::PartyMain, {b});
      if (!r)
        return std::nullopt;
      if (*r)
        progress = 1;
      break;
    }
    case 27:
      if (!request(F::ProtectSetup, {b}))
        return std::nullopt;
      store(menu, 28);
      if (!request(F::Restore))
        return std::nullopt;
      [[fallthrough]];
    case 28: {
      const auto cancel = request(F::Trigger, {0x200});
      if (!cancel)
        return std::nullopt;
      if (*cancel) {
        if (!request(F::Sound, {2521}) || !request(F::ProtectEnd, {b}))
          return std::nullopt;
        store(menu, 20);
      } else {
        const auto confirm = request(F::Trigger, {0x100});
        if (!confirm)
          return std::nullopt;
        if (*confirm) {
          store(menu, 29);
          if (!request(F::ProtectEnd, {b}))
            return std::nullopt;
        }
      }
      break;
    }
    case 29:
      if (!request(F::SelectAttack, {u, menu}))
        return std::nullopt;
      store(menu + 4, word(menu + 4) & ~0x01000000u);
      store(b + 0xef8, word(b + 0xef8) & ~1u);
      if (!request(F::LightOff, {0, 0}))
        return std::nullopt;
      return 1;
    default:
      break;
    }
    if (byte(b + 0x1c74)) {
      if (!request(F::Restore))
        return std::nullopt;
      clearProgress = false;
      continue;
    }
    if (progress) {
      clearProgress = true;
      continue;
    }
    return 0;
  }
}
} // namespace ttyd
