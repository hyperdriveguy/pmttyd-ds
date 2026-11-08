#pragma once
#include "native_harbor_reward_counter.hpp"
#include "native_harbor_reward_presentation.hpp"
namespace ttyd {
struct NativeHarborRewardFlowServices {
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool(*current)(void*)=nullptr;
 // Actual owned victory stream and actual source key40 root admission.
 // A callback returning false must retain any completed admission prefix.
 bool(*victoryMusic)(void*)=nullptr;
 bool(*victoryEvent)(void*)=nullptr;
 bool(*victoryEventComplete)(void*)=nullptr;
};
// Finite native positive/no-level-up reward sequence. This owns no guest End
// work, field return receipt, finishing animation, fade or exported pouch.
class NativeHarborRewardFlow {
public:
 enum class Phase { Admission, Intro, Victory, Counting, Cleanup,
                    ReadyForFinishing, ZeroReward, LevelUp };
 NativeHarborRewardFlow(std::shared_ptr<NativeHarborRewardCounter>,
   std::shared_ptr<NativeHarborRewardPresentation>,NativeHarborRewardFlowServices);
 NativeHarborRewardFlow(const NativeHarborRewardFlow&)=delete;
 NativeHarborRewardFlow& operator=(const NativeHarborRewardFlow&)=delete;
 // Once an operation holds, later calls resume its original tick/input prefix.
 // Accepted completed duplicate ticks do no further work. Physical A/B is0x300.
 bool advance(uint64_t simulationTick,uint32_t held);
 bool current()const;
 Phase phase()const{return phase_;}
 int32_t wait()const{return wait_;}
private:
 bool visit();
 std::shared_ptr<NativeHarborRewardCounter>counter_;
 std::shared_ptr<NativeHarborRewardPresentation>presentation_;
 NativeHarborRewardFlowServices services_;
 Phase phase_=Phase::Admission;
 int32_t wait_=0;
 unsigned step_=0;
 uint64_t tick_=0,completedOuter_=0;
 uint32_t held_=0;
 bool hasTick_=false,done_=false,hasCompletedOuter_=false,invoking_=false;
 mutable bool checking_=false;
};
}
