#include "platform.hpp"
#include "camera_road_view.hpp"
#include "message_icons.hpp"
namespace ttyd {
bool Renderer::prepareFieldItemGetResources() {
 if(!itemGetGame || !itemGetAudio || itemGetGame->progress.room!=0 || !itemGetAudio->fieldMarioPostReady(itemGetGame->fieldMarioMotion()))return false;
 if(!itemGetAssetsAttempted){itemGetAssetsAttempted=true;itemGetAssetsReady=loadItemGetWindowAssets("romfs:");}
 if(!itemGetAssetsReady || !messageIconTexture.imported || !fieldItemGetAnimations.pose("M_I_U") ||
    !prepareItemGetEffect())return false;
 if(!fieldItemGetTexture.imported) {
  C3D_FrameSync();FILE* file=std::fopen("romfs:/field-item-get-mario.t3x","rb");if(!file)return false;
  fieldItemGetTexture.imported=Tex3DS_TextureImportStdio(file,&fieldItemGetTexture.texture,nullptr,false);std::fclose(file);
  if(!fieldItemGetTexture.imported)return false;
  C3D_TexSetFilter(&fieldItemGetTexture.texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&fieldItemGetTexture.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
 }
 return true;
}
bool Renderer::prepareFieldItemGet() {
 if(!prepareFieldItemGetResources() || !itemGetAudio->prepareFieldItemGetAudio())return false;
 if(!itemGetGame->progress.originalPouch)return false;
 bool freeKeySlot=false;for(unsigned slot=0;slot<121;++slot)freeKeySlot|=itemGetGame->progress.originalPouch->half(0xa0+slot*2)==0;
 if(!freeKeySlot)return false; // Original full-storage/discard native provider remains unbound.
 if(!itemGetGame->acquisitionWindows) {
  auto windows=std::make_shared<ItemGetWindows>();if(!windows->prepare(harborMessageFont()))return false;
  itemGetGame->acquisitionWindows=std::move(windows);
 }
 return !itemGetGame->acquisitionWindows->retained() &&
     (!itemGetGame->coinWindows || !itemGetGame->coinWindows->retained()) && !itemGetGame->acquisitionEffect.active();
}
bool Renderer::prepareFieldCoinGet() {
 if(!prepareFieldItemGetResources() || !itemGetAudio->prepareFieldCoinGetAudio() || !itemGetGame->progress.originalPouch || !itemGetGame->fieldItemCamera5Frame)return false;
 const auto* coin=messageIcon(403);
 if(!coin || coin->frames.size()!=4 || (itemGetGame->acquisitionWindows && itemGetGame->acquisitionWindows->retained()))return false;
 bool freeIcon=false;
 for(const auto& icon:itemGetGame->fieldItems().icons())freeIcon|=!(icon.flags&1);
 if(!freeIcon)return false;
 if(!itemGetGame->coinWindows) {
  auto window=std::make_shared<CoinGetWindow>();if(!window->prepare(harborMessageFont()))return false;
  itemGetGame->coinWindows=std::move(window);
 }
 return !itemGetGame->coinWindows->retained() && !itemGetGame->acquisitionEffect.active();
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
 FieldCoinGetServices coin;coin.user=this;
 coin.ready=[](void* p){return static_cast<Renderer*>(p)->prepareFieldCoinGet();};
 coin.mario=s.mario;
 coin.heading=[](void* p){const auto& f=static_cast<Renderer*>(p)->itemGetGame->fieldCameraFrame;
  return fieldNpcAngles().angle(f.eye.x,f.eye.z,f.at.x,f.at.z);};
 coin.clock=[](void*){const uint64_t raw=uint64_t(svcGetSystemTick());
  // Native raw-tick adapter preserves source divisor units. GP38 admitted-
  // delta publication/freeze is not yet bound to the original event globals.
  return FieldCoinClock{uint32_t(raw>>32),uint32_t(raw),uint32_t(4ull*SYSCLOCK_ARM11)};};
 coin.effect=[](void* p,int32_t kind,ItemGetPoint xyz){auto& g=*static_cast<Renderer*>(p)->itemGetGame;
  if(kind!=7)return uintptr_t(0);
  g.acquisitionEffect.beginCoin({xyz.x,xyz.y,xyz.z});return uintptr_t(1);};
 coin.effectPayload=[](void* p,uintptr_t handle,int32_t camera){if(handle==1)static_cast<Renderer*>(p)->itemGetGame->acquisitionEffect.camera(camera);};
 coin.softDelete=[](void* p,uintptr_t handle){if(handle==1)static_cast<Renderer*>(p)->itemGetGame->acquisitionEffect.softDelete();};
 coin.iconEnter=[](void* p,const char* name,uint32_t descriptor,ItemGetPoint xyz){if(descriptor==403)static_cast<Renderer*>(p)->itemGetGame->fieldItems().enterCoin403(name,xyz);};
 coin.iconFlags=[](void* p,const char* name,uint16_t on,uint16_t off){static_cast<Renderer*>(p)->itemGetGame->fieldItems().iconFlags(name,on,off);};
 coin.iconDelete=[](void* p,const char* name){static_cast<Renderer*>(p)->itemGetGame->fieldItems().deleteIcon(name);};
 coin.request=[](void* p,FieldCoinRequest request){auto& r=*static_cast<Renderer*>(p);auto& g=*r.itemGetGame;auto& a=*r.itemGetAudio;
  switch(request) {
  case FieldCoinRequest::GetMotion:g.applyFieldItemMarioRequest(ItemGetRequest::GetItemMotion);break;
  case FieldCoinRequest::StayMotion:g.applyFieldItemMarioRequest(ItemGetRequest::StayMotion);break;
  case FieldCoinRequest::BgmOff3800:a.retainItemGetFieldMusic(70,100,false);a.muteItemGetFieldMusic(TickRate);break;
  case FieldCoinRequest::FanfareOn201:a.beginCoinFanfare(TickRate);break;
  case FieldCoinRequest::BgmResumeA0:a.resumeItemGetFieldMusic(TickRate);break;
  case FieldCoinRequest::BgmOff201:a.fadeKeyItemFanfare(1000,TickRate);break;
  }
 };
 coin.window=[](void* p,uint32_t definition,const int32_t* parameter){return static_cast<Renderer*>(p)->itemGetGame->coinWindows->enter(definition,parameter);};
 coin.windowClose=[](void* p,int32_t slot){static_cast<Renderer*>(p)->itemGetGame->coinWindows->closeAutoDelete(slot);};
 coin.windowAction=[](void* p,int32_t slot){return static_cast<Renderer*>(p)->itemGetGame->coinWindows->action(slot);};
 coin.windowRetained=[](void* p,int32_t slot){return slot==0 && static_cast<Renderer*>(p)->itemGetGame->coinWindows->retained();};
 coin.addCoins=[](void* p,int32_t amount){return static_cast<Renderer*>(p)->itemGetGame->progress.originalPouch->addCoins(amount);};
 game.bindFieldCoinGetPresentation(coin);

}
void Renderer::fieldItemGetRequest(ItemGetRequest r) {
 auto& game=*itemGetGame;auto& audio=*itemGetAudio;
 if(game.applyFieldItemStateRequest(r))return;
 auto* icon=game.fieldItems().findIcon("irai_item");
 switch(r) {
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
 if(game.coinWindows) {
  ItemSelectionSequenceServices seq{nullptr,[](void*){return int32_t(TickRate);},[](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return cameraInterpolation(mode,step,count,a,b).value_or(a);}};
  game.coinWindows->managerTick(seq);game.retireFieldCoinWindow();
 }
 game.acquisitionEffect.tick();
}
void Renderer::advanceFieldIcons() {
 if(!itemGetGame || itemGetGame->progress.room!=0 || itemGetGame->scene==Scene::Battle)return;
 const auto* coin=messageIcon(403);
 itemGetGame->fieldItems().advanceIcons(messageIconTexture.imported && coin && coin->frames.size()==4);
}
void Renderer::drawFieldItemGet(const Game& game) {
 if(game.progress.room!=0 || game.scene==Scene::Battle)return;
 const auto matrix=[](const FieldCameraFrame& frame){C3D_Mtx projection,view,out;
  Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(frame.fov),C3D_AspectRatioTop,frame.nearPlane,frame.farPlane,false);
  Mtx_LookAt(&view,FVec3_New(frame.eye.x,frame.eye.y,frame.eye.z),FVec3_New(frame.at.x,frame.at.y,frame.at.z),FVec3_New(0,1,0),false);
  Mtx_Multiply(&out,&projection,&view);return out;};
 const auto& frame=game.fieldItemCamera5Frame;
 if(game.acquisitionEffect.active()) {
  const auto* camera=game.acquisitionEffect.camera()==5 && frame?&*frame:
      game.acquisitionEffect.camera()==4?&game.fieldCameraFrame:nullptr;
  if(camera)itemGetEffect(game.acquisitionEffect,game.acquisitionEffect.camera(),matrix(*camera),camera->viewport,camera->yaw,0x000000ff);
 }
 if(frame)for(const auto& icon:game.fieldItems().icons())if(icon.flags&1) {
  if(icon.id==403)coin403Icon(icon,5,matrix(*frame),frame->viewport,frame->yaw);
  else item95Icon(icon,5,matrix(*frame),frame->viewport,frame->yaw,uint32_t(game.ticks));
 }
}
}
