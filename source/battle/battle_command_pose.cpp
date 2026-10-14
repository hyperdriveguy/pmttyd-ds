#include "battle_command_pose.hpp"
#include "battle_packed_part.hpp"
#include "battle_packed_pose_lookup.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
} // namespace
int32_t packedBattleBodyPartId(uint32_t u, const EventMemoryServices &m) {
  if (!u)
    return -1;
  for (auto p = m.word(m.user, u + 0x14); p; p = m.word(m.user, p))
    if (m.word(m.user, p + 0x1ac) & 1)
      return integer(m.word(m.user, m.word(m.user, p + 4)));
  return -1;
}
std::optional<int32_t>
packedBattleStatusPoseType(uint32_t u, const BattleCommandPoseServices &s) {
  const auto &m = s.memory.memory;
  if (m.word(m.user, u + 0x104) & 0x20000)
    return 27;
  for (uint32_t table = 0x80416ab0;; table += 4) {
    const auto id = integer(m.word(m.user, table));
    if (id == -1)
      return 28;
    if (!s.status)
      return std::nullopt;
    const auto low = uint8_t(id);
    const auto active =
        s.status(s.user, u, low < 128 ? int32_t(low) : int32_t(low) - 256);
    if (!active)
      return std::nullopt;
    if (*active)
      return id;
  }
}
void packedBattlePoseAnime(uint32_t p, const EventMemoryServices &m) {
  auto change = [&](uint32_t clear, uint32_t set) {
    m.storeWord(m.user, p + 0x204, (m.word(m.user, p + 0x204) & ~clear) | set);
  };
  change(0x12, 0);
  change(0, 1);
  change(0x80, 0);
  change(4, 0);
}
bool packedBattleSetAnimationType(uint32_t p, int32_t type,
                                  const BattleCommandPoseServices &s) {
  const auto &m = s.memory.memory;
  const auto name = packedBattlePoseName(p, type, m);
  const bool force = type >= 29 && type < 40;
  if (!s.copyName || !s.copyName(s.user, p, name))
    return false;
  auto change = [&](uint32_t clear, uint32_t set) {
    m.storeWord(m.user, p + 0x204, (m.word(m.user, p + 0x204) & ~clear) | set);
  };
  change(0, 2);
  change(0, 1);
  change(0x80, 0);
  change(0x200, 0);
  change(0x10, 0);
  if (force)
    change(0, 8);
  else
    change(8, 0);
  if (type >= 29 && type < 31)
    change(0, 0x10);
  return true;
}
bool packedBattleCommandPose(uint32_t u, const BattleCommandPoseServices &s) {
  const auto &m = s.memory.memory;
  assert(m.word && m.storeWord);
  const auto id = packedBattleBodyPartId(u, m);
  const auto p = packedUnitPart(u, id, m);
  bool command = false;
  if (!(m.word(m.user, u + 0x138) & 0x30000000)) {
    const auto pose = packedBattleStatusPoseType(u, s);
    if (!pose)
      return false;
    command = *pose == 28;
  }
  if (command) {
    if (!packedBattleSetAnimationType(p, 67, s))
      return false;
    m.storeWord(m.user, p + 0x204, m.word(m.user, p + 0x204) | 0x80);
  } else
    packedBattlePoseAnime(p, m);
  return true;
}
} // namespace ttyd
