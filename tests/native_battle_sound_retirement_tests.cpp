#include "native_battle_sound.hpp"
#include "positional_sfx_start.hpp"
#include "event_native_runtime.hpp"
#include "battle_timing_a.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace ttyd;
namespace {
struct Stop {
 NativeBattleSound* owner=nullptr;unsigned calls=0;bool ready=false;
 PositionalSfxStopServices services(){return {this,nullptr,[](void* p,PositionalSfxStopKind,int32_t engine){auto& x=*static_cast<Stop*>(p);++x.calls;assert(engine==71);assert(!x.owner->menu(2520));assert(!x.owner->retireOwned(x.services()));return x.ready;}};}
};
}
int main(){
 EventNativePlatform platform;platform.truncate=[](void*,float v){assert(std::isfinite(v));return int32_t(v);};
 EventNativeRuntime vm(0x81000000,platform);EventScriptAssets original;assert(original.load("romfs/harbor-tutorial-events.pmevt"));assert(vm.memory.importScripts(original));
 assert(vm.memory.writeWord(vm.gpAddresses.currentAddress()+4,60));assert(vm.memory.writeWord(vm.gpAddresses.currentAddress()+0x174,4));
 auto registry=std::make_shared<PositionalSfxRegistry>();auto life=std::make_shared<int>(1);bool alive=true;BattleRandom random(0);
 auto binding=NativeBattleSoundBinding{&vm,&random,registry,life,&alive,[](void* p){return *static_cast<bool*>(p);}};
 NativeBattleSound owner(binding),foreign(binding);Stop stop;stop.owner=&owner;
 assert(!registry->slotLifetime(40));for(unsigned i=0;i<40;++i)assert(!registry->slotLifetime(i));
 // A completed source rejection has no incarnation, no native owned slot.
 auto rejected=owner.menu(2522);assert(rejected && *rejected==-1 && !registry->allocationGeneration);
 auto first=owner.menu(2520),second=owner.menu(2521);assert(first && second && *first!=-1 && *second!=-1);
 const unsigned a=uint32_t(*first)&255,b=uint32_t(*second)&255;assert(a==39 && b==38);
 const auto prefix=owner.menu(2523);assert(prefix && (uint32_t(*prefix)&255)==37);
 const auto oldA=registry->slotLifetime(a),oldB=registry->slotLifetime(b);assert(oldA && oldB && oldA!=oldB);
 // Genuine external Off retires the actual incarnation. Numeric generation
 // wrap/reuse cannot grant this owner the subsequent foreign birth.
 assert(registry->off(*first,{}));assert(!registry->slotLifetime(a));
 registry->allocationGeneration=uint16_t((uint32_t(*first)>>8)-1);
 const auto reused=foreign.menu(2520);assert(reused && *reused==*first);
 const auto foreignA=registry->slotLifetime(a);assert(foreignA && foreignA!=oldA);
 // Native copies are distinct actual banks even when metadata/tokens match.
 PositionalSfxRegistry other=*registry;assert(!owner.usesRegistry(&other));
 // Ascending retirement reaches this actual started voice first. A held real
 // backend stop leaves it owned. Reentry cannot allocate or recursively stop.
 registry->slots[b].flags=0;registry->slots[b].engine=71;
 assert(!owner.retireOwned(stop.services()) && stop.calls==1);
 assert(!registry->slotLifetime(37)); // successful earlier pending stop is never replayed
 assert(registry->slotLifetime(b)==oldB && registry->slotLifetime(a)==foreignA);
 PositionalSfxStopServices different=stop.services();different.user=nullptr;
 assert(!owner.retireOwned(different) && stop.calls==1);
 EventEntry untouched;assert(!owner.voice(untouched)); // blocked before event access during retirement
 stop.ready=true;assert(owner.retireOwned(stop.services()) && stop.calls==2);
 assert(!registry->slotLifetime(b) && registry->slotLifetime(a)==foreignA);
 assert(owner.retireOwned(stop.services()) && stop.calls==2 && !owner.pending());
 assert(!owner.menu(2520));
 // Original external Off intentionally ignores the handle generation; this
 // native safeguard must not change original source stale-handle semantics.
 assert(registry->off(*first,{}));assert(!registry->slotLifetime(a));
 const auto third=foreign.menu(2523);assert(third && *third!=-1);
 const unsigned c=uint32_t(*third)&255;const auto preInit=registry->slotLifetime(c);assert(preInit);
 registry->initializePublicSlots();for(unsigned i=0;i<40;++i)assert(!registry->slotLifetime(i));
 assert(foreign.retireOwned({}));
 NativeBattleSound pending(binding);assert(pending.menu(2520));assert(pending.retireOwned({}));
 for(unsigned i=0;i<40;++i)assert(!registry->slotLifetime(i));
 // Completed backend work is not replayed if the actual lifetime is lost
 // immediately afterward. Source Off has already cleared its incarnation.
 NativeBattleSound losing(binding);assert(losing.menu(2520));
 registry->slots[39].flags=0;registry->slots[39].engine=17;
 struct Loss{bool* alive;unsigned calls=0;} loss{&alive};
 PositionalSfxStopServices lossStop{&loss,nullptr,[](void* p,PositionalSfxStopKind,int32_t e){auto& v=*static_cast<Loss*>(p);assert(e==17);++v.calls;*v.alive=false;return true;}};
 assert(!losing.retireOwned(lossStop) && loss.calls==1 && !registry->slotLifetime(39));
 alive=true;assert(losing.retireOwned(lossStop) && loss.calls==1);
 NativeBattleSound dead(binding);assert(dead.menu(2520));const auto deadToken=registry->slotLifetime(39);alive=false;
 assert(!dead.retireOwned({}) && registry->slotLifetime(39)==deadToken);
 std::puts("Actual public-slot births, foreign reuse, wrapped handles, held retirement, reentry and init ownership pass");return 0;
}
