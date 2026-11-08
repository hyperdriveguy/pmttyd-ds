// Exact private production implementation, for actual Runtime/VM ownership
// checks only. TEST pre-conclusion deletion/status setup is not gameplay proof.
#ifndef HARBOR_COMPLETION_IMPLEMENTATION
#define HARBOR_COMPLETION_IMPLEMENTATION "../source/battle/harbor_tutorial.cpp"
#endif
#include HARBOR_COMPLETION_IMPLEMENTATION
#include "native_harbor_completion.hpp"
#include "prologue_pouch.hpp"
#include "game.hpp"
#include "native_mario_constructor.hpp"
#include "native_field_animation.hpp"
#include <cmath>
#include <cassert>
#include <cstdio>
namespace ttyd {
struct NativeHarborCompletionFixture {
 static void canonicalReturn(){
  assert(loadFieldPlayerPoseAssets("romfs")&&loadHarborEncounterAssets("romfs"));
  EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  platform.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};
  platform.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};
  platform.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};
  auto game=std::make_unique<Game>(false);game->progress.originalPouch=createProloguePouch();assert(game->progress.originalPouch);
  game->progress.originalPouch->setHP(8);game->progress.originalPouch->half(0x74,3);game->progress.originalPouch->half(0x78,233);
  game->progress.hp=8;game->progress.room=1;game->progress.originalStory=4;
  game->progress.position={0,-1};game->groundHeight=game->progress.elevation=.15f;
  assert(game->initializeNativeUiFieldAnimation("romfs",NativeFileCache::create(),platform));
  const auto field=game->nativeMarioConstructor();const auto canonical=game->progress.originalPouch;
  assert(field&&field->sharedPouch()==canonical);const auto address=field->pouchAddress();const auto initial=canonical->bytes;
  auto& fieldVm=field->ui()->fieldProcess()->process().vm();
  assert(!game->beginHarborTutorialFromField()); // No source setup2 request.
  HarborTutorial refused;
  NativeHarborFieldPouchBinding foreign{field,std::make_shared<ProloguePouch>(*canonical),[]{return true;}};
  assert(!refused.beginFromNativePouch(0,4,foreign)&&!refused.triggered);
  foreign.pouch=std::shared_ptr<ProloguePouch>(canonical.get(),[](ProloguePouch*){});
  assert(!refused.beginFromNativePouch(0,4,foreign)&&!refused.triggered);
  foreign.pouch=canonical;foreign.current=[]{return false;};
  assert(!refused.beginFromNativePouch(0,4,foreign)&&!refused.triggered);
  // Execute the original132-command beforebattle adapter to its retained
  // setup2 boundary. TEST direct manager driving is not a field playthrough.
  assert(game->beginHarborEncounter(false));
  for(unsigned tick=1;tick<6000&&!game->harborEncounter.battleRequested;++tick){
   game->ticks=tick;game->harborEncounter.tick(tick*1000/60,tick%4==0?0x100:0,15);assert(!game->harborEncounter.failed);
  }
  assert(game->harborEncounter.battleSetup==2&&game->beginHarborTutorialFromField());
  auto&t=game->harborTutorial;const auto r=t.runtime_;assert(r->fieldPouch.pouch==canonical&&r->fieldPouch.owner==field);
  NativeHarborCompletionReceipt fake;fake.result=1;fake.pouch=*canonical;fake.battleLifetime=r;
  assert(!game->commitHarborPouchReturn(fake)&&canonical->bytes==initial);
  // TEST status27 conclusion/root retirement admits the real completion
  // owner. This is separate from physical fatal/reward/end acceptance.
  assert(r->vm.memory.writeByte(r->packed->unitAddress(2)+0x135,1));
  for(int i=0;i<r->vm.battle.capacity;++i)if(r->vm.battle.entries[i].flags&1)deleteEvent(r->vm.battle.entries[i],r->vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;t.initialized=t.introComplete=true;t.message=MessagePlayback{};t.pendingService=0;
  t.actorById(HarborTutorial::MarioId)->hp=4;
  assert(r->vm.memory.writeHalf(r->packed->unitAddress(1)+0x112,2));
  uint32_t battlePouch=0;assert(r->vm.memory.readWord(0x8041eb00,battlePouch)&&r->vm.memory.writeHalf(battlePouch+0x78,317));
  auto completion=t.beginNativeCompletion(1);assert(completion);
  struct Hooks{Game* game;bool visual=false,music=false;std::shared_ptr<ProloguePouch> replacement;const NativeHarborCompletionReceipt* recursive=nullptr;} hooks{game.get()};
  NativeHarborCompletionServices services;services.lifetime=std::make_shared<int>(1);services.user=&hooks;
  services.current=[](void*p){auto& h=*static_cast<Hooks*>(p);if(h.replacement)h.game->progress.originalPouch=h.replacement;if(h.recursive)assert(!h.game->commitHarborPouchReturn(*h.recursive));return true;};
  services.retirePresentation=[](void*p){return static_cast<Hooks*>(p)->visual;};
  services.retireMusic=[](void*p){return static_cast<Hooks*>(p)->music;};
  assert(!completion->resume(services)&&!game->commitHarborPouchReturn(fake)&&canonical->bytes==initial);
  hooks.visual=true;assert(!completion->resume(services)&&canonical->bytes==initial);
  hooks.music=true;assert(completion->resume(services));const auto receipt=completion->receipt();assert(receipt);
  assert(receipt->pouch.currentHP()==4&&receipt->pouch.half(0x74)==2&&receipt->pouch.half(0x78)==317&&canonical->bytes==initial);
  auto forged=*receipt;forged.result=0;assert(!game->commitHarborPouchReturn(forged));
  forged=*receipt;forged.pouch.bytes[0]^=1;assert(!game->commitHarborPouchReturn(forged));
  forged=*receipt;forged.battleLifetime=std::shared_ptr<const void>(receipt->battleLifetime.get(),[](const void*){});assert(!game->commitHarborPouchReturn(forged));
  auto detached=std::make_unique<Game>(*game);assert(!detached->commitHarborPouchReturn(*receipt));
  HarborTutorial copied=t;assert(!copied.commitNativePouchReturn(*receipt));
  hooks.replacement=std::make_shared<ProloguePouch>(*canonical);
  assert(!game->commitHarborPouchReturn(*receipt)&&canonical->bytes==initial); // Replacement during receipt validation.
  game->progress.originalPouch=canonical;hooks.replacement.reset();
  auto animation=field->ui()->fieldProcess();int lease=0;
  assert(animation->beginNpcPass(&lease,1,game->cameraManager,[]{return true;}));
  assert(!game->commitHarborPouchReturn(*receipt)&&canonical->bytes==initial&&!r->fieldPouchReturned);
  assert(animation->endNpcPass(&lease,1));
  hooks.recursive=&*receipt;assert(game->commitHarborPouchReturn(*receipt));hooks.recursive=nullptr;
  assert(game->progress.originalPouch==canonical&&canonical->bytes==receipt->pouch.bytes&&game->progress.hp==4);
  assert(field->pouchAddress()==address&&fieldVm.memory.hasRamBacking(address,canonical->bytes.data(),canonical->bytes.size()));
  uint16_t raw=0;assert(fieldVm.memory.readHalf(address+0x70,raw)&&raw==4);
  assert(fieldVm.memory.readHalf(address+0x74,raw)&&raw==2);assert(fieldVm.memory.readHalf(address+0x78,raw)&&raw==317);
  canonical->setHP(6);canonical->half(0x78,318);
  assert(game->commitHarborPouchReturn(*receipt)&&canonical->currentHP()==6&&canonical->half(0x78)==318&&game->progress.hp==6);
  assert(!game->commitHarborPouchReturn(forged)&&canonical->currentHP()==6);
  // A foreign physical mapping revokes the retained original SDK owner, even
  // when all copied bytes match. No repeated visit may mutate either storage.
  assert(fieldVm.memory.unregisterExternalRam(address,canonical->bytes.data()));
  assert(fieldVm.memory.registerRam(address,std::vector<uint8_t>(canonical->bytes.begin(),canonical->bytes.end())));
  assert(!game->commitHarborPouchReturn(*receipt)&&canonical->currentHP()==6);
  assert(fieldVm.memory.readHalf(address+0x70,raw)&&raw==6);
 }
 static void test(){
  assert(loadHarborTutorialAssets("romfs")&&loadProloguePouchAssets("romfs"));HarborTutorial t;
  ProloguePouch initial=initializeProloguePouch("Yoshi");initial.half(0x70,8);initial.half(0x74,3);
  assert(t.beginFromPouch(0,initial,4));auto r=t.runtime_;
  assert(!t.beginNativeCompletion(1)); // Original enemy group is not concluded.
  constexpr uint32_t args=0x93000000;assert(r->vm.memory.registerRam(args,std::vector<uint8_t>(16)));
  for(unsigned i=0;i<4;++i)assert(r->vm.memory.writeWord(args+4*i,i?i*10:2));
  EventEntry rotation;rotation.id=99;rotation.currentCommand=args;rotation.arguments=args;rotation.argumentCount=4;
  const auto math=r->vm.platform.fmodDouble;r->vm.platform.fmodDouble=nullptr;
  assert(r->call(rotation,0x80111720,true)==0&&t.pendingService==0x80111720);
  assert((t.actors[0].rotation==std::array<float,3>{0,0,0}));
  r->vm.platform.fmodDouble=math;assert(r->call(rotation,0x80111720,false)==2&&!t.pendingService);
  assert((t.actors[0].rotation==std::array<float,3>{10,20,30}));
  assert(r->call(rotation,0x801115f8,true)==2);
  assert((t.actors[0].rotation==std::array<float,3>{20,40,60}));

  // TEST actual source predicate domain; independent of death/script completion.
  const auto enemy=r->packed->unitAddress(2);assert(enemy);
  assert(r->vm.memory.writeByte(enemy+0x135,1));
  t.pendingService=0x801115f8;assert(!t.beginNativeCompletion(1));
  t.pendingService=0;
  for(int i=0;i<r->vm.battle.capacity;++i)if(r->vm.battle.entries[i].flags&1)deleteEvent(r->vm.battle.entries[i],r->vm.creationServices());
  t.phaseActive=t.attackActive=t.damageActive=false;
  t.initialized=t.introComplete=true;t.message=MessagePlayback{};
  assert(t.actors[0].damage);auto& fatal=r->root(t.actors[0].damage,t.actors[0].id);
  assert(!t.beginNativeCompletion(1)); // Still holds even if U+2b4 is absent.
  deleteEvent(fatal,r->vm.creationServices());
  assert(t.nativeGameplayConclusion()==1);assert(!t.beginNativeCompletion(3));
  const auto owner=t.beginNativeCompletion(1);assert(owner&&t.beginNativeCompletion(1)==owner);
  const auto now=t.now;BattlePadSample pad;pad.snapshot.buttons=0x100;
  t.tick(100,0x100,&pad);assert(t.now==now); // No source simulation replay while retiring.
  struct Hooks{bool alive=true;unsigned visuals=0,music=0;} hooks;
  auto life=std::make_shared<int>(1);NativeHarborCompletionServices s{life,&hooks,[](void* p){return static_cast<Hooks*>(p)->alive;},[](void* p){++static_cast<Hooks*>(p)->visuals;return true;},[](void* p){++static_cast<Hooks*>(p)->music;return true;},{}};
  assert(owner->resume(s));const auto receipt=owner->receipt();assert(receipt&&receipt->result==1&&receipt->pouch.half(0x70)==8&&receipt->pouch.half(0x74)==3);
  assert(hooks.visuals==1&&hooks.music==1);
  assert(!t.commitNativePouchReturn(*receipt)); // No native field entry association.
  // The retained owner pins the old VM, but public facade retirement revokes
  // permission before reading it. Replacement is not the old battle owner.
  t=HarborTutorial{};assert(!owner->resume(s)&&!owner->receipt());
  assert(hooks.visuals==1&&hooks.music==1);
 }
};
}
int main(){ttyd::NativeHarborCompletionFixture::test();ttyd::NativeHarborCompletionFixture::canonicalReturn();std::puts("Actual Harbor Runtime factory, source predicate/held death guard, frozen simulation and facade retirement pass");return 0;}
