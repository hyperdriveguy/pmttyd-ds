#include "field_map_sdk.hpp"
#include "animation_shape_render.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t VectorAdd=0x8029eb80,VectorSubtract=0x8029eba4,VectorScale=0x8029ebc8,VectorCross=0x8029eca4,SquareDistance=0x8029ee8c,Distance=0x8029eeb4;
constexpr uint32_t DistanceHalf=0x80429394,DistanceThree=0x80429398;
constexpr unsigned WordBytes=sizeof(uint32_t),VectorBytes=3*WordBytes;
float scalar(uint32_t w){float v;std::memcpy(&v,&w,WordBytes);return v;}
uint32_t bits(float v){uint32_t w;std::memcpy(&w,&v,WordBytes);return w;}
bool overlaps(uint32_t a,unsigned n,uint32_t b,unsigned m){return uint64_t(a)<uint64_t(b)+m && uint64_t(b)<uint64_t(a)+n;}
}
FieldMapSdk::FieldMapSdk(EventAddressSpace& m,std::function<bool()> current,Estimate estimate):memory_(m),current_(std::move(current)),matrices_(m,current_),estimate_(std::move(estimate)){}
bool FieldMapSdk::advance(FieldMapSourceCall& q){
 if(!current_ || !current_())return false;
 if(distance_.active)return finishDistance(q);
 const auto function=q.function;const auto& a=q.arguments;
 if(function!=animation_sdk::Identity && function!=animation_sdk::Translate && function!=animation_sdk::Scale && function!=animation_sdk::RotateRadians && function!=animation_sdk::Concat)return !matrices_.pending() && vector(q);
 AnimationSourceMatrixCall request;request.function=function;
 if(function==animation_sdk::Concat){request.left={true,a[0]};request.right={true,a[1]};request.output={true,a[2]};}
 else {
  if(!matrices_.pending())destination_=a[0];
  if(destination_!=a[0] || !memory_.contiguousRam(destination_,animation_sdk::MatrixBytes))return false;
  request.output={false,0};request.axis=a[1];for(unsigned i=0;i<3;++i)request.xyz[i]=scalar(q.scalars[i]);request.radians=scalar(q.scalars[0]);
 }
 AnimationSdkLocals local;local.user=this;local.count=1;
 local.store=[](void* u,uint32_t slot,const BattleActMatrix& m){auto& o=*static_cast<FieldMapSdk*>(u);if(slot || !o.current_() || !o.memory_.contiguousRam(o.destination_,animation_sdk::MatrixBytes))return false;for(unsigned i=0;i<m.size();++i)if(!o.memory_.writeWord(o.destination_+WordBytes*i,bits(m[i])))return false;return true;};
 return matrices_.advance(request,local);
}
bool FieldMapSdk::finishDistance(FieldMapSourceCall& q){
 if(q.function!=distance_.call.function || q.arguments!=distance_.call.arguments || q.scalars!=distance_.call.scalars || !estimate_)return false;
 const auto seed=estimate_(distance_.squared);if(!seed || !current_())return false;
 if(!std::isfinite(*seed) || *seed<=0)return false;
 // frsqrte delivers a binary64 estimate. Source fmuls narrows each product,
 // then fnmsubs fuses the correction before the two final single products.
 const float squared=float(*seed * *seed),half=float(*seed * double(distance_.half));
 const float correction=-std::fma(squared,distance_.squared,-distance_.three);
 const float refined=correction*half,result=distance_.squared*refined;
 if(!std::isfinite(squared) || !std::isfinite(half) || !std::isfinite(correction) || !std::isfinite(refined) || !std::isfinite(result))return false;
 q.floatingResult=bits(result);distance_={};return true;
}
bool FieldMapSdk::vector(FieldMapSourceCall& q){
 const auto& a=q.arguments;const auto function=q.function;
 const bool point=function==animation_sdk::MultiplyPoint;
 const bool binary=function==VectorAdd || function==VectorSubtract || function==VectorCross;
 const bool metric=function==SquareDistance || function==Distance;
 if(!point && !binary && function!=VectorScale && !metric)return false;
 const uint32_t first=a[0],second=a[1];
 if(!memory_.contiguousRam(first,point?animation_sdk::MatrixBytes:VectorBytes) || !memory_.contiguousRam(second,VectorBytes))return false;
 const uint32_t destination=point || binary?a[2]:function==VectorScale?a[1]:0;
 if(!metric && !memory_.contiguousRam(destination,VectorBytes))return false;
 // Stable native constructor domain: exact in-place vectors or disjoint
 // ranges. Reject partial aliases whose source SDK reads observe prior stores.
 if(point && (overlaps(first,animation_sdk::MatrixBytes,second,VectorBytes) || overlaps(first,animation_sdk::MatrixBytes,destination,VectorBytes)))return false;
 if(!point && !metric){
  if(destination!=first && overlaps(destination,VectorBytes,first,VectorBytes))return false;
  if(binary && destination!=second && overlaps(destination,VectorBytes,second,VectorBytes))return false;
 }
 std::array<float,12> left{};std::array<float,3> right{},out{};
 for(unsigned i=0;i<(point?12:3);++i){uint32_t w=0;if(!memory_.readWord(first+i*WordBytes,w) || !std::isfinite(scalar(w)))return false;left[i]=scalar(w);}
 if(function!=VectorScale)for(unsigned i=0;i<3;++i){uint32_t w=0;if(!memory_.readWord(second+i*WordBytes,w) || !std::isfinite(scalar(w)))return false;right[i]=scalar(w);}
 if(point){for(unsigned row=0;row<3;++row){const auto i=row*4;auto value=animationShapeNativePointRow({left[i],left[i+1],left[i+2],left[i+3]},right);if(!value)return false;out[row]=*value;}}
 else if(function==VectorAdd){for(unsigned i=0;i<3;++i)out[i]=left[i]+right[i];}
 else if(function==VectorSubtract){for(unsigned i=0;i<3;++i)out[i]=left[i]-right[i];}
 else if(function==VectorCross){
  const float yz=right[1]*left[2],xz=right[0]*left[2];
  out[0]=std::fma(left[1],right[2],-yz);out[1]=-std::fma(left[0],right[2],-xz);out[2]=-std::fma(left[1],right[0],-float(right[1]*left[0]));
  // Products/grouping follow the paired source, including final negation.
 }
 else if(function==VectorScale){const auto factor=scalar(q.scalars[0]);if(!std::isfinite(factor))return false;for(unsigned i=0;i<3;++i)out[i]=left[i]*factor;}
 else {
  // PSVECSquareDistance, NOT Distance: Y/Z products narrow first, then
  // fused X*X+Y*Y, then single +Z*Z. No square-root child is reached.
  const float dx=left[0]-right[0],dy=left[1]-right[1],dz=left[2]-right[2];
  const float yy=dy*dy,zz=dz*dz;const float value=std::fma(dx,dx,yy)+zz;
  if(!std::isfinite(value))return false;
  if(function==SquareDistance){q.floatingResult=bits(value);return true;}
  uint32_t half=0,three=0;if(!memory_.readWord(DistanceHalf,half))return false;
  const float h=scalar(half);if(!std::isfinite(h))return false;
  if(value==h-h){q.floatingResult=bits(value);return true;}
  if(value<=0 || !memory_.readWord(DistanceThree,three) || !std::isfinite(scalar(three)))return false;
  distance_={true,q,value,h,scalar(three)};return finishDistance(q);
 }
 for(float value:out)if(!std::isfinite(value))return false;
 for(unsigned i=0;i<out.size();++i)if(!memory_.writeWord(destination+i*WordBytes,bits(out[i])))return false;
 return true;
}
}
