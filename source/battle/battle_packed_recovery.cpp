#include "battle_packed_recovery.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v) {
  int32_t n;
  std::memcpy(&n, &v, 4);
  return n;
}
int32_t half(uint32_t at, const BattleActorServices &a) {
  const auto n = a.memory.half(a.memory.user, at);
  return n < 32768 ? int32_t(n) : int32_t(n) - 65536;
}
bool party(int32_t kind) { return kind >= 224 && kind < 231; }
} // namespace
int32_t packedBattleMaxFp(uint32_t u, const BattleActorServices &a) {
  const auto kind = signedWord(a.memory.word(a.memory.user, u + 8));
  const auto b = currentBattleAddress(a);
  const auto own = half(u + 0x10e, a);
  return party(kind) ? half(battleMarioUnit(b, a) + 0x10e, a) : own;
}
void recoverPackedBattleHp(uint32_t u, int32_t amount,
                           const BattleActorServices &a) {
  const auto &m = a.memory;
  const auto hp = u + 0x10c;
  m.storeHalf(m.user, hp, uint16_t(uint32_t(half(hp, a)) + uint32_t(amount)));
  const auto value = half(hp, a), maximum = half(u + 0x108, a);
  if (value > maximum)
    m.storeHalf(m.user, hp, uint16_t(maximum));
}
void recoverPackedBattleFp(uint32_t u, int32_t amount,
                           const BattleActorServices &a) {
  const auto &m = a.memory;
  const auto kind = signedWord(m.word(m.user, u + 8));
  const auto b = currentBattleAddress(a);
  uint32_t owner = u;
  int32_t maximum;
  if (party(kind)) {
    maximum = packedBattleMaxFp(battleMarioUnit(b, a), a);
    owner = battleMarioUnit(b, a);
  } else
    maximum = packedBattleMaxFp(u, a);
  const auto at = owner + 0x112;
  m.storeHalf(m.user, at, uint16_t(uint32_t(half(at, a)) + uint32_t(amount)));
  if (half(at, a) > maximum)
    m.storeHalf(m.user, at, uint16_t(maximum));
}
} // namespace ttyd
