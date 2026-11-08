#pragma once
#include <memory>
#include <functional>
#include "native_source_icon_process.hpp"
#include "source_icon_gx.hpp"
#include "source_palette_texture_object.hpp"
#include "battle_submenu_packets.hpp"
namespace ttyd {
struct BattleWeaponIconTextureBinding {
 std::shared_ptr<NativeSourceIconProcess>process;
 std::shared_ptr<const void>lifetime; // Independent actual W/resource proof.
 uint32_t privateAddress=0;
 std::function<bool()>longCurrent,sourceCurrent;
 // Actual VM plus all native camera/private view domains, not a guessed range.
 std::function<bool(uint32_t,uint32_t)>unmapped;
 std::function<std::optional<int32_t>(float)>truncate;
};
// Whole source TEX and GXInit/LOD CPU effects over the genuine loaded Icon
// palette. Native private output address is caller-admitted, never an image
// index, guessed guest SP, atlas readiness, or a successful GX acknowledgment.
class BattleWeaponIconTextures {
public:
 static std::unique_ptr<BattleWeaponIconTextures>create(BattleWeaponIconTextureBinding);
 ~BattleWeaponIconTextures();
 BattleWeaponIconTextures(const BattleWeaponIconTextures&)=delete;
 BattleWeaponIconTextures&operator=(const BattleWeaponIconTextures&)=delete;
 // The actual TEX adapter's opaque token is this admitted output address.
 std::optional<uint32_t>construct(uint32_t actualPalette,uint32_t image);
 bool lod(const SourceIconGXCall&); // Actual post-TEX IconGX override.
 // Copy only after genuine whole constructors complete. Its resource proof
 // survives legitimate private output reuse and short camera lease retirement.
 std::optional<BattleSubmenuTextureObject>snapshot(uint32_t handle)const;
 std::optional<uint32_t>dimension(uint32_t function,uint32_t handle)const;
 // Release only after the actual whole quad is accepted; later width/height
 // queries still need the private object after LoadTexObj. Never import.
 bool finish(uint32_t handle);
 bool current()const;uint32_t pending()const;
private:
 BattleWeaponIconTextures();struct State;std::unique_ptr<State>state_;mutable bool invoking_=false;
};
}
