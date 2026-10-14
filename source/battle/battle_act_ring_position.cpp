#include "battle_act_ring_position.hpp"
#include <cstring>
namespace ttyd {
namespace {
float constant(int32_t offset, const BattleActRingPositionServices &s) {
  const auto n = s.memory.memory.word(s.memory.memory.user,
                                      0x80427440u + uint32_t(offset));
  float f;
  std::memcpy(&f, &n, 4);
  return f;
}
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
} // namespace
bool packedBattleActRingCenter(std::array<float, 3> &result,
                               const BattleActRingPositionServices &s) {
  const auto &m = s.memory.memory;
  const auto b = currentBattleAddress(s.memory);
  const auto unit =
      battleRegistryUnit(b, integer(m.word(m.user, b + 0x420)), s.memory);
  if (!s.project)
    return false;
  const auto position = s.project(s.user, unit + 0x3c);
  if (!position)
    return false;
  result = *position;
  result[0] += constant(-20220, s);
  result[1] += constant(-20216, s);
  result[2] = constant(-20500, s);
  const auto minimum = constant(-20212, s);
  if (result[0] <= minimum)
    result[0] = minimum;
  return true;
}
bool packedBattleActRingOffset(float theta, std::array<float, 2> &result,
                               const BattleActRingPositionServices &s) {
  const float angle = constant(-20224, s) - theta;
  if (!s.cosine)
    return false;
  const auto cosine = s.cosine(s.user, double(angle));
  if (!cosine)
    return false;
  result[0] = constant(-20240, s) * float(*cosine);
  if (!s.sine)
    return false;
  const auto sine = s.sine(s.user, double(angle));
  if (!sine)
    return false;
  result[1] = constant(-20284, s) * float(*sine);
  return true;
}
} // namespace ttyd
