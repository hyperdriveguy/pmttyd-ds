#pragma once
#include <memory>
#include "battle_weapon_ordered_publication.hpp"
namespace ttyd {
class Renderer;
enum class BattleWeaponRasterKind {Font,Submenu,Icon,Window};
struct BattleWeaponRasterVertex {float x=0,y=0,z=0,u=0,v=0;uint32_t rgba=0;};
struct BattleWeaponRasterPrimitive {
 BattleWeaponRasterKind kind=BattleWeaponRasterKind::Font;
 const void*texture=nullptr;CameraProjectionMatrix transform{};
 NativeCameraRect viewport{},scissor{};float depthScale=0,depthOffset=0;
 NativeIconPlan iconMaterial;NativeDisplayPipeline iconPipeline;
 std::array<BattleWeaponRasterVertex,36>vertices{};uint8_t count=0;
};
struct BattleWeaponRasterResources {
 const BattleFontGpuResource*font=nullptr;
 const BattleSubmenuSurfaceGpuResource*submenu=nullptr;
 const BattleWindowSurfaceResource*window=nullptr;
};
// Immutable prepared geometry. Source ownership/current and actual Renderer
// residency proofs remain mandatory external/native admission, never inferred.
class BattleWeaponRasterPlan {
public:
 const std::shared_ptr<const BattleWeaponOrderedBatch>&batch()const{return batch_;}
 const std::vector<BattleWeaponRasterPrimitive>&primitives()const{return primitives_;}
 bool current(const void*owner,const BattleWeaponRasterResources&)const;
private:
 BattleWeaponRasterPlan()=default;
 const void*owner_=nullptr;BattleFontLease fontAnchor_;mutable bool checking_=false;
 std::shared_ptr<const BattleWeaponOrderedBatch>batch_;
 std::vector<BattleWeaponRasterPrimitive>primitives_;
 friend class Renderer;
 friend std::shared_ptr<const BattleWeaponRasterPlan>prepareBattleWeaponRasterGeometry(const void*,std::shared_ptr<const BattleWeaponOrderedBatch>,const BattleWeaponRasterResources&);
};
// Allocates and validates all geometry/resources; performs no GPU effects.
// Icon actual-native residency additionally requires the Renderer preflight.
std::shared_ptr<const BattleWeaponRasterPlan>prepareBattleWeaponRasterGeometry(const void*actualOwner,std::shared_ptr<const BattleWeaponOrderedBatch>,const BattleWeaponRasterResources&);
}
