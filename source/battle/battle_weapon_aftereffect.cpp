#include "battle_weapon_aftereffect.hpp"
namespace ttyd {
namespace {
int32_t sw(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000LL);
}
} // namespace
// Weapon operand resolves; current B is captured first. Initial weights load
// b6, b5, b7, b4 for irand(sum), then fresh b4/b5/b6 cumulative subtraction picks
// A1+A2/A1/A2/none. AFTER the draw,force wordB19078 masks10/20 seed A1/A2
// before cumulative selection;RNG callbacks may mutate those source fields.
std::optional<int32_t>
packedBattleWeaponAftereffectEvent(EventEntry &e,
                                   const BattleWeaponAftereffectServices &s) {
  const auto &m = s.memory.memory;
  const auto b = currentBattleAddress(s.memory), args = e.arguments;
  const auto weapon =
      uint32_t(s.values.resolve(s.values.user, e, sw(m.word(m.user, args))));
  auto byte = [&](uint32_t at) { return s.memory.byte(s.memory.user, at); };
  auto force = [&]() { return m.word(m.user, b + 0x19078); };
  auto request = [&](uint32_t f, int32_t argument = 0) {
    return s.request && s.request(s.user, f, argument);
  };
  const auto w6 = byte(weapon + 0xb6), w5 = byte(weapon + 0xb5),
             w7 = byte(weapon + 0xb7), w4 = byte(weapon + 0xb4);
  auto draw = s.random(s.user, w6 + w5 + w7 + w4);
  const auto initialForce = force();
  bool a1 = (initialForce & 0x10) != 0, a2 = (initialForce & 0x20) != 0,
       destructionB = false;
  draw = sw(uint32_t(draw) - byte(weapon + 0xb4));
  if (draw < 0) {
    a1 = true;
    a2 = true;
  } else {
    draw = sw(uint32_t(draw) - byte(weapon + 0xb5));
    if (draw < 0)
      a1 = true;
    else {
      draw = sw(uint32_t(draw) - byte(weapon + 0xb6));
      if (draw < 0)
        a2 = true;
    }
  }
  // Stage byte180f8 masks4/2 suppress A2/A1. Stage mask2 with
  // mask8 clear draws100 for weapon b8 or fresh force40: DestroyB, plus A2
  // unless stage mask4 now set. Dispatch A1, A2, B in that order.
  const auto stage = byte(b + 0x180f8);
  if (stage & 4)
    a2 = false;
  if (stage & 2)
    a1 = false;
  if ((stage & 2) && !(stage & 8)) {
    const auto roll = s.random(s.user, 100);
    if (roll < int32_t(byte(weapon + 0xb8)) || (force() & 0x40)) {
      destructionB = true;
      if (!(byte(b + 0x180f8) & 4))
        a2 = true;
    }
  }
  if (a1 && !request(0x801496f0))
    return std::nullopt;
  if (a2 && !request(0x80149684))
    return std::nullopt;
  if (destructionB && !request(0x801495e0))
    return std::nullopt;
  // Always draw each100 roll before fresh threshold b9/ba/bb and force
  //200/100/80: NozzleDirChange, NozzleWork(mode1), IronFrameFall. Final stage
  //mask20 admits ObjectFall: force1 skips its RNG, otherwise draw vs weapon bc.
  // Original request addresses select actual stage services, not visual guesses.
  for (unsigned i = 0; i < 3; ++i) {
    const auto roll = s.random(s.user, 100);
    const uint32_t masks[]{0x200, 0x100, 0x80},
        functions[]{0x80148bf0, 0x80148414, 0x801487c0};
    if (roll < int32_t(byte(weapon + 0xb9 + i)) || (force() & masks[i]))
      if (!request(functions[i], i == 1 ? 1 : 0))
        return std::nullopt;
  }
  if (byte(b + 0x180f8) & 0x20) {
    if (force() & 1) {
      if (!request(0x80147644))
        return std::nullopt;
    } else {
      const auto roll = s.random(s.user, 100);
      if (roll < int32_t(byte(weapon + 0xbc)) && !request(0x80147644))
        return std::nullopt;
    }
  }
  return 2;
}
} // namespace ttyd
