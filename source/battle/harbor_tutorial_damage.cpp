#include "harbor_tutorial_packed.hpp"
#include "battle_hit_code_events.hpp"
#include "battle_packed_counter.hpp"
#include "battle_packed_hit_event.hpp"
#include "battle_weapon_power.hpp"
#include "battle_weapon_cost.hpp"
#include "pouch_item_count.hpp"
#include "battle_packed_part.hpp"
#include <cassert>
#include <cstdlib>
namespace ttyd {
namespace {
constexpr int32_t LifeShroomItem=151,MarioKind=222,FirstPartnerKind=224,LastPartnerKind=230;
constexpr uint32_t PouchPointer=0x8041eb00,RegularItemsOffset=0x192,RegularItemCount=20;
uint32_t damageBodyPart(HarborTutorialPacked& p,uint32_t unit){
 const auto memory=nativeBattleActorServices(p.vm).memory;
 return unit?packedUnitPart(unit,packedBodyPartId(unit,memory),memory):0;
}
}
// Translate packed BE unit/part fields into typed calculator storage. Native
// slots0..3 remain typed array indices; guest units and body parts come from
// the actual live source registry/part chain. Source kind pointer is unit+10.
// Source offsets:108/10c max/current HP,10e/112 max/current FP;118..135
//30 status bytes;138 status flags;27c flags;264/268/26c requested HP/FP/cost;
//270..272 byte reports,274 pattern,1fe..206 reaction halfwords. Unnamed
// field200/202/204/206 remain numeric until their meaning is established.
void HarborTutorialPacked::readDamage(unsigned id){
 auto& d=damageUnits[id];const auto u=unitAddress(int32_t(id)),p=damageBodyPart(*this,u),k=word(u+0x10);
 // Interpret the unsigned BE half as signed16 explicitly.32768/65536 are
 // the sign boundary/modulus, not HP caps; unreadable admitted memory aborts.
 auto half=[&](uint32_t at){uint16_t v=0;if(!vm.memory.readHalf(at,v))std::abort();return int16_t(v<32768?int(v):int(v)-65536);};
 d.kind=int32_t(word(u+8));d.belong=int8_t(byte(u+0xc));d.heldItem=word(u+0x308);d.enemyAudienceByte=byte(u+0xd);d.hp=half(u+0x10c);d.maxHp=half(u+0x108);d.fp=half(u+0x112);d.maxFp=half(u+0x10e);
 d.flags=word(u+0x27c);d.statusFlags=word(u+0x138);d.attributes=word(u+0x104);d.extraDefense=int32_t(word(u+0x21c));d.damageSequence=half(u+0x260);d.attackSequence=int32_t(word(u+0x280));
 d.fpCostReduction=byte(u+0x2f5);d.danger=byte(k+0xc);d.peril=byte(k+0xd);d.zapTap=byte(u+0x2f0);
 for(unsigned i=0;i<30;++i)d.statusBytes[i]=byte(u+0x118+i);
 //2e4..301 are original badge/bonus bytes, not a contiguous native badge
 // enum. Kind+c/d supply danger/peril thresholds; unit+2f0 is ZapTap count.
 auto& b=d.badges;b.powerPlus=byte(u+0x2e4);b.pUpDDown=byte(u+0x2e5);b.allOrNothing=byte(u+0x2e6);b.megaRush=byte(u+0x2e7);b.powerRush=byte(u+0x2e8);b.pDownDUp=byte(u+0x2e9);b.doublePain=byte(u+0x2ea);b.lastStand=byte(u+0x2eb);b.defendPlus=byte(u+0x2ec);b.damageDodge=byte(u+0x2ed);b.pityFlower=byte(u+0x2f8);b.hpDrain=byte(u+0x2f9);b.fpDrain=byte(u+0x2fa);b.icePower=byte(u+0x301);
 d.hpBefore=int32_t(word(u+0x208));d.hpAfter=int32_t(word(u+0x20c));d.lastHp=byte(u+0x270);d.lastFp=byte(u+0x271);d.lastConsumedFp=byte(u+0x272);d.pattern=int32_t(word(u+0x274));
 d.dealtHp=int32_t(word(u+0x284));d.requestedHp=int32_t(word(u+0x264));d.requestedFp=int32_t(word(u+0x268));d.consumedFp=int32_t(word(u+0x26c));d.damageFlags=uint16_t(half(u+0x1fe));
 d.field200=uint16_t(half(u+0x200));d.field202=uint16_t(half(u+0x202));d.field204=uint16_t(half(u+0x204));d.field206=uint16_t(half(u+0x206));
 damageParts[id].attributes=word(p+0x1ac);
 // Part+1ac attributes;1b4/1b8 point to five signed element defenses and
 // attributes. Kind+9c has four sound pointers;128 is a native string-copy
 // safety bound, not an observed original maximum. Owned strings back c_str().
 for(unsigned i=0;i<5;++i){defenses[id][i]=int8_t(byte(word(p+0x1b4)+i));defenseAttributes[id][i]=int8_t(byte(word(p+0x1b8)+i));}
 damageParts[id].defense=defenses[id].data();damageParts[id].defenseAttributes=defenseAttributes[id].data();
 for(unsigned i=0;i<4;++i){auto& name=damageSounds[id][i];name.clear();const auto at=word(k+0x9c+4*i);if(at)for(unsigned n=0;n<128;++n){const auto c=byte(at+n);if(!c)break;name+=char(c);}d.damageSounds[i]=at?name.c_str():nullptr;}
}
// Publish only fields this admitted damage path mutates. Do not bulk-copy
// the typed snapshot: other source fields/guest owners must remain intact.
void HarborTutorialPacked::writeDamage(unsigned id){
 const auto& d=damageUnits[id];const auto u=unitAddress(int32_t(id));
 half(u+0x10c,d.hp);half(u+0x112,d.fp);word(u+0x27c,d.flags);word(u+0x284,d.dealtHp);
 word(u+0x208,d.hpBefore);word(u+0x20c,d.hpAfter);word(u+0x264,d.requestedHp);word(u+0x268,d.requestedFp);word(u+0x26c,d.consumedFp);
 byte(u+0x270,d.lastHp);byte(u+0x271,d.lastFp);byte(u+0x272,d.lastConsumedFp);word(u+0x274,d.pattern);half(u+0x1fe,d.damageFlags);half(u+0x200,d.field200);half(u+0x204,d.field204);
 for(unsigned i=0;i<30;++i)byte(u+0x118+i,d.statusBytes[i]);
}
// Typed addresses point into fixed arrays for registered slots0..3. Effects,
// sounds and notices are retained requests, not proof of rendered/audio output.
// Hit callbacks publish the target's current damage snapshot BEFORE dispatch.
DamageApplicationServices HarborTutorialPacked::damageApplication(){
 DamageApplicationServices s;s.user=this;
 s.getUnit=[](void* self,int32_t id)->DamageUnit*{auto& p=*static_cast<HarborTutorialPacked*>(self);return id>=0 && id<4 && p.unitAddress(id)?&p.damageUnits[size_t(id)]:nullptr;};
 s.bodyPart=[](void* self,DamageUnit& d){auto& p=*static_cast<HarborTutorialPacked*>(self);return &p.damageParts[size_t(&d-p.damageUnits.data())];};
 s.hitPosition=[](void* self,DamageUnit& d,DamagePart&){auto& p=*static_cast<HarborTutorialPacked*>(self);const auto id=unsigned(&d-p.damageUnits.data());const auto unit=p.unitAddress(int32_t(id));const auto pos=packedHitPosition(unit,damageBodyPart(p,unit),nativeBattlePartPositionServices(p.vm));return DamagePosition{pos[0],pos[1],pos[2]};};
 s.effect=[](void* self,const DamageEffect& effect){static_cast<HarborTutorialPacked*>(self)->damageEffects.push_back(effect);};
 s.sound=[](void* self,const char* key,DamagePosition position){if(!key)return;auto& p=*static_cast<HarborTutorialPacked*>(self);HarborTutorialSoundRequest request;request.name=key;request.key=0;const auto id=p.word(p.damageSoundUnit);for(unsigned n=0;n<4;++n)if(id<4 && p.damageSounds[id][n]==key){request.key=p.word(p.word(p.damageSoundUnit+0x10)+0x9c+4*n);break;}request.handle=p.nextSoundHandle++;request.owner=int32_t(id);request.position={position.x,position.y,position.z};p.sounds.push_back(std::move(request));};
 s.notice=[](void* self,DamageUnit&,DamageNotice notice){auto& p=*static_cast<HarborTutorialPacked*>(self);p.damageNotices.push_back(notice);if(notice.kind==DamageNoticeKind::ActionCount){auto v=p.byte(p.battleAddress()+notice.fieldOffset);addActionCount(v);p.byte(p.battleAddress()+notice.fieldOffset,v);}else if(notice.kind==DamageNoticeKind::ActionPoint){auto v=p.byte(p.battleAddress()+notice.fieldOffset);addActionPoint(v,notice.value);p.byte(p.battleAddress()+notice.fieldOffset,v);}};
 s.hitEvent=[](void* self,DamageUnit& d,uint32_t code){auto& p=*static_cast<HarborTutorialPacked*>(self);const auto id=unsigned(&d-p.damageUnits.data());p.writeDamage(id);packedBattleRunHitEvent(p.unitAddress(int32_t(id)),code,nativeBattleUnitEventServices(p.vm));};
 s.inventory.user=this;s.inventory.itemCount=[](void* self,int32_t item){auto& p=*static_cast<HarborTutorialPacked*>(self);return packedPouchItemCount(item,nativeBattleActorServices(p.vm).memory);};
 return s;
}
// Scope is Mario(slot1) <-> Crump(slot2):source power callbacks80189d30
// default,80189cb0 jump,80189c38 hammer. Reject unsupported branches BEFORE
// live calculation/RNG. False is native unavailability, never a miss/zero hit.
bool HarborTutorialPacked::checkDamageCore(uint32_t a,uint32_t d,uint32_t part,uint32_t w,uint32_t flags,int32_t& result){
 // Admission is explicit until guard effects, nonordinary status/counter branches
 // and general inventory/recovery providers are bound. No guessed result exits.
 if(!a || !d)return false;const auto ai=word(a),di=word(d),callback=word(w+0x1c);
 const bool boss=ai==2 && di==1 && callback==0x80189d30;
 const bool mario=ai==1 && di==2 && (callback==0x80189cb0 || callback==0x80189c38);
 // Weapon+40 FP callback must be null;6c element0..4,6d unused mode0;
 // B+18ff9 battle mode0. B+1cb0 defense results above3 would need more guard
 // providers. Require weapon80..ae status payload, unit118..135 status and
 //2e4..301 badge/bonus bytes all zero; this is narrow admission, not game rules.
 if(ai>=damageUnits.size() || di>=damageUnits.size() || unitAddress(int32_t(ai))!=a || unitAddress(int32_t(di))!=d)return false;
 if((!boss && !mario) || part!=damageBodyPart(*this,d) || word(w+0x40) || byte(w+0x6c)>=5 || byte(w+0x6d)!=0 || byte(battleAddress()+0x18ff9)!=0)return false;
 if(packedACPossibility(d,nativeBattleActorServices(vm)) && int32_t(word(battleAddress()+0x1cb0))>3)return false;
 for(unsigned i=0x80;i<=0xae;++i)if(byte(w+i))return false;
 for(auto u:{a,d}){for(unsigned i=0x118;i<=0x135;++i)if(byte(u+i))return false;for(unsigned i=0x2e4;i<=0x301;++i)if(byte(u+i))return false;}
 // Run source counter precheck, then reject a counter in work word9.
 // No ordinary-hit substitution is permitted for an unsupported counter.
 BattleCounterWork counter;counter.init();BattlePackedCounterServices cs;cs.memory=nativeBattleActorServices(vm);cs.user=this;cs.status=[](void* self,uint32_t u,int32_t type){auto& p=*static_cast<HarborTutorialPacked*>(self);return checkPackedBattleStatus(u,type,nativeBattleActorServices(p.vm));};auto checkedFlags=flags;packedBattleCheckCounter(counter,a,d,part,w,checkedFlags,cs);if(counter.words[9])return false;
 readDamage(ai);readDamage(di);
 struct Power{HarborTutorialPacked* p;uint32_t actor,weapon;};Power power{this,a,w};WeaponPowerServices ps;ps.user=&power;
 // B+1cb8 is source action result. Global pouch pointer8041eb00 supplies
 //98/99 jump/hammer level, unit2ff/300 their bonuses, weapon20 power words.
 // Weapon74 retains special flags. Only no-LifeShroom ordinary lethal hits
 // are admitted below; source application/event code owns the actual HP store.
 ps.actionResult=[](void* self){return static_cast<Power*>(self)->p->word(static_cast<Power*>(self)->p->battleAddress()+0x1cb8);};
 ps.pouchLevel=[](void* self,MarioPowerKind kind){auto& c=*static_cast<Power*>(self);return c.p->byte(c.p->word(0x8041eb00)+(kind==MarioPowerKind::Jump?0x98:0x99));};
 ps.bonus=[](void* self,const DamageUnit&,MarioPowerKind kind){auto& c=*static_cast<Power*>(self);return c.p->byte(c.actor+(kind==MarioPowerKind::Jump?0x2ff:0x300));};
 ps.powerWord=[](void* self,int32_t index){auto& c=*static_cast<Power*>(self);return int32_t(c.p->word(c.weapon+0x20+4*uint32_t(index)));};
 DamageWeapon weapon;weapon.hpPower=boss?defaultWeaponPower:callback==0x80189cb0?jumpWeaponPower:hammerWeaponPower;weapon.callbackData=&ps;weapon.defaultPower=int32_t(word(w+0x20));weapon.element=byte(w+0x6c);weapon.specialFlags=word(w+0x74);
 // A real player/partner LifeShroom path needs its downstream consumption and
 // revival event owner. Validate the actual complete pouch count domain BEFORE
 // any live damage/RNG. Missing memory is unavailable, never itemCount0.
 const auto base=weapon.hpPower(&damageUnits[ai],weapon,damageUnits[di],damageParts[di]);
 if(damageUnits[di].hp<=base){
  const auto kind=damageUnits[di].kind;
  if(kind==MarioKind || (kind>=FirstPartnerKind && kind<=LastPartnerKind)){
   uint32_t pouch=0;if(!vm.memory.readWord(PouchPointer,pouch))return false;
   for(unsigned i=0;i<RegularItemCount;++i){uint16_t value;if(!vm.memory.readHalf(pouch+RegularItemsOffset+2*i,value))return false;}
   if(packedPouchItemCount(LifeShroomItem,nativeBattleActorServices(vm).memory)>0)return false;
  }else if(damageUnits[di].heldItem==LifeShroomItem)return false;
 }
 BattleDamageOrchestrationServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
 struct Calculation{HarborTutorialPacked* self;DamageWeapon* weapon;uint32_t ai,di;};Calculation calculation{this,&weapon,ai,di};
 // Calculators consume the actual source unit/part/weapon tables. The context
 // stays on this synchronous core invocation; presentation requests are retained.
 s.user=&calculation;
 s.status=[](void* context,uint32_t u,int32_t type){auto& p=*static_cast<Calculation*>(context)->self;return checkPackedBattleStatus(u,type,nativeBattleActorServices(p.vm));};
 s.acPossible=[](void* context,uint32_t u){auto& p=*static_cast<Calculation*>(context)->self;return packedACPossibility(u,nativeBattleActorServices(p.vm));};
 s.defenseResult=[](void* context){return int32_t(static_cast<Calculation*>(context)->self->word(static_cast<Calculation*>(context)->self->battleAddress()+0x1cb0));};
 s.audience=[](void* context,DamageAudienceRequest request,int32_t amount){static_cast<Calculation*>(context)->self->damageAudienceRequests.emplace_back(request,amount);};
 s.hpDamage=[](void* context,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t& code,uint32_t f){auto& c=*static_cast<Calculation*>(context);auto& p=*c.self;DamageBattleState state;state.actionResult=p.word(p.battleAddress()+0x1cb8);state.mode=p.byte(p.battleAddress()+0x18ff9);const auto amount=calculateHpDamage(&p.damageUnits[c.ai],p.damageUnits[c.di],p.damageParts[c.di],*c.weapon,code,f,state,storedDamageStatusServices());p.word(p.unitAddress(int32_t(c.ai))+0x27c,p.damageUnits[c.ai].flags);return amount;};
 s.fpDamage=[](void* context,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t& code,uint32_t f){auto& c=*static_cast<Calculation*>(context);auto& p=*c.self;DamageBattleState state;state.actionResult=p.word(p.battleAddress()+0x1cb8);state.mode=p.byte(p.battleAddress()+0x18ff9);return calculateFpDamage(&p.damageUnits[c.ai],p.damageUnits[c.di],p.damageParts[c.di],*c.weapon,code,f,state);};
 s.weaponStatus=[](void*,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t){return uint32_t(0);}; // All original status parameter bytes admitted zero above.
 s.irand=[](void* context,int32_t bound){return static_cast<Calculation*>(context)->self->random.bounded(bound);};
 // Temporarily bind the defender's sound owner during application, restore
 // the previous owner afterward, then publish attacker dealt-HP report284.
 // Presentation vectors retain requests; application itself updates live state.
 s.directDamage=[](void* context,int32_t attacker,uint32_t,uint32_t,int32_t hp,int32_t fp,uint32_t code,int32_t pattern,int32_t recovery){auto& c=*static_cast<Calculation*>(context);auto& p=*c.self;const auto old=p.damageSoundUnit;p.damageSoundUnit=p.unitAddress(int32_t(c.di));applyDirectDamage(attacker,p.damageUnits[c.di],&p.damageParts[c.di],hp,fp,code,pattern,recovery,p.random,p.damageApplication());p.damageSoundUnit=old;p.word(p.unitAddress(int32_t(c.ai))+0x284,p.damageUnits[c.ai].dealtHp);};
 result=checkPackedBattleDamage(a,d,part,w,flags,s);return true;
}
}
