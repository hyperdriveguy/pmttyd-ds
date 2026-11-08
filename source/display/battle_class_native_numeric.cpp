#include "battle_class_native_numeric.hpp"
#include "field_camera_events.hpp"
#include <cmath>
#include <cstdlib>
namespace ttyd {
BattleClassNativeNumeric::BattleClassNativeNumeric(BattleClassNativeFrame& frame,
    BattleActorServices actors,uint32_t base,
    std::function<std::optional<uint32_t>(uint32_t)> word,std::function<bool()> current,
    BattleClassNativeNumericFooters footers)
  :frame_(frame),actors_(actors),footers_(footers),base_(base),sourceWord_(std::move(word)),current_(std::move(current)) {}
// Match both ends of the complete 13*0x400 native token reservation, using
// wider arithmetic so a range crossing 2^32 cannot wrap into guest memory.
bool BattleClassNativeNumeric::current() const {
  if(!base_ || uint64_t(base_)+13*0x400>0x100000000ULL || !current_ || !current_())return false;
  const auto* camera=frame_.camera();
  return camera && camera->owns(base_) && camera->owns(uint32_t(uint64_t(base_)+13*0x400-1));
}
bool BattleClassNativeNumeric::owns(uint32_t at) const {
  return base_ && uint64_t(at)>=base_ && uint64_t(at)<uint64_t(base_)+13*0x400;
}
std::optional<uint32_t> BattleClassNativeNumeric::word(uint32_t at) const {
  if(!current())return {};
  // The complete virtual reservation is owned, including undefined fields.
  // No unsupported camera word falls through to an actor/source reader.
  const auto value=owns(at)?frame_.camera()->word(at):sourceWord_?sourceWord_(at):std::nullopt;
  return current()?value:std::nullopt;
}
std::optional<uint32_t> BattleClassNativeNumeric::camera(std::optional<uint32_t> index) const {
  if(!current())return {};
  const auto result=frame_.camera()->camera(index);
  return current()?result:std::nullopt;
}
std::optional<std::array<float,3>> BattleClassNativeNumeric::project(uint32_t position) const {
  if(!current())return {};
  const auto result=frame_.camera()->screenPoint(position);
  return current()?result:std::nullopt;
}
// Publish only output from a completed SDK call under the same live epoch.
bool BattleClassNativeNumeric::matrix(BattleAudienceMatrixRequest& request) const {
  if(!current())return false;
  auto retained=request;
  if(!frame_.camera()->matrix(retained) || !current())return false;
  request.output=retained.output;return true;
}
// Raster Camera8 admission snapshots all twelve affine view words at +0x11c.
// No total-memory camera token escapes after a partial or stale optional read.
std::optional<uint32_t> BattleClassNativeNumeric::captureCamera(uint32_t id) {
  if(id!=8 || !current())return {};
  const auto actual=camera(id);if(!actual || *actual!=base_+8*0x400)return {};
  std::array<uint32_t,12> view;
  for(unsigned i=0;i<view.size();++i){const auto value=word(*actual+0x11c+4*i);if(!value)return {};view[i]=*value;}
  if(!current())return {};
  capturedCamera_=*actual;view_=view;captured_=true;return actual;
}
uint32_t BattleClassNativeNumeric::totalWord(uint32_t at) const {
  // These are asserted source domains, not availability fallbacks. Original
  // circle/bubble only read the twelve view words after acknowledged Camera8.
  // Violating that private total-reader contract is a native programming error;
  // optional public word() remains the API for arbitrary/unavailable queries.
  if(!current())std::abort();
  if(owns(at)){
    const uint32_t first=capturedCamera_+0x11c;
    if(!captured_ || at<first || uint64_t(at)>=uint64_t(first)+48 || (at-first)%4)std::abort();
    return view_[(at-first)/4];
  }
  if(!actors_.memory.word)std::abort();
  return actors_.memory.word(actors_.memory.user,at);
}
BattleActDrawServices BattleClassNativeNumeric::draw() {
  BattleActDrawServices s;s.memory=actors_;s.user=this;
  s.camera=[](void* p,uint32_t id){return bool(static_cast<BattleClassNativeNumeric*>(p)->camera(id));};
  s.center=[](void* p)->std::optional<std::array<float,3>>{auto& a=*static_cast<BattleClassNativeNumeric*>(p);if(!a.current())return {};const auto out=a.frame_.camera()->ringCenter();return a.current()?out:std::nullopt;};
  s.offset=[](void* p,float angle)->std::optional<std::array<float,2>>{auto& a=*static_cast<BattleClassNativeNumeric*>(p);if(!a.current())return {};const auto out=a.frame_.camera()->ringOffset(angle);return a.current()?out:std::nullopt;};
  s.interpolate=[](void* p,int32_t mode,int32_t step,int32_t duration,float start,float end)->std::optional<float>{
    auto& a=*static_cast<BattleClassNativeNumeric*>(p);if(!a.current())return {};
    const auto out=cameraInterpolation(mode,step,duration,start,end);
    return a.current() && out && std::isfinite(*out)?out:std::nullopt;};
  // Source SDK tokens: translate8029e230, rotate8029dff0 (radians, ASCII
  // x/y/z=120/121/122), scale8029e2b0, concat8029dd64; identity8029dd04 below.
  s.translation=[](void* p,const std::array<float,3>& xyz)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029e230;q.xyz=xyz;return static_cast<BattleClassNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
  s.rotation=[](void* p,float angle,uint32_t axis)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029dff0;q.radians=angle;q.axis=axis;return static_cast<BattleClassNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
  s.scale=[](void* p,float x,float y,float z)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029e2b0;q.xyz={x,y,z};return static_cast<BattleClassNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
  s.concat=[](void* p,const BattleActMatrix& left,const BattleActMatrix& right)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029dd64;q.left=left;q.right=right;return static_cast<BattleClassNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
  // Optional children keep their own footer userdata; draw.user remains this
  // numeric context. Their retained owners, not this adapter, resume partial work.
  if(footers_.party)s.partyButton=[](void* p){auto& a=*static_cast<BattleClassNativeNumeric*>(p);if(!a.current())return false;const bool out=a.footers_.party(a.footers_.user);return a.current() && out;};
  if(footers_.pinch)s.pinchMark=[](void* p){auto& a=*static_cast<BattleClassNativeNumeric*>(p);if(!a.current())return false;const bool out=a.footers_.pinch(a.footers_.user);return a.current() && out;};
  return s;
}
BattleClassRasterServices BattleClassNativeNumeric::raster() {
  BattleClassRasterServices s;s.user=this;s.math=draw();s.memory=actors_;
  // Raster memory is read-only in both original local bodies. Preserve actual
  // actor half/byte readers with their original contexts; no stores are exposed.
  s.memory.byte=nullptr;s.memory.audienceLifecycle={};
  s.memory.memory={};s.memory.memory.user=this;
  s.memory.memory.word=[](void* p,uint32_t at){return static_cast<BattleClassNativeNumeric*>(p)->totalWord(at);};
  if(actors_.memory.half)s.memory.memory.half=[](void* p,uint32_t at){auto& a=*static_cast<BattleClassNativeNumeric*>(p);if(!a.current() || a.owns(at))std::abort();return a.actors_.memory.half(a.actors_.memory.user,at);};
  s.request=[](void* p,const BattleClassRasterRequest& r)->std::optional<uint32_t>{
    if(r.function!=BattleClassRasterService::Camera || r.integers!=std::vector<uint32_t>{8} || !r.reals.empty() || r.matrix)return {};
    return static_cast<BattleClassNativeNumeric*>(p)->captureCamera(8);};
  s.identity=[](void* p)->std::optional<BattleActMatrix>{BattleAudienceMatrixRequest q;q.function=0x8029dd04;return static_cast<BattleClassNativeNumeric*>(p)->matrix(q)?std::optional<BattleActMatrix>(q.output):std::nullopt;};
  // Native finite signed32 conversion domain is [-2^31,2^31), toward zero.
  // Reject exceptional/overflow input before C++ cast; no guessed fctiwz result.
  s.truncate=[](void* p,float value)->std::optional<int32_t>{
    auto& a=*static_cast<BattleClassNativeNumeric*>(p);
    if(!a.current() || !std::isfinite(value) || double(value)<-2147483648. || double(value)>=2147483648.)return {};
    const auto out=int32_t(std::trunc(double(value)));return a.current()?std::optional<int32_t>(out):std::nullopt;};
  return s;
}
}
