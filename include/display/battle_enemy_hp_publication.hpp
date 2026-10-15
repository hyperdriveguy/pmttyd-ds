#pragma once
#include "battle_enemy_hp_gpu.hpp"
#include "retained_icon_frame.hpp"
#include <variant>
namespace ttyd {
// Actual successfully imported SourceIconCell allocation, not a guest pointer,
// source counter, image-index stand-in or inferred resource generation.
struct BattleEnemyHpIconResidency {
 const void* texture=nullptr;
 std::shared_ptr<const uint8_t> generation;
 uint32_t image=0,width=0,height=0,gpuWidth=0,gpuHeight=0,format=0;
};
struct BattleEnemyHpIconPacket {RetainedIconPacket packet;BattleEnemyHpIconResidency residency;};
struct BattleEnemyHpRasterView {
 CameraProjectionMatrix projection{};NativeCameraRect viewport{},scissor{};
 float depthScale=-1,depthOffset=0;
};
struct BattleEnemyHpRasterPacket {BattleEnemyHpGpuQuad quad;BattleEnemyHpRasterView view;};
using BattleEnemyHpOrderedPacket=std::variant<BattleEnemyHpIconPacket,BattleEnemyHpRasterPacket>;
struct BattleEnemyHpPublication {
 uint64_t allocation=0,pass=0;
 std::shared_ptr<const uint8_t> owner;
 std::vector<BattleEnemyHpOrderedPacket> packets;
};
struct BattleEnemyHpPublicationServices {
 void* user=nullptr;
 // Must query actual live allocation ownership and actual retained source HP
 // frame completion. A nonzero caller key or guessed B address is not proof.
 bool(*current)(void*,uint64_t allocation)=nullptr;
 bool(*complete)(void*,uint64_t pass)=nullptr;
 std::optional<BattleEnemyHpIconResidency>(*icon)(void*,uint32_t image)=nullptr;
 std::optional<BattleEnemyHpRasterView>(*view)(void*)=nullptr;
 const BattleEnemyHpGpuResource* hp=nullptr;
};
bool validBattleEnemyHpIconPacket(const RetainedIconPacket&);
bool validBattleEnemyHpIconResidency(const BattleEnemyHpIconResidency&);
class BattleEnemyHpOrderedPresentation;
// Caller owns this stable adapter through held source requests. An old sink
// retains its original pass and cannot append to a later pass by accident.
struct BattleEnemyHpOrderedHpSink {
 BattleEnemyHpOrderedPresentation* owner=nullptr;uint64_t pass=0;
 BattleEnemyHpQuadSink services();
};
class BattleEnemyHpOrderedPresentation {
public:
 BattleEnemyHpOrderedPresentation()=default;
 BattleEnemyHpOrderedPresentation(const BattleEnemyHpOrderedPresentation&)=delete;
 BattleEnemyHpOrderedPresentation& operator=(const BattleEnemyHpOrderedPresentation&)=delete;
 bool bind(uint64_t allocation,BattleEnemyHpPublicationServices);
 bool begin(uint64_t pass);
 bool appendIcon(uint64_t pass,const RetainedIconPacket&);
 bool appendHp(uint64_t pass,const BattleEnemyHpGpuQuad&);
 bool publish(uint64_t pass);
 // Entire immutable combined batch is checked before any native draw. Checks
 // actual allocation and every icon/HP generation, with no source mutation.
 bool validate(const BattleEnemyHpPublication&);
 std::shared_ptr<const BattleEnemyHpPublication> snapshot()const{return published_;}
 uint64_t buildingPass()const{return active_?pass_:0;}
 bool retains(uint32_t image)const;
 void retire();
private:
 uint64_t allocation_=0,pass_=0,lastPass_=0;
 bool active_=false,running_=false,retireRequested_=false,invalid_=false;
 std::shared_ptr<const uint8_t> owner_;
 BattleEnemyHpPublicationServices services_;
 std::vector<BattleEnemyHpOrderedPacket> building_;
 std::shared_ptr<const BattleEnemyHpPublication> published_;
 bool live();bool preflight(const std::vector<BattleEnemyHpOrderedPacket>&);
 bool enter();void leave();void clear();
 struct Guard {BattleEnemyHpOrderedPresentation& owner;~Guard(){owner.leave();}};
};
}
