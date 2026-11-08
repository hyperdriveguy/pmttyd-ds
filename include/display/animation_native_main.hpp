#pragma once
#include "animation_facing_binding.hpp"
#include "animation_main_interpolation.hpp"
#include "animation_main_time_binding.hpp"
#include "animation_main_replay_binding.hpp"
#include <vector>
namespace ttyd {
class AnimationStartupProcess;
// Native semantic SPR owner, shared across all Main/linked invocations in one
// process. 3DS signed-byte decoding adapts the established GQR4 state; no PPC
// instruction execution or full FPSCR emulation is implied.
class AnimationQuantizationState {
public:
 bool write(uint32_t spr,uint32_t value);
 std::optional<uint32_t> read(unsigned index) const;
 uint64_t writes() const{return writes_;}
private:
 std::array<std::optional<uint32_t>,8> words_{};uint64_t writes_=0;
};
// Process-owned executor over the actual source VM/pool/files. One retained
// pass, stable explicit linked frames, no recursive native-stack traversal or
// typed evaluate fallback. Producer must retain camera/files/pose state until
// complete; current(pass) proves that at every primitive. False may follow
// source stores, linked progress, cooperative yields or exhausted visit budget.
// Nested diagnostic phases are controller labels,not original cycle/timing data.
struct AnimationNativeMainProgress {size_t depth=0;uint32_t main=0,geometry=0,cache=0,replay=0,interpolation=0;};
class AnimationNativeMain {
public:
 ~AnimationNativeMain();
 // Default 64 bounds pumps per resume. Same root handle/pass must persist;
 // explicit source recursion has no invented total depth/visit completion cap.
 bool advance(uint32_t handle,uint64_t pass,unsigned visits=64);
 bool pending() const{return active_;}
 size_t depth() const{return frames_.size();}
 uint64_t pass() const{return pass_;}
 uint32_t handle() const{return handle_;}
 uint64_t frameAllocations() const{return allocations_;}
 uint64_t bulkCopies() const{return replay_?replay_->bulkCopies():0;}
 uint32_t phase() const;
 AnimationNativeMainProgress progress() const;
 AnimationNativeMain(const AnimationNativeMain&)=delete;
 AnimationNativeMain& operator=(const AnimationNativeMain&)=delete;
private:
 friend class AnimationStartupProcess;
 static std::unique_ptr<AnimationNativeMain> bind(AnimationStartupProcess&,
  std::shared_ptr<const CameraManager>,AnimationFacingAddressPolicy,
  std::function<bool(uint64_t)> current);
 struct Frame;
 AnimationNativeMain(AnimationStartupProcess&,std::shared_ptr<const CameraManager>,AnimationFacingAddressPolicy,std::function<bool(uint64_t)>);
 bool stable() const;bool available() const;
 bool push(uint32_t);
 bool linked(Frame&,uint32_t);
 AnimationMainServices services(Frame&);
 AnimationFacingServices facingServices();
 bool camera();bool owns(uint32_t) const;
 std::optional<uint32_t> word(uint32_t) const;
 AnimationStartupProcess& process_;const AnimationPoolMemory& aliases_;
 std::shared_ptr<BattlePosePool> pool_;std::shared_ptr<const CameraManager> cameras_;
 AnimationFacingAddressPolicy addresses_;std::function<bool(uint64_t)> current_;
 std::unique_ptr<AnimationMainTimeBinding> time_;
 std::unique_ptr<AnimationMainReplayBinding> replay_;
 std::unique_ptr<AnimationFacingBinding> facing_;
 std::vector<std::unique_ptr<Frame>> frames_,free_;
 uint64_t allocations_=0;
 bool resumeReady_=false,active_=false;uint32_t handle_=0;uint64_t pass_=0;
};
}
