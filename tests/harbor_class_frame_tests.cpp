#include "fixtures/harbor_class_frame_fixture.hpp"
#include <iostream>
int main(){
 assert(ttyd::loadHarborTutorialAssets("romfs"));HarborClassFrameFixture f;assert(f.prepare("romfs"));
 const auto w=f.binding.allocation;
 for(unsigned i=0;i<100;++i){
  if(!f.advance()){std::cerr<<"source UI held at pass "<<f.sequence<<" mode "<<f.word(w+0xc)<<"\n";return 1;}
 }
 assert(f.word(w+0xc)==1 && f.source.byte(f.source.user,w+0x14)==1);
 assert(f.pipeline.publication().batch() && !f.pipeline.publication().batch()->quads.empty());
 const auto payload=f.word(f.word(f.word(0x840163f8)+0xa0));
 assert(f.rngCalls>0 && f.word(payload)==0x0020af30 && f.word(payload+4)==97);
 // Physical A now works from genuine source readiness, without the old
 // classMenuPresented() development acknowledgement.
 const auto old=f.binding;ttyd::BattlePadSample released;
 f.scene.tick(f.scene.now+17,0,&released);assert(f.scene.inputCommand(released)==0);
 ttyd::BattlePadSample confirm;
 confirm.snapshot.buttons=confirm.pressed=0x100;
 f.scene.tick(f.scene.now+17,0,&confirm);
 const auto result=f.scene.inputCommand(confirm);
 assert(result==0 && !f.scene.classMenuOpen && f.scene.weaponMenuOpen);
 assert(!old.current() && !f.pipeline.publication().batch());
 assert(!f.pipeline.resume({old.owner,old.allocation,101},8,0x84000000));
 f.pipeline.retire();assert(f.scene.endClassMenuResources());
}
