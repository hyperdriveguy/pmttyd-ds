#pragma once
#include "actor_animation.hpp"
#include "animation_draw_resource.hpp"
#include "actor_animation_cache.hpp"
#include <memory>
#include <cassert>
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
    std::shared_ptr<const class AnimationTextureResource> textures; // Original immutable TPL companion.
    std::shared_ptr<const AnimationDrawResource> draw; // Unchanged original group/shape/draw/index data.
};
std::shared_ptr<const BattlePoseResource> loadBattlePoseResource(const char* directory,const char* pack);
struct BattlePoseClock {uint64_t modeZero=0,modeNonzero=0;}; // GP40/44 and GP38/3c millisecond quotients, not rendering frames.
// Scalar state0x170 bytes preserves source offsets/endianness; native CPU
// resources/cache/workspace remain separately owned. Linked output sentinel
//0xffffffff and source handle IDs must not be cast to host pointers.
struct BattlePose {
    // Offset view of the admitted source scalar state, not mapped SDK pointers.
    std::array<uint8_t,0x170> state{};
    uint64_t nativeGeneration=0; // Native lifetime identity, not serialized source state.
    std::shared_ptr<const BattlePoseResource> resource;
    ActorAnimationSample shape; // Real per-pose CPU output and retained hierarchy workspace.
    ActorAnimationCache cache; // Fully replayed current-key state, separate from fractional output.
    std::shared_ptr<class AnimationScratchPose> sourceBuffers; // Actual source-backed sampling/consumer views.
    int32_t archiveType=0;
    uint32_t word(unsigned offset) const;
    void word(unsigned offset,uint32_t value);
    float scalar(unsigned offset) const;
    void scalar(unsigned offset,float value);
    uint64_t timestamp() const {return uint64_t(word(0x18))<<32|word(0x1c);}
};
// Same BE row bytes are consumed by typed cache operations and VM aliases.
// Native metadata is separate and never reinterpreted as source record layout.
template<unsigned Size> struct AnimationCacheRecord {
    std::array<uint8_t,Size> state{};
    uint32_t word(unsigned offset) const {assert(offset+4<=Size);return uint32_t(state[offset])<<24|uint32_t(state[offset+1])<<16|uint32_t(state[offset+2])<<8|state[offset+3];}
    void word(unsigned offset,uint32_t value){assert(offset+4<=Size);for(unsigned i=0;i<4;++i)state[offset+i]=uint8_t(value>>(24-8*i));}
    uint32_t flags() const{return word(0);}void flags(uint32_t v){word(0,v);}
    uint32_t references() const{return word(4);}void references(uint32_t v){word(4,v);}
    uint32_t file() const{return word(8);}void file(uint32_t v){word(8,v);}
};
struct BattlePoseModelCache:AnimationCacheRecord<16> {
    uint32_t texture() const{return word(12);}void texture(uint32_t v){word(12,v);}
    std::shared_ptr<const BattlePoseResource> resource;
};
struct BattlePoseTextureCache:AnimationCacheRecord<12> {
    std::string name;
    std::shared_ptr<const BattlePoseResource> resource;
};
class AnimationPoolMemory;
// Source-sized pool used by distinct native battle/field owners. A pool instance
// owns its handles; numbers do not assert original global allocation history.
class BattlePosePool {
public:
    BattlePosePool()=default;
    BattlePosePool(const BattlePosePool&);
    BattlePosePool& operator=(const BattlePosePool&)=delete;
    std::optional<int32_t> enter(std::shared_ptr<const BattlePoseResource>,int32_t mode,BattlePoseClock,
        const char* requestedModel=nullptr);
    // NPC source constructor consumes raw-2 fallback; typed callers keep their
    // valid-handle-only API. Empty still means an unfinished native child.
    std::optional<int32_t> enterSource(std::shared_ptr<const BattlePoseResource>,int32_t,BattlePoseClock,const char* requestedModel=nullptr);
    std::optional<int32_t> enterSourceOriginal(std::shared_ptr<const BattlePoseResource>,int32_t,BattlePoseClock,uint32_t name);
    bool sourceEntryAvailable() const{return !nativeEntryAvailable_ || nativeEntryAvailable_(nativeProvider_);}
    BattlePose* pose(int32_t handle);
    const BattlePose* pose(int32_t handle) const;
    std::optional<bool> testTranslucent(int32_t handle) const;
    const BattlePose* slot(unsigned index) const {return index<poses_.size()?poses_[index].get():nullptr;}
    bool paperOn(int32_t handle);
    bool materialLightFlagOn(int32_t handle,uint32_t flags);
    std::optional<int32_t> vivianType(int32_t handle) const;
    std::optional<float> height(int32_t handle) const;
    std::optional<float> radius(int32_t handle) const;
    bool setRate(int32_t handle,float rate);
    bool main(int32_t handle,BattlePoseClock);
    uint32_t outputSelector() const;
    void outputSelector(uint32_t);
    // Positive signed reference counts and matching native resource metadata.
    // Other source domains require their real file/entry/release providers.
    bool cacheMetadataReady() const;
    bool sourceArraysCurrent() const{return !sourceGuard_ || sourceGuard_(sourceGuardUser_);}
    const AnimationTangentTable* animationTangents() const{return tangents_.get();}
    bool selectName(int32_t handle,const std::string& name,bool force,BattlePoseClock);
    bool selectIndex(int32_t handle,uint32_t index,bool force,BattlePoseClock);
    bool evaluate(int32_t handle,BattlePoseClock);
    bool clearEffect(int32_t handle); // Real unlinked SetEffect(handle,0,0) domain only.
    bool release(int32_t handle); // Ordinary owned records; unsupported smart buffers stay pending.
    const std::array<BattlePoseModelCache,64>& models() const {return models_;}
    const std::array<BattlePoseTextureCache,64>& textures() const {return textures_;}
    int32_t archiveType() const {return archiveType_;}
private:
    friend class AnimationPoolMemory;
    friend class AnimationStartupProcess;
    friend class AnimationTangentMemory;
    friend class AnimationScratchMemory;
    friend class AnimationScratchPose;
    friend class NativeFieldAnimation;
    std::optional<int32_t> (*nativeSourceEntry_)(void*,std::shared_ptr<const BattlePoseResource>,int32_t,BattlePoseClock,const char*)=nullptr;
    std::optional<int32_t> (*nativeOriginalEntry_)(void*,std::shared_ptr<const BattlePoseResource>,int32_t,BattlePoseClock,uint32_t)=nullptr;
    bool (*nativeEntryAvailable_)(const void*)=nullptr;
    bool resolveClock(BattlePoseClock& clock) const{return !nativeClock_ || nativeClock_(nativeProvider_,clock);}
    void* nativeProvider_=nullptr;
    std::optional<int32_t> (*nativeEntry_)(void*,std::shared_ptr<const BattlePoseResource>,int32_t,BattlePoseClock,const char*)=nullptr;
    bool (*nativeClock_)(const void*,BattlePoseClock&)=nullptr;
    bool (*nativeRelease_)(void*,int32_t)=nullptr;
    void restart(BattlePose&,BattlePoseClock);
    bool tangentSamplingAvailable() const;
    std::array<std::unique_ptr<BattlePose>,256> poses_{};
    std::array<BattlePoseModelCache,64> models_{};
    std::array<BattlePoseTextureCache,64> textures_{};
    int32_t archiveType_=0;
    uint64_t nextGeneration_=1;
    std::array<uint8_t,0x110> work_{}; // Only e8 has typed initial history before source attachment.
    bool workEstablished_=false;
    std::unique_ptr<AnimationTangentTable> tangents_; // Source table, absent only in detached legacy mode.
    const class AnimationTangentMemory* tangentOwner_=nullptr;
    const void* sourceGuardUser_=nullptr;
    const class EventAddressSpace* sourceMemory_=nullptr;
    bool (*sourceGuard_)(const void*)=nullptr;
};
}
