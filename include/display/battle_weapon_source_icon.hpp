#pragma once
#include <memory>
#include <functional>
#include "battle_weapon_icon_textures.hpp"
#include "battle_enemy_hp_publication.hpp"
#include "battle_weapon_source_icon_pin.hpp"
#include "source_icon_matrix.hpp"
#include "source_gx_setter_routing.hpp"
namespace ttyd {
struct BattleWeaponSourceIconBinding {
 BattleWeaponIconTextureBinding textures;
 BattleSubmenuCameraProof camera; // Actual admitted camera8 projection/raster.
 std::shared_ptr<SourceGxSetterRouting>gx;
 std::shared_ptr<BattleWeaponSourceIconPin>pin;
 std::function<std::optional<uint32_t>(uint32_t)>word;
 std::function<std::optional<uint16_t>(uint32_t)>half;
 std::function<std::optional<uint32_t>(std::optional<uint32_t>)>cameraAddress;
 std::function<bool(BattleAudienceMatrixRequest&)>matrix;
 std::function<std::optional<BattleEnemyHpIconResidency>(uint32_t)>residency;
 // Atomic actual ordered append. False consumes no quad. No imports/GPU/source
 // parent scheduling may happen in this callback.
 std::function<bool(const RetainedIconPacket&)>append;
 // Root resource proof retains immutable per-Load evidence independently of
 // this temporary child/camera. False must not consume an evidence prefix.
 std::function<bool(std::shared_ptr<const SourceGxInheritedSnapshot>)>retainOrdinary;
 // Map/light/world material parents remain explicit unavailable frontiers;
 // ordinary UI flags16 bypass them. No map handle/opaque acknowledgment.
};
// Source Matrix/Position→GX→TEX/SDK→finite material/pipeline→ordered packet.
// No direct GPU, texture import, queue/source-ready or wholeFrame completion.
class BattleWeaponSourceIcon {
public:
 static std::unique_ptr<BattleWeaponSourceIcon>create(BattleWeaponSourceIconBinding);
 ~BattleWeaponSourceIcon();
 BattleWeaponSourceIcon(const BattleWeaponSourceIcon&)=delete;
 BattleWeaponSourceIcon&operator=(const BattleWeaponSourceIcon&)=delete;
 bool resumeMatrix(const BattleAudienceIconRequest&);
 // Stable table for the existing whole WeaponCursor's SourceIconPosition
 // child. Caller retains this owner through all child holds; it must preserve
 // that original position helper's late borrowed-position capture.
 const SourceIconMatrixServices&matrixServices()const;
 bool current()const;uint32_t pending()const;
 std::optional<uint32_t>pendingImage()const;
private:
 BattleWeaponSourceIcon();struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
