#pragma once
#include "native_harbor_star_point.hpp"
#include "message_icons.hpp"
#include "battle_scene_views.hpp"
namespace ttyd {
struct NativeHarborStarPointVertex {
 float x=0,y=0,z=0,u=0,v=0;
 std::array<float,4> color{};
};
struct NativeHarborStarPointGeometry {
 std::vector<NativeHarborStarPointVertex> vertices;
 std::array<std::array<unsigned,2>,NativeHarborStarPointVisual::Capacity> ranges{};
};
// The actual imported atlas supplies its dimensions. Metadata remains original;
// editable pixel contents are not rejected by a hardcoded pixel hash.
bool validNativeHarborStarPointIcon(const MessageIcon&,uint32_t atlasWidth,uint32_t atlasHeight);
// Native tutorial camera has yaw0. Source Disp's Y billboard is therefore
// identity. Source icon404 flags0 uses bottom anchoring and world scale .6.
// Particle alpha gates visibility only; it never modulates the icon color.
std::optional<NativeHarborStarPointGeometry> nativeHarborStarPointGeometry(
 const std::array<NativeHarborStarPointVisual::Instance,NativeHarborStarPointVisual::Capacity>&,
 const MessageIcon&,uint64_t iconTicks,uint32_t sourceIconRgba,
 const BattleSceneMatrix& projectionView);
}
