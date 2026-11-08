#pragma once
#include "animation_main_replay.hpp"
#include "animation_pool_memory.hpp"
#include <functional>
namespace ttyd {
// Shared pool ownership; VM/aliases/platform quantization owner are borrowed.
// All must outlive suspended cursors. current and gqr4 are pure/non-reentrant.
class AnimationMainReplayBinding {
public:
 static std::unique_ptr<AnimationMainReplayBinding> bind(std::shared_ptr<BattlePosePool>,EventAddressSpace&,const AnimationPoolMemory&,std::function<bool()> current,std::function<std::optional<uint32_t>()> gqr4,bool nativeCopies=false);
 // Native copy opt-in changes only admitted disjoint raw-RAM acceleration;
 // default false keeps scalar access boundaries for arbitrary source aliases.
 uint64_t bulkCopies() const{return bulkCopies_;}
 bool available() const;
 AnimationMainReplayServices services();
 AnimationMainReplayBinding(const AnimationMainReplayBinding&)=delete;
 AnimationMainReplayBinding& operator=(const AnimationMainReplayBinding&)=delete;
private:
 AnimationMainReplayBinding(std::shared_ptr<BattlePosePool>,EventAddressSpace&,const AnimationPoolMemory&,std::function<bool()>,std::function<std::optional<uint32_t>()>,bool);
 std::shared_ptr<BattlePosePool> pool_;EventAddressSpace& memory_;const AnimationPoolMemory& aliases_;
 bool nativeCopies_=false;uint64_t bulkCopies_=0;
 std::function<bool()> current_;std::function<std::optional<uint32_t>()> gqr4_;
};
}
