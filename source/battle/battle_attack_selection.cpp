#include "battle_attack_selection.hpp"
#include <cassert>
#include <initializer_list>
namespace ttyd {
namespace {
uint32_t word(const BattleActorServices& m,uint32_t at){assert(m.memory.word);return m.memory.word(m.memory.user,at);}
int32_t byte(const BattleActorServices& m,uint32_t at){assert(m.byte);const auto v=m.byte(m.user,at);return v<128?int32_t(v):int32_t(v)-256;}
void store(const BattleActorServices& m,uint32_t at,uint32_t v){assert(m.memory.storeWord);m.memory.storeWord(m.memory.user,at,v);}
void count(const BattleActorServices& m,uint32_t at){assert(m.byte && m.memory.storeByte);const auto v=m.byte(m.user,at);if(v<100)m.memory.storeByte(m.memory.user,at,uint8_t(v+1));}
}
// Predicate only, not damage evaluation: nonnull HP callback1c or exact
// signed status -lane patterns qualify. FP power alone is not checked here.
// Preserve positive pairs/negative third-lane tests rather than any-nonzero.
int32_t packedWeaponIsAttack(uint32_t w,const BattleActorServices& m){
 if(!w)return 0;
 if(word(m,w+0x1c))return 1;
 for(auto at:{0x80u,0x82u,0x84u,0x86u,0x89u,0x8fu,0x91u})if(byte(m,w+at)>0 && byte(m,w+at+1)>0)return 1;
 for(auto at:{0x93u,0x96u,0x99u})if(byte(m,w+at)>0 && byte(m,w+at+1)>0 && byte(m,w+at+2)<0)return 1;
 for(auto at:{0x9eu,0xa4u,0xa5u})if(byte(m,w+at)>0)return 1;
 return byte(m,w+0xa2)>0 && byte(m,w+0xa3)>0;
}
uint32_t packedUnitData(uint32_t u,uint32_t key,const BattleActorServices& m){
 auto table=word(m,u+0x18);if(!table)return 0;
 for(;;table+=8){const auto candidate=word(m,table);if(!candidate)return 0;if(candidate==key)return word(m,table+4);}
}
// Selection writes event2a0/category2ac and action-record bytes (<100 only),
// but creates NO event root. override551 nonzero resets unit state 20=0 and
// publishes Wait 0x80416b40/category1. Other unknown types use Wait/category0.
bool resumePackedBattleAttackSelection(uint32_t u, uint32_t menu,
                                       const BattleAttackSelectionServices &s,
                                       BattleAttackSelectionCursor &c) {
  const auto &m = s.memory;
  assert(m.byte && m.memory.storeByte);
  if (c.active &&
      (c.owner.user != m.memory.user || c.owner.word != m.memory.word ||
       c.owner.storeWord != m.memory.storeWord ||
       c.owner.storeByte != m.memory.storeByte || c.actorOwner != m.user ||
       c.byteReader != m.byte || c.unit != u || c.menu != menu))
    return false;
  auto finish = [&]() {
    c = {};
    return true;
  };
  auto continueMessage = [&]() {
    if (s.continueMessageReady) {
      if (!s.continueMessageReady(s.user, c.message))
        return false;
    } else if (s.continueMessage)
      s.continueMessage(s.user, c.message);
    else
      return false;
    return finish();
  };
  if (c.active && c.phase == 2)
    return continueMessage();
  if (!c.active) {
    c.active = true;
    c.phase = 1;
    c.owner = m.memory;
    c.actorOwner = m.user;
    c.byteReader = m.byte;
    c.unit = u;
    c.menu = menu;
    c.overrideByte = m.byte(m.user, menu + 0x551);
    c.battle = word(m, 0x8041eb90);
    c.type = word(m, menu + 0x540);
  }
  const auto overrideByte = c.overrideByte;
  const auto b = c.battle, type = c.type;
  auto event = [&](uint32_t script, uint32_t category) {
    store(m, u + 0x2a0, script);
    store(m, u + 0x2ac, category);
  };
  auto familyCount = [&](uint32_t mario, uint32_t other) {
    count(m, b + (word(m, u + 8) == 222 ? mario : other));
  };
  constexpr uint32_t Wait = 0x80416b40;
  if (overrideByte) {
    m.memory.storeByte(m.memory.user, u + 0x20, 0);
    event(Wait, 1);
    return finish();
  }
  switch (type) {
  // Types 0/1/2/4/6 get28-byte weapon rows, script+b0/category0; null script
  // alone falls back Wait. Record selectors16f38/39, 3a/3b, 55/56 and family
  //41..44 differ by type/attack predicate. Type 2 weapon8036aa84 skips count only.
  case 0:
  case 1:
  case 2:
  case 4:
  case 6: {
    auto &cursor = c.cursor;
    assert(s.cursor);
    if (!s.cursor(s.user, menu, type, cursor))
      return false;
    const auto w = word(m, menu + 28 * word(m, cursor) + 0x80);
    event(word(m, w + 0xb0), 0);
    if (!word(m, u + 0x2a0))
      store(m, u + 0x2a0, Wait);
    if (type == 0)
      count(m, b + 0x16f38);
    else if (type == 1)
      count(m, b + 0x16f39);
    else if (type == 4)
      count(m, b + (packedWeaponIsAttack(w, m) ? 0x16f3a : 0x16f3b));
    else if (type == 6)
      count(m, b + (packedWeaponIsAttack(w, m) ? 0x16f55 : 0x16f56));
    else if (w != 0x8036aa84) {
      const bool mario = word(m, u + 8) == 222;
      const bool attack = packedWeaponIsAttack(w, m) != 0;
      count(m, b + (mario ? (attack ? 0x16f41 : 0x16f42)
                          : (attack ? 0x16f43 : 0x16f44)));
    }
    return finish();
  }
  // Operation548 selects exact scripts/categories/counters below. Sub1/2
  // use fixed Mario/partner weapon scripts without ordinary Wait fallback;
  // sub 8 uses data key2/category3 even script0. Unused/unknown sub defaults Wait1.
  case 5: {
    const auto sub = word(m, menu + 0x548);
    switch (sub) {
    case 0:
      event(0x8034de14, 1);
      store(m, menu + 0x4a4, 0);
      if (word(m, 0x80354208) & 1)
        m.memory.storeByte(m.memory.user, u + 0x20, 3);
      familyCount(0x16f45, 0x16f46);
      break;
    case 1:
    case 2: {
      store(m, u + 0x2ac, 0);
      const bool mario = word(m, u + 8) == 222;
      const auto w = sub == 1 ? (mario ? 0x803550a8 : 0x80355168)
                              : (mario ? 0x80355228 : 0x803552e8);
      store(m, u + 0x2a0, word(m, w + 0xb0));
      count(m, b + (sub == 1 ? (mario ? 0x16f4f : 0x16f50)
                             : (mario ? 0x16f51 : 0x16f52)));
      break;
    }
    case 3:
      event(0x8034e7a4, 1);
      familyCount(0x16f53, 0x16f54);
      break;
    case 4:
      event(0x80416b10, 1);
      break;
    case 5:
      event(0x8034e41c, 1);
      familyCount(0x16f49, 0x16f4a);
      break;
    case 6:
      store(m, u + 0x2ac, 1);
      store(m, u + 0x2a0, word(m, u + 8) == 222 ? 0x80416b6c : 0x80416b88);
      break;
    case 7:
      event(Wait, 1);
      familyCount(0x16f49, 0x16f4a);
      break;
    case 8: {
      const auto script = packedUnitData(u, 2, m);
      event(script, 3);
      break;
    }
    default:
      event(Wait, 1);
      break;
    }
    return finish();
  }
  case 9:
    m.memory.storeByte(m.memory.user, u + 0x20, 1);
    event(0x80351754, 7);
    return finish();
  // Types 11/12 rewind state 6->5, 7->6, 9->8, else4. Type 11 recaptures
  // global registry actor for data6 (fallback80373d00), counts47/48, then may
  // hold message19094 from captured B+0x19090 mask 1. Resume phase 2 performs
  // ONLY continuation, never state/counter publication again. Type 12 independently
  // reads current audience band16040 to choose fixed source script; type 13 is fixed.
  case 11:
  case 12: {
    const auto old = byte(m, u + 0x20);
    m.memory.storeByte(m.memory.user, u + 0x20,
                       uint8_t(old == 6   ? 5
                               : old == 7 ? 6
                               : old == 9 ? 8
                                          : 4));
    if (type == 11) {
      const auto id = word(m, u), current = word(m, 0x8041eb90);
      const auto actor = battleRegistryUnit(current, int32_t(id), m);
      auto script = packedUnitData(actor, 6, m);
      if (!script)
        script = 0x80373d00;
      event(script, 3);
      familyCount(0x16f47, 0x16f48);
      if (word(m, b + 0x19090) & 1) {
        c.message = word(m, b + 0x19094);
        c.phase = 2;
        return continueMessage();
      }
    } else {
      (void)word(m, u);
      const auto current = word(m, 0x8041eb90);
      event(word(m, current + 0x16040) == 1 ? 0x80373d3c : 0x80373d74, 3);
    }
    return finish();
  }
  case 13:
    event(0x80416b54, 3);
    return finish();
  default:
    event(Wait, 0);
    return finish();
  }
}
bool packedSetBattleAttackEvent(uint32_t u, uint32_t menu,
                                const BattleAttackSelectionServices &s) {
  BattleAttackSelectionCursor cursor;
  return resumePackedBattleAttackSelection(u, menu, s, cursor);
}

}
