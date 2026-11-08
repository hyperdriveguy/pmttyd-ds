// TEST-only real source UI constructor entry; no product Main/queue fixture.
#include <cassert>
#include "../source/battle/trio_attack.cpp"
namespace ttyd {
struct TrioWeaponRuntimeLifetimeFixture {
 static void init(TrioAttack&b){auto r=b.runtime_;assert(r&&r->menuResourceInitialized);assert(resumePackedBattleActUiInit(Battle,Battle+0x1bb4,Battle+0x1724,0,r->originalActMenuUiServices(),r->actMenuUiCursor));}
 static void key(TrioAttack&b,uint32_t address,const char*id){auto r=b.runtime_;std::vector<uint8_t>bytes(id,id+std::strlen(id)+1);assert(r&&r->vm.memory.registerRam(address,std::move(bytes)));}
 static uint32_t originalClassPointer(TrioAttack&b,uint32_t key){auto r=b.runtime_;assert(r&&r->originalClassMessages);const auto p=r->originalClassMessages->lookup(r->originalClassMessages->user,key);assert(p);return *p;}
 static std::string text(TrioAttack&b,uint32_t address){auto r=b.runtime_;std::string text;for(uint8_t c;;++address){assert(r&&r->vm.memory.readByte(address,c));if(!c)return text;text.push_back(char(c));}}
};
}
using namespace ttyd;
int main(){
 assert(loadProfessorStoryAssets("romfs")&&loadUndergroundStoryAssets("romfs")&&loadTrioBattleAssets("romfs")&&loadTrioAttackAssets("romfs"));
 auto messages=std::make_shared<MessageAssets>();assert(messages->loadGlobal("romfs/messages/global.txt")&&messages->loadRoom("romfs/messages/moo_00.txt"));
 ProfessorStory scene;TrioBattleCommands menu;assert(scene.beginTrioBattleIntro(0)&&menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);assert(menu.attack);
 TrioAttack original;assert(original.begin(menu,scene,10,{},nullptr,nullptr,nullptr,nullptr,{},{},{},{},{},{},{},messages));TrioWeaponRuntimeLifetimeFixture::init(original);
 constexpr uint32_t Key=0x72001000;const char*id="btl_hlp_cmd_operation_changeparty";
 TrioWeaponRuntimeLifetimeFixture::key(original,Key,id);auto first=original.weaponMenuFrameBinding({});assert(first&&first->messageLookup);const auto pointer=first->messageLookup(Key);assert(pointer&&TrioWeaponRuntimeLifetimeFixture::text(original,*pointer)==messages->find(id));
 constexpr uint32_t ClassKey=Key+0x200;TrioWeaponRuntimeLifetimeFixture::key(original,ClassKey,"btl_cmd_act_jump");assert(first->messageLookup(ClassKey)==TrioWeaponRuntimeLifetimeFixture::originalClassPointer(original,0x802f0018));
 TrioAttack copy(original);auto second=copy.weaponMenuFrameBinding({});assert(second&&second->messageLookup&&second->messageLookup(Key)==pointer);
 original=TrioAttack{};assert(!first->messageLookup(Key)&&second->messageLookup(Key)==pointer);
 constexpr uint32_t MissingKey=Key+0x100;TrioWeaponRuntimeLifetimeFixture::key(copy,MissingKey,"missing_actual_resource_id");assert(!second->messageLookup(MissingKey));
 copy=TrioAttack{};assert(!second->messageLookup(Key));return 0;
}
