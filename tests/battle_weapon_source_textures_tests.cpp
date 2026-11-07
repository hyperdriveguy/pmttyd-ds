#include "battle_weapon_source_textures.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x84000000,Private=0x60018000;
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float v){assert(std::isfinite(v)&&double(v)>=-2147483648.&&double(v)<2147483648.);return int32_t(v);};return p;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>startup;
 std::shared_ptr<EventGpLanguageAlias>alias;std::shared_ptr<EventNativeRuntime>vm=std::make_shared<EventNativeRuntime>(0x81100000,platform());
 std::shared_ptr<BattleWeaponSourceMemory>memory;std::shared_ptr<BattleMenuFileCache>files=std::make_shared<BattleMenuFileCache>();
 std::shared_ptr<const uint8_t>allocation=std::make_shared<const uint8_t>(0);TrioWeaponMenuFrameBinding source;std::shared_ptr<BattleWeaponSourceTextures>textures;
 uint32_t palette=0,descriptor=0;bool live=true,sourceLive=true,holdTruncate=false,reenter=false;unsigned truncateCalls=0;EventScriptAssets script;
 Fixture(){
  process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());startup=NativeUiStartup::create(process,"romfs");assert(startup&&startup->resumeFont());
  assert(script.load("romfs/trio-attack-events.pmevt")&&vm->memory.importScripts(script));
  assert(vm->memory.registerRam(B,std::vector<uint8_t>(0x16400)));
  // Explicit TEST source global B; original MenuResource Init below produces
  // the real descriptor slot and relocated palette. No old B+a0 write.
  assert(vm->memory.registerRam(0x8041eb90,{0x84,0,0,0}));
  assert(files->load("romfs/battle-class-common.tpl")&&files->bind(vm->memory));
  BattleMenuFileBinding binding(files,nativeBattleActorServices(*vm));auto services=binding.services();BattleMenuResourceCursor cursor;assert(initializePackedBattleMenuResources(services,cursor));
  assert(vm->memory.readWord(B+0x163f8,descriptor));uint32_t cell;assert(vm->memory.readWord(descriptor+0xa0,cell)&&vm->memory.readWord(cell,palette));
  uint32_t gp;assert(process->process().vm().memory.readWord(0x80415fd0,gp));alias=std::make_shared<EventGpLanguageAlias>(process->process().vm().gpAddresses,gp,startup->lifetime(),[u=startup]{return u->current();});
  memory=BattleWeaponSourceMemory::create(*vm,startup,alias,vm,[this]{return live;},"romfs/source-ui-startup/memory.pmevt");assert(memory);
  source.native.battle=B;source.native.allocation=B+0x100;source.native.allocationOwner=allocation;source.native.generation=1;source.native.sourceResourceOwner=files;
  source.native.addresses.base=0x60014000;source.native.addresses.unmapped=[this](uint32_t a,uint32_t n){return vm->memory.unmappedRange(a,n);};
  source.native.allocationCurrent=[this]{return live;};source.native.sourceResourceCurrent=[this]{uint32_t d;return live&&vm->memory.readWord(B+0x163f8,d)&&d==descriptor;};
  textures=make(Private);assert(textures);
 }
 std::shared_ptr<BattleWeaponSourceTextures>make(uint32_t address){return BattleWeaponSourceTextures::create(source,memory,address,[this]{return sourceLive;},[this](float v)->std::optional<int32_t>{++truncateCalls;if(reenter){assert(!textures->texture(palette,0)&&!textures->current());}if(holdTruncate)return {};assert(std::isfinite(v));return int32_t(v);});}
};
void genuineConstructorAndReuse(){Fixture f;for(uint32_t i=0;i<4;++i){const auto v=f.textures->texture(f.palette,i);assert(v&&v->handle==Private&&v->bytes&&v->current(v->user)&&v->witness(v->user,*v));assert(v->image.index==i&&v->image.imageHeader&&v->image.data);assert(v->image.width==(i==0?40:i==1?128:i==2?16:8));assert(v->image.height==(i==0?40:i==1?80:24));
  const auto calls=f.truncateCalls;const auto retry=f.textures->texture(f.palette,i);assert(retry&&retry->bytes==v->bytes&&f.truncateCalls==calls);assert(!f.textures->texture(f.palette,i^1)&&!f.textures->acknowledge(f.palette,i^1));assert(f.textures->acknowledge(f.palette,i));
 }
 assert(!f.textures->texture(f.palette,4)&&!f.textures->texture(f.palette+4,0));
 const auto v=f.textures->texture(f.palette,0);assert(v);assert(f.vm->memory.writeWord(B+0x163f8,0));assert(!v->current(v->user)&&!v->witness(v->user,*v));assert(f.vm->memory.writeWord(B+0x163f8,f.descriptor)&&!f.textures->current());
}
void actualHoldAndAliasAdmission(){Fixture f;assert(!f.make(0)&&!f.make(0x60014000)&&!f.make(B+0x20)&&!f.make(0xfffffff0));
 f.holdTruncate=true;assert(!f.textures->texture(f.palette,0));const auto pc=f.textures->pending();assert(pc);const auto calls=f.truncateCalls;assert(!f.textures->texture(f.palette,0)&&f.textures->pending()==pc&&f.truncateCalls==calls+1);assert(!f.textures->texture(f.palette,1));
 f.holdTruncate=false;f.reenter=true;const auto v=f.textures->texture(f.palette,0);assert(v&&v->witness(v->user,*v));const auto n=f.truncateCalls;assert(f.textures->texture(f.palette,0)&&f.truncateCalls==n);assert(f.textures->acknowledge(f.palette,0));
 // A newly owned private address invalidates the native reservation; no
 // fallback to unrelated VM words may certify the former private object.
 assert(f.vm->memory.registerRam(Private,std::vector<uint8_t>(32)));assert(!f.textures->current()&&!v->current(v->user));
}
}
int main(){genuineConstructorAndReuse();actualHoldAndAliasAdmission();return 0;}
