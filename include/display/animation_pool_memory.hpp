#pragma once
#include "battle_pose_pool.hpp"
#include <memory>
#include <vector>
namespace ttyd {
class EventAddressSpace;
// A scoped alias of existing caller-owned W/RAM and the SAME typed pool bytes.
// Requires matching complete source records; never constructs W, file handles,
// pools with a reduced count, or a substitute cold-start history.
class AnimationPoolMemory {
public:
 static std::unique_ptr<AnimationPoolMemory> bind(std::shared_ptr<BattlePosePool>,EventAddressSpace&);
 ~AnimationPoolMemory();
 AnimationPoolMemory(const AnimationPoolMemory&)=delete;
 AnimationPoolMemory& operator=(const AnimationPoolMemory&)=delete;
 bool matchesWork() const;
 bool detach(); // Writes CURRENT bytes back to RAM, never the pre-attach snapshot.
 uint32_t poseAddress(unsigned i) const{return i<256?poses_+i*0x170:0;}
private:
 AnimationPoolMemory(std::shared_ptr<BattlePosePool>,EventAddressSpace&);
 struct Row {uint32_t address,size;uint8_t* bytes;};
 struct Span {uint32_t address,size;std::vector<uint8_t> saved;};
 std::shared_ptr<BattlePosePool> pool_;EventAddressSpace& memory_;
 uint32_t work_=0,models_=0,textures_=0,poses_=0;
 bool active_=false;
 std::vector<Row> rows_;std::vector<Span> spans_;
};
}
