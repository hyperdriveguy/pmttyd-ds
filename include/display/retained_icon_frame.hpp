#pragma once
#include "native_camera_draw.hpp"
#include "native_display_state.hpp"
#include "native_icon_program.hpp"
#include <memory>
#include <vector>
namespace ttyd {
// Renderer admission metadata, never an original GP counter or simulation tick.
// Shared token is lifetime identity, not an allocated source byte. Sequence0
// is unstarted; monotonic renderer publication sequence is not elapsed time.
struct SourceIconPass {
  std::shared_ptr<const uint8_t> owner;
  uint64_t sequence = 0;
  bool complete = false;
};
struct RetainedIconCamera {
  CameraProjectionMatrix projection{};
  CameraViewMatrix view{};
  NativeCameraRect viewport{};
  float sourceAspect = 0;
};
// Self-contained copied draw state; image is original TPL index, camera is
// source camera domain. Default depth(-1, 0) is native reversed full range.
struct RetainedIconPacket {
  SourceIconGXQuad quad;
  NativeIconPlan material;
  NativeDisplayPipeline pipeline;
  CameraProjectionMatrix transform{};
  NativeCameraRect viewport{}, scissor{};
  float depthScale = -1, depthOffset = 0;
  uint32_t image = 0, camera = 0;
};
// Exact FIFO acknowledgement owns copied geometry/state. Only the real whole
// camera draw return publishes a frame. A held next pass preserves the last one.
class RetainedIconFrames {
public:
  void observe(const SourceIconPass&);
  bool append(const SourceIconPass&, const RetainedIconPacket&);
  bool camera(const SourceIconPass&, const RetainedIconCamera&);
  void reset();
  bool retains(uint32_t image) const;
  const std::vector<RetainedIconPacket>& published() const { return published_; }
  const std::optional<RetainedIconCamera>& sceneCamera() const { return publishedCamera_; }
private:
  bool building(const SourceIconPass&) const;
  std::shared_ptr<const uint8_t> owner_;
  uint64_t sequence_ = 0;
  bool complete_ = false;
  // building/published storage must remain distinct until source pass return;
  // successful individual quad acknowledgements do not publish the pass.
  std::vector<RetainedIconPacket> building_, published_;
  std::optional<RetainedIconCamera> buildingCamera_, publishedCamera_;
};
// Handheld scene adapter: retain the actual source P*V (including roll), adapt
// horizontal aspect only. PICA matrices are tilted: row1 is display horizontal.
CameraProjectionMatrix iconSceneTransform(const CameraProjectionMatrix&, const RetainedIconCamera&);
CameraProjectionMatrix iconViewportTransform(const CameraProjectionMatrix&, const NativeCameraRect&);
CameraProjectionMatrix iconSceneWorldTransform(const RetainedIconCamera&);
} // namespace ttyd
