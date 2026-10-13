#include "harbor_tutorial.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(){
 assert(loadHarborTutorialAssets("romfs"));HarborTutorial t;assert(t.begin(0,7));
 for(uint64_t tick=1;tick<300 && !t.message.active();++tick)t.tick(tick*1000/60,0);
 assert(!t.failed);assert(t.initialized);assert(t.actors[0].hp==5 && t.actors[0].maxHp==5);
 assert(t.actors[0].work[0]==2 && t.actors[2].id==2 && t.actors[2].kind==0xdd);
 assert(t.actors[0].belong==1 && t.actors[0].movePhase==0x04000004 && t.actors[2].belong==2 && t.actors[2].movePhase==0x04000001);
 assert(t.actors[2].hp==10 && t.actors[2].maxMoves==0 && t.actors[2].phase==0);
 assert(t.actors[2].position==t.actors[2].home);assert(t.actors[2].position[0]==-140);
 assert(t.battleFlags==0x1000 && t.actors[0].attack && t.actors[0].damage && t.actors[0].phase);
 assert(t.messageOrder==std::vector<std::string>{"mac_0_b_t1_000"});assert(t.speaker==2 && !t.statusVisible);
 uint64_t tick=t.now*60/1000+1;
 for(unsigned i=0;i<1000 && t.message.active();++i){t.tick((++tick)*1000/60,i%4==0?0x100:0);}
 assert(!t.message.active());
 for(unsigned i=0;i<100;++i)t.tick((++tick)*1000/60,0);
 assert(t.pendingService==0x80106980 && t.actors[1].pose=="M_N_2");
 // Unbound animation completion must not synthesize phase completion or combat.
 assert(t.phaseActive && !t.introComplete && t.result==0);
 t.animationComplete(1);
 for(unsigned i=0;i<100 && t.phaseActive;++i)t.tick((++tick)*1000/60,0);
 assert(t.introComplete && !t.phaseActive && t.actors[1].pose=="M_S_1" && t.actors[1].rotation[1]==0 && t.result==0);
 // The source NPC attack is a three-command wait-only hook, never a lecture.
 assert(!t.dispatchInstalledAttack(1));assert(t.dispatchInstalledAttack(2));
 for(unsigned i=0;i<10 && t.attackActive;++i)t.tick((++tick)*1000/60,0);
 assert(!t.attackActive && !t.pendingService && t.actors[2].maxMoves==0);
 t.actors[0].heldItem=128;assert(t.dispatchInstalledAttack(0));t.tick((++tick)*1000/60,0);assert(t.pendingService==0x80104498);
 t.actors[0].heldItem=0;
 for(unsigned i=0;i<10 && t.chosenUnit!=1;++i)t.tick((++tick)*1000/60,0);
 assert(t.attackActive && t.chosenUnit==1 && t.chosenPart==1 && t.pendingService==0x8012be38 && t.result==0);
 uint32_t packed=0;assert(t.readPackedWord(0x84000020,packed) && packed==0x84100000);assert(t.readPackedWord(0x84100008,packed) && packed==uint32_t(t.actors[0].kind));
 assert(t.readPackedWord(0x84100010,packed) && packed==t.actors[0].sourceKind);assert(t.readPackedWord(0x84100218,packed) && packed==2);
 assert(t.readPackedWord(0x84101010,packed) && packed==0x80372afc);assert(t.readPackedWord(0x8410200c,packed) && packed>>24==2);
 assert(t.readPackedWord(0x8410202c,packed) && packed==0x04000001);
 assert(t.readPackedWord(0x841002a8,packed) && packed!=0);assert(t.readPackedWord(0x84100290,packed) && packed==0);
 bool retained=false;for(const auto& r:t.requests)if(r.owner==0 && r.function==t.pendingService && !r.rawOperands.empty())retained=true;assert(retained);
 assert(!t.dispatchPhase(2,0x04000003));
 // Exercise guarded feedback roots on a separately initialized VM without
 // bypassing the live introduction in the primary instance.
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
