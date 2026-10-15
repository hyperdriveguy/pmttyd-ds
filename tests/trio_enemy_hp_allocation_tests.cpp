#include "trio_attack.hpp"
#include "battle_enemy_hp_gpu.hpp"
#include <cassert>
using namespace ttyd;
namespace {
constexpr uint32_t Battle=0x84000000;
uint32_t read(const TrioAttack& battle,uint32_t address){
 const auto value=battle.enemyHpResourceWord(address);assert(value);return *value;
}
void begin(TrioAttack& battle,ProfessorStory& scene){
 assert(scene.beginTrioBattleIntro(0));
 TrioBattleCommands menu;assert(menu.begin(scene));
 menu.input(0x100,0);menu.input(0x100,0);
 assert(battle.begin(menu,scene,10));
}
void resources(const TrioAttack& battle){
 const auto entry=read(battle,Battle+0x163f8);assert(entry);
 const auto holder=read(battle,entry+0xa0);assert(holder);
 const auto palette=read(battle,holder);assert(palette);
 assert(read(battle,palette)==0x0020af30 && read(battle,palette+4)==97);
 BattleEnemyHpMemoryReader memory{const_cast<TrioAttack*>(&battle),
  [](void* p,uint32_t a){return static_cast<TrioAttack*>(p)->enemyHpResourceWord(a);}};
 for(uint32_t index:{17u,18u}){
  const auto image=readBattleEnemyHpTexture(memory,palette,index);
  assert(image && image->index==index && image->width==40 && image->height==16 && image->format==14);
 }
 assert(!readBattleEnemyHpTexture(memory,palette,16));
 assert(!battle.enemyHpCamera() && !battle.enemyHpCamera(4) && !battle.enemyHpCamera(8));
 // Original asset admission cannot manufacture a camera lease or source pass.
 assert(!battle.enemyHpResourceWord(0x60000000));
 assert(!battle.enemyHpFrameComplete(0) && !battle.enemyHpFrameComplete(1));
 BattleAudienceMatrixRequest matrix;matrix.function=0x8029e230;
 assert(!battle.enemyHpMatrix(matrix));
}
}
int main(){
 assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") &&
        loadTrioBattleAssets("romfs") && loadTrioAttackAssets("romfs"));
 TrioAttack first;ProfessorStory firstScene;
 assert(!first.enemyHpAllocation() && !first.enemyHpResourceWord(Battle));
 begin(first,firstScene);const auto owner=first.enemyHpAllocation();assert(owner && *owner);
 const auto pinnedOwner=first.enemyHpAllocationOwner();assert(pinnedOwner);
 assert(first.enemyHpAllocationCurrent(*owner) && !first.enemyHpAllocationCurrent(*owner+1));
 resources(first);
 const auto entry=read(first,Battle+0x163f8);assert((read(first,entry)&0xffff)==1);
 TrioAttack alias=first;
 first=TrioAttack{};assert(!first.enemyHpAllocation());
 assert(alias.enemyHpAllocation()==owner && alias.enemyHpAllocationCurrent(*owner));
 TrioAttack second;ProfessorStory secondScene;begin(second,secondScene);
 assert(second.enemyHpAllocation() && second.enemyHpAllocation()!=owner);
 assert(!second.enemyHpAllocationCurrent(*owner));resources(second);
 // Real process file backing is shared; each source Init owns one reference.
 assert(read(second,Battle+0x163f8)==entry && (read(second,entry)&0xffff)==2);
 alias=TrioAttack{};assert((read(second,entry)&0xffff)==1);
 assert(pinnedOwner!=second.enemyHpAllocationOwner() && !second.enemyHpAllocationCurrent(*owner));
 unsigned callbacks=0;TrioEnemyHpNativeServices external;
 external.lifetime=std::make_shared<const unsigned>(1);external.user=&callbacks;
 external.begin=[](void* p,uint64_t,uint64_t){++*static_cast<unsigned*>(p);return true;};
 external.publish=external.begin;
 assert(!second.resumeEnemyHpDraw(8,Battle,external));
 assert(!second.resumeEnemyHpDraw(4,Battle,external));
 assert(!second.resumeEnemyHpDraw(8,Battle+4,external));
 assert(callbacks==0); // Active battle has no genuine HP dispatch yet.
 return 0;
}
