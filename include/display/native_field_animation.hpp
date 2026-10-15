#pragma once
#include "animation_startup_process.hpp"
#include "mario_presentation.hpp"
namespace ttyd {
// Native simulation service over the real cold VM, source files and one pose
// namespace. Creation precedes Mario's three Entries. Copies of the typed pool
// deliberately do not inherit these borrowing services.
class NativeFieldAnimation {
public:
 static std::shared_ptr<NativeFieldAnimation> create(const char* root,std::shared_ptr<NativeFileCache>,const EventNativePlatform&);
 ~NativeFieldAnimation();
 std::shared_ptr<BattlePosePool> pool() const{return pool_;}
 AnimationStartupProcess& process(){return *process_;}
 bool ready() const;
 bool clock(BattlePoseClock&) const;
 // One simulation visit, including paused visits if the caller supplies them.
 // Native timebase is162MHz/4; rational60Hz simulation inputs are explicit port
 // policy. Source retrace prefix owns bank/freeze arithmetic, not render calls.
 bool tick(bool battle,bool paused=false);
 bool mario(const MarioPresentation&,const std::array<int32_t,3>& handles);
 const BattlePose* marioPose() const{return sampled_>=0?pool_->pose(sampled_):nullptr;}
 unsigned marioBank() const{return bank_;}
private:
 NativeFieldAnimation()=default;
 std::optional<int32_t> enter(std::shared_ptr<const BattlePoseResource>,int32_t,const char*);
 std::shared_ptr<BattlePosePool> pool_;
 std::shared_ptr<NativeFileCache> files_;
 std::unique_ptr<AnimationStartupProcess> process_;
 std::string name_;std::shared_ptr<const BattlePoseResource> entering_;int32_t mode_=0;
 uint64_t raw_=0,uploaded_=UINT64_MAX;int32_t selected_=-1,sampled_=-1;unsigned bank_=0;
};
}
