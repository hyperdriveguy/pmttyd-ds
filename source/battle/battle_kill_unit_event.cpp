#include "battle_kill_unit_event.hpp"
#include "battle_packed_unit_initialization.hpp"
#include "event_manager.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, CurrentHp = 0x10c,
                   UnitFlags = 0x104, RemovedFlag = 0x20000,
                   DeadStatus = 0x135, Attributes = 0x1c,
                   InactiveAttribute = 0x80000000;
constexpr uint32_t ClearStatus = 0x80127b58;
int32_t signedWord(uint32_t v) {
  return v <= 0x7fffffff ? int32_t(v) : int32_t(int64_t(v)-0x100000000ll);
}
}
int32_t battleKillUnitEvent(EventEntry& e, const BattleUnitEventServices& s,
                           const BattleActorServices& a) {
  const auto& m=s.memory;
  assert(m.word && s.values.resolve && s.translate && s.unit);
  const auto args=e.arguments;
  const auto actor=s.values.resolve(s.values.user,e,signedWord(m.word(m.user,args)));
  const auto id=s.translate(s.user,e,actor);
  const auto battle=m.word(m.user,BattleGlobal);
  const auto unit=s.unit(s.user,battle,id);
  const auto mode=m.word(m.user,args+4); // Raw, captured after registry even null.
  if(!unit)return 2;
  assert(m.storeHalf && m.storeWord && m.storeByte);
  m.storeHalf(m.user,unit+CurrentHp,0);
  const auto completed=initializePackedBattleUnitHelper(ClearStatus,unit,a);
  assert(completed);(void)completed;
  if(mode&2){
    const auto flags=m.word(m.user,unit+UnitFlags);
    m.storeWord(m.user,unit+UnitFlags,flags|RemovedFlag);
    return 2;
  }
  // Whole SetStatus(unit,27,1,1) path: dispatches directly to80127f1c.
  m.storeByte(m.user,unit+DeadStatus,1);
  if(!(mode&1)){
    const auto attributes=m.word(m.user,unit+Attributes);
    m.storeWord(m.user,unit+Attributes,attributes|InactiveAttribute);
  }
  return 2;
}
}
