#include "battle_weapon_camera_bridge.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
bool equalFloat(float a,float b){uint32_t x,y;std::memcpy(&x,&a,4);std::memcpy(&y,&b,4);return x==y;}
}
bool BattleWeaponCameraBridge::bind(std::shared_ptr<const CameraManager> bank,uint64_t generation,
 BattleActorServices actors,BattleClassCameraAddressPolicy addresses,std::function<bool(BattleClassCameraEpoch)> current){
 if(bank_||!engine_.bindCallback(bank,generation,actors,std::move(addresses),std::move(current),0x80121ce8))return false;
 bank_=std::move(bank);return true;
}
bool BattleWeaponCameraBridge::begin(BattleClassCameraEpoch epoch,const CameraMainCursor& main,
 const CameraDrawCursor& draw,const DisplayQueue& queue,uint32_t battle){
 if(stale_||!engine_.begin(epoch,main,draw,queue,battle))return false;
 if(!view_){const auto* c=bank_->camera(8);view_=BattleWeaponCameraView{c->matrices,c->viewport,c->scissor,c->projectionKind};}
 return available();
}
bool BattleWeaponCameraBridge::available()const{
 if(stale_||!view_||!engine_.available())return false;
 const auto* c=bank_->camera(8);
 if(!c||c->projectionKind!=view_->projectionKind||c->scissor!=view_->scissor){stale_=true;return false;}
 for(unsigned i=0;i<6;++i)if(!std::isfinite(c->viewport[i])||!equalFloat(c->viewport[i],view_->viewport[i])){stale_=true;return false;}
 return true;
}
bool BattleWeaponCameraBridge::finish(BattleClassCameraEpoch epoch){
 if(!available()||!engine_.finish(epoch))return false;
 view_.reset();return true;
}
void BattleWeaponCameraBridge::retire(){engine_.retire();bank_.reset();view_.reset();stale_=false;}
bool BattleWeaponCameraBridge::owns(uint32_t a)const{return engine_.owns(a);}
std::optional<uint32_t> BattleWeaponCameraBridge::camera(std::optional<uint32_t> index)const{return available()?engine_.camera(index):std::nullopt;}
std::optional<uint32_t> BattleWeaponCameraBridge::word(uint32_t a)const{return available()?engine_.word(a):std::nullopt;}
std::optional<BattleWeaponCameraView> BattleWeaponCameraBridge::view()const{return available()?view_:std::nullopt;}
std::optional<std::array<float,3>> BattleWeaponCameraBridge::ringCenter()const{return available()?engine_.ringCenter():std::nullopt;}
std::optional<std::array<float,2>> BattleWeaponCameraBridge::ringOffset(float a)const{return available()?engine_.ringOffset(a):std::nullopt;}
bool BattleWeaponCameraBridge::matrix(BattleAudienceMatrixRequest& q)const{return available()&&engine_.matrix(q)&&available();}
}
