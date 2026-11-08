#pragma once
#include <memory>
namespace ttyd {
class BattleSequenceBinding;
struct HarborBattleCameraReceipt;
// Source-event receipt, not a mode test or CameraMain publication receipt.
class BattleSequenceCameraReceipt {
public:
 ~BattleSequenceCameraReceipt();
 HarborBattleCameraReceipt publication()const;
private:
 struct State;std::shared_ptr<State>state_;
 BattleSequenceCameraReceipt();
 static std::shared_ptr<BattleSequenceCameraReceipt> capture(BattleSequenceBinding&);
 friend class BattleSequenceBinding;
};
}
