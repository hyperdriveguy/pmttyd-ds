#include "harbor_tutorial.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(){
 assert(loadHarborTutorialAssets("romfs"));HarborTutorial t;assert(t.begin(0,7));
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
