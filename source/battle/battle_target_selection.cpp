#include "battle_target_selection.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t sb(uint8_t v) { return v < 128 ? int32_t(v) : int32_t(v) - 256; }
int32_t sh(uint16_t v) { return v < 32768 ? int32_t(v) : int32_t(v) - 65536; }
} // namespace
bool processPackedBattleTargetSelection(
    uint32_t b, uint32_t u, const BattleTargetSelectionServices &s) {
  const auto &m = s.memory;
  assert(m.memory.word && m.memory.storeWord && m.memory.half && m.byte &&
         m.memory.storeByte && s.cursor && s.privateCursor && s.refresh &&
         s.repeat && s.trigger && s.sound);
  auto word = [&](uint32_t at) { return m.memory.word(m.memory.user, at); };
  auto store = [&](uint32_t at, uint32_t v) {
    m.memory.storeWord(m.memory.user, at, v);
  };
  auto byte = [&](uint32_t at) { return m.byte(m.user, at); };
  auto put = [&](uint32_t at, uint8_t v) {
    m.memory.storeByte(m.memory.user, at, v);
  };
  auto half = [&](uint32_t at) { return sh(m.memory.half(m.memory.user, at)); };
  const auto menu = b + 0x171c, type = word(menu + 0x540), buffer = b + 0x428;
  s.cursor(s.user, menu, type, s.privateCursor);
  const auto old = sb(byte(buffer + 0xab7)),
             ordinal = sb(byte(buffer + 0xa6d + uint32_t(old)));
  const auto row = buffer + 4 + 36 * uint32_t(ordinal);
  const auto id = half(row), part = half(row + 2);
  if (!s.refresh(s.user, buffer))
    return false;
  const auto count = sb(byte(buffer + 0xa6c));
  int32_t selected = 0;
  for (; selected < count; ++selected) {
    const auto permutation = sb(byte(buffer + 0xa6d + uint32_t(selected)));
    const auto candidate = buffer + 4 + 36 * uint32_t(permutation);
    if (half(candidate) == id && half(candidate + 2) == part)
      break;
  }
  if (selected >= count)
    selected = 0;
  put(buffer + 0xab7, uint8_t(selected));
  const auto cursor = word(s.privateCursor),
             weapon = word(menu + 0x80 + 28 * word(cursor));
  if (word(weapon + 0x64) & 0x01000000) {
    if (s.repeat(s.user, 0x80000)) {
      put(buffer + 0xab7, uint8_t(byte(buffer + 0xab7) + 1));
      if (sb(byte(buffer + 0xab7)) >= sb(byte(buffer + 0xa6c)))
        put(buffer + 0xab7, 0);
      if (sb(byte(buffer + 0xa6c)) >= 2)
        s.sound(s.user, 0x802efe98);
    }
    if (s.repeat(s.user, 0x40000)) {
      put(buffer + 0xab7, uint8_t(byte(buffer + 0xab7) - 1));
      if (sb(byte(buffer + 0xab7)) < 0)
        put(buffer + 0xab7, uint8_t(byte(buffer + 0xa6c) - 1));
      if (sb(byte(buffer + 0xa6c)) >= 2)
        s.sound(s.user, 0x802efe98);
    }
  }
  assert(s.endTarget && s.help && s.commandPose && s.offEnemyLight &&
         s.offLight);
  if (s.trigger(s.user, 0x200)) {
    s.sound(s.user, 2521);
    if (!s.endTarget(s.user, b))
      return false;
    store(menu, 6);
    store(b + 0xef4, word(b + 0xef4) & ~4u);
    s.help(s.user, 0);
    if (!s.commandPose(s.user, u))
      return false;
    const auto currentType = word(menu + 0x540);
    if ((currentType == 0 && (word(b + 0xef4) & 0x200)) ||
        (currentType == 1 && (word(b + 0xef4) & 0x400))) {
      store(menu, 1);
      s.offEnemyLight(s.user);
    }
    return true;
  }
  if (s.trigger(s.user, 0x100)) {
    s.sound(s.user, 2520);
    s.offLight(s.user, 1, 1);
    store(menu, 29);
    if (!s.endTarget(s.user, b))
      return false;
    store(b + 0xef4, word(b + 0xef4) | 4);
    if (type == 2 && sb(byte(u + 0x20)) == 0) {
      const auto operation = byte(menu + 0x550);
      if (operation == 2 || operation == 3) {
        put(u + 0x20, operation == 2 ? 8 : 5);
        assert(s.pay);
        if (!s.pay(s.user, u, operation == 2 ? 0x803553a8 : 0x80355468))
          return false;
      }
    }
    s.help(s.user, 2);
  }
  assert(s.audience);
  const auto audience = s.audience(s.user);
  if (!audience)
    return false;
  if (*audience) {
    put(b + 0x1c74, 1);
    if (!s.endTarget(s.user, b))
      return false;
    s.help(s.user, 0);
  }
  return true;
}
std::optional<int32_t>
finalizePackedBattleCommand(uint32_t b, uint32_t u,
                            const BattleCommandFinalizeServices &s) {
  const auto &m = s.memory;
  assert(m.memory.word && m.memory.storeWord && s.selectAttack && s.offLight);
  const auto menu = b + 0x171c;
  if (!s.selectAttack(s.user, u, menu))
    return std::nullopt;
  const auto flags = m.memory.word(m.memory.user, menu + 4);
  m.memory.storeWord(m.memory.user, menu + 4, flags & ~0x01000000u);
  const auto battleFlags = m.memory.word(m.memory.user, b + 0xef8);
  m.memory.storeWord(m.memory.user, b + 0xef8, battleFlags & ~1u);
  s.offLight(s.user, 0, 0);
  return 1;
}
} // namespace ttyd
