#pragma once
#include "battle_pose_pool.hpp"
#include "animation_pool_memory.hpp"
#include "native_file_cache.hpp"
namespace ttyd {
class EventAddressSpace;
class AnimationScratchPose;
// Actual source scratch RAM remains authoritative. This owner is retired before
// VM/array aliases. Native vectors are decoded presentation snapshots only.
class AnimationScratchMemory:public std::enable_shared_from_this<AnimationScratchMemory> {
public:
 static std::shared_ptr<AnimationScratchMemory> bind(std::shared_ptr<BattlePosePool>,std::shared_ptr<NativeFileCache>,EventAddressSpace&,const AnimationPoolMemory&);
 ~AnimationScratchMemory();
 void retire();
 bool current() const;
 // A view pins native handle/generation and checks exact resource/owner on use.
 // Holding this descriptor does not keep the scratch context or VM alive.
 std::shared_ptr<AnimationScratchPose> pose(unsigned handle,uint64_t generation);
private:
 friend class AnimationScratchPose;
 bool snapshot(BattlePose&,bool) const;
 AnimationScratchMemory(std::shared_ptr<BattlePosePool>,std::shared_ptr<NativeFileCache>,EventAddressSpace&,const AnimationPoolMemory&);
 std::weak_ptr<BattlePosePool> pool_;std::shared_ptr<NativeFileCache> files_;
 EventAddressSpace& memory_;const AnimationPoolMemory& arrays_;bool active_=true;
};
class AnimationScratchPose {
public:
 // Native finite private/unlinked sampler. False may retain pose metadata
 // stores; success still is not whole original Main/GQR/facing/ring completion.
 bool evaluate(BattlePose&,BattlePoseClock) const;
 bool snapshot(BattlePose&,bool cache=false) const;
 std::optional<ActorVector> vector(const BattlePose&,bool normal,uint32_t index) const;
 std::optional<float> node(const BattlePose&,uint32_t index) const;
 std::optional<uint8_t> byte(const BattlePose&,bool texture,uint32_t index) const;
 std::optional<std::array<float,5>> transform(const BattlePose&,uint32_t index) const;
private:
 friend class AnimationScratchMemory;
 AnimationScratchPose(std::shared_ptr<AnimationScratchMemory> owner,unsigned handle,uint64_t generation):owner_(owner),identity_(owner.get()),handle_(handle),generation_(generation){}
 std::shared_ptr<AnimationScratchMemory> owner(const BattlePose&) const;
 std::weak_ptr<AnimationScratchMemory> owner_;const AnimationScratchMemory* identity_;unsigned handle_;uint64_t generation_;
};
// Resolve at the original consumer capture point, including after held children.
std::optional<ActorVector> animationOutputVector(const BattlePose&,bool normal,uint32_t index);
std::optional<float> animationOutputNode(const BattlePose&,uint32_t index);
std::optional<uint8_t> animationOutputByte(const BattlePose&,bool texture,uint32_t index);
std::optional<std::array<float,5>> animationOutputTransform(const BattlePose&,uint32_t index);
}
