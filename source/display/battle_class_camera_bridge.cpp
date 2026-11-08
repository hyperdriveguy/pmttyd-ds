#include "battle_class_camera_bridge.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// Native virtual reservation: thirteen camera slots, 0x400 bytes each. This
// stride is adapter policy, not a recovered original Camera struct size. Fixed
// source callbacks are class8012210c, HP8011fde0 and weapon80121ce8 below.
constexpr uint32_t Stride=0x400,Bytes=13*Stride,ClassCallback=0x8012210c,HpCallback=0x8011fde0;
bool same(BattleClassCameraEpoch a,BattleClassCameraEpoch b){return a.generation==b.generation && a.pass==b.pass;}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
bool finite(const CameraDepthPoint& p){for(float f:p)if(!std::isfinite(f))return false;return true;}
bool finite(const CameraDepthSnapshot& s){
  for(const auto& r:s.view)for(float f:r)if(!std::isfinite(f))return false;
  for(const auto& r:s.projection)for(float f:r)if(!std::isfinite(f))return false;
  return true;
}
bool equal(const CameraDepthSnapshot& a,const CameraDepthSnapshot& b){
  for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c)if(bits(a.view[r][c])!=bits(b.view[r][c]))return false;
  for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c)if(bits(a.projection[r][c])!=bits(b.projection[r][c]))return false;
  return true;
}
}
bool BattleClassCameraBridge::bind(std::shared_ptr<const CameraManager> bank,
    uint64_t generation,BattleActorServices actors,BattleClassCameraAddressPolicy addresses,
    std::function<bool(BattleClassCameraEpoch)> current){
  return bindCallback(std::move(bank),generation,actors,std::move(addresses),std::move(current),ClassCallback);
}
bool BattleClassCameraBridge::bindCallback(std::shared_ptr<const CameraManager> bank,
    uint64_t generation,BattleActorServices actors,BattleClassCameraAddressPolicy addresses,
    std::function<bool(BattleClassCameraEpoch)> current,uint32_t callback){
  if(callback!=ClassCallback && callback!=HpCallback && callback!=0x80121ce8)return false;
  if(bank_ || !bank || !generation || !actors.memory.word || !current ||
      !addresses.base || addresses.base%4 || uint64_t(addresses.base)+Bytes>0x100000000ULL ||
      !addresses.unmapped || !addresses.unmapped(addresses.base,Bytes))return false;
  callback_=callback;bank_=std::move(bank);generation_=generation;actors_=actors;
  addresses_=std::move(addresses);current_=std::move(current);return true;
}
// Main index13/inactive means the thirteen-camera producer finished. Draw
// stage8/index8 must be inside the actual Camera8 queue invocation, with both
// retained dispatcher and current queue record matching callback/userdata.
// resetEpoch prevents reset/repopulation from reviving a borrowed invocation.
bool BattleClassCameraBridge::dispatchCurrent() const {
  if(!bank_ || !main_ || !draw_ || !queue_ || !queueEpoch_ || queue_->resetEpoch()!=queueEpoch_ || main_->active || main_->index!=13 ||
      !draw_->active || draw_->stage!=8 || draw_->index!=8 || bank_->currentIndex()!=8)return false;
  const auto& d=draw_->display;const auto* q=queue_->current();
  return d.active && d.invoking && d.camera==8 && d.callback==callback_ &&
      d.userdata==battle_ && q && q->camera==8 && q->callback==callback_ && q->userdata==battle_;
}
bool BattleClassCameraBridge::begin(BattleClassCameraEpoch epoch,
    const CameraMainCursor& main,const CameraDrawCursor& draw,const DisplayQueue& queue,uint32_t battle){
  if(!bank_ || invalid_ || !epoch.pass || epoch.generation!=generation_)return false;
  // A held pass pins cursor/queue object identities and battle userdata.
  if(held_)return same(epoch_,epoch) && main_==&main && draw_==&draw && queue_==&queue && battle_==battle && available();
  if(epoch.pass<=lastPass_ || !current_(epoch) || !addresses_.unmapped(addresses_.base,Bytes))return false;
  epoch_=epoch;main_=&main;draw_=&draw;queue_=&queue;queueEpoch_=queue.resetEpoch();battle_=battle;
  if(!dispatchCurrent()){main_=nullptr;draw_=nullptr;queue_=nullptr;return false;}
  // Only world Camera4 and UI Camera8 are exposed; copy actual published
  // view/projection/yaw once, then compare their bits at every provider boundary.
  for(unsigned i=0;i<2;++i){
    const auto* c=bank_->camera(i?8:4);
    if(!c || !c->viewPublished || !finite(c->matrices) || !std::isfinite(c->yaw)){
      main_=nullptr;draw_=nullptr;queue_=nullptr;return false;
    }
    snapshots_[i]=c->matrices;yaw_[i]=c->yaw;
  }
  held_=true;lastPass_=epoch.pass;return available();
}
// Loss while held is sticky even if old numeric values are later restored.
// The whole token range must still be genuinely unmapped, including file caches.
bool BattleClassCameraBridge::available() const {
  if(!held_ || invalid_)return false;
  if(!current_(epoch_) || !addresses_.unmapped(addresses_.base,Bytes) || !dispatchCurrent()){
    invalid_=true;return false;
  }
  for(unsigned i=0;i<2;++i){
    const auto* c=bank_->camera(i?8:4);
    if(!c || !c->viewPublished || !equal(c->matrices,snapshots_[i]) || bits(c->yaw)!=bits(yaw_[i])){
      invalid_=true;return false;
    }
  }
  return true;
}
// Finish the short camera lease only after whole source completion, before
// Draw advances. Long copied publication proofs must not depend on this lease.
bool BattleClassCameraBridge::finish(BattleClassCameraEpoch epoch){
  if(!held_ || !same(epoch,epoch_) || !available())return false;
  held_=false;main_=nullptr;draw_=nullptr;queue_=nullptr;return true;
}
void BattleClassCameraBridge::retire(){
  bank_.reset();actors_={};addresses_={};current_={};generation_=lastPass_=0;
  epoch_={};battle_=0;callback_=ClassCallback;queueEpoch_=0;main_=nullptr;draw_=nullptr;queue_=nullptr;held_=invalid_=false;
}
// Reservation ownership survives unavailability; composed readers must never
// fall through inside it. retire() is the explicit metadata-only range release.
bool BattleClassCameraBridge::owns(uint32_t a) const {
  return bank_ && a>=addresses_.base && uint64_t(a)-addresses_.base<Bytes;
}
std::optional<uint32_t> BattleClassCameraBridge::camera(std::optional<uint32_t> index) const {
  if(!available())return {};
  const auto selected=index?index:bank_->currentIndex();
  if(!selected || (*selected!=4 && *selected!=8))return {};
  return addresses_.base+Stride*(*selected);
}
std::optional<uint32_t> BattleClassCameraBridge::word(uint32_t a) const {
  if(!owns(a) || !available())return {};
  const uint32_t index=(a-addresses_.base)/Stride,offset=(a-addresses_.base)%Stride;
  if((index!=4 && index!=8) || offset%4)return {};
  const unsigned i=index==8;
  // Original field offsets: yaw +0x114; affine view +0x11c..0x14b
  // (12 words, 3x4); projection +0x15c..0x19b (16 words, 4x4).
  if(offset==0x114)return bits(yaw_[i]);
  if(offset>=0x11c && offset<0x14c){const unsigned n=(offset-0x11c)/4;return bits(snapshots_[i].view[n/4][n%4]);}
  if(offset>=0x15c && offset<0x19c){const unsigned n=(offset-0x15c)/4;return bits(snapshots_[i].projection[n/4][n%4]);}
  return {};
}
bool BattleClassCameraBridge::transform(uint32_t address,const BattleScreenPoint& in,
    BattleScreenPoint& out,bool project) const {
  if(!available() || !finite(in))return false;
  for(unsigned i=0;i<2;++i)if(address==addresses_.base+Stride*(i?8:4)+(project?0x15c:0x11c)){
    const auto result=project?cameraDepthProject(snapshots_[i],in):cameraDepthView(snapshots_[i],in);
    if(!finite(result) || !available())return false;
    out=result;return true;
  }
  return false;
}
// Reuse the original ScreenPoint/ring helpers with finite native SDK math.
// Source positions stay in source units; this supplies no fixture scale/lens.
BattleScreenPointServices BattleClassCameraBridge::screenServices() const {
  BattleScreenPointServices s;s.memory=actors_.memory;s.user=const_cast<BattleClassCameraBridge*>(this);
  s.camera=[](void* u,uint32_t id){return static_cast<BattleClassCameraBridge*>(u)->camera(id);};
  s.view=[](void* u,uint32_t a,const BattleScreenPoint& in,BattleScreenPoint& out){return static_cast<BattleClassCameraBridge*>(u)->transform(a,in,out,false);};
  s.project=[](void* u,uint32_t a,const BattleScreenPoint& in,BattleScreenPoint& out){return static_cast<BattleClassCameraBridge*>(u)->transform(a,in,out,true);};
  return s;
}
std::optional<BattleScreenPoint> BattleClassCameraBridge::screenPoint(uint32_t position) const {
  if(!available())return {};
  BattleScreenPoint out;return packedBattleScreenPoint(position,out,screenServices()) && finite(out) && available()?std::optional<BattleScreenPoint>(out):std::nullopt;
}
std::optional<BattleScreenPoint> BattleClassCameraBridge::screenPointCopied(const BattleScreenPoint& position) const {
  if(!available())return {};
  BattleScreenPoint out;return packedBattleScreenPoint(position,out,screenServices()) && finite(out) && available()?std::optional<BattleScreenPoint>(out):std::nullopt;
}
std::optional<std::array<float,3>> BattleClassCameraBridge::ringCenter() const {
  if(!available())return {};
  BattleActRingPositionServices s;s.memory=actors_;s.user=const_cast<BattleClassCameraBridge*>(this);
  s.project=[](void* u,uint32_t a){return static_cast<BattleClassCameraBridge*>(u)->screenPoint(a);};
  std::array<float,3> out;return packedBattleActRingCenter(out,s) && finite(out) && available()?std::optional<std::array<float,3>>(out):std::nullopt;
}
std::optional<std::array<float,2>> BattleClassCameraBridge::ringOffset(float angle) const {
  if(!available() || !std::isfinite(angle))return {};
  BattleActRingPositionServices s;s.memory=actors_;
  // Host/native libm is an admitted finite numeric policy, not measured PPC
  // libm/FPSCR parity. Original ring helper owns angle conversion/constants.
  s.cosine=[](void*,double a)->std::optional<double>{return std::isfinite(a)?std::optional<double>(std::cos(a)):std::nullopt;};
  s.sine=[](void*,double a)->std::optional<double>{return std::isfinite(a)?std::optional<double>(std::sin(a)):std::nullopt;};
  std::array<float,2> out;
  if(!packedBattleActRingOffset(angle,out,s) || !available())return {};
  for(float f:out)if(!std::isfinite(f))return {};
  return out;
}
bool BattleClassCameraBridge::matrix(BattleAudienceMatrixRequest& q) const {
  if(!available())return false;
  BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);
  auto retained=q;
  if(!math.matrix(nullptr,retained) || !available())return false;
  q.output=retained.output;return true;
}
}
