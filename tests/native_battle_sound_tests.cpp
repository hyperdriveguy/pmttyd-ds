#include "native_battle_sound.hpp"
#include "event_native_runtime.hpp"
#include "battle_timing_a.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
using namespace ttyd;
int main(){
 EventNativePlatform platform;platform.truncate=[](void*,float v){assert(std::isfinite(v) && v>=-2147483648.f && v<2147483648.f);return int32_t(v);};
 EventNativeRuntime vm(0x81000000,platform);EventScriptAssets original;assert(original.load("romfs/harbor-tutorial-events.pmevt"));assert(vm.memory.importScripts(original));
 constexpr uint32_t Battle=0x84000000,Unit=0x84101000,Arguments=0x92000000;
 assert(vm.memory.registerRam(Battle,std::vector<uint8_t>(0x120)) && vm.memory.registerRam(Unit,std::vector<uint8_t>(0x1000)) && vm.memory.registerRam(Arguments,std::vector<uint8_t>(8)) && vm.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
 auto word=[&](uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));};
 word(0x8041eb90,Battle);word(Battle+0x24,Unit);word(Unit,1);word(Unit+8,222);word(Arguments,1);word(Arguments+4,1);
 const float position[]{1,2,3};for(unsigned i=0;i<3;++i){uint32_t v;std::memcpy(&v,&position[i],4);word(Unit+0x3c+4*i,v);}
 vm.gp.battleContext=1;word(vm.gpAddresses.currentAddress()+4,60);word(vm.gpAddresses.currentAddress()+0x174,4);
 auto registry=std::make_shared<PositionalSfxRegistry>();auto life=std::make_shared<int>(1);bool alive=true;BattleRandom random(0);
 NativeBattleSound owner({&vm,&random,registry,life,&alive,[](void* p){return *static_cast<bool*>(p);}});
 EventEntry event;event.id=7;event.currentCommand=0x90000000;event.arguments=Arguments;
 BattleRandom expected=random;expected.bounded(5);const auto expectedPitch=expected.bounded(41)-20;
 const auto result=owner.voice(event);assert(result && *result==2 && !owner.pending());
 assert(random.state==expected.state && registry->allocationGeneration==1);
 const auto& voice=registry->slots.back();assert(voice.key!=-1 && (voice.flags&3)==3 && voice.engine==-1 && voice.volume==127 && voice.pitch==uint16_t(expectedPitch));assert((voice.position==std::array<float,3>{1,2,3}));
 word(Arguments+4,2);event.currentCommand+=4;BattleRandom second=random;second.bounded(10);const auto secondPitch=second.bounded(41)-20;
 const auto secondVoice=owner.voice(event);assert(secondVoice && *secondVoice==2 && random.state==second.state);
 assert(registry->slots[38].pitch==uint16_t(secondPitch));
 for(uint32_t key=2520;key<=2523;++key){auto handle=owner.menu(key);assert(handle);if(key==2522){assert(*handle==-1);continue;}assert(*handle!=-1);const auto& slot=registry->slots[uint32_t(*handle)&255];assert(slot.key==int32_t(key) && (slot.flags&1) && !(slot.flags&6) && slot.engine==-1);}
 assert(registry->allocationGeneration==5);
 // Unsupported actual started-engine stealing domain holds before On effects.
 auto& started=registry->slots[0];started.key=2520;started.flags=0;started.engine=99;
 const auto generation=registry->allocationGeneration;assert(!owner.menu(2520));assert(registry->allocationGeneration==generation);
 started.key=-1;started.engine=-1;
 for(unsigned i=5;i<registry->slots.size();++i){const auto handle=owner.menu(2520);assert(handle && *handle!=-1);}
 assert(registry->allocationGeneration==registry->slots.size());
 event.currentCommand+=4;const auto fullGeneration=registry->allocationGeneration;
 const auto rejected=owner.voice(event);assert(rejected && *rejected==2 && registry->allocationGeneration==fullGeneration);
 alive=false;const auto seed=random.state;assert(!owner.menu(2520) && !owner.voice(event));assert(random.state==seed && registry->allocationGeneration==fullGeneration);
 std::puts("Actual source voice tables/PSND rows/VM/random/public pending-slot closure passes");
}
