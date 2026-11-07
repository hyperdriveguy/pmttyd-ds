#pragma once
#include "battle_weapon_native_frame.hpp"
#include "battle_font_gpu.hpp"
#include "battle_window_packets.hpp"
#include "battle_class_surface_packets.hpp"
#include "battle_submenu_packets.hpp"
#include "battle_enemy_hp_publication.hpp"
#include <variant>
namespace ttyd {
using BattleWeaponOrderedPacket=std::variant<BattleFontGlyphPacket,BattleSubmenuPacketPtr,BattleEnemyHpIconPacket,BattleWindowPacketPtr,BattleClassSurfacePacketPtr>;
struct BattleWeaponOrderedBatch {
 std::shared_ptr<const uint8_t> allocationOwner;
 std::shared_ptr<const void> sourceResourceOwner;
 uint32_t allocation=0;uint64_t generation=0,pass=0;
 std::vector<BattleWeaponOrderedPacket> packets;
};
struct BattleWeaponOrderedBinding {
 std::shared_ptr<const uint8_t> allocationOwner;
 std::shared_ptr<const void> sourceResourceOwner,resources;
 uint32_t allocation=0;uint64_t generation=0;size_t capacity=0;
 void* user=nullptr;
 bool(*current)(void*,uint32_t,uint64_t)=nullptr; // long W/source-bank ownership
 bool(*sourceCurrent)(void*,uint64_t)=nullptr; // actual executing weapon callback
 bool(*sourceComplete)(void*,uint64_t)=nullptr; // actual retained whole source parent
 std::optional<BattleEnemyHpIconResidency>(*icon)(void*,uint32_t)=nullptr;
 const BattleFontGpuResource* font=nullptr;
 const BattleSubmenuSurfaceGpuResource* submenu=nullptr;
 // Optional until the original Window child is requested; absence holds it.
 const BattleWindowSurfaceResource* window=nullptr;
 const BattleClassSurfaceGpuResource* classes=nullptr;
 // Pure whole mixed native preflight. draw is synchronous, nonfailing and
 // cannot reenter, retire resources or invoke source/RNG. No GPU-prefix retry.
 bool(*ready)(void*,const BattleWeaponOrderedBatch&)=nullptr;
 void(*draw)(void*,const BattleWeaponOrderedBatch&)=nullptr;
 // Alternative actual native submission. False MUST precede every GPU/
 // presentation effect; retain the sealed batch unchanged for a later retry.
 // True commits once. Exactly one of draw/tryDraw must be supplied.
 bool(*tryDraw)(void*,const BattleWeaponOrderedBatch&)=nullptr;
};
class BattleWeaponOrderedPublication {
public:
 BattleWeaponOrderedPublication()=default;
 BattleWeaponOrderedPublication(const BattleWeaponOrderedPublication&)=delete;
 BattleWeaponOrderedPublication&operator=(const BattleWeaponOrderedPublication&)=delete;
 bool bind(BattleWeaponOrderedBinding);
 bool begin(uint64_t pass);
 bool reserveFont(uint64_t pass,const BattleFontGlyphPacket&);
 // Called only inside the admitted atomic glyph, after its two original RNG
 // calls. No callback/allocation/failure is permitted between reserve/commit.
 void commitFont(uint64_t pass,const BattleFontGlyphPacket&);
 bool appendSubmenu(uint64_t pass,BattleSubmenuPacketPtr);
 bool appendIcon(uint64_t pass,const RetainedIconPacket&);
 bool appendWindow(uint64_t pass,BattleWindowPacketPtr);
 bool appendClassSurface(uint64_t pass,BattleClassSurfacePacketPtr);
 std::shared_ptr<const BattleWeaponOrderedBatch> seal(uint64_t pass);
 bool submit(uint64_t pass,const std::shared_ptr<const BattleWeaponOrderedBatch>&);
 bool validate(const std::shared_ptr<const BattleWeaponOrderedBatch>&);
 BattleWeaponNativeSubmission submission();
 bool retains(uint32_t image)const;
 void retire();
private:
 BattleWeaponOrderedBinding binding_;
 uint64_t pass_=0,lastPass_=0;bool bound_=false,running_=false,invalid_=false,submitted_=false;
 std::vector<BattleWeaponOrderedPacket> building_;
 std::optional<BattleFontGlyphPacket> pending_;
 std::shared_ptr<const BattleWeaponOrderedBatch> sealed_;
 bool live();bool source(uint64_t);bool preflight(const std::vector<BattleWeaponOrderedPacket>&);
 bool enter();void leave();
 struct Guard{BattleWeaponOrderedPublication&self;~Guard(){self.leave();}};
};
}
