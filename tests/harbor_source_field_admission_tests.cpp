#include "harbor_source_field_admission.hpp"
#include "battle_sequence_binding.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_menu_file_cache.hpp"
#include "field_npc_registry_owner.hpp"
#include "game.hpp"
#include <cassert>
#include <iostream>
#include <fstream>
#include "source_mario_dvd_root.hpp"
using namespace ttyd;
namespace ttyd {
// TEST-only prefix positioning. The real field child at phase36 emits the
// original BattleInit call; no preceding unavailable animation is acknowledged.
struct HarborSourceFieldBoundaryFixture {
 static uint32_t position(BattleSequenceBinding& b){
  assert(b.fieldInit_.active&&b.fieldInit_.pending==0x80041c58);
  b.animation_={};b.fieldInit_.phase=36;b.fieldInit_.queued=false;b.fieldInit_.call={};
  return b.fieldInit_.information;
 }
 static void changeFutureOperand(BattleSequenceBinding&b){b.fieldInit_.information+=4;}
};
}
namespace {
uint32_t be(const std::vector<uint8_t>&b,unsigned at){return uint32_t(b[at])<<24|uint32_t(b[at+1])<<16|uint32_t(b[at+2])<<8|b[at+3];}
std::vector<uint8_t> originalRoot(){
 std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);
 auto read=[&](uint32_t at,unsigned count){std::vector<uint8_t>b(count);iso.seekg(at);iso.read(reinterpret_cast<char*>(b.data()),count);assert(iso);return b;};
 const auto dol=be(read(0x420,4),0),root=sourceMarioDvdRoot(OriginalMarioDvdSourceR2);const auto header=read(dol,256);
 for(unsigned i=0;i<18;++i){const auto address=be(header,0x48+4*i),size=be(header,0x90+4*i);if(root>=address&&uint64_t(root)+2<=uint64_t(address)+size)return read(dol+be(header,4*i)+root-address,2);}
 assert(false);return {};
}
struct Resources {bool alive=true;unsigned requests=0;const BattleColdInitRequest*first=nullptr;BattleSequenceBinding*binding=nullptr;std::weak_ptr<HarborSourceFieldAdmission>owner;std::shared_ptr<BattleMenuFileBinding>menu;std::shared_ptr<BattleMenuFileCache>files;};
struct Fixture {
 struct Cache{unsigned lines=0,barriers=0;}cache;
 std::unique_ptr<Game>game;
 std::unique_ptr<EventNativeRuntime>vm;
 std::unique_ptr<BattleSequenceBinding>binding;
 std::unique_ptr<FieldNpcRegistryBinding>registry;
 std::shared_ptr<BattleMenuFileCache>files;
 std::shared_ptr<BattleMenuFileBinding>menu;
 std::shared_ptr<Resources>resources=std::make_shared<Resources>();
 std::shared_ptr<const HarborSourceFieldServices>services;
 BattleSequenceEntryServices parent;
 uint32_t information=0;
 Fixture(bool withMenu,bool bindNow=true){
 EventNativePlatform platform;platform.user=&cache;platform.flushBlock=[](void* p,uint32_t){++static_cast<Cache*>(p)->lines;};platform.systemCall=[](void* p){++static_cast<Cache*>(p)->barriers;};
 game=std::make_unique<Game>(false);vm=std::make_unique<EventNativeRuntime>(0x81000000,platform);
 EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));
 registry=game->initializeOriginalFieldNpcs(vm->memory,0x85000000);assert(registry);
 auto resource=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(resource && game->originalFieldNpcs->poses().install(resource));
 std::vector<uint8_t> model(64);for(unsigned i=0;i<64;++i)model[i]=resource->draw->byte(4+i);assert(vm->memory.registerRam(0x86000000,std::move(model)));
 uint32_t name=0;assert(vm->memory.readWord(0x90524f10+0x5c,name) && registry->enter(name,0x86000000,{})==0);
 auto n=*registry->lookup(name);uint32_t gp=0,f=0;assert(vm->memory.readWord(0x80415fd0,gp) && vm->memory.readWord(gp+0x168,f));
 // Explicit caller fixture, not the authored tik_01 encounter bridge.
 assert(vm->memory.writeWord(f+8,n));writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,11,vm->interpreterServices().operands);
 assert(loadProloguePouchAssets("romfs"));game->progress.originalPouch=createProloguePouch();assert(game->progress.originalPouch);
 auto storage=BattleSequenceStorage::cold(game->progress.originalPouch,0x88000000,0x100000,0x89000000);assert(storage);
 assert(!BattleSequenceStorage::cold({},0x88000000,0x100000,0x89000000));
 assert(!BattleSequenceStorage::cold(game->progress.originalPouch,0x88000001,0x100000,0x89000000));
 binding=game->bindOriginalBattleSequence(*vm,"romfs");assert(binding);
 assert(vm->memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
 assert(!binding->attachStorage(storage));uint32_t absent=0;assert(!vm->memory.readWord(0x8041e808,absent) && !game->battleSequenceProcess->storage);
 assert(vm->memory.unregisterRam(0x8041eb90) && binding->attachStorage(storage));
 assert(!binding->attachStorage(storage));
 assert(vm->memory.writeHalf(0x89000070,7) && game->progress.originalPouch->half(0x70)==7);


  // Real immutable source constants only, in missing TEST resource domains.
  EventScriptAssets original;assert(original.load("romfs/harbor-tutorial-events.pmevt"));
  for(const auto span:{std::pair<uint32_t,unsigned>{0x802f0240,12},{0x80422174,4}}){
   if(!vm->memory.contiguousRam(span.first,span.second)){
    std::vector<uint8_t>bytes(span.second);for(unsigned i=0;i<span.second;++i)assert(original.readByte(span.first+i,bytes[i]));
    assert(vm->memory.unmappedRange(span.first,span.second)&&vm->memory.registerRam(span.first,std::move(bytes)));
   }
  }
  // Exact original format bytes, including its own NUL; no invented tail.
  if(!vm->memory.contiguousRam(0x802ef810,1)){
   std::vector<uint8_t>text;for(unsigned i=0;i<256;++i){uint8_t c=0;assert(original.readByte(0x802ef810+i,c));text.push_back(c);if(!c)break;}
   assert(!text.empty()&&text.back()==0&&vm->memory.unmappedRange(0x802ef810,text.size())&&vm->memory.registerRam(0x802ef810,std::move(text)));
  }
  const auto root=sourceMarioDvdRoot(OriginalMarioDvdSourceR2);
  if(vm->memory.unmappedRange(root,2))assert(vm->memory.registerRam(root,originalRoot()));
  auto native=std::make_shared<HarborSourceFieldServices>();native->lifetime=resources;native->current=[r=resources]{return r->alive;};
  if(withMenu){
   files=std::make_shared<BattleMenuFileCache>();assert(files->load("romfs/battle-class-common.tpl")&&files->bind(vm->memory));
   menu=std::make_shared<BattleMenuFileBinding>(files,nativeBattleActorServices(*vm));native->menu=menu->services();resources->menu=menu;resources->files=files;
  }
  resources->binding=binding.get();
  native->coldChild=[r=resources](const BattleColdInitRequest&q)->std::optional<uint32_t>{
   assert(q.function==0x80139780&&q.callsite==0x800f901c);if(r->first)assert(r->first==&q);r->first=&q;++r->requests;
   assert(!r->binding->bindHarborSourceServices({}));
   if(auto o=r->binding->sourceHarbor()){assert(!o->resumeUnitSetup(o->battleAddress()));}
   return {}; // Actual Stage provider is deliberately unavailable.
  };
  services=native;if(bindNow)assert(binding->bindHarborSourceServices(services));
  assert(!binding->sourceHarbor());
  parent.call=[](void*,BattleSequenceCall&c){return c.function==0x800daefc||c.function==0x800db7a0||c.function==0x800d76b8||c.function==0x80005f28;};
  auto&work=game->originalSequence->state.work;assert(binding->callback(0x80073f20,work,parent));work[1]=4;
  assert(!binding->callback(0x80073b24,work,parent)&&binding->pending()==0x80041c58&&!binding->sourceHarbor());
  information=HarborSourceFieldBoundaryFixture::position(*binding);
 }
 bool resume(){return binding->callback(0x80073b24,game->originalSequence->state.work,parent);}
 uint32_t word(uint32_t at){uint32_t value=0;assert(vm->memory.readWord(at,value));return value;}
};
void actualBoundaryAndRetry(){
 Fixture f(true);assert(!f.resume());auto owner=f.binding->sourceHarbor();assert(owner&&owner->current());
 const auto b=owner->battleAddress();assert(b&&b==owner->pool()->root()+32&&f.word(0x8041eb90)==b);
 assert(f.word(b+0x2738)==f.information&&f.word(b+0xf0c)==0x01000000&&f.word(b+0x424)==0xffffffff);
 assert(!owner->coldComplete()&&!owner->unitSetupComplete()&&!owner->resumeUnitSetup(b));
 assert(owner->pending()==0x80139780&&f.resources->requests==1&&f.binding->pending()==0x800f8f04);
 assert(f.vm->memory.unmappedRange(0x84000000,0x19400)&&f.vm->memory.unmappedRange(0x84100000,0x4000)&&f.vm->memory.unmappedRange(0x84200000,0x4000));
 assert(owner->cameraReceipt().current(*f.game->cameraManager));
 const auto entry=f.word(b+0x163f8);assert(entry&&(f.word(entry)&0xffff)==1);
 auto replacementCalls=std::make_shared<unsigned>(0);
 auto& changed=const_cast<HarborSourceFieldServices&>(*f.services);changed.current=[] {return false;};
 changed.coldChild=[replacementCalls](const BattleColdInitRequest&)->std::optional<uint32_t>{++*replacementCalls;return {};};
 assert(owner->current()); // Bound provider snapshot survives caller mutation.
 auto life=owner->pool()->allocationLifetime(b);const auto flush=f.cache.lines,barriers=f.cache.barriers;
 HarborSourceFieldBoundaryFixture::changeFutureOperand(*f.binding);
 assert(!f.resume()&&f.binding->sourceHarbor()==owner&&f.resources->requests==2&&*replacementCalls==0);
 assert(owner->pool()->allocationLifetime(b)==life&&f.cache.lines==flush&&f.cache.barriers==barriers&&f.word(b+0x2738)==f.information&&f.word(b+0x163f8)==entry&&(f.word(entry)&0xffff)==1);
 assert(f.binding->bindHarborSourceServices(f.services));auto replacement=std::make_shared<HarborSourceFieldServices>(*f.services);assert(!f.binding->bindHarborSourceServices(replacement));
 f.vm->reinitializeContext();assert(owner->current()&&!f.resume()&&f.resources->requests==3);
 f.resources->alive=false;assert(!f.resume()&&!owner->current());f.resources->alive=true;assert(!owner->current());
}
void missingProvidersAndLateConfiguration(){
 {Fixture f(false);assert(!f.resume());auto o=f.binding->sourceHarbor();assert(o&&o->pending()==0x80119da0&&!o->coldComplete()&&f.resources->requests==0);const auto b=o->battleAddress();assert(!f.resume()&&o->battleAddress()==b&&f.resources->requests==0);}
 {Fixture f(true,false);assert(!f.resume()&&!f.binding->sourceHarbor());assert(f.binding->bindHarborSourceServices(f.services));assert(!f.resume()&&f.binding->sourceHarbor()&&f.resources->requests==1);}
}
void lifetime(){
 {Fixture f(true);assert(!f.resume());auto o=f.binding->sourceHarbor();f.registry.reset();assert(!o->current()&&!f.resume()&&f.resources->requests==1);}
 {Fixture f(true);assert(!f.resume());auto o=f.binding->sourceHarbor();auto pool=o->pool();const auto b=o->battleAddress();auto old=pool->allocationLifetime(b);assert(pool->free(pool->root(),b));auto reused=pool->allocate(pool->root(),0x19098);assert(reused&&*reused==b&&pool->allocationLifetime(b)!=old);assert(!o->current()&&!f.resume()&&f.resources->requests==1);}
 {Fixture f(true);assert(!f.resume());auto o=f.binding->sourceHarbor();f.registry.reset();f.vm.reset();assert(!o->current()&&!o->pool()&&!o->battleAddress());f.binding.reset();}
 {Fixture f(true);assert(!f.resume());auto o=f.binding->sourceHarbor();f.game.reset();assert(!o->current());f.binding.reset();}
 {Fixture f(true);assert(!f.resume());auto o=f.binding->sourceHarbor();f.binding.reset();assert(!o->current()&&!o->cameraReceipt().lifetime);}
 {Fixture f(true);assert(!f.resume());auto o=f.binding->sourceHarbor();f.game->restore(f.game->progress);assert(!o->current());}
}
}
int main(){actualBoundaryAndRetry();missingProvidersAndLateConfiguration();lifetime();std::cout<<"actual field-to-Harbor admission tests PASS\n";return 0;}
