#include "harbor_tutorial_packed.hpp"
#include "battle_hit_code_events.hpp"
#include "battle_packed_counter.hpp"
#include "battle_packed_hit_event.hpp"
#include "battle_weapon_power.hpp"
#include "battle_weapon_cost.hpp"
#include <cassert>
#include <cstdlib>
namespace ttyd {
void HarborTutorialPacked::readDamage(unsigned id){
 auto& d=damageUnits[id];const auto u=Units+id*0x1000,p=Parts+id*0x1000,k=word(u+0x10);
 auto half=[&](uint32_t at){uint16_t v=0;if(!vm.memory.readHalf(at,v))std::abort();return int16_t(v<32768?int(v):int(v)-65536);};
 d.kind=int32_t(word(u+8));d.belong=int8_t(byte(u+0xc));d.hp=half(u+0x10c);d.maxHp=half(u+0x108);d.fp=half(u+0x112);d.maxFp=half(u+0x10e);
 d.flags=word(u+0x27c);d.statusFlags=word(u+0x138);d.attributes=word(u+0x104);d.extraDefense=int32_t(word(u+0x21c));d.damageSequence=half(u+0x260);d.attackSequence=int32_t(word(u+0x280));
 d.danger=byte(k+0xc);d.peril=byte(k+0xd);d.zapTap=byte(u+0x2f0);
 for(unsigned i=0;i<30;++i)d.statusBytes[i]=byte(u+0x118+i);
 auto& b=d.badges;b.powerPlus=byte(u+0x2e4);b.pUpDDown=byte(u+0x2e5);b.allOrNothing=byte(u+0x2e6);b.megaRush=byte(u+0x2e7);b.powerRush=byte(u+0x2e8);b.pDownDUp=byte(u+0x2e9);b.doublePain=byte(u+0x2ea);b.lastStand=byte(u+0x2eb);b.defendPlus=byte(u+0x2ec);b.damageDodge=byte(u+0x2ed);b.pityFlower=byte(u+0x2f8);b.hpDrain=byte(u+0x2f9);b.fpDrain=byte(u+0x2fa);b.icePower=byte(u+0x301);
 d.hpBefore=int32_t(word(u+0x208));d.hpAfter=int32_t(word(u+0x20c));d.lastHp=byte(u+0x270);d.lastFp=byte(u+0x271);d.lastConsumedFp=byte(u+0x272);d.pattern=int32_t(word(u+0x274));
 d.dealtHp=int32_t(word(u+0x284));d.requestedHp=int32_t(word(u+0x264));d.requestedFp=int32_t(word(u+0x268));d.consumedFp=int32_t(word(u+0x26c));d.damageFlags=uint16_t(half(u+0x1fe));
 d.field200=uint16_t(half(u+0x200));d.field202=uint16_t(half(u+0x202));d.field204=uint16_t(half(u+0x204));d.field206=uint16_t(half(u+0x206));
 damageParts[id].attributes=word(p+0x1ac);
 for(unsigned i=0;i<5;++i){defenses[id][i]=int8_t(byte(word(p+0x1b4)+i));defenseAttributes[id][i]=int8_t(byte(word(p+0x1b8)+i));}
 damageParts[id].defense=defenses[id].data();damageParts[id].defenseAttributes=defenseAttributes[id].data();
 for(unsigned i=0;i<4;++i){auto& name=damageSounds[id][i];name.clear();const auto at=word(k+0x9c+4*i);if(at)for(unsigned n=0;n<128;++n){const auto c=byte(at+n);if(!c)break;name+=char(c);}d.damageSounds[i]=at?name.c_str():nullptr;}
}
void HarborTutorialPacked::writeDamage(unsigned id){
 const auto& d=damageUnits[id];const auto u=Units+id*0x1000;
 half(u+0x10c,d.hp);half(u+0x112,d.fp);word(u+0x27c,d.flags);word(u+0x284,d.dealtHp);
 word(u+0x208,d.hpBefore);word(u+0x20c,d.hpAfter);word(u+0x264,d.requestedHp);word(u+0x268,d.requestedFp);word(u+0x26c,d.consumedFp);
 byte(u+0x270,d.lastHp);byte(u+0x271,d.lastFp);byte(u+0x272,d.lastConsumedFp);word(u+0x274,d.pattern);half(u+0x1fe,d.damageFlags);half(u+0x200,d.field200);half(u+0x204,d.field204);
 for(unsigned i=0;i<30;++i)byte(u+0x118+i,d.statusBytes[i]);
}
DamageApplicationServices HarborTutorialPacked::damageApplication(){
 DamageApplicationServices s;s.user=this;
 s.getUnit=[](void* self,int32_t id)->DamageUnit*{auto& p=*static_cast<HarborTutorialPacked*>(self);return id>=0 && id<4 && p.registered[size_t(id)]?&p.damageUnits[size_t(id)]:nullptr;};
 s.bodyPart=[](void* self,DamageUnit& d){auto& p=*static_cast<HarborTutorialPacked*>(self);return &p.damageParts[size_t(&d-p.damageUnits.data())];};
 s.hitPosition=[](void* self,DamageUnit& d,DamagePart&){auto& p=*static_cast<HarborTutorialPacked*>(self);const auto id=unsigned(&d-p.damageUnits.data());const auto pos=packedHitPosition(Units+id*0x1000,Parts+id*0x1000,nativeBattlePartPositionServices(p.vm));return DamagePosition{pos[0],pos[1],pos[2]};};
 s.effect=[](void* self,const DamageEffect& effect){static_cast<HarborTutorialPacked*>(self)->damageEffects.push_back(effect);};
 s.sound=[](void* self,const char* key,DamagePosition position){if(!key)return;auto& p=*static_cast<HarborTutorialPacked*>(self);HarborTutorialSoundRequest request;request.name=key;request.key=p.word(p.word(Units+0x1000+0x10)+0x9c);request.handle=p.nextSoundHandle++;request.owner=1;request.position={position.x,position.y,position.z};p.sounds.push_back(std::move(request));};
 s.notice=[](void* self,DamageUnit&,DamageNotice notice){auto& p=*static_cast<HarborTutorialPacked*>(self);p.damageNotices.push_back(notice);if(notice.kind==DamageNoticeKind::ActionCount){auto v=p.byte(Battle+notice.fieldOffset);addActionCount(v);p.byte(Battle+notice.fieldOffset,v);}else if(notice.kind==DamageNoticeKind::ActionPoint){auto v=p.byte(Battle+notice.fieldOffset);addActionPoint(v,notice.value);p.byte(Battle+notice.fieldOffset,v);}};
 s.hitEvent=[](void* self,DamageUnit& d,uint32_t code){auto& p=*static_cast<HarborTutorialPacked*>(self);const auto id=unsigned(&d-p.damageUnits.data());p.writeDamage(id);packedBattleRunHitEvent(Units+id*0x1000,code,nativeBattleUnitEventServices(p.vm));};
 return s;
}
bool HarborTutorialPacked::checkDamageCore(uint32_t a,uint32_t d,uint32_t part,uint32_t w,uint32_t flags,int32_t& result){
 // Admission is explicit until guard effects, nonordinary status/counter branches
 // and general inventory/recovery providers are bound. No guessed result exits.
 if(!a || !d || word(a)!=2 || word(d)!=1 || part!=Parts+0x1000 || word(w+0x1c)!=0x80189d30 || word(w+0x40) || byte(w+0x6c)>=5 || byte(w+0x6d)!=0 || byte(Battle+0x18ff9)!=0)return false;
 if(packedACPossibility(d,nativeBattleActorServices(vm)) && int32_t(word(Battle+0x1cb0))>3)return false;
 for(unsigned i=0x80;i<=0xae;++i)if(byte(w+i))return false;
 for(auto u:{a,d}){for(unsigned i=0x118;i<=0x135;++i)if(byte(u+i))return false;for(unsigned i=0x2e4;i<=0x301;++i)if(byte(u+i))return false;}
 BattleCounterWork counter;counter.init();BattlePackedCounterServices cs;cs.memory=nativeBattleActorServices(vm);cs.user=this;cs.status=[](void* self,uint32_t u,int32_t type){auto& p=*static_cast<HarborTutorialPacked*>(self);return checkPackedBattleStatus(u,type,nativeBattleActorServices(p.vm));};auto checkedFlags=flags;packedBattleCheckCounter(counter,a,d,part,w,checkedFlags,cs);if(counter.words[9])return false;
 readDamage(1);readDamage(2);if(damageUnits[1].hp<=int32_t(word(w+0x20)))return false; // Lethal inventory/event admission remains separate.
 DamageWeapon weapon;weapon.hpPower=defaultWeaponPower;weapon.defaultPower=int32_t(word(w+0x20));weapon.element=byte(w+0x6c);weapon.specialFlags=word(w+0x74);
 BattleDamageOrchestrationServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
 struct Calculation{HarborTutorialPacked* self;DamageWeapon* weapon;};Calculation calculation{this,&weapon};
 // Calculators consume the actual source unit/part/weapon tables. The context
 // stays on this synchronous core invocation; presentation requests are retained.
 s.user=&calculation;
 s.status=[](void* context,uint32_t u,int32_t type){auto& p=*static_cast<Calculation*>(context)->self;return checkPackedBattleStatus(u,type,nativeBattleActorServices(p.vm));};
 s.acPossible=[](void* context,uint32_t u){auto& p=*static_cast<Calculation*>(context)->self;return packedACPossibility(u,nativeBattleActorServices(p.vm));};
 s.defenseResult=[](void* context){return int32_t(static_cast<Calculation*>(context)->self->word(Battle+0x1cb0));};
 s.audience=[](void* context,DamageAudienceRequest request,int32_t amount){static_cast<Calculation*>(context)->self->damageAudienceRequests.emplace_back(request,amount);};
 s.hpDamage=[](void* context,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t& code,uint32_t f){auto& c=*static_cast<Calculation*>(context);auto& p=*c.self;DamageBattleState state;state.actionResult=p.word(Battle+0x1cb8);state.mode=p.byte(Battle+0x18ff9);const auto amount=calculateHpDamage(&p.damageUnits[2],p.damageUnits[1],p.damageParts[1],*c.weapon,code,f,state,storedDamageStatusServices());p.word(Units+0x2000+0x27c,p.damageUnits[2].flags);return amount;};
 s.fpDamage=[](void* context,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t& code,uint32_t f){auto& c=*static_cast<Calculation*>(context);auto& p=*c.self;DamageBattleState state;state.actionResult=p.word(Battle+0x1cb8);state.mode=p.byte(Battle+0x18ff9);return calculateFpDamage(&p.damageUnits[2],p.damageUnits[1],p.damageParts[1],*c.weapon,code,f,state);};
 s.weaponStatus=[](void*,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t){return uint32_t(0);}; // All original status parameter bytes admitted zero above.
 s.irand=[](void* context,int32_t bound){return static_cast<Calculation*>(context)->self->random.bounded(bound);};
 s.directDamage=[](void* context,int32_t attacker,uint32_t,uint32_t,int32_t hp,int32_t fp,uint32_t code,int32_t pattern,int32_t recovery){auto& p=*static_cast<Calculation*>(context)->self;applyDirectDamage(attacker,p.damageUnits[1],&p.damageParts[1],hp,fp,code,pattern,recovery,p.random,p.damageApplication());p.word(Units+0x2000+0x284,p.damageUnits[2].dealtHp);};
 result=checkPackedBattleDamage(a,d,part,w,flags,s);return true;
}
}
