#include "battle_ap_report.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t sw(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000LL);
}
uint32_t base(const BattleActorServices &s) {
  return currentBattleAddress(s) + 0x2858;
}
uint32_t record(int32_t i, const BattleActorServices &s) {
  return base(s) + 0x1bc + 0x134 * uint32_t(i);
}
bool exists(int32_t i, const BattleActorServices &s) {
  (void)base(s);
  const auto p = record(i, s), f = s.memory.word(s.memory.user, p);
  return (f & 1) && !(f & 0x102);
}
bool present(int32_t i, const BattleActorServices &s) {
  const auto p = record(i, s);
  if (!exists(i, s))
    return false;
  const auto state = s.byte(s.user, p + 0x19);
  return state != 12 && state != 13 && state != 17;
}
float real(uint32_t at, const EventMemoryServices &m) {
  const auto bits = m.word(m.user, at);
  float v;
  std::memcpy(&v, &bits, 4);
  return v;
}
} // namespace
int32_t packedBattleAudiencePpKindCount(int32_t kind,
                                        const BattleActorServices &s) {
  (void)base(s);
  int32_t count = 0;
  for (int32_t i = 0; i < 200; ++i)
    if (present(i, s) && s.byte(s.user, record(i, s) + 0x1b) == kind)
      ++count;
  return count;
}
bool packedBattleBuildApReport(uint32_t input,
                               const BattleApReportServices &s) {
  const auto &m = s.memory.memory;
  auto word = [&](uint32_t at) { return m.word(m.user, at); };
  auto byte = [&](uint32_t at) { return s.memory.byte(s.memory.user, at); };
  auto store = [&](uint32_t at, uint32_t v) { m.storeWord(m.user, at, v); };
  const auto b = currentBattleAddress(s.memory), a = base(s.memory);
  const auto pointer = a + 0x137c8;
  if (!word(pointer)) {
    if (!s.allocate)
      return false;
    const auto out = s.allocate(s.user, 12);
    if (!out)
      return false;
    store(pointer, *out);
  }
  const auto out = word(pointer);
  for (unsigned i = 0; i < 12; ++i)
    m.storeByte(m.user, out + i, byte(input + i));
  const auto mario = s.mario(s.user, b), party = s.party(s.user, b);
  float weight = real(0x80425548, m);
  for (int32_t i = 0; i < 13; ++i) {
    const auto count = s.kindCount(s.user, i);
    const auto coefficient = real(0x803b4de4 + 0x20 * uint32_t(i) + 0x14, m);
    weight = s.fma(s.user, coefficient, s.wordFloat(s.user, count), weight);
  }
  const auto audience =
      s.roundFloat(s.user, s.squareRoot(s.user, double(weight)));
  const auto rawAddition = byte(input + 8);
  const auto ac = real(input + 4, m);
  const auto addition = s.wordFloat(s.user, rawAddition);
  float factors = real(0x80425548, m);
  const auto one = real(0x80425550, m);
  if (ac > real(0x80425548, m))
    factors = s.add(s.user, factors, one);
  if (addition > real(0x80425548, m))
    factors = s.add(s.user, factors, one);
  const auto mf = word(mario + 0x138);
  const auto marioFactor = real(mf & 0x20000000   ? 0x8042558c
                                : mf & 0x10000000 ? 0x80425590
                                                  : 0x80425550,
                                m);
  float partyFactor = one;
  if (party) {
    const auto pf = word(party + 0x138);
    partyFactor = real(pf & 0x20000000   ? 0x80425590
                       : pf & 0x10000000 ? 0x804255a0
                                         : 0x80425550,
                       m);
  }
  const auto turn = s.breakTurn(s.user);
  const auto slot = turn >= 1 ? s.breakSlot(s.user) : 0;
  const auto sum = s.add(s.user, ac, addition);
  float gain;
  uint32_t old;
  if (turn >= 1) {
    const auto bonus = real(slot + 0x14, m);
    gain = s.multiply(s.user, one, audience);
    old = word(a + 0x137c4);
    gain = s.multiply(s.user, gain, sum);
    gain = s.multiply(s.user, gain, factors);
    gain = s.multiply(s.user, gain, marioFactor);
    gain = s.multiply(s.user, gain, partyFactor);
    gain = s.multiply(s.user, gain, bonus);
  } else {
    old = word(a + 0x137c4);
    gain = s.multiply(s.user, one, audience);
    gain = s.multiply(s.user, gain, sum);
    gain = s.multiply(s.user, gain, factors);
    gain = s.multiply(s.user, gain, marioFactor);
    gain = s.multiply(s.user, gain, partyFactor);
  }
  store(a + 0x137c4, old + uint32_t(s.truncate(s.user, gain)));
  const auto draw = s.random(s.user, 100);
  if (draw < int32_t(byte(input + 0xa)))
    return s.pointIncrement && s.pointIncrement(s.user);
  return true;
}
bool packedBattleInviteApReport(const BattleApReportServices &s) {
  const auto input = currentBattleAddress(s.memory) + 0x18ffc;
  if (!packedBattleBuildApReport(input, s))
    return false;
  for (unsigned i = 0; i < 12; ++i)
    s.memory.memory.storeByte(s.memory.memory.user, input + i, 0);
  return true;
}
} // namespace ttyd
