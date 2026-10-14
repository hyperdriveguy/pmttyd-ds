#include "battle_weapon_aftereffect.hpp"
#include "harbor_tutorial_packed.hpp"
namespace ttyd {
bool HarborTutorialPacked::aftereffectEvent(EventEntry &e, int32_t &result) {
  struct Context {
    HarborTutorialPacked *p;
    BattleRandom *random;
  };
  auto preview = random;
  Context c{this, &preview};
  BattleWeaponAftereffectServices s;
  s.memory = nativeBattleActorServices(vm);
  s.values = nativeBattleUnitEventServices(vm).values;
  s.user = &c;
  s.random = [](void *self, int32_t bound) {
    return static_cast<Context *>(self)->random->bounded(bound);
  };
  s.request = [](void *self, uint32_t function, int32_t argument) {
    auto &p = *static_cast<Context *>(self)->p;
    const auto b = p.word(0x8041eb90);
    if (function == 0x80148bf0)
      return p.word(b + 0x18110) == 0; // Complete original null table leaf.
    if (function == 0x80148414) {
      (void)argument;
      uint16_t value = 0;
      if (!p.vm.memory.readHalf(p.word(0x8041eb00) + 0x88, value))
        return false;
      return int16_t(value) <= 0;
    }
    if (function == 0x801487c0)
      return (p.byte(b + 0x180f8) & 0x60) != 0; // Source already-falling gate.
    if (function == 0x80147644)
      return (p.word(b + 0xef4) & 0x1000) !=
             0; // Actual tutorial suppression, not a generic stage skip.
    return false;
  };
  // Native admission preview uses a copy of RNG and only non-mutating complete
  // leaf exits. Reject unsupported event-producing paths before live draws;
  // never search for another draw or synthesize a hazard result.
  if (!packedBattleWeaponAftereffectEvent(e, s))
    return false;
  c.random = &random;
  const auto completion = packedBattleWeaponAftereffectEvent(e, s);
  if (!completion)
    return false;
  result = *completion;
  return true;
}
} // namespace ttyd
