// TEST-only implementation view: enter the real retained source UI constructor
// without adding a product harness method or fabricating Main/queue/pass state.
#include <cassert>
#include "../source/battle/trio_attack.cpp"
namespace ttyd {
struct TrioWeaponRuntimeLifetimeFixture {
 static void init(TrioAttack&battle){auto r=battle.runtime_;assert(r&&r->menuResourceInitialized);assert(resumePackedBattleActUiInit(Battle,Battle+0x1bb4,Battle+0x1724,0,r->originalActMenuUiServices(),r->actMenuUiCursor));}
 static void end(TrioAttack&battle){auto r=battle.runtime_;assert(r&&resumePackedBattleActUiEnd(Battle,r->originalActMenuUiServices(),r->actMenuUiCursor));}
};
}
using namespace ttyd;
namespace {
EventNativePlatform actualPlatform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float v){assert(std::isfinite(v)&&double(v)>=-2147483648.&&double(v)<2147483648.);return int32_t(v);};return p;}
std::shared_ptr<NativeUiStartup>actualStartup(){auto p=NativeFieldAnimation::create("romfs",NativeFileCache::create(),actualPlatform());assert(p&&p->ready());auto u=NativeUiStartup::create(p,"romfs");assert(u&&u->resumeFont());return u;}
struct Fixture {
 ProfessorStory scene;TrioBattleCommands menu;
 Fixture(){assert(scene.beginTrioBattleIntro(0)&&menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);assert(menu.attack);}
 void begin(TrioAttack&b){assert(b.begin(menu,scene,10));TrioWeaponRuntimeLifetimeFixture::init(b);}
};
struct Retained {
 std::shared_ptr<BattleWeaponSourceMemory>memory;TrioWeaponMenuFrameBinding frame;
 Retained(TrioAttack&battle,const std::shared_ptr<NativeUiStartup>&ui){memory=battle.weaponMenuSourceMemory(ui,"romfs/source-ui-startup/memory.pmevt");const auto f=battle.weaponMenuFrameBinding({});assert(memory&&memory->current()&&f);frame=*f;assert(frame.native.allocationCurrent()&&frame.native.sourceResourceCurrent());assert(memory==battle.weaponMenuSourceMemory(ui,"romfs/source-ui-startup/memory.pmevt"));}
 void live()const{assert(memory->current()&&frame.native.allocationCurrent()&&frame.native.sourceResourceCurrent());}
 void retired()const{assert(!memory->current()&&!frame.native.allocationCurrent()&&!frame.native.sourceResourceCurrent());assert(!frame.native.producerCurrent({}));assert(!memory->word(0x8041e978));}
};
void destructionAndRealEnd(){Fixture f;auto ui=actualStartup();std::unique_ptr<Retained>held;
 {auto owner=std::make_unique<TrioAttack>();f.begin(*owner);held=std::make_unique<Retained>(*owner,ui);held->live();owner.reset();}
 // Both adapter and frame retain actual runtime storage past facade deletion.
 // The former raw-this predicates dereferenced freed selector memory here.
 held->retired();held.reset();
 TrioAttack owner;f.begin(owner);Retained current(owner,ui);TrioWeaponRuntimeLifetimeFixture::end(owner);assert(!current.frame.native.allocationCurrent());assert(current.memory->current());owner=TrioAttack{};current.retired();
}
void moveAssign(TrioAttack&to,TrioAttack&from){to=std::move(from);}
void copiesAndAssignment(){Fixture f;auto ui=actualStartup();TrioAttack original;f.begin(original);Retained first(original,ui);
 TrioAttack copy(original);Retained second(copy,ui);assert(first.memory!=second.memory);first.live();second.live();
 original=original;moveAssign(copy,copy);first.live();second.live();
 // Same-runtime assignment keeps the destination's own genuine registration.
 copy=original;first.live();second.live();
 original=TrioAttack{};first.retired();second.live();
 TrioAttack replacement;f.begin(replacement);Retained replacementView(replacement,ui);copy=replacement;second.retired();replacementView.live();Retained adopted(copy,ui);assert(adopted.memory!=replacementView.memory);adopted.live();
 replacement=TrioAttack{};replacementView.retired();adopted.live();
 copy=TrioAttack{};adopted.retired();
}
void moves(){Fixture f;auto ui=actualStartup();TrioAttack from;f.begin(from);Retained old(from,ui);
 TrioAttack to(std::move(from));assert(!from.hasBattleRuntime()&&to.hasBattleRuntime());old.retired();Retained current(to,ui);current.live();
 TrioAttack same(to);Retained sameView(same,ui);same=std::move(to);assert(!to.hasBattleRuntime());current.retired();sameView.live();
 moveAssign(same,same);sameView.live();
 TrioAttack other;f.begin(other);Retained otherView(other,ui);other=std::move(same);sameView.retired();otherView.retired();Retained moved(other,ui);moved.live();other=TrioAttack{};moved.retired();
}
}
int main(){assert(loadProfessorStoryAssets("romfs")&&loadUndergroundStoryAssets("romfs")&&loadTrioBattleAssets("romfs")&&loadTrioAttackAssets("romfs"));destructionAndRealEnd();copiesAndAssignment();moves();return 0;}
