#include "harbor_tutorial.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
uint32_t word(const HarborTutorial& scene,uint32_t at){uint32_t value=0;assert(scene.readPackedWord(at,value));return value;}
void admit(HarborTutorial& scene){assert(scene.beginFresh(0,7,10,4));for(uint64_t n=1;n<300 && !scene.phaseActive;++n)scene.tick(n*1000/60,0);assert(scene.initialized && scene.phaseActive && !scene.failed);}
int main(){
 assert(loadHarborTutorialAssets("romfs"));
 HarborTutorial guarded;admit(guarded);const auto phase=word(guarded,0x8410229c);assert(phase);
 assert(word(guarded,0x841012d4)==1 && word(guarded,0x841022d4)==1);
 assert(guarded.dispatchHitEvent(2,0x100|40)==0); // Original Crump table lacks key46.
 assert(word(guarded,0x8410229c)==phase && !guarded.damageActive);
 assert(!guarded.dispatchHitEvent(-1,18) && !guarded.dispatchHitEvent(63,18));
 // Null direct script still cancels source roots; cancellation is not intro success.
 assert(guarded.dispatchDirectHitEvent(2,18,0)==0);
 assert(word(guarded,0x8410229c)==0 && word(guarded,0x84102290)==0);
 guarded.tick(guarded.now+17,0);assert(!guarded.phaseActive && !guarded.introComplete && !guarded.damageActive);
 // Invisible system owner executes its actual installed tiny source hook.
 const auto system=guarded.dispatchHitEvent(0,0x123);assert(system && guarded.damageActive);
 assert(word(guarded,0x84100278)==0x123 && word(guarded,0x841002b4)==system);
 assert(!guarded.dispatchPhase(2,0x04000003));
 guarded.tick(guarded.now+17,0);assert(!guarded.damageActive && !guarded.failed);
 assert(word(guarded,0x841002b4)==system); // No invented completed-handle cleanup.
 HarborTutorial noDamage;admit(noDamage);const auto noHp=noDamage.actors[0].hp;
 const auto noReaction=noDamage.dispatchHitEvent(2,0x100|18);assert(noReaction && noDamage.damageActive);
 for(unsigned n=0;n<60 && noDamage.damageActive;++n)noDamage.tick(noDamage.now+17,0);
 assert(!noDamage.failed && !noDamage.damageActive && !noDamage.pendingService && !noDamage.introComplete);
 assert(noDamage.actors[0].hp==noHp && noDamage.result==0 && word(noDamage,0x841022b4)==noReaction);
 HarborTutorial hit;admit(hit);const auto hp=hit.actors[0].hp;
 const auto reaction=hit.dispatchHitEvent(2,0x300|23);assert(reaction && hit.damageActive);
 assert(word(hit,0x84102278)==(0x300|23) && word(hit,0x841022b4)==reaction);
 assert(word(hit,0x8410229c)==0 && word(hit,0x841022a8)==0);
 for(unsigned n=0;n<60 && !hit.pendingService;++n)hit.tick(hit.now+17,0);
 assert(!hit.failed && hit.damageActive && hit.pendingService && !hit.introComplete);
 assert(hit.actors[0].hp==hp && hit.result==0);
 bool originalChild=false;for(const auto& request:hit.requests)if(request.owner==2 && request.function==hit.pendingService)originalChild=true;
 assert(originalChild);
 std::cout<<"Original Crump reaction child retained at "<<std::hex<<hit.pendingService<<"; HP/result unchanged\n";
 // Replacement really deletes that live source reaction before creating a new root.
 const auto replacement=hit.dispatchDirectHitEvent(2,0x456,0x80419ef8);assert(replacement && replacement!=reaction && !hit.pendingService);
 assert(word(hit,0x841022b4)==replacement && word(hit,0x84102278)==0x456);
 hit.tick(hit.now+17,0);assert(!hit.damageActive && !hit.failed && !hit.pendingService && hit.actors[0].hp==hp);
}
