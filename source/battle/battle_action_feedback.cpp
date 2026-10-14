#include "battle_action_feedback.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t sw(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000LL);
}
uint32_t base(const BattleActorServices &m) {
  return currentBattleAddress(m) + 0x2858;
}
uint32_t actor(int32_t id, const BattleActorServices &m) {
  return base(m) + 0x1bc + 0x134 * uint32_t(id);
}
bool exists(int32_t id, const BattleActorServices &m) {
  (void)base(m);
  const auto p = actor(id, m), f = m.memory.word(m.memory.user, p);
  return (f & 1) && !(f & 0x100) && !(f & 2);
}
bool sysctrl(int32_t id, const BattleActorServices &m) {
  (void)base(m);
  const auto p = actor(id, m);
  return exists(id, m) && !(m.memory.word(m.memory.user, p) & 0x10);
}
float real(uint32_t at, const EventMemoryServices &m) {
  const auto v = m.word(m.user, at);
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
} // namespace
int32_t packedBattleAudienceWaiting(int32_t id, const BattleActorServices &m) {
  (void)base(m);
  const auto p = actor(id, m);
  if (!sysctrl(id, m))
    return 0;
  if (m.memory.half(m.memory.user, p + 0x1c) != 0xffff)
    return 0;
  return m.byte(m.user, p + 0x19) <= 3 ? 1 : 0;
}
int32_t packedBattleAudienceWaitingCount(const BattleActorServices &m) {
  int32_t count = 0;
  for (int32_t i = 0; i < 200; ++i)
    if (uint8_t(packedBattleAudienceWaiting(i, m)) == 1)
      ++count;
  return count;
}
bool packedBattleAudienceActionFeedback(
    uint32_t weapon, bool good, const BattleAudienceFeedbackServices &s) {
  const auto a = base(s.memory);
  const auto count = packedBattleAudienceWaitingCount(s.memory);
  if (count) {
    if (!s.request)
      return false;
    if (good) {
      if (!s.request(s.user, 0x801a0aec, 180, 120))
        return false;
    } else if (!s.request(s.user, 0x801a0fd0, 0, 0))
      return false;
    if (!s.request(s.user, 0x801a12fc, 0, 0))
      return false;
    if (!good &&
        s.memory.memory.word(s.memory.memory.user, weapon + 0x1c) == 0x80189c38)
      if (!s.request(s.user, 0x801a1520, 7, 0))
        return false;
  }
  const auto &m = s.memory.memory;
  m.storeWord(m.user, a, m.word(m.user, a) | (good ? 0x10u : 0x20u));
  return true;
}
void packedBattleAudienceAcrobatNoTry(const BattleActorServices &m) {
  (void)base(m);
}
std::optional<int32_t>
packedBattleDeclareActionResultEvent(EventEntry &e,
                                     const BattleActionFeedbackServices &s) {
  const auto &m = s.memory.memory;
  const auto args = e.arguments, b = currentBattleAddress(s.memory);
  const auto weapon =
      uint32_t(s.values.resolve(s.values.user, e, sw(m.word(m.user, args))));
  const auto result =
      s.values.resolve(s.values.user, e, sw(m.word(m.user, args + 4)));
  const auto count = b + (result == -1 ? 0x16f59u : 0x16f58u);
  const auto old = s.memory.byte(s.memory.user, count);
  if (old < 200)
    m.storeByte(m.user, count, uint8_t(old + 1));
  m.storeByte(m.user, b + 0x19074, result == -1 ? 0 : 1);
  if (result != -1) {
    const auto f = s.signedWordFloat(s.user, result);
    const auto multiplier =
        s.fma(s.user, real(0x804221a4, m), f, real(0x804221a0, m));
    m.storeWord(m.user, b + 0x18ffc, weapon);
    uint32_t bits;
    std::memcpy(&bits, &multiplier, 4);
    m.storeWord(m.user, b + 0x19000, bits);
    m.storeByte(m.user, b + 0x19005,
                s.memory.byte(s.memory.user, weapon + 0x19));
    m.storeByte(m.user, b + 0x19006,
                s.memory.byte(s.memory.user, weapon + 0x1a));
  }
  if (!s.audience || !s.audience(s.user, weapon, result != -1))
    return std::nullopt;
  return 2;
}
} // namespace ttyd
