#pragma once
#include "battle_pose_pool.hpp"
#include <memory>
namespace ttyd {
class EventAddressSpace;
// Aliases the existing source-initialized803d9580 table and CPU consumers to
// the SAME BE bytes. No regeneration, table reset or static fallback on bind.
class AnimationTangentMemory {
public:
 static std::unique_ptr<AnimationTangentMemory> bind(std::shared_ptr<BattlePosePool>,EventAddressSpace&);
 ~AnimationTangentMemory();
 AnimationTangentMemory(const AnimationTangentMemory&)=delete;
 AnimationTangentMemory& operator=(const AnimationTangentMemory&)=delete;
 bool current() const;
 bool detach(); // Preserve current bytes in VM RAM and retained CPU table.
 // Original runtime table address,not an asset pointer or newly generated
 // fallback. Its178 words share one mutable BE backing with CPU consumers.
 static constexpr uint32_t Address=0x803d9580;
private:
 AnimationTangentMemory(std::shared_ptr<BattlePosePool>,EventAddressSpace&);
 std::shared_ptr<BattlePosePool> pool_;
 EventAddressSpace& memory_;
 AnimationTangentTable* table_=nullptr;
 bool active_=false;
};
}
