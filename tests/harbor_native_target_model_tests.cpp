#include "native_harbor_gameplay.hpp"
#include "harbor_native_command_menu.hpp"
#include "harbor_tutorial_animation_selection.hpp"
#include "native_battle_sound.hpp"
#include "message_assets.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <cstdio>
using namespace ttyd;
int main(){
 assert(loadHarborTutorialAssets("romfs"));
 std::array<ActorAnimationPack,3> animation;ActorAnimationPack mario;
 const char* names[]={"crump","mario","goombella"};
 for(unsigned i=0;i<3;++i)assert(animation[i].load((std::string("romfs/harbor-tutorial-")+names[i]+".pmanim").c_str()));
 assert(mario.load("romfs/trio-attack-mario.pmanim"));
 MessageAssets messages;assert(messages.loadGlobal("romfs/messages/global.txt")&&messages.loadRoom("romfs/messages/gor_00.txt"));
 auto pouch=initializeProloguePouch("Yoshi");HarborTutorial scene;NativeHarborGameplay driver;
 assert(scene.beginFromPouch(0,pouch,4));auto sound=std::make_shared<PositionalSfxRegistry>();assert(scene.bindPublicSound(sound));
 for(uint64_t tick=1;tick<7000&&!scene.weaponTargetMenuOpen;++tick){
  BattlePadSample physical;
  if(tick%8==0&&(scene.message.active()||scene.classMenuOpen||scene.weaponMenuOpen))physical.snapshot.buttons=physical.pressed=BattleButtonA;
  assert(driver.advance(scene,tick,tick*1000/60,physical.pressed,&physical)&&!scene.failed);
  for(unsigned i=0;i<3;++i){const auto&a=scene.actors[i];const auto selected=selectHarborTutorialAnimation(i,a.pose.c_str(),animation[i],&mario);if(!selected)continue;const auto*p=selected->pose;ActorAnimationSample sample;const auto time=float(scene.now-a.poseAt)*.06f;if(p&&p->end>p->start&&p->evaluate(time,sample)&&time>=p->end)scene.animationComplete(a.id);}
  // Explicit TEST GPU receipts. Native admission remains actual FrameEnd-owned.
  if(scene.classMenuOpen)scene.classMenuPresented();
  if(scene.weaponMenuOpen)scene.weaponMenuPresented();
 }
 assert(scene.weaponTargetMenuOpen&&!scene.attackActive&&!scene.weaponTargets.empty());
 const auto model=scene.nativeCommandMenu();assert(model&&model->kind==HarborNativeMenuKind::Target&&!model->rows.empty());
 constexpr uint32_t ManualBattle=0x84000000,RegistryOffset=0x20,PartHeadOffset=0x14,DefinitionOffset=4,NameOffset=8;
 auto word=[&](uint32_t a){uint32_t value=0;assert(scene.readPackedWord(a,value));return value;};
 for(size_t i=0;i<model->rows.size();++i){const auto&row=model->rows[i];const auto&target=scene.weaponTargets[i];const auto unit=word(ManualBattle+RegistryOffset+4*uint32_t(target[0]));auto part=word(unit+PartHeadOffset);assert(part);
  if(target[1]>=0&&word(part))while(part&&word(word(part+DefinitionOffset))!=uint32_t(target[1]))part=word(part);
  assert(part);const auto definition=word(part+DefinitionOffset),name=word(part+NameOffset);assert(name&&name==word(definition+4)&&row.sourceName==name);
  std::string key;bool terminated=false;for(unsigned n=0;n<256;++n){const auto value=word((name+n)&~3u);const auto byte=uint8_t(value>>(24-8*((name+n)&3)));if(!byte){terminated=true;break;}key.push_back(char(byte));}
  assert(terminated&&!key.empty());const auto* original=messages.find(key.c_str());assert(original&&*original&&row.label==original);std::printf("actual target unit%ld part%ld key=%s label=%s\n",long(target[0]),long(target[1]),key.c_str(),row.label.c_str());
 }
 assert(scene.nativeCommandMenuCurrent(*model));
}
