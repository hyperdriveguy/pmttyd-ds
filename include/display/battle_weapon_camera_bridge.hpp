#pragma once
#include "battle_class_camera_bridge.hpp"
namespace ttyd {
struct BattleWeaponCameraView {
 CameraDepthSnapshot matrices;
 std::array<float,6> viewport{};
 std::array<uint16_t,4> scissor{};
 uint32_t projectionKind=0;
};
// Fixed original weapon callback80121ce8; never substitutes the class callback.
class BattleWeaponCameraBridge {
public:
 bool bind(std::shared_ptr<const CameraManager>,uint64_t,BattleActorServices,
           BattleClassCameraAddressPolicy,std::function<bool(BattleClassCameraEpoch)>);
 bool begin(BattleClassCameraEpoch,const CameraMainCursor&,const CameraDrawCursor&,
            const DisplayQueue&,uint32_t battle);
 bool finish(BattleClassCameraEpoch);
 void retire();
 bool available()const;
 bool owns(uint32_t)const;
 std::optional<uint32_t> camera(std::optional<uint32_t> index={})const;
 std::optional<uint32_t> word(uint32_t)const;
 std::optional<BattleWeaponCameraView> view()const;
 std::optional<std::array<float,3>> ringCenter()const;
 std::optional<std::array<float,2>> ringOffset(float)const;
 bool matrix(BattleAudienceMatrixRequest&)const;
private:
 BattleClassCameraBridge engine_;
 std::shared_ptr<const CameraManager> bank_;
 std::optional<BattleWeaponCameraView> view_;
 mutable bool stale_=false;
};
}
