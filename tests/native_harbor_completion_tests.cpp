#include "native_harbor_completion.hpp"
#include "harbor_tutorial_packed.hpp"
#include "battle_menu_file_cache.hpp"
#include "native_battle_sound.hpp"
#include <cassert>
#include <cstdio>
#include <cmath>
using namespace ttyd;
namespace {
struct Fixture {
 static constexpr uint32_t B=HarborTutorialPacked::Battle,P=0x84400000,U=0x84100000,Partner=U+0x1000,I=0x84500000,S=I+0x100;
 EventNativePlatform platform;EventNativeRuntime vm{0x81000000,platform};HarborTutorialPacked packed{vm};
 std::shared_ptr<BattleMenuFileCache> files=std::make_shared<BattleMenuFileCache>();
 std::shared_ptr<int> life=std::make_shared<int>(1);bool alive=true;unsigned presentation=0,music=0;bool visualReady=false,musicReady=false;
 std::array<EventEntry,2> entries{};std::unique_ptr<NativeHarborCompletion> owner;uint32_t descriptor=0;
 Fixture(){
  for(auto span:std::array<std::pair<uint32_t,uint32_t>,6>{{{0x8041eb90,4},{B,0x19400},{0x8041eb00,4},{P,0x5d4},{U,0x2000},{I,0x200}}})assert(vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
  put(0x8041eb90,B);put(0x8041eb00,P);put(B+0x24,U);put(B+0x28,Partner);put(U+8,222);put(Partner+8,224);
  half(U+0x10c,4);half(U+0x112,2);half(Partner+0x10c,3);half(P+0x72,10);half(P+0x76,5);half(P+0x8e,10);half(P+0x90,5);
  put(B+0x2738,I);put(I+0xc,S);put(Partner+0x114,0x3f800000);put(Partner+0x138,0x12345678);half(Partner+0x134,7);half(Partner+0x13c,8);half(Partner+0x13e,9);
  for(unsigned i=0;i<7;++i)put(Partner+0x118+4*i,10+i);
  assert(files->load("romfs/battle-class-common.tpl")&&files->bind(vm.memory));const auto acquired=files->acquire("./battle/common/battle_common.tpl",4);assert(acquired&&*acquired);descriptor=*acquired;put(B+0x163f8,descriptor);
  // TEST native entry storage, not original event Init/whole BattleInit.
  vm.gp.battleContext=1;vm.battle.entries=entries.data();vm.battle.capacity=2;
  entries[0].flags=1;entries[0].id=7;vm.runtime.allocationCount=1;
 }
 void put(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}
 void half(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}
 uint16_t half(uint32_t a){uint16_t v=0;assert(vm.memory.readHalf(a,v));return v;}
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm.memory.readWord(a,v));return v;}
 void bind(int32_t result=1){owner=std::make_unique<NativeHarborCompletion>(NativeHarborCompletionBinding{&vm,&packed,life,[this]{return alive;},files,result});}
 NativeHarborCompletionServices services(){return {life,this,[](void* p){return static_cast<Fixture*>(p)->alive;},[](void* p){auto& f=*static_cast<Fixture*>(p);++f.presentation;assert(!f.owner->resume(f.services()));assert(!f.owner->receipt());return f.visualReady;},[](void* p){auto& f=*static_cast<Fixture*>(p);++f.music;return f.musicReady;},{}};}
};
void heldCleanupAndExport(){Fixture f;f.bind();assert(!f.owner->receipt());assert(!f.owner->resume(f.services()));assert(f.presentation==1&&!f.music&&f.entries[0].flags==1);
 f.visualReady=true;assert(!f.owner->resume(f.services()));assert(f.presentation==2&&f.music==1&&f.entries[0].flags==1);
 auto swapped=f.services();swapped.retireMusic=[](void*){return true;};assert(!f.owner->resume(swapped)&&f.music==1);
 f.musicReady=true;assert(f.owner->resume(f.services()));assert(f.presentation==2&&f.music==2&&f.vm.runtime.allocationCount==0&&!f.entries[0].flags);
 auto receipt=f.owner->receipt();assert(receipt&&receipt->result==1&&receipt->pouch.half(0x70)==4&&receipt->pouch.half(0x74)==2&&receipt->pouch.half(0x72)==10&&receipt->pouch.half(0x76)==5&&receipt->pouch.half(20)==3);
 assert(f.word(Fixture::B+0x162a0)==0x3f800000&&f.word(Fixture::B+0x162a0+0x24)==0x12345678);
 assert(f.owner->resume(f.services())&&f.presentation==2&&f.music==2);
 assert(f.half(f.descriptor+2)==0&&f.word(Fixture::B+0x163f8)==f.descriptor&&f.word(Fixture::I+0x10)==0);
 f.alive=false;assert(!f.owner->receipt()&&!f.owner->resume(f.services()));
}
void actualSoundHoldRetainsExport(){Fixture f;f.vm.platform.truncate=[](void*,float v){assert(std::isfinite(v));return int32_t(v);};EventScriptAssets original;assert(original.load("romfs/harbor-tutorial-events.pmevt")&&f.vm.memory.importScripts(original));
 assert(f.vm.memory.writeWord(f.vm.gpAddresses.currentAddress()+4,60)&&f.vm.memory.writeWord(f.vm.gpAddresses.currentAddress()+0x174,4));
 auto registry=std::make_shared<PositionalSfxRegistry>();
 f.packed.sourceSound=std::make_shared<NativeBattleSound>(NativeBattleSoundBinding{&f.vm,&f.packed.random,registry,f.life,&f,[](void*p){return static_cast<Fixture*>(p)->alive;}});
 const auto h=f.packed.sourceSound->menu(2520);assert(h&&*h!=-1);const auto index=uint32_t(*h)&255;registry->slots[index].flags=0;registry->slots[index].engine=7;
 struct Backend{bool ready=false;unsigned stops=0;} backend;
 auto services=f.services();services.publicSound={&backend,nullptr,[](void*p,PositionalSfxStopKind,int32_t engine){auto& x=*static_cast<Backend*>(p);assert(engine==7);++x.stops;return x.ready;}};
 f.visualReady=f.musicReady=true;f.bind();assert(!f.owner->resume(services)&&backend.stops==1&&!f.owner->receipt());
 assert(!f.entries[0].flags&&f.half(Fixture::P+0x70)==4&&f.half(f.descriptor+2)==1);
 f.half(Fixture::U+0x10c,9);assert(!f.owner->resume(services)&&backend.stops==2&&f.half(Fixture::P+0x70)==4);
 backend.ready=true;assert(f.owner->resume(services)&&backend.stops==3&&!registry->slotLifetime(index));
 assert(f.owner->receipt()->pouch.half(0x70)==4&&f.half(f.descriptor+2)==0);
}
void originalHealCondition(){for(unsigned flags:{0u,0x40u}){Fixture f;f.put(Fixture::S+4,flags);f.half(Fixture::U+0x10c,0);f.visualReady=f.musicReady=true;f.bind();assert(f.owner->resume(f.services()));assert(f.owner->receipt()->pouch.half(0x70)==(flags?1:0));}}
void invalidOrMissingAdmission(){Fixture f;f.bind(3);f.visualReady=f.musicReady=true;assert(!f.owner->resume(f.services())&&!f.presentation&&f.entries[0].flags==1);
 f.bind();f.vm.schedule.managerActive=1;assert(!f.owner->resume(f.services())&&!f.presentation);f.vm.schedule.managerActive=0;
 f.entries[0].unresolvedLinkMask=1;assert(!f.owner->resume(f.services())&&f.entries[0].flags==1);f.entries[0].unresolvedLinkMask=0;
 f.put(0x8041eb90,0);assert(!f.owner->resume(f.services())&&!f.owner->receipt());
}
}
int main(){heldCleanupAndExport();actualSoundHoldRetainsExport();originalHealCondition();invalidOrMissingAdmission();std::puts("Actual VM event deletion, original packed Mario/party export, native service holds and receipt ownership pass");return 0;}
