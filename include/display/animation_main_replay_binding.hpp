#pragma once
#include "animation_main_replay.hpp"
#include "animation_pool_memory.hpp"
#include <functional>
namespace ttyd {
// Shared pool ownership; VM/aliases/platform quantization owner are borrowed.
// All must outlive suspended cursors. current and gqr4 are pure/non-reentrant.
class AnimationMainReplayBinding {
public:
 static std::unique_ptr<AnimationMainReplayBinding> bind(std::shared_ptr<BattlePosePool>,EventAddressSpace&,const AnimationPoolMemory&,std::function<bool()> current,std::function<std::optional<uint32_t>()> gqr4);
 bool available() const;
 AnimationMainReplayServices services();
 AnimationMainReplayBinding(const AnimationMainReplayBinding&)=delete;
 AnimationMainReplayBinding& operator=(const AnimationMainReplayBinding&)=delete;
private:
 AnimationMainReplayBinding(std::shared_ptr<BattlePosePool>,EventAddressSpace&,const AnimationPoolMemory&,std::function<bool()>,std::function<std::optional<uint32_t>()>);
 std::shared_ptr<BattlePosePool> pool_;EventAddressSpace& memory_;const AnimationPoolMemory& aliases_;
 std::function<bool()> current_;std::function<std::optional<uint32_t>()> gqr4_;
};
}
