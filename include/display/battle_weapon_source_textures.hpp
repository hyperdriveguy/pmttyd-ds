#pragma once
#include "battle_weapon_source_memory.hpp"
#include "trio_weapon_menu_frame_binding.hpp"
#include "battle_class_surface_packets.hpp"
#include "source_palette_texture_object.hpp"
namespace ttyd {
// Genuine common-bank TEX80037a80→GXInit/LOD output. One explicitly reserved
// native32B private output; no guest stack alias or index-as-handle policy.
class BattleWeaponSourceTextures:public std::enable_shared_from_this<BattleWeaponSourceTextures> {
public:
 static std::shared_ptr<BattleWeaponSourceTextures>create(TrioWeaponMenuFrameBinding,
   std::shared_ptr<BattleWeaponSourceMemory>,uint32_t privateAddress,
   std::function<bool()>actualSourceCurrent,
   std::function<std::optional<int32_t>(float)>actualTruncate);
 BattleWeaponSourceTextures(const BattleWeaponSourceTextures&)=delete;
 BattleWeaponSourceTextures&operator=(const BattleWeaponSourceTextures&)=delete;
 std::optional<BattleClassSurfaceTextureObject>texture(uint32_t palette,uint32_t index);
 // Only after the actual provider acknowledged this original TextureObject
 // request. Failed downstream validation retains the same completed result.
 bool acknowledge(uint32_t palette,uint32_t index);
 bool current()const;
 uint32_t pending()const;
private:
 BattleWeaponSourceTextures()=default;
 TrioWeaponMenuFrameBinding source_;std::shared_ptr<BattleWeaponSourceMemory>memory_;
 std::function<bool()>sourceCurrent_;std::function<std::optional<int32_t>(float)>truncate_;
 SourcePrivateTextureObject object_;SourcePaletteTextureObjectCursor cursor_;
 SourcePaletteTextureObjectServices services_;
 // observed_ is a 13-bit source-read mask; clearPosition_ retains memset progress.
 uint32_t address_=0,palette_=0,index_=0,table_=0,observed_=0,clearPosition_=0;
 // Private storage binding, retained request and completed-but-unacknowledged
 // result are distinct states. invalid_ is sticky; reentry remains unavailable.
 bool bound_=false,held_=false,completed_=false;mutable bool invoking_=false,invalid_=false;
 AnimationTPLImage image_{};
 bool longCurrent()const;bool sourceCurrent()const;
 std::optional<uint32_t>read(uint32_t,uint32_t);
 void observe(uint32_t,uint32_t);
 bool witness(const BattleClassSurfaceTextureObject&)const;
 static bool objectCurrent(void*);
 static bool objectWitness(void*,const BattleClassSurfaceTextureObject&);
};
}
