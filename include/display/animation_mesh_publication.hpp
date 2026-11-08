#pragma once
#include "animation_native_main.hpp"
namespace ttyd {
class AnimationStartupProcess;
// Native mesh adapter: fresh post-Main capture, not source Draw/Inner load
// ordering and not a snapshot of Main's earlier captured output registers.
// Same source pass stays owned until atomic candidate publication. No VM stores.
class AnimationMeshPublication {
public:
 ~AnimationMeshPublication();
 bool reserve(uint32_t handle,uint64_t pass);
 // Default 8192 counts native work visits,not source frames or AC timing.
 bool advance(uint32_t handle,uint64_t pass,unsigned budget=8192);
 bool pending() const{return active_;}
 uint32_t phase() const{return phase_;}
 uint64_t reads() const{return reads_;}
 uint64_t fastSlices() const{return fastSlices_;}
 uint64_t publications() const{return publications_;}
 uint64_t pass() const{return pass_;}
 uint32_t handle() const{return handle_;}
 const ActorAnimationSample& candidate() const{return candidate_;}
private:
 friend class AnimationStartupProcess;
 AnimationMeshPublication(AnimationStartupProcess&,std::function<bool(uint64_t)>,bool privateCapture);
 bool available() const;
 bool prepare();bool domain() const;
 bool capture(unsigned& visits,unsigned budget);
 bool word(uint32_t,uint32_t&);bool real(uint32_t,float&);bool byte(uint32_t,uint8_t&);
 AnimationStartupProcess& process_;std::shared_ptr<BattlePosePool> pool_;
 const AnimationPoolMemory& aliases_;std::function<bool(uint64_t)> current_;
 std::shared_ptr<const BattlePoseResource> resource_;std::shared_ptr<AnimationScratchPose> buffers_;
 const ActorAnimationPose* pose_=nullptr;ActorAnimationSample candidate_;
 // Temporary capture proofs reset on every raw borrow exit,including holds.
 bool privateCapture_=false,fastCapture_=false,captureReady_=false,active_=false;uint32_t handle_=0,phase_=0;uint64_t pass_=0,generation_=0,reads_=0,publications_=0,fastSlices_=0;
 uint32_t work_=0,models_=0,record_=0,entry_=0,holder_=0,model_=0,index_=0,selector_=0,channel_=0,item_=0,lane_=0;
 std::array<uint32_t,5> counts_{},outputs_{};
};
}
