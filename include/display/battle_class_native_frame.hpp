#pragma once
#include "battle_class_camera_bridge.hpp"
#include "battle_class_frame_pipeline.hpp"
namespace ttyd {
class BattleClassFooterProvider;
struct BattleClassNativeSubmission {
  void* user=nullptr;
  // Genuine ordered native submission in this exact queued callback. False
  // retains source completion and camera ownership; successful submission is
  // never replayed after a later guard/finish failure.
  bool (*submit)(void*,const BattleClassBackend&)=nullptr;
};
// Actual VM/queue ownership, supplied by the executing battle runtime. Pure
// predicates must prove current B+1c78/token and successful CameraMain pass.
// They retain the real VM; no stack callback contexts or inferred view epoch.
struct BattleClassNativeFrameBinding {
  std::shared_ptr<const uint8_t> allocationOwner;
  uint32_t allocation=0;
  uint64_t generation=0;
  std::shared_ptr<const CameraManager> cameras;
  BattleActorServices actors;
  BattleClassCameraAddressPolicy addresses;
  std::function<bool()> allocationCurrent;
  std::function<bool(BattleClassCameraEpoch)> producerCurrent;
};
// Executes the already queued original callback. This owns neither scheduling
// nor source activation. Source/math/footer tables and backend callbacks are
// copied at bind; their actual resources/context are retained by resources.
// Camera providers may use camera() ONLY during this owner's resume call.
class BattleClassNativeFrame {
public:
  BattleClassNativeFrame();
  ~BattleClassNativeFrame();
  BattleClassNativeFrame(const BattleClassNativeFrame&)=delete;
  BattleClassNativeFrame& operator=(const BattleClassNativeFrame&)=delete;
  bool bind(BattleClassNativeFrameBinding,std::shared_ptr<const void> resources,
            const BattleClassDisplayServices&,const BattleClassRasterServices&,
            const BattleClassBackend&);
  bool resume(uint64_t successfulMainPass,const CameraMainCursor&,
              const CameraDrawCursor&,const DisplayQueue&,uint32_t camera,uint32_t battle,
              BattleClassNativeSubmission={});
  // Only original footer children executing within this exact live callback
  // may append/route to the private copied backend. No mutable backend escapes.
  bool appendFooter(const BattleClassFooterAtlasQuad&);
  std::optional<uint32_t> footerRasterRequest(const BattleClassRasterRequest&,
                                             BattleClassFooterProvider&);
  // Borrowed numeric services are usable only inside the guarded resume call.
  const BattleClassCameraBridge* camera() const;
  const BattleClassPublication& publication() const;
  bool admit(const BattleClassPublicationKey&);
  std::optional<BattleClassPublicationKey> consumeAdmission();
  void retire();
private:
  struct State;
  std::unique_ptr<State> state_;
  BattleClassPublication unavailable_;
  // Reentry guard plus deferred destruction requested by an in-flight provider.
  bool invoking_=false,retiring_=false;
};
}
