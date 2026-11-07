#pragma once
#include "animation_source_render.hpp"
#include "animation_sdk_matrix_binding.hpp"
#include "camera_manager.hpp"
#include "actor_animation.hpp"
#include <memory>
namespace ttyd {
// One immutable semantic camera bank per producer configuration. Actual camera
// selection and published view bytes stay live at each reached getter/pair.
class AnimationRenderSdkBinding {
public:
 AnimationRenderSdkBinding(const AnimationRenderSdkBinding&)=delete;
 AnimationRenderSdkBinding& operator=(const AnimationRenderSdkBinding&)=delete;
 static constexpr uint32_t CameraBase=0x81910000,CameraStride=0x280,CameraCount=13,CameraSize=CameraCount*CameraStride;
 static constexpr uint32_t CameraViewOffset=0x11c,CurrentCameraGetter=0x8000f2e4,CameraByIndexGetter=0x8000f2fc;
 AnimationRenderSdkBinding(EventAddressSpace&,std::shared_ptr<const CameraManager>,
  const AnimationSourceRenderChildren&,std::function<bool()> current);
 AnimationSourceRenderChildren services();
private:
 bool available() const;bool owns(uint32_t) const;
 bool cameraPair(uint32_t,std::array<uint32_t,2>&) const;
 AnimationSdkLocals storage(AnimationRenderLocals&);
 bool call(const AnimationRenderCall&,AnimationRenderLocals&,uint32_t&);
 bool point(const AnimationRenderCall&,AnimationRenderLocals&);
 EventAddressSpace& memory_;std::shared_ptr<const CameraManager> cameras_;
 AnimationSourceRenderChildren backend_;std::function<bool()> current_;
 AnimationSdkMatrixBinding matrices_;
 bool pointActive_=false;AnimationRenderCall pointRequest_;AnimationRenderLocals* pointLocals_=nullptr;
 enum PointStage {CaptureVectorXY,CaptureRow0XY,CaptureVectorZ,CaptureRow0ZT,CaptureRow1XY,CaptureRow1ZT,StoreX,ComputeY,CaptureRow2XY,CaptureRow2ZT,StoreY,StoreZ};
 PointStage pointStage_=CaptureVectorXY;ActorVector vector_{};BattleActMatrix matrix_{};float y_=0;
};
}
