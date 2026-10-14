#pragma once
#include "actor_animation.hpp"
#include "actor_animation_cache.hpp"
#include <memory>
#include <optional>

namespace ttyd {
struct BattlePoseDiscreteKey {float time=0;uint32_t value=0;};
// Original descriptor identity is independent of the converted subset order.
struct BattlePoseResource {
    std::string modelName,textureName;
    std::vector<std::string> names;
    std::optional<uint32_t> modelFlags; // Original MODEL+c4, independent of pose flags.
    std::vector<std::vector<BattlePoseDiscreteKey>> discrete;
    std::shared_ptr<const ActorAnimationPack> bank;
};
std::shared_ptr<const BattlePoseResource> loadBattlePoseResource(const char* directory,const char* pack);
struct BattlePoseClock {uint64_t modeZero=0,modeNonzero=0;}; // GP40/44 and GP38/3c millisecond quotients, not rendering frames.
// Scalar state0x170 bytes preserves source offsets/endianness; native CPU
// resources/cache/workspace remain separately owned. Linked output sentinel
//0xffffffff and source handle IDs must not be cast to host pointers.
struct BattlePose {
    // Offset view of the admitted source scalar state, not mapped SDK pointers.
    std::array<uint8_t,0x170> state{};
    std::shared_ptr<const BattlePoseResource> resource;
    ActorAnimationSample shape; // Real per-pose CPU output and retained hierarchy workspace.
    ActorAnimationCache cache; // Fully replayed current-key state, separate from fractional output.
    int32_t archiveType=0;
    uint32_t word(unsigned offset) const;
    void word(unsigned offset,uint32_t value);
    float scalar(unsigned offset) const;
    void scalar(unsigned offset,float value);
    uint64_t timestamp() const {return uint64_t(word(0x18))<<32|word(0x1c);}
};
struct BattlePoseModelCache {
    uint32_t flags=0,references=0,texture=0;
    std::shared_ptr<const BattlePoseResource> resource;
};
struct BattlePoseTextureCache {
    uint32_t flags=0,references=0;
    std::string name;
    std::shared_ptr<const BattlePoseResource> resource; // Owns converted texture descriptors/palettes.
};
// Scoped native battle handle namespace. Field handles belong to their own owner;
// these numbers do not assert the original global animation allocation history.
class BattlePosePool {
public:
    std::optional<int32_t> enter(std::shared_ptr<const BattlePoseResource>,int32_t mode,BattlePoseClock);
    BattlePose* pose(int32_t handle);
    const BattlePose* pose(int32_t handle) const;
    std::optional<bool> testTranslucent(int32_t handle) const;
    const BattlePose* slot(unsigned index) const {return index<poses_.size()?poses_[index].get():nullptr;}
    bool setRate(int32_t handle,float rate);
    bool main(int32_t handle,BattlePoseClock);
    uint32_t outputSelector=0; // Native work owner's source animInit W+e8 history.
    bool selectName(int32_t handle,const std::string& name,bool force,BattlePoseClock);
    bool selectIndex(int32_t handle,uint32_t index,bool force,BattlePoseClock);
    bool evaluate(int32_t handle,BattlePoseClock);
    bool clearEffect(int32_t handle); // Real unlinked SetEffect(handle,0,0) domain only.
    bool release(int32_t handle); // Ordinary owned records; unsupported smart buffers stay pending.
    const std::array<BattlePoseModelCache,64>& models() const {return models_;}
    const std::array<BattlePoseTextureCache,64>& textures() const {return textures_;}
    int32_t archiveType() const {return archiveType_;}
private:
    void restart(BattlePose&,BattlePoseClock);
    std::array<std::unique_ptr<BattlePose>,256> poses_{};
    std::array<BattlePoseModelCache,64> models_{};
    std::array<BattlePoseTextureCache,64> textures_{};
    int32_t archiveType_=0;
};
}
