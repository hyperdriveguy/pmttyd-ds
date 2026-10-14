#include "harbor_tutorial.hpp"
#include "actor_animation.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
uint64_t prepareCrumpAttack(HarborTutorial& scene,int32_t hp){
 assert(scene.beginFresh(0,hp,10,4));uint64_t tick=0;
 for(unsigned n=0;n<1500 && !scene.introComplete;++n){
  if(scene.pendingService==0x80106980)scene.animationComplete(1);
  scene.tick((++tick)*1000/60,n%4==0?0x100:0);
 }
 assert(scene.introComplete && !scene.failed && scene.dispatchInstalledAttack(2));
 return tick;
}
void ordinaryCrumpImpact(){
 ActorAnimationPack art;assert(art.load("romfs/harbor-tutorial-mario.pmanim"));
 const auto* damagePose=art.pose("M_D_1");ActorAnimationSample damageSample;
 assert(damagePose && damagePose->evaluate(0,damageSample) && !damageSample.vertices.empty());
 HarborTutorial scene;auto tick=prepareCrumpAttack(scene,7);
 BattlePadSample noInput;
 for(unsigned n=0;n<600 && scene.actors[1].hp==7 && !scene.failed;++n)scene.tick((++tick)*1000/60,0,&noInput);
 assert(!scene.failed && scene.actors[1].hp==6 && scene.result==0 && scene.damageActive);
 auto word=[&](uint32_t at){uint32_t value=0;assert(scene.readPackedWord(at,value));return value;};
 assert(word(0x84101208)==7 && word(0x8410120c)==6 && word(0x84101264)==1);
 assert(word(0x84016f3c)==0x01000100);
 assert(word(0x84102284)==1 && word(0x84102280)==1 && (word(0x84101270)>>24)==1);
 assert(word(0x841012b4)!=0 && word(0x84101278)==0x10117);
 bool sourceFlags=false;for(const auto& request:scene.requests)if(request.function==0x8010a78c && request.owner==2){assert(request.rawOperands[4]==0x100);sourceFlags=true;}assert(sourceFlags);
 assert(scene.damageEffects.size()==2 && scene.damageEffects[0].kind==DamageEffectKind::Hit && scene.damageEffects[1].kind==DamageEffectKind::DamageStar && scene.damageEffects[1].integers[1]==1);
 assert(scene.damageAudienceRequests.size()==1 && scene.damageAudienceRequests[0].first==DamageAudienceRequest::GuardBad);
 assert(scene.damageNotices.size()==3 && scene.damageNotices[0].kind==DamageNoticeKind::ActionCount && scene.damageNotices[1].kind==DamageNoticeKind::ActionPoint && scene.damageNotices[2].kind==DamageNoticeKind::MarioBigDamage);
 for(unsigned n=0;n<400 && !scene.pendingService && (scene.attackActive || scene.damageActive);++n)scene.tick((++tick)*1000/60,0,&noInput);
 assert(!scene.pendingService && !scene.damageActive && !scene.attackActive && scene.actors[1].pose=="M_S_1");
 assert(word(0x84101290)!=0 && word(0x84102290)!=0);
 bool shakeRequest=false;for(const auto& request:scene.requests)if(request.function==0x8012b3d0){assert(request.rawOperands.size()==5);assert(word(0x84002834)==uint32_t(request.rawOperands[3]) && word(0x8400283c)==uint32_t(request.rawOperands[3]) && word(0x8400282c)==uint32_t(request.rawOperands[4]));shakeRequest=true;}assert(shakeRequest && (word(0x84002754)&2));
 assert(!scene.rumbleRequests.empty());for(auto channel:scene.rumbleRequests)assert(channel==0);
 bool damageSound=false;for(const auto& request:scene.soundRequests)if(request.owner==1 && request.name=="SFX_MARIO_DAMAGE1"){assert(request.key && request.handle);damageSound=true;}assert(damageSound);
 assert(!scene.failed && scene.actors[1].hp==6 && scene.result==0);

}
void retainedLethalImpact(){
 HarborTutorial scene;auto tick=prepareCrumpAttack(scene,1);BattlePadSample noInput;
 for(unsigned n=0;n<600 && scene.pendingService!=0x8010a78c;++n)scene.tick((++tick)*1000/60,0,&noInput);
 assert(!scene.failed && scene.attackActive && scene.pendingService==0x8010a78c && scene.actors[1].hp==1);
 for(unsigned n=0;n<10;++n)scene.tick((++tick)*1000/60,0,&noInput);
 assert(scene.actors[1].hp==1 && scene.damageEffects.empty() && scene.damageNotices.empty() && scene.damageAudienceRequests.empty() && !scene.damageActive && scene.result==0);
}
void nativeWeaponRows(){
 auto pouch=initializeProloguePouch("Yoshi");pouch.half(0x70,7);pouch.half(0x74,2);
 HarborTutorial scene;assert(scene.beginFromPouch(0,pouch,4));assert(!scene.buildWeaponMenu(0));
 for(unsigned n=0;n<1500 && !scene.introComplete;++n){if(scene.pendingService==0x80106980)scene.animationComplete(1);scene.tick((n+1)*1000/60,n%4==0?0x100:0);}
 assert(scene.introComplete && scene.buildWeaponMenu(0) && scene.weaponRows.size()==1);
 const auto jump=scene.weaponRows[0];
 assert(jump.weapon==0x803538a8 && jump.enabled && jump.reason==0 && jump.nameAddress && jump.label=="Normal Jump" && jump.icon==167);
 assert((scene.selectWeapon(0) && scene.weaponTargets==std::vector<std::array<int32_t,2>>({{2,1}})));
 uint32_t selectedByte=0;assert(scene.readPackedWord(0x84000edc,selectedByte) && (selectedByte&255)==0);
 assert(scene.actors[1].hp==7 && !scene.attackActive && !scene.damageActive);assert(!scene.selectWeapon(1));
 assert(scene.buildWeaponMenu(1) && scene.weaponRows.size()==1 && scene.weaponRows[0].weapon==0x80353de8 && scene.weaponRows[0].enabled);
 assert(scene.actors[1].hp==7 && scene.actors[0].hp==5 && !scene.attackActive && !scene.damageActive && scene.result==0);
 assert(!scene.buildWeaponMenu(6));
 uint32_t data=0;assert(scene.readPackedWord(0x84101110,data) && uint16_t(data)==2);
 assert(scene.readPackedWord(0x84400098,data) && (data>>24)==1 && ((data>>16)&255)==1);
 assert(scene.readPackedWord(0x84400070,data) && (data>>16)==7);
 assert(scene.buildWeaponMenu(2) && scene.weaponRows.size()==1 && scene.weaponRows[0].weapon==0x80363e60 && !scene.weaponRows[0].enabled && scene.weaponRows[0].reason==3);
 assert(scene.buildWeaponMenu(4) && scene.weaponRows.empty());
 pouch.bytes[0x98]=3;pouch.half(0x74,4);assert(scene.buildWeaponMenu(0) && scene.weaponRows.size()==1 && scene.weaponRows[0].icon==167); // Battle entry owns a complete immutable-entry copy.
 assert(scene.readPackedWord(0x84101110,data) && uint16_t(data)==2);
 assert(scene.openWeaponMenu(0) && scene.weaponMenuOpen);BattlePadSample press;press.snapshot.buttons=BattleButtonA;press.pressed=BattleButtonA;
 scene.tick(scene.now+17,0,&press);assert(scene.inputWeaponMenu(press)==0 && scene.weaponMenuOpen); // UI readiness is an explicit presented signal.
 scene.weaponMenuPresented();scene.tick(scene.now+17,0,&press);assert(scene.inputWeaponMenu(press)==1 && !scene.weaponMenuOpen && scene.weaponTargetMenuOpen);
 assert((scene.weaponTargets==std::vector<std::array<int32_t,2>>({{2,1}})) && !scene.attackActive && scene.result==0);
 assert(scene.openWeaponMenu(0));scene.weaponMenuPresented();BattlePadSample cancel;cancel.snapshot.buttons=BattleButtonB;cancel.pressed=BattleButtonB;scene.tick(scene.now+17,0,&cancel);assert(scene.inputWeaponMenu(cancel)==1 && !scene.weaponMenuOpen && !scene.weaponTargetMenuOpen);


}
void nativePlayerAttack(){
 auto pouch=initializeProloguePouch("Yoshi");pouch.half(0x70,7);
 HarborTutorial scene;assert(!scene.dispatchSelectedAttack());assert(scene.beginFromPouch(0,pouch,4));
 assert(!scene.dispatchSelectedAttack());
 for(unsigned n=0;n<1500 && !scene.introComplete;++n){if(scene.pendingService==0x80106980)scene.animationComplete(1);scene.tick((n+1)*1000/60,n%4==0?0x100:0);}
 assert(scene.introComplete && !scene.dispatchSelectedAttack());
 assert(scene.openWeaponMenu(0));scene.weaponMenuPresented();BattlePadSample press;press.snapshot.buttons=BattleButtonA;press.pressed=BattleButtonA;
 scene.tick(scene.now+17,0,&press);assert(scene.inputWeaponMenu(press)==1);
 assert(scene.weaponTargetMenuOpen && scene.dispatchSelectedAttack() && scene.attackActive && !scene.weaponTargetMenuOpen);
 uint32_t value=0;auto word=[&](uint32_t at){assert(scene.readPackedWord(at,value));return value;};
 assert(scene.actors[1].attack==0x80355cf0 && word(0x841012ac)==0);
 assert(word(0x841012a8)!=0 && word(0x84101290)==0);
 assert(word(0x84000f20)==0x06000007 && word(0x84002744)==0 && !scene.statusVisible);
 assert(!scene.dispatchSelectedAttack());
 for(unsigned n=0;n<100 && !scene.pendingService;++n)scene.tick(scene.now+17,0);
 std::cout<<"Selected Mario Jump frontier: "<<std::hex<<scene.pendingService<<std::dec<<"\n";
 assert(!scene.failed && scene.attackActive && scene.pendingService==0x80107788 && scene.actors[0].hp==5 && scene.result==0);
 assert(scene.chosenUnit==2 && scene.chosenPart==1);
 assert(word(0x84001c90)==0x84101000 && word(0x84001c94)==1 && word(0x84001c9c)==0);
 assert(word(0x84001ca0)==0x8012aae0 && word(0x84001ca4)==0x8012a8c4 && word(0x84001ca8)==0x8012a7cc && word(0x84001cac)==0x8012a7c0);
 assert(((word(0x84001cbc)>>24)&255)==3 && ((word(0x84001cbc)>>16)&255)==3);
 assert(scene.camera.owner==1 && scene.camera.ownerPart==1 && scene.camera.target==2 && scene.camera.targetPart==1);

 assert((word(0x84016f4c)>>24)==1 && uint16_t(word(0x84101110))==5);
 for(unsigned n=0;n<5;++n)scene.tick(scene.now+17,0);
 assert(scene.pendingService==0x80107788 && (word(0x84016f4c)>>24)==1 && scene.actors[1].hp==7);
 bool marioRoot=false;for(const auto& request:scene.requests)if(request.owner==1 && request.function==scene.pendingService)marioRoot=true;assert(marioRoot);
}
int main(){
 assert(loadHarborTutorialAssets("romfs"));ordinaryCrumpImpact();retainedLethalImpact();nativeWeaponRows();nativePlayerAttack();HarborTutorial t;assert(t.begin(0,7));
 for(uint64_t tick=1;tick<300 && !t.message.active();++tick)t.tick(tick*1000/60,0);
 uint32_t packedSystem=0;
 assert(!t.failed);assert(t.initialized);assert(t.actors[0].hp==5 && t.actors[0].maxHp==5);
 assert(t.actors[0].work[0]==3 && t.actors[2].id==3 && t.actors[2].kind==0xdd);
 assert(t.systemActor.id==0 && t.systemActor.kind==220 && t.systemActor.hp==999 && t.systemActor.maxMoves==0);
 assert(t.actors[0].id==2 && t.actors[1].id==1);
 assert(t.readPackedWord(0x84000020,packedSystem) && packedSystem==0x84100000);
 assert(t.readPackedWord(0x84100008,packedSystem) && packedSystem==220);
 assert(t.readPackedWord(0x8410028c,packedSystem) && packedSystem==0x80419f08);
 assert(t.readPackedWord(0x841002a0,packedSystem) && packedSystem==0x80419f00);
 assert(t.readPackedWord(0x841002b0,packedSystem) && packedSystem==0x80419ef8);
 assert(t.readPackedWord(0x84100020,packedSystem) && (packedSystem&0x00ffff00)==0);
 for(uint32_t id=0;id<4;++id)assert(t.readPackedWord(0x84100288+id*0x1000,packedSystem) && packedSystem==0);
 assert(t.visualSlot(0)==-1 && t.visualSlot(1)==1 && t.visualSlot(2)==0 && t.visualSlot(3)==2);
 assert(t.actorById(0)==&t.systemActor && t.actorById(3)==&t.actors[2] && !t.actorById(4));
 assert(t.actors[0].belong==1 && t.actors[0].movePhase==0x04000004 && t.actors[2].belong==2 && t.actors[2].movePhase==0x04000001);
 assert(t.actors[2].hp==10 && t.actors[2].maxMoves==0 && t.actors[2].phase==0);
 assert(t.actors[2].position==t.actors[2].home);assert(t.actors[2].position[0]==-140);
 assert(t.battleFlags==0x1000 && t.actors[0].attack && t.actors[0].damage && t.actors[0].phase);
 assert(t.messageOrder==std::vector<std::string>{"mac_0_b_t1_000"});assert(t.speaker==3 && !t.statusVisible);
 uint64_t tick=t.now*60/1000+1;
 for(unsigned i=0;i<1000 && t.message.active();++i){t.tick((++tick)*1000/60,i%4==0?0x100:0);}
 assert(!t.message.active());
 for(unsigned i=0;i<100;++i)t.tick((++tick)*1000/60,0);
 assert(t.pendingService==0x80106980 && t.actors[1].pose=="M_N_2");
 // Unbound animation completion must not synthesize phase completion or combat.
 assert(t.phaseActive && !t.introComplete && t.result==0);
 // Registry0 is invisible; Crump2/helper3 acknowledgments must not alias
 // Mario's visual slot1, even when their pose timestamps coincide.
 for(int32_t id:{0,2,3}){t.animationComplete(id);t.tick((++tick)*1000/60,0);assert(t.phaseActive && !t.introComplete && t.pendingService==0x80106980);}
 t.animationComplete(1);
 for(unsigned i=0;i<100 && t.phaseActive;++i)t.tick((++tick)*1000/60,0);
 assert(t.introComplete && !t.phaseActive && t.actors[1].pose=="M_S_1" && t.actors[1].rotation[1]==0 && t.result==0);
 assert(!t.dispatchInstalledAttack(0));
 // The source NPC attack is a three-command wait-only hook, never a lecture.
 assert(!t.dispatchInstalledAttack(1));assert(t.dispatchInstalledAttack(3));
 for(unsigned i=0;i<10 && t.attackActive;++i)t.tick((++tick)*1000/60,0);
 assert(!t.attackActive && !t.pendingService && t.actors[2].maxMoves==0);
 t.actors[0].heldItem=128;assert(t.dispatchInstalledAttack(2));t.tick((++tick)*1000/60,0);assert(t.pendingService==0x80104498);
 t.actors[0].heldItem=0;
 for(unsigned i=0;i<300 && t.pendingService!=0x80107694;++i)t.tick((++tick)*1000/60,0);
 assert(t.attackActive && t.chosenUnit==1 && t.chosenPart==1 && t.pendingService==0x80107694 && t.result==0);
 uint32_t packed=0;assert(t.readPackedWord(0x84000028,packed) && packed==0x84102000);assert(t.readPackedWord(0x84102008,packed) && packed==uint32_t(t.actors[0].kind));
 assert(t.readPackedWord(0x84102010,packed) && packed==t.actors[0].sourceKind);assert(t.readPackedWord(0x84102218,packed) && packed==3);
 assert(t.readPackedWord(0x84101010,packed) && packed==0x80372afc);assert(t.readPackedWord(0x8410300c,packed) && packed>>24==2);
 assert(t.readPackedWord(0x8410302c,packed) && packed==0x04000001);
 assert(t.readPackedWord(0x841022a8,packed) && packed!=0);assert(t.readPackedWord(0x84102290,packed) && packed==0);
 bool retained=false;for(const auto& r:t.requests)if(r.owner==2 && r.function==t.pendingService && !r.rawOperands.empty())retained=true;assert(retained);
 // Dialogue edges do not feed defense. Missing physical samples preserve the gate.
 for(unsigned i=0;i<3;++i)t.tick((++tick)*1000/60,0x100);
 assert(t.pendingService==0x80107694);
 const auto hpBeforeDefense=t.actors[1].hp;BattlePadSample physical;physical.snapshot.buttons=BattleButtonA;physical.pressed=BattleButtonA;
 t.tick((++tick)*1000/60,0,&physical);
 assert(t.readPackedWord(0x84001cb0,packed) && packed==4);
 assert(t.pendingService==0x8010a78c && t.actors[1].hp==hpBeforeDefense && t.result==0);
 assert(t.damageEffects.empty() && t.damageNotices.empty() && t.damageAudienceRequests.empty());
 bool originalSound=false;for(const auto& r:t.soundRequests)if(r.operation==HarborTutorialSoundRequest::Operation::Trigger && r.owner==2){assert(r.key && !r.name.empty() && r.handle && r.time>0);originalSound=true;}assert(originalSound);
 // ResultACDefence clears the private history; a second pass at this timestamp
 // must not resample the producer trigger and resurrect that cleared history.
 const auto impactTime=t.now;t.tick(impactTime,0,&physical);
 assert(t.readPackedWord(0x84001d34,packed) && packed==0);
 assert(!t.dispatchPhase(2,0x04000003));
 // Exercise guarded feedback roots on a separately initialized VM without
 // bypassing the live introduction in the primary instance.
 HarborTutorial fresh;assert(fresh.beginFresh(0,7,10,4));
 for(uint64_t i=1;i<300 && !fresh.message.active();++i)fresh.tick(i*1000/60,0);
 assert(!fresh.failed && fresh.initialized && fresh.pendingService==0 && fresh.actors[1].pose=="M_S_1");
 std::vector<int32_t> entryOwners;
 for(const auto& r:fresh.requests)if(entryOwners.empty() || entryOwners.back()!=r.owner)entryOwners.push_back(r.owner);
 assert(entryOwners.size()>=3 && entryOwners[0]==0 && entryOwners[1]==1 && entryOwners[2]==2);
 const HarborTutorial& constFresh=fresh;assert(constFresh.actorById(0)==&fresh.systemActor && constFresh.actorById(2)==&fresh.actors[0] && !constFresh.actorById(-1));
 assert(fresh.systemActor.maxMoves==0 && fresh.systemActor.wait==0x80419f08 && fresh.actors[0].work[0]==3);
 assert(fresh.readPackedWord(0x80419f10,packed) && packed==0x100);
 assert(fresh.readPackedWord(0x84201204,packed) && packed==1);
 assert(fresh.actors[1].wait==0x80417c38 && fresh.actors[1].attack==0x80417c30 && fresh.actors[1].damage==0x80417c08 && fresh.actors[1].confusion==0x8034f1e8 && fresh.actors[1].entry==0x80372bc0);
 assert(fresh.readPackedWord(0x84016f28,packed) && packed==0x42b40000);
 assert(fresh.readPackedWord(0x84016f30,packed) && packed==0xc2480000);
 assert(fresh.readPackedWord(0x8041eb00,packed) && packed==0x84400000);
 assert(fresh.readPackedWord(0x84002738,packed) && packed==0x84500020);
 assert(fresh.readPackedWord(0x84500028,packed) && packed==0);
 assert(fresh.readPackedWord(0x8450002c,packed) && packed==0x84501230);
 assert(fresh.readPackedWord(0x84501230,packed) && packed==0x95540870);
 assert(fresh.readPackedWord(0x84501248,packed) && packed==0x95540c08);
 assert(fresh.readPackedWord(0x84501234,packed) && packed==0);
 assert(fresh.readPackedWord(0x84400070,packed) && packed==0x0007000a);
 assert(fresh.readPackedWord(0x841011c4,packed) && packed==0x802f4018);
 assert(fresh.readPackedWord(0x841011c8,packed) && packed==0x802f4030);
 assert(fresh.readPackedWord(0x841011dc,packed) && packed==0x802f4048);
 assert(fresh.readPackedWord(0x841011cc,packed) && packed==0x802f4048);
 assert(fresh.readPackedWord(0x842014d0,packed) && packed==0x80417be0);
 assert(fresh.readPackedWord(0x842014d8,packed) && packed==0);
 assert(fresh.readPackedWord(0x84203130,packed) && packed==0);
 assert(fresh.messageOrder==std::vector<std::string>{"mac_0_b_t1_000"});
 bool emblem=false,makkuro=false;for(const auto& r:fresh.requests){if(r.owner==1 && r.function==0x80145404)emblem=true;if(r.function==0x80145614)makkuro=true;}
 assert(emblem && !makkuro && fresh.result==0);
 HarborTutorial black;assert(black.beginFresh(0,7,10,201));for(unsigned i=1;i<50;++i)black.tick(i*1000/60,0);assert(!black.failed && !black.initialized && black.messageOrder.empty() && black.pendingService==0x80145614);
 assert(black.readPackedWord(0x84100288,packed) && packed==0); // Finished system cleans despite live Mario init.
 assert(black.readPackedWord(0x84101288,packed) && packed!=0); // Original blocked Mario root remains owned.
 HarborTutorial f;assert(f.begin(0,7));
 // Wrong phase is checked before any opening message can run by changing the
 // supplied controller word before its admitted phase root executes.
 uint64_t fi=1;while(!f.initialized){f.tick(fi++*1000/60,0);}
 f.phaseWord=0x04000002;
 for(;fi<10;++fi)f.tick(fi*1000/60,0);
 assert(!f.phaseActive && f.messageOrder.empty());
 assert(f.dispatchPhase(1,0x04000003));f.actors[0].hp=2;f.successDeclarations=1;f.failureDeclarations=1;
 for(uint64_t i=10;i<1000 && f.messageOrder.empty();++i)f.tick(i*1000/60,0);
 assert(f.messageOrder==std::vector<std::string>{"mac_0_b_t1_003"});assert(f.actors[0].work[3]==1 && f.actors[0].work[1]==0 && f.actors[0].work[2]==0);
 uint64_t clock=f.now;
 for(unsigned i=0;i<1000 && f.phaseActive;++i){clock+=17;f.tick(clock,i%4==0?0x100:0);}
 assert(!f.phaseActive);assert(f.dispatchPhase(2,0x04000003));
 for(unsigned i=0;i<1000 && f.phaseActive;++i){clock+=17;f.tick(clock,i%4==0?0x100:0);}
 assert((f.messageOrder==std::vector<std::string>{"mac_0_b_t1_003","mac_0_b_t1_001","mac_0_b_t1_002"}));
 assert(f.actors[0].work[1]==1 && f.actors[0].work[2]==1 && f.statusVisible && f.result==0);
 assert(f.dispatchPhase(2,0x04000003));for(unsigned i=0;i<10;++i){clock+=17;f.tick(clock,0);}assert(f.messageOrder.size()==3);
 std::cout<<"Original setup2 init, distinct helper, intro and phase feedback guards passed\n";
}
