#include "fixtures/harbor_class_footer_fixture.hpp"
#include "battle_display_footer.hpp"
int main(){
 assert(ttyd::loadHarborTutorialAssets("romfs"));
 ttyd::BattleClassFooterGpuResource resource;
 ttyd::BattleClassFooterTextureServices import;
 import.user=&resource;
 import.import=[](void* p,const char*)->std::optional<ttyd::BattleClassFooterTextureAdmission>{return ttyd::BattleClassFooterTextureAdmission{p,512,256,0};};
 import.release=[](void*,const void*){};
 // Host admission is an explicit test GPU provider. Native proof must instead
 // supply Renderer::classFooterResources() and actual GPU draw admission.
 assert(resource.load("romfs",import));
 HarborClassFrameFixture f;assert(f.prepare("romfs"));
 assert(f.scene.bindCommandDisplayPalette(std::make_shared<ttyd::BattleCommandPalette>()));
 HarborClassFooterFixture footer(f,resource);footer.marioHp(5);
 bool held=false;
 for(unsigned i=0;i<100;++i){if(!f.advance()){held=true;break;}}
 // HP5 emits the original pinch cue without a party-change sound request.
 assert(!held);
 const auto gp=f.word(0x80415fd0),language=f.word(gp+0x16c);
 const auto danger=f.memory.memory.half(f.memory.memory.user,0x80416d24+2*language);
 auto check=[&](const ttyd::BattleClassBackend& batch,uint32_t index,uint32_t color){
  unsigned cues=0;
  for(const auto& q:batch.quads)if(q.texture==ttyd::BattleClassTexture::Footer){
   assert(q.footer && q.footer->index==index);
   for(const auto& v:q.footer->vertices)assert(v.rgba==color);
   ++cues;
  }
  assert(cues==1 && ttyd::validateBattleClassFooterBatch(batch,&resource));
 };
 check(*f.pipeline.publication().batch(),danger,0xffe940ff);
 footer.marioHp(1);assert(f.advance());
 const auto critical=f.memory.memory.half(f.memory.memory.user,0x80416d30+2*language);
 const auto batch=*f.pipeline.publication().batch();
 check(batch,critical,f.word(gp+0x1c)%50>24?0xe9522fff:0xffe940ff);
 f.pipeline.retire();assert(f.scene.endClassMenuResources());resource.retire();
 assert(!ttyd::validateBattleClassFooterBatch(batch,&resource));
}
