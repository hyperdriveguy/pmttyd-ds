#pragma once
#include "source_camera_bootstrap.hpp"
#include "field_camera_road_owner.hpp"
#include "camera_manager.hpp"
#include <functional>
#include <memory>
namespace ttyd {
class NativeShadowProcess;
struct NativeCameraNumerics {
 // Explicit native numerical providers, not measured Gekko libm/frsqrte parity.
 std::function<std::optional<double>(uint32_t,double)> tangent;
 std::function<std::optional<double>(float)> reciprocalSquareRoot;
};
NativeCameraNumerics nativeCameraLibmNumerics();
class NativeCameraProcess {
public:
 static constexpr uint32_t Table=FieldCameraRoadOwner::CameraTable,CameraCount=13,CameraBytes=0x260;
 static constexpr uint32_t RenderPointer=0x8041e7b4,RenderObject=0x803c9f58,RenderBytes=60;
 static constexpr uint32_t NtscInterlaced=0x8030c2a8,NtscProgressive=0x8030c2e4;
 struct Counts {unsigned allocations=0,clears=0,copies=0,distances=0,tangents=0;};
 // Adapted native gfx boot chooses an original nonnull US descriptor. The
 // bounded CPU contribution is separate from complete DEMOInit/GX/VI readiness.
 static std::shared_ptr<NativeCameraProcess> create(std::shared_ptr<NativeShadowProcess>,const char* root,NativeCameraNumerics,uint32_t descriptor=NtscInterlaced);
 ~NativeCameraProcess();
 bool initialize(unsigned visits=4096);
 bool current()const;bool ready()const;bool busy()const;uint32_t pending()const;
 Counts counts()const;
 std::optional<FieldCameraEntryBorrow> fieldCamera()const;
 FieldCameraRoadOwner* roads()const;
 std::shared_ptr<NativeShadowProcess> shadows()const;
 // One cold publication into the semantic bank. Actual later source camMain/
 // road/GX writers need their own bridge; this is no view/extra/draw receipt.
 bool publishBoot(const std::shared_ptr<CameraManager>&);
 bool bootPublishedFor(const std::shared_ptr<CameraManager>&)const;
private:
 NativeCameraProcess();struct State;std::shared_ptr<State> state_;bool invoking_=false;
};
}
