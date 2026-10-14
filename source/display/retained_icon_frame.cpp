#include "retained_icon_frame.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
namespace {
bool valid(const NativeCameraRect& r) {
  return r.width && r.height && r.x <= 240 && r.y <= 400 &&
         r.width <= 240-r.x && r.height <= 400-r.y;
}
template<class Matrix> bool finite(const Matrix& m) {
  for (const auto& row : m) for (float v : row) if (!std::isfinite(v)) return false;
  return true;
}
}
void RetainedIconFrames::reset() {
  owner_.reset(); sequence_=0; complete_=false;
  building_.clear(); published_.clear(); buildingCamera_.reset(); publishedCamera_.reset();
}
void RetainedIconFrames::observe(const SourceIconPass& p) {
  if (!p.owner) { reset(); return; }
  if (owner_ != p.owner) { reset(); owner_=p.owner; }
  if (!p.sequence || p.sequence < sequence_) return;
  if (p.sequence != sequence_) {
    sequence_=p.sequence; complete_=false;
    building_.clear(); buildingCamera_.reset();
    // Original auxiliary callbacks scan100 items and100 AP entries.
    building_.reserve(200);
  }
  if (p.complete && !complete_) {
    published_.swap(building_); building_.clear();
    publishedCamera_=buildingCamera_; buildingCamera_.reset(); complete_=true;
  }
}
bool RetainedIconFrames::building(const SourceIconPass& p) const {
  return p.owner && p.owner==owner_ && p.sequence && p.sequence==sequence_ && !p.complete && !complete_;
}
bool RetainedIconFrames::append(const SourceIconPass& p,const RetainedIconPacket& q) {
  // Caller observes pass boundaries. Stale packets cannot replace newer frames.
  if (!building(p) || (q.camera!=4 && q.camera!=8) || (q.camera==4 && !buildingCamera_) || !valid(q.viewport) ||
      !valid(q.scissor) || !finite(q.transform) || !finite(q.quad.positions) ||
      !finite(q.quad.uv) || !std::isfinite(q.depthScale) ||
      !std::isfinite(q.depthOffset) || !q.pipeline.rasterAdmitted()) return false;
  building_.push_back(q); return true;
}
bool RetainedIconFrames::camera(const SourceIconPass& p,const RetainedIconCamera& c) {
  if (!building(p) || !valid(c.viewport) || !finite(c.projection) || !finite(c.view) ||
      !std::isfinite(c.sourceAspect) || c.sourceAspect<=0) return false;
  buildingCamera_=c; return true;
}
bool RetainedIconFrames::retains(uint32_t image) const {
  const auto has=[image](const auto& packets){return std::any_of(packets.begin(),packets.end(),
    [image](const auto& p){return p.image==image;});};
  return has(building_) || has(published_);
}
CameraProjectionMatrix iconSceneTransform(const CameraProjectionMatrix& m,const RetainedIconCamera& c) {
  auto result=m;
  const float scale=c.sourceAspect*float(c.viewport.width)/float(c.viewport.height);
  for (auto& v:result[1]) v*=scale;
  return result;
}
CameraProjectionMatrix iconViewportTransform(const CameraProjectionMatrix& m,const NativeCameraRect& v) {
  auto result=m;
  const float x=float(v.width)/240.f,y=float(v.height)/400.f;
  for (unsigned i=0;i<4;++i) {
    result[0][i]=x*m[0][i]+(2.f*float(v.x)/240.f+x-1.f)*m[3][i];
    result[1][i]=y*m[1][i]+(2.f*float(v.y)/400.f+y-1.f)*m[3][i];
  }
  return result;
}
CameraProjectionMatrix iconSceneWorldTransform(const RetainedIconCamera& c) {
  auto m=iconSceneTransform(nativeCameraTransform(c.projection,c.view),c);
  // Independent world vertices use sourceXYZ*.01; camera matrices use source units.
  for (auto& row:m) for (unsigned col=0;col<3;++col) row[col]*=100.f;
  return iconViewportTransform(m,c.viewport);
}
} // namespace ttyd
