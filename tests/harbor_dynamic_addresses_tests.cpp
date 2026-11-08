// Explicit relocated native fixture: real VM, source map allocator and original
// assets; this does not claim original ColdInit or normal battle activation.
#include "harbor_tutorial.cpp"
#include "battle_packed_pad.hpp"
#include <cassert>
#include <iostream>
namespace ttyd {
struct HarborDynamicAddressesFixture {
 static constexpr uint32_t BattleBytes=0x19400,UnitBytes=0x1000,PartBytes=0x1000;
 HarborTutorial scene;std::shared_ptr<HarborTutorialRuntime> runtime;
 uint32_t battle=0;std::array<uint32_t,4> units{},parts{};
 HarborDynamicAddressesFixture(){
  assert(scene.beginFresh(0,10,10,0));runtime=scene.runtime_;assert(runtime&&runtime->packed);
  auto&p=*runtime->packed;auto&pool=*p.sourceMapPool_;
  battle=*pool.allocate(pool.root(),BattleBytes);assert(battle&&battle!=p.Battle);copy(p.Battle,battle,BattleBytes);
  for(unsigned id=0;id<4;++id){const auto old=p.unitAddress(int32_t(id));if(!old)continue;
   units[id]=*pool.allocate(pool.root(),UnitBytes);parts[id]=*pool.allocate(pool.root(),PartBytes);assert(units[id]&&parts[id]);copy(old,units[id],UnitBytes);
   const auto head=p.word(old+0x14);assert(head);copy(head,parts[id],PartBytes);p.word(units[id]+0x14,parts[id]);
   auto at=head;while(at){const auto offset=at-head;assert(offset<PartBytes);const auto next=p.word(at);p.word(parts[id]+offset,next?parts[id]+next-head:0);p.word(parts[id]+offset+0x4ec,units[id]);at=next;}
   p.word(battle+0x20+4*id,units[id]);
  }
  p.word(0x8041eb90,battle);assert(p.battleAddress()==battle&&p.sourceAllocationLifetime(battle));
 }
 void copy(uint32_t from,uint32_t to,uint32_t size){auto&p=*runtime->packed;for(uint32_t i=0;i<size;++i)p.byte(to+i,p.byte(from+i));}
 void recordsAndProjection(){
  auto&p=*runtime->packed;assert(p.unitAddress(1)==units[1]);assert(!p.unitAddress(-1)&&!p.unitAddress(64));
  const auto oldInit=p.word(p.Units+0x1000+0x288);p.recordInit(1,123);assert(p.word(units[1]+0x288)==123&&p.word(p.Units+0x1000+0x288)==oldInit);
  p.recordPhase(1,124);assert(p.word(units[1]+0x29c)==124);p.recordAttack(1,125);assert(p.word(units[1]+0x2a8)==125);
#ifdef HARBOR_TEST_PROJECTION_ADDRESSES
  p.half(units[1]+0x10c,4);p.word(units[1]+0x218,0x12345678);p.real(units[1]+0x3c,34);p.capture(scene);assert(scene.actors[1].hp==4&&scene.actors[1].work[0]==0x12345678&&scene.actors[1].position[0]==34);
  const auto oldFlags=p.word(p.Battle+0xef4);scene.battleFlags=0x1200;scene.actors[1].work[0]=0x778899;p.publish(scene);
  assert(p.word(battle+0xef4)==0x1200&&p.word(units[1]+0x218)==0x778899&&p.word(p.Battle+0xef4)==oldFlags);
#endif
  // Source part identity comes from actual registry/head/body definition, not
  // the former native Parts slot. Damage mathematics is deliberately unchanged.
  p.readDamage(1);assert(p.damageParts[1].attributes==p.word(parts[1]+0x1ac));
  p.damageUnits[1].hp=3;p.writeDamage(1);uint16_t hp=0;assert(runtime->vm.memory.readHalf(units[1]+0x10c,hp)&&hp==3);
  const auto replacement=*p.sourceMapPool_->allocate(p.sourceMapPool_->root(),UnitBytes);copy(units[1],replacement,UnitBytes);p.word(battle+0x24,replacement);assert(p.unitAddress(1)==replacement);p.recordPhase(1,991);assert(p.word(replacement+0x29c)==991&&p.word(units[1]+0x29c)==124);
  p.word(battle+0x24,units[1]);
 }
 void originalEventLookup(){
  auto&p=*runtime->packed;constexpr uint32_t Arguments=0x84900000;assert(runtime->vm.memory.registerRam(Arguments,std::vector<uint8_t>(8)));
  EventEntry e;e.owner=1;e.arguments=Arguments;e.argumentCount=2;p.word(Arguments,1);p.word(Arguments+4,0x1234);int32_t result=0;
  assert(p.invoke(e,0x8010a500,1,result)&&result==2&&p.word(units[1]+0x28c)==0x1234);
  p.word(Arguments+4,0x40);p.word(battle+0x1f50,0);assert(p.invoke(e,0x80106f40,1,result)&&p.word(battle+0x1f50)==0x40);
 }
 uint32_t reuseBattleAllocation(){
  auto&p=*runtime->packed;auto&pool=*p.sourceMapPool_;const auto root=pool.root();assert(pool.free(root,battle));
  // A freed source block records payload length, while allocate requires a
  // further header. Retire the actual adjacent unit as teardown would so the
  // original free path can coalesce; an isolated same-size hole is not a fit.
  uint32_t neighbor=UINT32_MAX;for(auto unit:units)if(unit>battle)neighbor=std::min(neighbor,unit);for(auto part:parts)if(part>battle)neighbor=std::min(neighbor,part);
  assert(neighbor!=UINT32_MAX&&pool.free(root,neighbor));
  const auto next=pool.allocate(root,BattleBytes);assert(next&&*next==battle);return *next;
 }
 void retainedManagerRetirement(){
  auto&p=*runtime->packed;BattlePadSample sample{};sample.snapshot.buttons=BattleButtonA;sample.pressed=BattleButtonA;p.samplePhysical(100,&sample);
  assert(readPackedBattlePad(battle+0xf2c,nativeBattleActorServices(runtime->vm)).current.buttons==BattleButtonA&&readPackedBattlePad(p.Battle+0xf2c,nativeBattleActorServices(runtime->vm)).current.buttons==0);
  constexpr uint32_t UnsupportedMain=0x80123400;p.word(battle+0x1ca0,UnsupportedMain);
  assert(!p.actionManager(100)&&p.actionManagerPending());const auto first=p.actionBattleOwner;assert(first&&p.actionBattle==battle);
  assert(reuseBattleAllocation()==battle&&p.sourceAllocationLifetime(battle)!=first);
  p.word(battle+0x1ca0,0x99887766);assert(!p.actionManager(100)&&p.actionManagerPending()&&p.word(battle+0x1ca0)==0x99887766&&p.actionDisplays.count()==0);
 }
 void heldSelectionRegistryReplacement(){
  auto&p=*runtime->packed;p.word(battle+0x171c+0x540,11);p.byte(battle+0x171c+0x551,0);p.word(battle+0x19090,1);p.word(battle+0x19094,17);
  assert(!p.selectAttackEvent()&&p.attackSelectionCursor.active&&p.attackSelectionCursor.phase==2);
  const auto count=p.byte(battle+0x16f47);const auto script=p.word(units[1]+0x2a0);
  const auto replacement=*p.sourceMapPool_->allocate(p.sourceMapPool_->root(),UnitBytes);copy(units[1],replacement,UnitBytes);p.word(battle+0x24,replacement);p.word(replacement+0x2a0,0x99887766);
  assert(!p.selectAttackEvent()&&p.attackSelectionCursor.phase==2&&p.byte(battle+0x16f47)==count&&p.word(replacement+0x2a0)==0x99887766&&p.word(units[1]+0x2a0)==script);
 }
 void statusRetirement(){
  auto&p=*runtime->packed;const auto snapshot=scene.statusSnapshot();assert(snapshot&&snapshot->values[NativeBattleStatusSnapshot::MarioHp]==10);
  const auto first=p.sourceAllocationLifetime(battle);assert(reuseBattleAllocation()==battle&&p.sourceAllocationLifetime(battle)!=first);
  assert(!scene.statusSnapshot()); // reject before reading replacement zero bytes
 }
};
}
int main(){using namespace ttyd;assert(loadHarborTutorialAssets("romfs"));{HarborDynamicAddressesFixture f;f.recordsAndProjection();f.originalEventLookup();}{HarborDynamicAddressesFixture f;f.retainedManagerRetirement();}{HarborDynamicAddressesFixture f;f.statusRetirement();}{HarborDynamicAddressesFixture f;f.heldSelectionRegistryReplacement();}std::cout<<"relocated Harbor source B/U/P and allocation retirement tests PASS (TEST relocation, no activation)\n";return 0;}
