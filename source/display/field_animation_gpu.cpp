#include "field_animation_gpu.hpp"
#include <cstring>
#include <new>
namespace ttyd {
namespace {
using namespace field_animation_gx;using K=AnimationRenderKind;
// Compact slots 0/1/2 are POS/NRM/CLR0; 3..10 are TEX0..7.
int slot(uint32_t a){return a>=Position && a<=Color?int(a-Position):a>=FirstUv && a<FirstUv+UvCount?int(3+a-FirstUv):-1;}
bool same(const AnimationRenderCall& a,const AnimationRenderCall& b){if(a.function!=b.function || a.count!=b.count || a.count>a.arguments.size())return false;for(unsigned i=0;i<a.count;++i)if(a.arguments[i].kind!=b.arguments[i].kind || a.arguments[i].value!=b.arguments[i].value)return false;return true;}
bool args(const AnimationRenderCall& q,std::initializer_list<uint32_t> values){if(q.count!=values.size())return false;unsigned i=0;for(auto value:values){if(q.arguments[i].kind!=K::Scalar || q.arguments[i].value!=value)return false;++i;}return true;}
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
}
FieldAnimationGpu::FieldAnimationGpu(EventAddressSpace& m,const FieldAnimationPhysical& p,const FieldAnimationSubmit& s,const AnimationSourceRenderChildren& f,std::function<bool()> current):memory_(m),physical_(p),submit_(s),fallback_(f),current_(std::move(current)){}
// Sampled validity, not sticky invalidation. Producer owns epoch/lifetime policy;
// a later true predicate can permit work again (as fixture repair requires).
bool FieldAnimationGpu::current() const{return current_ && current_();}
NativeCameraSubmit FieldAnimationGpu::cameraSubmit() const{return {const_cast<FieldAnimationGpu*>(this),[](void* u,const NativeCameraPacket& p){auto& b=*static_cast<FieldAnimationGpu*>(u);return b.current() && b.physical_.camera && b.physical_.camera(b.physical_.user,p,b.camera_);}};}
bool FieldAnimationGpu::camera(const CameraDrawRequest& q){return current() && !active_ && !cullPending_ && camera_.request(q,cameraSubmit());}
// Reapply acknowledged camera/cull state, including while a final fan holds.
// This does not acknowledge a pending setter or submit the staged fan.
bool FieldAnimationGpu::restore() const{
 if(!current() || !camera_.restore(cameraSubmit()))return false;
 return !matrices_.cull || (physical_.cull && physical_.cull(physical_.user,*matrices_.cull));
}
bool FieldAnimationGpu::call(const AnimationRenderCall& q,AnimationRenderLocals& locals,uint32_t& result){
 using namespace field_animation_gx;
 if(!current() || active_)return false;
 if((matrixPending_ || cullPending_) && (!same(q,matrixRequest_) || matrixLocals_!=&locals))return false;
 // Position upload may wait for a real projection; retain its matrix before
 // physical callbacks. Normal upload preserves raw exceptional bits for lighting.
 if(q.function==PositionMatrix || q.function==NormalMatrix){
  if(q.count!=2 || q.arguments[0].kind!=K::Matrix || q.arguments[0].value>=locals.matrices.size() || q.arguments[1].kind!=K::Scalar || q.arguments[1].value!=0)return false;
  if(!matrixPending_){if(!locals.matrices[q.arguments[0].value])return false;matrixPending_=true;matrixRequest_=q;matrixLocals_=&locals;matrixInput_=*locals.matrices[q.arguments[0].value];}
  if(q.function==PositionMatrix){CameraViewMatrix view;for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c)view[r][c]=matrixInput_[4*r+c];if(!camera_.position(view,cameraSubmit()))return false;matrices_.position=matrixInput_;}
  else matrices_.normal=matrixInput_; // Source GX normal register: preserve exceptional bits; the lit consumer remains explicit.
  matrixPending_=false;return true;
 }
 if(q.function==CurrentMatrix){if(!args(q,{0}))return false;matrices_.current=0;return true;}
 if(q.function==Cull){
  if(q.count!=1 || q.arguments[0].kind!=K::Scalar || q.arguments[0].value>3)return false;
  if(!cullPending_){cullPending_=true;cullInput_=q.arguments[0].value;matrixRequest_=q;matrixLocals_=&locals;}
  if(!physical_.cull || !physical_.cull(physical_.user,cullInput_))return false;
  matrices_.cull=cullInput_;cullPending_=false;return true;
 }
 // GXClearVtxDesc clears enables only; array bases and VAT formats survive.
 if(q.function==ClearDescriptors){if(!args(q,{}))return false;for(auto& binding:bindings_)binding.descriptor=false;return true;}
 if(q.function==Descriptor){if(q.count!=2 || q.arguments[0].kind!=K::Scalar || q.arguments[1].kind!=K::Scalar || q.arguments[1].value!=Index16)return false;
  const int i=slot(q.arguments[0].value);if(i<0)return false;bindings_[i].descriptor=true;return true;}
 if(q.function==Format){
  if(q.count!=5 || q.arguments[1].kind!=K::Scalar)return false;
  const int i=slot(q.arguments[1].value);if(i<0)return false;
  // VAT 0: XYZ=1, NRM_XYZ=0, RGBA=1 or ST=1; fraction bits=0.
  if(!args(q,{0,q.arguments[1].value,i==1?0u:1u,i==2?Rgba8:Float32,0}))return false;
  bindings_[i].format=true;return true;
 }
 if(q.function==Array){
  if(q.count!=3 || q.arguments[0].kind!=K::Scalar || q.arguments[1].kind!=K::Guest || q.arguments[2].kind!=K::Scalar)return false;
  const int i=slot(q.arguments[0].value);if(i<0)return false;
  // Tightly packed source arrays: RGBA8=4 bytes, XYZ/normal=12, ST=8.
  const uint32_t stride=i==2?4:i<2?12:8;if(q.arguments[2].value!=stride)return false;
  bindings_[i].base=q.arguments[1].value;bindings_[i].stride=stride;bindings_[i].array=true;return true;
 }
 if(q.function==Begin){
  if(q.count!=3 || q.arguments[2].kind!=K::Scalar || !args(q,{TriangleFan,0,q.arguments[2].value}))return false;
  // Source narrows count to low16. Zero needs no matrices/arrays/FIFO.
  const uint32_t count=uint16_t(q.arguments[2].value);if(!count){count_=used_=0;return true;}
  if(!matrices_.position || !matrices_.normal || matrices_.current!=0 || !matrices_.cull)return false;
  for(unsigned i=0;i<3;++i)if(!bindings_[i].descriptor || !bindings_[i].format || !bindings_[i].array)return false;
  // Enabled UV arrays must be a contiguous prefix TEX0..TEXn.
  unsigned uv=0;bool gap=false;for(unsigned i=3;i<bindings_.size();++i){const auto& b=bindings_[i];if(b.descriptor){if(gap || !b.format || !b.array)return false;++uv;}else gap=true;}
  // Allocate once per new high-water count; failure holds without starting.
  // Short fans (1/2) still consume all indices, but cannot emit a triangle.
  if(count>capacity_){std::unique_ptr<FieldAnimationVertex[]> next(new(std::nothrow) FieldAnimationVertex[count]);if(!next)return false;vertices_=std::move(next);capacity_=count;}
  count_=count;used_=attribute_=component_=0;uvCount_=uv;indexPending_=ready_=false;active_=true;fanMatrices_=matrices_;return true;
 }
 return fallback_.call && fallback_.call(fallback_.user,q,locals,result);
}
bool FieldAnimationGpu::fifo(uint32_t address,uint16_t index){
 using namespace field_animation_gx;
 if(!current() || address!=Fifo || !active_ || matrixPending_ || cullPending_ || (indexPending_ && index!=index_))return false;
 if(!indexPending_){indexPending_=true;index_=index;component_=0;}
 if(!ready_){
  // Deliberately unsigned 32-bit guest address arithmetic. Parent already
  // applied shape/UV selection; do not add native offsets or dereference base.
  auto& v=vertices_[used_];const auto& b=bindings_[attribute_];const uint32_t base=b.base+uint32_t(index_)*b.stride;
  // Each FIFO halfword indexes ONE enabled attribute, not the whole vertex.
  // Read RGBA as four bytes; XYZ/normal/ST words are guest-endian binary32 bits.
  const unsigned width=attribute_==2?4:attribute_<2?3:2;
  // Completed components stay captured across sparse-memory holds; later
  // components remain live until read. A retry must keep the same index.
  while(component_<width){if(!current())return false;
   if(attribute_==2){uint8_t value;if(!memory_.readByte(base+component_,value))return false;v.color[component_]=value;}
   else {uint32_t value;if(!memory_.readWord(base+4*component_,value))return false;float x=real(value);if(attribute_==0)v.position[component_]=x;else if(attribute_==1)v.normal[component_]=x;else v.uv[attribute_-3][component_]=x;}
   ++component_;
  }
  if(++attribute_<3+uvCount_){indexPending_=false;return true;}
  attribute_=0;++used_;
  if(used_<count_){indexPending_=false;return true;}
  ready_=true;
 }
 // Complete fans are atomic submission units. A held final FIFO index retains
 // every decoded component; no immediate GPU Begin survives a yield.
 // Cull ALL=3 consumes the full source stream but emits nothing. Only
 // accepted real submissions increment counters; restore/retry do not.
 if(count_>=3 && fanMatrices_.cull!=3){
  const FieldAnimationBatch batch{vertices_.get(),count_,uvCount_,&fanMatrices_};
  if(!submit_.submit || !submit_.submit(submit_.user,batch))return false;
  ++submittedFans_;submittedVertices_+=count_;
 }
 active_=ready_=indexPending_=false;return true;
}
AnimationSourceRenderChildren FieldAnimationGpu::services(){return {this,[](void* u,const AnimationRenderCall& q,AnimationRenderLocals& l,uint32_t& r){return static_cast<FieldAnimationGpu*>(u)->call(q,l,r);},[](void* u,uint32_t a,uint16_t v){return static_cast<FieldAnimationGpu*>(u)->fifo(a,v);}};}
}
