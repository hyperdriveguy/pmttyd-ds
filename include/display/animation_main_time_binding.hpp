#pragma once
#include "animation_main_time.hpp"
#include "animation_pool_memory.hpp"
#include <functional>
namespace ttyd {
// Retains the pool; borrows the actual VM and alias owner. Both and this stable
// binding must outlive suspended cursors. The pure, non-reentrant current
// predicate proves the same producer pass/lifetime at every primitive. Numeric
// adapters are native policies; original libm/FPSCR parity is not measured.
class AnimationMainTimeBinding {
public:
 static std::unique_ptr<AnimationMainTimeBinding> bind(std::shared_ptr<BattlePosePool>,EventAddressSpace&,const AnimationPoolMemory&,std::function<bool()> current);
 bool available() const;
 AnimationMainTimeServices services();
 AnimationMainTimeBinding(const AnimationMainTimeBinding&)=delete;
 AnimationMainTimeBinding& operator=(const AnimationMainTimeBinding&)=delete;
private:
 AnimationMainTimeBinding(std::shared_ptr<BattlePosePool>,EventAddressSpace&,const AnimationPoolMemory&,std::function<bool()>);
 std::shared_ptr<BattlePosePool> pool_;EventAddressSpace& memory_;const AnimationPoolMemory& aliases_;std::function<bool()> current_;
};
}
