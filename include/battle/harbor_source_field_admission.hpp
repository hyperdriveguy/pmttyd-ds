#pragma once
#include "harbor_source_startup.hpp"
#include "harbor_source_display_pass.hpp"
namespace ttyd {
class BattleSequenceBinding;
// Immutable native provider table. current proves resource-owner lifetime, not
// ready data; unavailable reached resources must hold their actual child.
// Binding takes an owned snapshot. Menu source memory is always the actual VM,
// independently of this provider table; callbacks still require real owners.
struct HarborSourceFieldServices {
 std::shared_ptr<const void> lifetime;
 std::function<bool()> current;
 BattleMenuResourceServices menu;
 std::shared_ptr<NativeFieldAnimation> animation;
 std::shared_ptr<const HarborTutorialPoseResources> poseResources;
 std::function<std::optional<uint32_t>(const BattleColdInitRequest&)> coldChild;
 std::function<std::optional<BattleUnitSetupOutput>(const BattleUnitSetupRequest&)> unitChild;
};
// Shared actual field VM startup owner. It never enters the legacy scene/VM
// bootstrap, projects scene values, samples controls or schedules source Main.
class HarborSourceFieldAdmission {
public:
 ~HarborSourceFieldAdmission();
 bool current()const;
 bool coldComplete()const;
 bool unitSetupComplete()const;
 uint32_t battleAddress()const;
 uint32_t pending()const;
 HarborBattleCameraReceipt cameraReceipt()const;
 // The later ACTUAL UnitSetup child may delegate here after whole ColdInit.
 bool resumeUnitSetup(uint32_t actualBattle);
 std::shared_ptr<HarborSourceMapPool> pool()const;
private:
 HarborSourceFieldAdmission();
 struct State;std::unique_ptr<State>state_;bool invoking_=false;
 static std::shared_ptr<HarborSourceFieldAdmission> prepare(BattleSequenceBinding&);
 bool resumeCold(uint32_t actualFieldInformation);
 friend class BattleSequenceBinding;
};
}
