#pragma once
#include "battle_weapon_camera_bridge.hpp"
#include "battle_weapon_draw_continuation.hpp"
namespace ttyd {
// These callbacks own actual ordered batch work. A false begin/submit or empty
// seal retains their own partial effects; no captured request is completion.
// The immutable batch retains every resource needed after camera.finish.
struct BattleWeaponNativeSubmission {
 void* user=nullptr;
 bool (*begin)(void*,uint64_t pass)=nullptr;
 std::shared_ptr<const void> (*seal)(void*,uint64_t pass)=nullptr;
 bool (*submit)(void*,uint64_t pass,const std::shared_ptr<const void>&)=nullptr;
 // Pure non-reentrant long resource/publication proof, independent of camera.
 bool (*current)(void*,const std::shared_ptr<const void>&)=nullptr;
};
struct BattleWeaponNativeFrameBinding {
 std::shared_ptr<const uint8_t> allocationOwner;
 std::shared_ptr<const void> sourceResourceOwner;
 uint32_t allocation=0,battle=0;
 uint64_t generation=0;
 std::shared_ptr<const CameraManager> cameras;
 BattleActorServices actors;
 BattleClassCameraAddressPolicy addresses;
 // UI allocation incarnation and common-bank resource lifetime are independent.
 std::function<bool()> allocationCurrent,sourceResourceCurrent;
 std::function<bool(BattleClassCameraEpoch)> producerCurrent;
};
// Owns one actual original UI allocation, not source scheduling or activation.
// Resources pin the complete linked source/GX/provider contexts and immutable
// source tables. Banks/VM must not change inside synchronous source reads.
class BattleWeaponNativeFrame {
public:
 BattleWeaponNativeFrame();~BattleWeaponNativeFrame();
 BattleWeaponNativeFrame(const BattleWeaponNativeFrame&)=delete;
 BattleWeaponNativeFrame& operator=(const BattleWeaponNativeFrame&)=delete;
 bool bind(BattleWeaponNativeFrameBinding,std::shared_ptr<const void> resources,
           const BattleWeaponDrawServices&,BattleWeaponNativeSubmission);
 // Native generation/pass metadata is distinct from simulation/render clocks.
 // Requires actual invoking queue callback80121ce8, camera8 and passed B;
 // no scheduler registration or readiness producer is supplied here.
 bool resume(uint64_t successfulMainPass,const CameraMainCursor&,const CameraDrawCursor&,
             const DisplayQueue&,uint32_t camera,uint32_t battle);
 const BattleWeaponCameraBridge* camera()const;
 // Whole source draw completion precedes seal/submit/camera finish. It does
 // not expose a publication; publication additionally needs all those owners.
 bool sourceComplete(uint64_t pass)const;
 uint64_t publishedPass()const;
 std::shared_ptr<const void> publication()const;
 // Source-cursor diagnostic only, not the state of external seal/submission.
 uint32_t pending()const;
 void retire();
private:
 struct State;std::unique_ptr<State> state_;
 bool invoking_=false,retiring_=false;
};
}
