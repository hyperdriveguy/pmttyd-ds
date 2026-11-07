#include "animation_render_sdk_binding.hpp"
#include "animation_shape_render.hpp"
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
bool arg(const AnimationRenderCall& q,unsigned i,AnimationRenderKind kind,uint64_t limit=0x100000000ull){return i<q.count && q.arguments[i].kind==kind && q.arguments[i].value<limit;}
bool same(const AnimationRenderCall& a,const AnimationRenderCall& b){if(a.function!=b.function || a.count!=b.count || a.count>8)return false;for(unsigned i=0;i<a.count;++i)if(a.arguments[i].kind!=b.arguments[i].kind || a.arguments[i].value!=b.arguments[i].value)return false;return true;}
}
AnimationRenderSdkBinding::AnimationRenderSdkBinding(EventAddressSpace& m,std::shared_ptr<const CameraManager> c,const AnimationSourceRenderChildren& b,std::function<bool()> current):memory_(m),cameras_(std::move(c)),backend_(b),current_(std::move(current)),matrices_(m,current_,{this,[](void* u,uint32_t a){return static_cast<AnimationRenderSdkBinding*>(u)->owns(a);},[](void* u,uint32_t a,std::array<uint32_t,2>& words){return static_cast<AnimationRenderSdkBinding*>(u)->cameraPair(a,words);}}){}
bool AnimationRenderSdkBinding::owns(uint32_t a) const{return a>=CameraBase && uint64_t(a)<uint64_t(CameraBase)+CameraSize;}
bool AnimationRenderSdkBinding::available() const{return current_ && current_() && memory_.unmappedRange(CameraBase,CameraSize);}
bool AnimationRenderSdkBinding::cameraPair(uint32_t a,std::array<uint32_t,2>& out) const{
 if(!available() || !cameras_ || !owns(a))return false;
 const uint32_t index=(a-CameraBase)/CameraStride,offset=(a-CameraBase)%CameraStride;
 const auto* c=cameras_->camera(index);if(!c || !c->viewPublished || offset<CameraViewOffset || offset>CameraViewOffset+animation_sdk::LastPairOffset || (offset-CameraViewOffset)%sizeof(float))return false;
 const unsigned element=(offset-CameraViewOffset)/sizeof(float);out={bits(c->matrices.view[element/4][element%4]),bits(c->matrices.view[(element+1)/4][(element+1)%4])};return true;
}
AnimationSdkLocals AnimationRenderSdkBinding::storage(AnimationRenderLocals& local){
 AnimationSdkLocals s;s.user=&local;s.count=local.matrices.size();
 s.pair=[](void* u,uint32_t slot,unsigned offset,std::array<uint32_t,2>& out){auto& l=*static_cast<AnimationRenderLocals*>(u);if(slot>=l.matrices.size() || !l.matrices[slot] || offset>animation_sdk::LastPairOffset || offset%4)return false;const auto& m=*l.matrices[slot];out={bits(m[offset/4]),bits(m[offset/4+1])};return true;};
 s.store=[](void* u,uint32_t slot,const BattleActMatrix& m){auto& l=*static_cast<AnimationRenderLocals*>(u);if(slot>=l.matrices.size())return false;l.matrices[slot]=m;return true;};return s;
}
bool AnimationRenderSdkBinding::point(const AnimationRenderCall& incoming,AnimationRenderLocals& local){
 if(incoming.count!=3 || !arg(incoming,0,AnimationRenderKind::Guest) || !arg(incoming,1,AnimationRenderKind::Vector,2) || !arg(incoming,2,AnimationRenderKind::Vector,2))return false;
 if(!pointActive_){pointActive_=true;pointRequest_=incoming;pointLocals_=&local;pointStage_=CaptureVectorXY;}
 if(pointLocals_!=&local || !same(incoming,pointRequest_))return false;
 const auto& q=pointRequest_;auto& locals=*pointLocals_;
 auto pair=[&](unsigned offset){std::array<uint32_t,2> words;if(!matrices_.pair({true,q.arguments[0].value},offset,words,storage(locals)))return false;matrix_[offset/4]=real(words[0]);matrix_[offset/4+1]=real(words[1]);return true;};
 auto row=[&](unsigned i){return animationShapeNativePointRow({matrix_[i],matrix_[i+1],matrix_[i+2],matrix_[i+3]},vector_);};
 for(;;)switch(pointStage_){
 case CaptureVectorXY:vector_[0]=real(locals.vectors[q.arguments[1].value][0]);vector_[1]=real(locals.vectors[q.arguments[1].value][1]);pointStage_=CaptureRow0XY;break;
 case CaptureRow0XY:if(!pair(0))return false;pointStage_=CaptureVectorZ;break;
 case CaptureVectorZ:vector_[2]=real(locals.vectors[q.arguments[1].value][2]);pointStage_=CaptureRow0ZT;break;
 case CaptureRow0ZT:if(!pair(8))return false;pointStage_=CaptureRow1XY;break;
 case CaptureRow1XY:if(!pair(0x10))return false;pointStage_=CaptureRow1ZT;break;
 case CaptureRow1ZT:if(!pair(0x18))return false;pointStage_=StoreX;break;
 case StoreX:{auto x=row(0);if(!x)return false;locals.vectors[q.arguments[2].value][0]=bits(*x);pointStage_=ComputeY;break;}
 case ComputeY:{auto y=row(4);if(!y)return false;y_=*y;pointStage_=CaptureRow2XY;break;}
 case CaptureRow2XY:if(!pair(0x20))return false;pointStage_=CaptureRow2ZT;break;
 case CaptureRow2ZT:if(!pair(0x28))return false;pointStage_=StoreY;break;
 case StoreY:locals.vectors[q.arguments[2].value][1]=bits(y_);pointStage_=StoreZ;break;
 case StoreZ:{auto z=row(8);if(!z)return false;locals.vectors[q.arguments[2].value][2]=bits(*z);pointActive_=false;return true;}
 default:return false;
 }
}
bool AnimationRenderSdkBinding::call(const AnimationRenderCall& q,AnimationRenderLocals& local,uint32_t& result){
 if(!available())return false;
 // A partial point child cannot be replaced with another operation/local owner.
 if(pointActive_ && (pointLocals_!=&local || !same(q,pointRequest_)))return false;
 if(matrices_.pending() && q.function!=animation_sdk::Concat)return false;
 switch(q.function){
 case CurrentCameraGetter:{if(q.count || !cameras_)return false;auto i=cameras_->currentIndex();if(!i || !cameras_->camera(*i))return false;result=CameraBase+CameraStride*(*i);return true;}
 case CameraByIndexGetter:if(q.count!=1 || !arg(q,0,AnimationRenderKind::Scalar,CameraCount) || !cameras_ || !cameras_->camera(q.arguments[0].value))return false;result=CameraBase+CameraStride*q.arguments[0].value;return true;
 case animation_sdk::Concat:{if(q.count!=3 || !arg(q,0,AnimationRenderKind::Guest) || !arg(q,1,AnimationRenderKind::Guest) || !arg(q,2,AnimationRenderKind::Matrix,2))return false;AnimationSourceMatrixCall request;request.function=q.function;request.left={true,q.arguments[0].value};request.right={true,q.arguments[1].value};request.output={false,q.arguments[2].value};return matrices_.advance(request,storage(local));}
 case animation_sdk::InverseTranspose:{if(q.count!=2 || !arg(q,0,AnimationRenderKind::Matrix,2) || !arg(q,1,AnimationRenderKind::Matrix,2) || !local.matrices[q.arguments[0].value])return false;
  AnimationShapeMath math;math.function=q.function;const auto& input=*local.matrices[q.arguments[0].value];for(unsigned row=0;row<3;++row)for(unsigned col=0;col<3;++col)math.input[row*4+col]=input[row*4+col];
  if(!animationShapeNativeMath(math))return false;
  if(math.status)local.matrices[q.arguments[1].value]=math.output;
  result=math.status;return true;}
 case animation_sdk::MultiplyPoint:return point(q,local);
 default:return backend_.call && backend_.call(backend_.user,q,local,result);
 }
}
AnimationSourceRenderChildren AnimationRenderSdkBinding::services(){AnimationSourceRenderChildren s;s.user=this;
 s.call=[](void* u,const AnimationRenderCall& q,AnimationRenderLocals& local,uint32_t& result){return static_cast<AnimationRenderSdkBinding*>(u)->call(q,local,result);};
 s.fifo=[](void* u,uint32_t a,uint16_t v){auto& b=*static_cast<AnimationRenderSdkBinding*>(u);return b.available() && !b.pointActive_ && !b.matrices_.pending() && b.backend_.fifo && b.backend_.fifo(b.backend_.user,a,v);};return s;
}
}
