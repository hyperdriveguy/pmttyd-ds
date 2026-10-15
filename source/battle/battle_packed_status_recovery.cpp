#include "battle_packed_status_recovery.hpp"
#include "battle_packed_move.hpp"
#include "battle_packed_status.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t sb(uint8_t n) { return n < 128 ? int32_t(n) : int32_t(n) - 256; }
void moveCount(uint32_t u, const BattleActorServices &s) {
  const auto &m = s.memory;
  assert(m.word);
  if (m.word(m.user, u + 0x27c) & 0x1000)
    return;
  const auto baseline = sb(s.byte(s.user, u + 0x23));
  if (baseline <= 0)
    return;
  if (baseline == sb(s.byte(s.user, u + 0x22)))
    resetPackedBattleMoveStatus(u, s);
}
// Only the SetStatus branches actually reached by Recovery. Reads here remain
// fresh after GetStatus; they cannot be reconstructed from its earlier outputs.
void setRecovered(uint32_t u, int32_t id, uint8_t duration, uint8_t strength,
                  const BattleActorServices &s) {
  auto byte = [&](uint32_t offset) { return s.byte(s.user, u + offset); };
  auto put = [&](uint32_t offset, uint8_t n) {
    assert(s.memory.storeByte);
    s.memory.storeByte(s.memory.user, u + offset, n);
  };
  static constexpr uint32_t offsets[]{
      0x12a, 0x118, 0x11a, 0x11b, 0x11c, 0x11e, 0x11f, 0x120, 0x121,
      0x122, 0x123, 0x123, 0x125, 0x125, 0x127, 0x127, 0x129, 0x12b,
      0x12c, 0x12f, 0x130, 0x12d, 0x12e, 0x131, 0x133};
  if (id == 0 || id == 21) {
    put(offsets[id], sb(duration) > 0 ? duration : 0);
    return;
  }
  if (id == 8 || id == 9) {
    const auto opposite = id == 8 ? 0x122u : 0x121u;
    if (sb(byte(opposite)) != 0) {
      put(opposite, 0);
      return;
    }
    put(offsets[id], duration);
    return;
  }
  if (id == 14 || id == 15) {
    const auto next = sb(strength);
    if (next > 0) {
      if (sb(byte(0x128)) < 0) {
        put(0x127, 0);
        put(0x128, 0);
      } else {
        put(0x127, duration);
        put(0x128, strength);
      }
      return;
    }
    if (next == 0) {
      const auto old = sb(byte(0x128));
      if (id == 15 ? old <= 0 : old >= 0) {
        put(0x127, 0);
        put(0x128, 0);
      }
      return;
    }
    if (sb(byte(0x128)) > 0) {
      put(0x127, 0);
      put(0x128, 0);
    } else {
      put(0x127, duration);
      put(0x128, strength);
    }
    return;
  }
  if (id == 19 || id == 20) {
    const auto opposite = id == 19 ? 0x130u : 0x12fu;
    if (sb(byte(opposite)) > 0 && sb(duration) > 0)
      put(opposite, 0);
    else
      put(offsets[id], duration);
    moveCount(u, s);
    return;
  }
  put(offsets[id], duration);
  if (id == 4 || id == 23 || id == 24)
    put(offsets[id] + 1, strength);
}
} // namespace
int32_t checkPackedBattleRecoveryStatus(uint32_t u, int32_t rawId,
                                        const BattleActorServices &s) {
  if (!u)
    return 0;
  if (checkPackedBattleStatus(u, 27, s))
    return 0;
  assert(s.memory.half);
  const auto hp = s.memory.half(s.memory.user, u + 0x10c);
  if (hp == 0 || hp >= 0x8000)
    return 0;
  const auto value = getPackedBattleStatus(u, rawId, s);
  if (sb(value.duration) == 0)
    return 0;
  const auto id = sb(uint8_t(rawId));
  if (id < 0 || id > 24 || id == 16 || id == 17)
    return 0;
  uint8_t duration = value.duration, strength = value.strength;
  auto put = [&](uint32_t offset, uint8_t n) {
    assert(s.memory.storeByte);
    s.memory.storeByte(s.memory.user, u + offset, n);
  };
  if (id == 1) {
    duration = sb(duration) > 1 ? uint8_t(duration - 1) : 0;
    put(0x118, duration);
    return sb(duration) == 0;
  }
  duration = uint8_t(sb(duration) > 0 ? duration - 1 : duration + 1);
  if (id >= 10 && id <= 13) {
    if (sb(duration) == 0)
      strength = 0;
    const auto offset = id < 12 ? 0x123u : 0x125u;
    put(offset, duration);
    put(offset + 1, strength);
    return sb(duration) == 0;
  }
  if (id != 4 && id != 14 && id != 15 && id != 23 && id != 24)
    strength = uint8_t(sb(strength) >= 0 ? strength - 1 : strength + 1);
  if (sb(duration) == 0)
    strength = 0;
  setRecovered(u, id, duration, strength, s);
  return sb(duration) == 0;
}
} // namespace ttyd
