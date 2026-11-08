#include "platform.hpp"
#include "native_harbor_reward_presentation.hpp"
#include <cmath>
namespace ttyd {
namespace {
constexpr unsigned RewardIcon=404,PointGetNamespace=0x10000;
constexpr float UiCenterX=200,UiCenterY=120,UiVerticalScale=.5f;
// Original reward window descriptor, adapted through the shared UI transform.
constexpr float RewardWindowX=-76.8999939f,RewardWindowY=21,
 RewardWindowWidth=165.7999878f,RewardWindowHeight=24.1999989f,RewardWindowBorder=10;
constexpr unsigned PointGetTextures[]={124,125,126,127,128,129,130,131};
constexpr uint32_t RewardData[]={0x8042722c,0x80427230,0x80427234,0x80427238,
 0x8042723c,0x80427240,0x80427244,0x804271e0,0x804271c0};
bool sameOwner(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){
 return a&&b&&a==b&&!a.owner_before(b)&&!b.owner_before(a);
}
bool sameArt(const MessageIcon&a,const MessageIcon&b){
 if(a.id!=b.id||a.frames.size()!=b.frames.size()||a.loop!=b.loop)return false;
 for(size_t i=0;i<a.frames.size();++i){const auto&x=a.frames[i];const auto&y=b.frames[i];
  if(x.delay!=y.delay||x.width!=y.width||x.height!=y.height||x.u0!=y.u0||x.v0!=y.v0||x.u1!=y.u1||x.v1!=y.v1)return false;
 }return true;
}

}
struct Renderer::NativeHarborRewards:std::enable_shared_from_this<NativeHarborRewards>{
 Renderer*renderer=nullptr;bool alive=true;
 std::weak_ptr<const void>runtime;
 const HarborTutorial*facade=nullptr;NativeFacadeRegistration::Proof facadeProof;
 Tex3DS_Texture imported=nullptr,panel=nullptr;const void*pixels=nullptr,*panelPixels=nullptr;
 // Pin the real ordinary Mario bank/atlas used by both original victory
 // clips and the final type42 M_R_1. A cached tutorial flag alone is no lease.
 std::shared_ptr<const BattlePoseResource>mario;
 Tex3DS_Texture marioImported=nullptr;const void*marioPixels=nullptr;
 std::array<MessageIcon,9>art;
 std::array<uint32_t,9>data{};
 std::shared_ptr<NativeHarborRewardPresentation>model;
 struct Proof{std::weak_ptr<NativeHarborRewards>context;};
 bool current()const{
  if(!alive||!facadeProof.current()||!facade||!renderer||runtime.expired())return false;
  if(!facade->nativeGameplayFacadeProof().current()||!sameOwner(runtime.lock(),facade->nativeGameplayLifetime().lock()))return false;
  const auto&r=*renderer;
  if(!r.c3dReady||!r.c2dReady||!r.programReady||r.transformLoc<0||!r.tutorialReady||
   r.messageIconTexture.imported!=imported||r.messageIconTexture.texture.data!=pixels||!pixels||
   r.messageIconTexture.texture.width!=512||r.messageIconTexture.texture.height!=1024||
   r.messagePanel.imported!=panel||r.messagePanel.texture.data!=panelPixels||!panelPixels||!r.windowFrameVertices)return false;
  if(!mario||r.trioMarioAnimations!=mario||!mario->bank||
   r.trioMarioTexture.imported!=marioImported||!marioImported||
   r.trioMarioTexture.texture.data!=marioPixels||!marioPixels)return false;
  for(const char*pose:{"M_V_2","M_V_1","M_R_1"})if(!mario->bank->pose(pose))return false;
  for(const auto&icon:art){const auto*actual=messageIcon(icon.id);if(!actual||!sameArt(icon,*actual))return false;}
  return Tex3DS_GetSubTexture(panel,0)!=nullptr;
 }
};
bool Renderer::prepareNativeHarborRewardPresentation(Game&g){
 if(!g.harborTutorial.running()||!g.harborTutorial.initialized||!g.harborTutorial.introComplete||
  !tutorialReady||!c3dReady||!c2dReady||!programReady||!messageIconTexture.imported||!messagePanel.imported||!windowFrameVertices)return false;
 const auto runtime=g.harborTutorial.nativeGameplayLifetime().lock();const auto proof=g.harborTutorial.nativeGameplayFacadeProof();if(!runtime||!proof.current())return false;
 if(nativeHarborRewards&&!sameOwner(runtime,nativeHarborRewards->runtime.lock()))retireNativeHarborRewards();
 if(!nativeHarborRewards){
  auto c=std::make_shared<NativeHarborRewards>();c->renderer=this;c->runtime=runtime;c->facade=&g.harborTutorial;c->facadeProof=proof;
  c->imported=messageIconTexture.imported;c->pixels=messageIconTexture.texture.data;
  c->panel=messagePanel.imported;c->panelPixels=messagePanel.texture.data;
  c->mario=trioMarioAnimations;c->marioImported=trioMarioTexture.imported;c->marioPixels=trioMarioTexture.texture.data;
  for(unsigned i=0;i<c->art.size();++i){const auto*icon=messageIcon(i?PointGetNamespace+PointGetTextures[i-1]:RewardIcon);
   if(!icon||!validNativeHarborRewardArt(*icon))return false;c->art[i]=*icon;}
  for(unsigned i=0;i<c->data.size();++i)if(!g.harborTutorial.readPackedWord(RewardData[i],c->data[i]))return false;
  auto proof=std::make_shared<NativeHarborRewards::Proof>();proof->context=c;
  NativeHarborRewardPresentationSource source;source.user=proof.get();source.lifetime=proof;
  source.current=[](void*p){const auto c=static_cast<NativeHarborRewards::Proof*>(p)->context.lock();return c&&c->current();};
  source.word=[](void*p,uint32_t a)->std::optional<uint32_t>{const auto c=static_cast<NativeHarborRewards::Proof*>(p)->context.lock();
   if(!c||!c->current())return {};for(unsigned i=0;i<c->data.size();++i)if(RewardData[i]==a)return c->data[i];return {};};
  c->model=std::make_shared<NativeHarborRewardPresentation>(source);
  if(!c->current())return false;nativeHarborRewards=std::move(c);
 }
 return nativeHarborRewards->current();
}
std::shared_ptr<NativeHarborRewardPresentation>Renderer::nativeHarborRewardPresentation(const std::shared_ptr<const void>&runtime)const{
 const auto c=nativeHarborRewards;
 return c&&sameOwner(runtime,c->runtime.lock())&&c->current()&&c->model->current()?c->model:nullptr;
}
bool Renderer::drawNativeHarborRewardPresentation(const Game&g){
 const auto c=nativeHarborRewards;
 if(!c||!c->current()||!c->model->admitted()||c->model->total()<=0||!sameOwner(g.harborTutorial.nativeGameplayLifetime().lock(),c->runtime.lock()))return false;
 const auto&model=*c->model;
 const auto pointQuads=pointGetQuads(model.pointGet());
 // Whole native plan/resource preflight before any state mutation. Rendering
 // never advances source simulation, counters, event roots or original End.
 if(!model.iconsActive()&&!model.pointGet().active)return false;
 if(model.iconsActive()&&windowFrameCount>=MaxWindowFrames)return false;
 for(const auto&icon:model.icons())if(!icon.hidden){
  for(float value:icon.position)if(!std::isfinite(value))return false;
  if(!std::isfinite(icon.scale)||icon.scale<0)return false;
 }
 for(const auto&q:pointQuads){
  if(!std::isfinite(q.centerX)||!std::isfinite(q.bottomY)||!std::isfinite(q.width)||!std::isfinite(q.height)||q.width<0||q.height<0)return false;
  if(q.texture<124||q.texture>131||!messageIcon(PointGetNamespace+q.texture))return false;
 }
 C2D_Flush();C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
 if(model.iconsActive()&&!sourceWindowFrame(type0WindowFrame(RewardWindowX,RewardWindowY,RewardWindowWidth,RewardWindowHeight,RewardWindowBorder,{255,255,255,128})))return false;
 for(const auto&icon:model.icons())if(!icon.hidden)
  messageIconImage(RewardIcon,icon.animationTick,UiCenterX+icon.position[0]*MessageWindowScale,UiCenterY-icon.position[1]*UiVerticalScale,icon.scale,icon.alpha);
 for(const auto&q:pointQuads){
  const auto&frame=messageIcon(PointGetNamespace+q.texture)->frames[0];
  Tex3DS_SubTexture sub{};sub.width=frame.width;sub.height=frame.height;
  sub.left=frame.u0+(frame.u1-frame.u0)*q.u0;sub.right=frame.u0+(frame.u1-frame.u0)*q.u1;sub.top=frame.v0;sub.bottom=frame.v1;
  C2D_Image image{&messageIconTexture.texture,&sub};C2D_ImageTint tint;
  C2D_PlainImageTint(&tint,q.shadow?C2D_Color32(0,0,0,128):C2D_Color32(255,255,255,255),q.shadow?1:0);
  C2D_DrawParams params={{UiCenterX+(q.centerX-q.width*.5f)*MessageWindowScale,UiCenterY-(q.bottomY+q.height)*UiVerticalScale,q.width*MessageWindowScale,q.height*UiVerticalScale},{0,0},.7f,0};
  if(!C2D_DrawImage(image,&params,&tint))return false;
 }
 C2D_Flush();C2D_Prepare();C2D_SceneBegin(top);return true;
}
bool Renderer::nativeHarborRewardsMatch(const std::shared_ptr<const void>&runtime)const{
 return !nativeHarborRewards||sameOwner(runtime,nativeHarborRewards->runtime.lock());
}
void Renderer::retireNativeHarborRewards(){
 const auto c=std::move(nativeHarborRewards);if(!c)return;
 c->alive=false;c->model->retire();if(c3dReady)C3D_FrameSync();c->renderer=nullptr;
}
}
