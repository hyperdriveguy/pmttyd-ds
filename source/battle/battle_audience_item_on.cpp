#include "battle_audience_item_on.hpp"
#include <cstring>
namespace ttyd {
void packedBattleAudienceItemOn(const std::array<uint32_t, 5> &out,
                                const BattleActorServices &s) {
  const auto &m = s.memory;
  auto word = [&](uint32_t a) { return m.word(m.user, a); };
  auto put = [&](uint32_t a, uint32_t n) { m.storeWord(m.user, a, n); };
  auto real = [&](uint32_t a) {
    const auto n = word(a);
    float f;
    std::memcpy(&f, &n, 4);
    return f;
  };
  auto floating = [&](uint32_t a, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    put(a, n);
  };
  auto half = [&](uint32_t a) {
    const auto n = m.half(m.user, a);
    return n < 32768 ? int32_t(n) : int32_t(n) - 65536;
  };
  const auto base = currentBattleAddress(s) + 0x2858;
  auto member = currentBattleAddress(s) + 0x2858 + 0x1bc;
  for (uint32_t index = 0; index < 200; ++index, member += 0x134) {
    const auto flags = word(member);
    if (!(flags & 1) || (flags & 0x100) || (flags & 2))
      continue;
    const auto item = half(member + 0x1c);
    if (item == -1)
      continue;
    const auto itemWork =
        currentBattleAddress(s) + 0x2858 + uint32_t(item) * 0x48 + 0xf25c;
    const auto raw = word(itemWork + 4);
    int32_t state;
    std::memcpy(&state, &raw, 4);
    if (state > 4 && state != 7)
      continue;
    if (out[0]) {
      put(base + 0x138bc, index);
      put(out[0], index);
    }
    put(base + 0x138c0, word(member + 0x48));
    put(base + 0x138c4, word(member + 0x4c));
    floating(base + 0x138c8, real(member + 0x50) + real(0x804249cc));
    if (out[1])
      put(out[1], word(base + 0x138c0));
    if (out[2])
      put(out[2], word(base + 0x138c4));
    if (out[3])
      put(out[3], word(base + 0x138c8));
    if (out[4]) {
      const auto freshItem = half(member + 0x1c);
      put(out[4],
          freshItem < 0 ? 0 : word(base + uint32_t(freshItem) * 0x48 + 0xf26c));
    }
    return;
  }
  put(out[0], uint32_t(-1));
}
} // namespace ttyd
