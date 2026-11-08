#include "native_harbor_reward_presentation.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <map>
using namespace ttyd;
namespace {
struct Fixture {
 EventScriptAssets assets;bool alive=true;uint32_t held=0;unsigned holds=0;
 std::map<uint32_t,unsigned>reads;
 NativeHarborRewardPresentationSource source(){NativeHarborRewardPresentationSource s;
  s.user=this;s.lifetime=std::make_shared<int>(1);
  s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);++f.reads[a];
   if(a==f.held&&f.holds){--f.holds;return {};}
   uint32_t value;if(!f.assets.readWord(a,value))return {};return value;};return s;
 }
};
}
int main(){
 assert(loadMessageIcons("romfs/message-icons.bin"));
 for(unsigned id:{404u,0x10000u+124,0x10000u+125,0x10000u+126,0x10000u+127,0x10000u+128,0x10000u+129,0x10000u+130,0x10000u+131}){
  const auto*icon=messageIcon(id);assert(icon&&validNativeHarborRewardArt(*icon));
  auto bad=*icon;++bad.frames[0].width;assert(!validNativeHarborRewardArt(bad));
 }

 for(uint32_t address:{0x8042722cu,0x80427230u,0x80427234u,0x80427238u,0x8042723cu,0x80427240u,0x80427244u,0x804271e0u,0x804271c0u}){
  Fixture held;assert(held.assets.load("romfs/harbor-tutorial-events.pmevt"));held.held=address;held.holds=2;
  NativeHarborRewardPresentation waiting(held.source());assert(!waiting.admit(9)&&!waiting.admit(9)&&waiting.admit(9));
  assert(held.reads[address]==3);for(const auto&read:held.reads)if(read.first!=address)assert(read.second==1);
 }
 Fixture f;assert(f.assets.load("romfs/harbor-tutorial-events.pmevt"));
 f.held=0x80427238;f.holds=2;
 NativeHarborRewardPresentation model(f.source());
 assert(!model.admit(29));assert(!model.admit(29));assert(!model.admit(30));assert(model.admit(29));
 assert(f.reads[0x8042722c]==1&&f.reads[0x80427238]==3);
 assert(model.admitted()&&!model.pointGetReady()&&!model.pointGetFinished());
 assert(model.enterPointGet()&&model.enterPointGet());
 assert(model.pointGet().amount==29&&model.pointGet().duration==0&&model.pointGet().x==-2000&&model.pointGet().y==60);
 assert(model.update(29,1));unsigned visible=0;
 for(const auto&i:model.icons())if(!i.hidden){++visible;assert(i.alpha==64);}
 assert(visible==11); // two tens plus nine ones.
 const auto timer=model.pointGet().timer;assert(model.update(29,1)&&model.pointGet().timer==timer);
 assert(!model.update(28,1)&&!model.update(30,2)&&!model.update(29,0));
 assert(model.update(20,2));visible=0;for(const auto&i:model.icons())if(!i.hidden){++visible;assert(i.alpha==128);}assert(visible==2);
 assert(model.icons()[18].animationTick==2&&model.icons()[18].hidden&&model.icons()[18].alpha==0);
 for(uint64_t tick=3;tick<=45;++tick)assert(model.update(20,tick));
 assert(model.pointGetReady()&&model.pointGet().phase==1);
 const auto iconClock=model.icons()[0].animationTick;assert(model.hideIcons()&&!model.iconsActive());
 assert(model.softDeletePointGet()&&model.update(0,46)&&model.pointGet().phase==2);
 for(uint64_t tick=47;tick<=106;++tick)assert(model.update(0,tick));
 assert(model.pointGetFinished()&&!model.pointGetReady()&&model.icons()[0].animationTick==iconClock);
 f.alive=false;assert(!model.current()&&!model.update(0,107)&&!model.softDeletePointGet());
 assert(model.retire()&&!model.admit(29));
 Fixture zero;assert(zero.assets.load("romfs/harbor-tutorial-events.pmevt"));NativeHarborRewardPresentation z(zero.source());
 assert(!z.admit(-1)&&!z.admit(32768)&&z.admit(0)&&z.update(0,0));
 for(const auto&i:z.icons())assert(i.hidden&&i.animationTick==0);
 assert(!z.iconsActive()&&!z.enterPointGet());
}
