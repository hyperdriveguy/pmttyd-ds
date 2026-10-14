#include "platform.hpp"
#include "camera_road_view.hpp"
#include "message_icons.hpp"
namespace ttyd {
bool Renderer::prepareFieldItemGet() {
 if(!itemGetGame || !itemGetAudio || itemGetGame->progress.room!=0 || !itemGetAudio->fieldMarioPostReady(itemGetGame->fieldMarioMotion()))return false;
 if(!itemGetAssetsAttempted){itemGetAssetsAttempted=true;itemGetAssetsReady=loadItemGetWindowAssets("romfs:");}
 if(!itemGetAssetsReady || !messageIconTexture.imported || !fieldItemGetAnimations.pose("M_I_U") ||
    !itemGetAudio->prepareFieldItemGetAudio() || !prepareItemGetEffect())return false;
 if(!itemGetGame->progress.originalPouch)return false;
 bool freeKeySlot=false;for(unsigned slot=0;slot<121;++slot)freeKeySlot|=itemGetGame->progress.originalPouch->half(0xa0+slot*2)==0;
 if(!freeKeySlot)return false; // Original full-storage/discard native provider remains unbound.
 if(!fieldItemGetTexture.imported) {
  C3D_FrameSync();FILE* file=std::fopen("romfs:/field-item-get-mario.t3x","rb");if(!file)return false;
  fieldItemGetTexture.imported=Tex3DS_TextureImportStdio(file,&fieldItemGetTexture.texture,nullptr,false);std::fclose(file);
  if(!fieldItemGetTexture.imported)return false;
  C3D_TexSetFilter(&fieldItemGetTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&fieldItemGetTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
 }
 if(!itemGetGame->acquisitionWindows) {
  auto windows=std::make_shared<ItemGetWindows>();if(!windows->prepare(harborMessageFont()))return false;
  itemGetGame->acquisitionWindows=std::move(windows);
 }
 return !itemGetGame->acquisitionWindows->retained() && !itemGetGame->acquisitionEffect.active();
}
void Renderer::bindFieldItemGet(Game& game,Audio& audio) {
 itemGetGame=&game;itemGetAudio=&audio;audio.muteFieldItemPresentation(!game.progress.sound);
 FieldItemGetServices s;s.user=this;
 s.ready=[](void* p){return static_cast<Renderer*>(p)->prepareFieldItemGet();};
 s.mario=[](void* p){return static_cast<Renderer*>(p)->itemGetGame->fieldItemMarioPosition();};
 s.request=[](void* p,ItemGetRequest r){static_cast<Renderer*>(p)->fieldItemGetRequest(r);};
 s.effect=[](void* p,int32_t type,ItemGetPoint xyz){auto& renderer=*static_cast<Renderer*>(p);if(type!=6)return uintptr_t(0);
  renderer.itemGetGame->acquisitionEffect.begin({xyz.x,xyz.y,xyz.z});return uintptr_t(1);};
 s.window=[](void* p,uint32_t definition,const char* name){auto& renderer=*static_cast<Renderer*>(p);return renderer.itemGetGame->acquisitionWindows->enter(definition,name);};
 s.windowAction=[](void* p,int32_t slot){return static_cast<Renderer*>(p)->itemGetGame->acquisitionWindows->action(slot);};
 s.bgmSilent=[](void* p){return static_cast<Renderer*>(p)->itemGetAudio->itemGetFieldMusicSilent();};
 s.bgmCheck=[](void* p,int32_t group){return group==1?static_cast<Renderer*>(p)->itemGetAudio->keyItemFanfareCheck():0;};
 game.bindFieldItemGetPresentation(s);
}
void Renderer::fieldItemGetRequest(ItemGetRequest r) {
 auto& game=*itemGetGame;auto& audio=*itemGetAudio;
 if(game.applyFieldItemStateRequest(r))return;
 auto* icon=game.fieldItems().findIcon("irai_item");
 switch(r) {
 case ItemGetRequest::EffectPayload5:break; // Finite effect6 owner already models payload kind5.
 case ItemGetRequest::IconFlag100On:if(icon)icon->flags|=0x100;break;
 case ItemGetRequest::IconFlag100Off:if(icon)icon->flags&=~uint16_t(0x100);break;
 case ItemGetRequest::IconFlag80On:if(icon)icon->flags|=0x80;break;
 case ItemGetRequest::IconFlag80Off:if(icon)icon->flags&=~uint16_t(0x80);break;
 case ItemGetRequest::IconFlag2On:if(icon)icon->flags|=2;break;
 case ItemGetRequest::BgmOff3800:
  audio.retainItemGetFieldMusic(70,100,false);audio.muteItemGetFieldMusic(TickRate);break;
 case ItemGetRequest::KeyItemFanfare:audio.beginKeyItemFanfare(TickRate);break;
 case ItemGetRequest::BgmOff201:audio.fadeKeyItemFanfare(1000,TickRate);break;
 case ItemGetRequest::BgmResumeA0:audio.resumeItemGetFieldMusic(TickRate);break;
 case ItemGetRequest::WindowCloseMain:game.acquisitionWindows->closeAutoDelete(game.fieldAcquisition().get().mainWindow());break;
 case ItemGetRequest::WindowCloseSecondary:game.acquisitionWindows->closeAutoDelete(game.fieldAcquisition().get().secondaryWindow());break;
 case ItemGetRequest::EffectSoftDelete:game.acquisitionEffect.softDelete();break;
 case ItemGetRequest::WindowCloseFailure:break; // Full-storage native UI remains a frontier.
 default:break;
 }
}
void Renderer::advanceFieldItemPresentation() {
 if(!itemGetGame)return;
 auto& game=*itemGetGame;
 if(game.acquisitionWindows) {
  ItemSelectionSequenceServices seq{nullptr,[](void*){return int32_t(TickRate);},[](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return cameraInterpolation(mode,step,count,a,b).value_or(a);}};
  game.acquisitionWindows->managerTick(game.physicalChannel().repeat,seq);
 }
 game.acquisitionEffect.tick();
}
void Renderer::drawFieldItemGet(const Game& game) {
 if(game.progress.room!=0 || game.scene==Scene::Battle)return;
 const auto matrix=[](const FieldCameraFrame& frame){C3D_Mtx projection,view,out;
  Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(frame.fov),C3D_AspectRatioTop,frame.nearPlane,frame.farPlane,false);
  Mtx_LookAt(&view,FVec3_New(frame.eye.x,frame.eye.y,frame.eye.z),FVec3_New(frame.at.x,frame.at.y,frame.at.z),FVec3_New(0,1,0),false);
  Mtx_Multiply(&out,&projection,&view);return out;};
 if(game.acquisitionEffect.active())itemGetEffect(game.acquisitionEffect,4,matrix(game.fieldCameraFrame),game.fieldCameraFrame.viewport,game.fieldCameraFrame.yaw,0x000000ff);
 const auto frame=itemGetCamera5Frame(game.fieldCameraFrame.eye,{0,1,0},game.fieldCameraFrame.at);
 if(frame)for(const auto& icon:game.fieldItems().icons())if(icon.flags&1)item95Icon(icon,5,matrix(*frame),frame->viewport,frame->yaw,uint32_t(game.ticks));
}
}
