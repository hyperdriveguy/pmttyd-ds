#pragma once
#include "battle_weapon_native_frame.hpp"
#include "battle_class_raster.hpp"
namespace ttyd {
// Actual executing weapon-camera math. This supplies no source child, GX,
// font, texture, display-completion or normal-activation acknowledgements.
class BattleWeaponNativeNumeric {
public:
 BattleWeaponNativeNumeric(BattleWeaponNativeFrame&,BattleActorServices,uint32_t reservedBase,
   std::function<std::optional<uint32_t>(uint32_t)> actualSourceWord,std::function<bool()> sourceCurrent);
 BattleWeaponNativeNumeric(const BattleWeaponNativeNumeric&)=delete;
 BattleWeaponNativeNumeric&operator=(const BattleWeaponNativeNumeric&)=delete;
 BattleWeaponDrawServices weapon();
 BattleActDrawServices draw();
 BattleClassRasterServices raster();
 std::optional<uint32_t>word(uint32_t)const;
 std::optional<uint32_t>camera(std::optional<uint32_t> index={})const;
 bool matrix(BattleAudienceMatrixRequest&)const;
 std::optional<int32_t>truncate(float)const;
 std::optional<float>fma(float,float,float)const;
 std::optional<BattleActMatrix>identity()const;
private:
 BattleWeaponNativeFrame&frame_;BattleActorServices actors_;uint32_t base_=0,captured_=0;
 std::array<uint32_t,12>view_{};bool hasView_=false;
 std::function<std::optional<uint32_t>(uint32_t)>sourceWord_;
 std::function<bool()>sourceCurrent_;
 bool current()const;bool owns(uint32_t)const;
 std::optional<uint32_t>capture(uint32_t);
 uint32_t totalWord(uint32_t)const;
};
}
