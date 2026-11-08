#pragma once
#include "animation_source_render.hpp"
#include "event_address_space.hpp"
#include "native_camera_draw.hpp"
#include <functional>
#include <memory>
namespace ttyd {
namespace field_animation_gx {
constexpr uint32_t PositionMatrix=0x802ba2fc,NormalMatrix=0x802ba34c,CurrentMatrix=0x802ba39c,Cull=0x802b5c8c;
constexpr uint32_t ClearDescriptors=0x802b4520,Descriptor=0x802b3c08,Format=0x802b4558,Array=0x802b4db0,Begin=0x802b5a6c,Fifo=0xcc008000;
constexpr unsigned Position=9,Normal=10,Color=11,FirstUv=13,UvCount=8,AttributeCount=3+UvCount;
constexpr uint32_t Index16=3,Float32=4,Rgba8=5,TriangleFan=0xa0,MaxVertices=0xffff;
}
struct FieldAnimationVertex {
 std::array<float,3> position{},normal{};std::array<uint8_t,4> color{};
 std::array<std::array<float,2>,field_animation_gx::UvCount> uv{};
};
struct FieldAnimationMatrices {
 std::optional<BattleActMatrix> position,normal;std::optional<uint32_t> current,cull;
};
struct FieldAnimationBatch {
 const FieldAnimationVertex* vertices=nullptr;uint32_t count=0,uvCount=0;
 const FieldAnimationMatrices* matrices=nullptr;
};
struct FieldAnimationSubmit {
 void* user=nullptr;
 // Complete fan only. False retains the same data/state until acknowledgement;
 // no physical immediate Begin may escape this synchronous callback.
 bool (*submit)(void*,const FieldAnimationBatch&)=nullptr;
};
struct FieldAnimationPhysical {
 void* user=nullptr;
 bool (*camera)(void*,const NativeCameraPacket&,const NativeCameraDraw&)=nullptr;
 bool (*cull)(void*,uint32_t sourceMode)=nullptr;
};
// One field producer/VM and its GPU/resource owners must outlive every hold.
// Raw array addresses remain source identities, never cast to native pointers.
// Material/texture/lighting calls stay with the explicit fallback owner.
class FieldAnimationGpu {
public:
 FieldAnimationGpu(EventAddressSpace&,const FieldAnimationPhysical&,const FieldAnimationSubmit&,
  const AnimationSourceRenderChildren&,std::function<bool()> current);
 FieldAnimationGpu(const FieldAnimationGpu&)=delete;FieldAnimationGpu& operator=(const FieldAnimationGpu&)=delete;
 AnimationSourceRenderChildren services();
 bool camera(const CameraDrawRequest&);
 bool restore() const;
 const FieldAnimationMatrices& matrices() const{return matrices_;}
 bool active() const{return active_;}
 uint32_t stagedVertices() const{return used_;}
 uint32_t capacity() const{return capacity_;}
 uint64_t submittedFans() const{return submittedFans_;}
 uint64_t submittedVertices() const{return submittedVertices_;}
private:
 struct Binding {bool descriptor=false,format=false,array=false;uint32_t base=0,stride=0;};
 bool current() const;NativeCameraSubmit cameraSubmit() const;
 bool call(const AnimationRenderCall&,AnimationRenderLocals&,uint32_t&);
 bool fifo(uint32_t,uint16_t);
 EventAddressSpace& memory_;FieldAnimationPhysical physical_;FieldAnimationSubmit submit_;
 AnimationSourceRenderChildren fallback_;std::function<bool()> current_;
 NativeCameraDraw camera_;FieldAnimationMatrices matrices_;
 std::array<Binding,field_animation_gx::AttributeCount> bindings_{};
 std::unique_ptr<FieldAnimationVertex[]> vertices_;uint32_t capacity_=0,count_=0,used_=0,uvCount_=0,attribute_=0,component_=0;
 bool active_=false,indexPending_=false,ready_=false;uint16_t index_=0;
 FieldAnimationMatrices fanMatrices_;uint64_t submittedFans_=0,submittedVertices_=0;
 bool cullPending_=false;uint32_t cullInput_=0;
 bool matrixPending_=false;AnimationRenderCall matrixRequest_;AnimationRenderLocals* matrixLocals_=nullptr;BattleActMatrix matrixInput_{};
};
}
