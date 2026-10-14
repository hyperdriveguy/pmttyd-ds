#pragma once
#include "animation_shape_render.hpp"
namespace ttyd {
struct AnimationFanVertex {
    ActorVector position{},normal{};std::array<float,2> uv{};std::array<uint8_t,4> color{};
};
struct AnimationFanTexture {
    std::shared_ptr<const BattlePoseResource> resource;
    uint16_t cell=0;float uvScale=1;uint32_t generator=60;
    std::optional<BattleActMatrix> matrix;
    bool operator==(const AnimationFanTexture& r)const{return resource==r.resource && cell==r.cell && uvScale==r.uvScale && generator==r.generator && matrix==r.matrix;}
};
struct AnimationFanBatch {
    const AnimationFanVertex* vertices=nullptr;uint32_t count=0,draw=0;
    const BattlePose* pose=nullptr;std::shared_ptr<const BattlePoseResource> resource;
    AnimationFanTexture texture;uint32_t slots=0;
};
struct AnimationFanSubmit {void* user=nullptr;bool (*submit)(void*,const AnimationFanBatch&)=nullptr;};
class AnimationFanStream {
public:
    // Native storage admission, not an original GX count limit (LOW16).
    static constexpr uint32_t Capacity=64;
    bool request(const AnimationShapeCall&,const AnimationFanTexture&,const AnimationFanSubmit&);
    void reset(){*this=AnimationFanStream{};}
    uint64_t submittedFans()const{return submittedFans_;}
    uint64_t submittedVertices()const{return submittedVertices_;}
    uint32_t stagedVertices()const{return used_;}
    bool active()const{return active_;}
private:
    struct Binding {bool descriptor=false,format=false,array=false;uint32_t base=0;};
    std::array<Binding,4> bindings_{};std::array<AnimationFanVertex,Capacity> vertices_{};
    std::shared_ptr<const BattlePoseResource> resource_;const BattlePose* pose_=nullptr;
    uint32_t draw_=0,slots_=0,count_=0,used_=0;bool active_=false,ready_=false;
    AnimationFanTexture texture_;std::array<uint16_t,11> lastIndices_{};
    uint64_t submittedFans_=0,submittedVertices_=0;
};
}
