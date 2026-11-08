#include "field_map_sdk.hpp"
#include "animation_shape_render.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t VectorAdd=0x8029eb80,VectorScale=0x8029ebc8,SquareDistance=0x8029ee8c;
constexpr unsigned WordBytes=sizeof(uint32_t),VectorBytes=3*WordBytes;
float scalar(uint32_t w){float v;std::memcpy(&v,&w,WordBytes);return v;}
uint32_t bits(float v){uint32_t w;std::memcpy(&w,&v,WordBytes);return w;}
bool overlaps(uint32_t a,unsigned n,uint32_t b,unsigned m){return uint64_t(a)<uint64_t(b)+m && uint64_t(b)<uint64_t(a)+n;}
}
FieldMapSdk::FieldMapSdk(EventAddressSpace& m,std::function<bool()> current):memory_(m),current_(std::move(current)),matrices_(m,current_){}
bool FieldMapSdk::advance(FieldMapSourceCall& q){
 if(!current_ || !current_())return false;
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
bool FieldMapSdk::vector(FieldMapSourceCall& q){
 const auto& a=q.arguments;const auto function=q.function;
 const bool point=function==animation_sdk::MultiplyPoint;
 if(!point && function!=VectorAdd && function!=VectorScale && function!=SquareDistance)return false;
 const uint32_t first=a[0],second=a[1];
 if(!memory_.contiguousRam(first,point?animation_sdk::MatrixBytes:VectorBytes) || !memory_.contiguousRam(second,VectorBytes))return false;
 const uint32_t destination=point || function==VectorAdd?a[2]:function==VectorScale?a[1]:0;
 if(function!=SquareDistance && !memory_.contiguousRam(destination,VectorBytes))return false;
 // Stable native constructor domain: exact in-place vectors or disjoint
 // ranges. Reject partial aliases whose source SDK reads observe prior stores.
 if(point && (overlaps(first,animation_sdk::MatrixBytes,second,VectorBytes) || overlaps(first,animation_sdk::MatrixBytes,destination,VectorBytes)))return false;
 if(!point && function!=SquareDistance){
  if(destination!=first && overlaps(destination,VectorBytes,first,VectorBytes))return false;
  if(function==VectorAdd && destination!=second && overlaps(destination,VectorBytes,second,VectorBytes))return false;
 }
 std::array<float,12> left{};std::array<float,3> right{},out{};
 for(unsigned i=0;i<(point?12:3);++i){uint32_t w=0;if(!memory_.readWord(first+i*WordBytes,w) || !std::isfinite(scalar(w)))return false;left[i]=scalar(w);}
 if(function!=VectorScale)for(unsigned i=0;i<3;++i){uint32_t w=0;if(!memory_.readWord(second+i*WordBytes,w) || !std::isfinite(scalar(w)))return false;right[i]=scalar(w);}
 if(point){for(unsigned row=0;row<3;++row){const auto i=row*4;auto value=animationShapeNativePointRow({left[i],left[i+1],left[i+2],left[i+3]},right);if(!value)return false;out[row]=*value;}}
 else if(function==VectorAdd){for(unsigned i=0;i<3;++i)out[i]=left[i]+right[i];}
 else if(function==VectorScale){const auto factor=scalar(q.scalars[0]);if(!std::isfinite(factor))return false;for(unsigned i=0;i<3;++i)out[i]=left[i]*factor;}
 else {
  // PSVECSquareDistance, NOT Distance: Y/Z products narrow first, then
  // fused X*X+Y*Y, then single +Z*Z. No square-root child is reached.
  const float dx=left[0]-right[0],dy=left[1]-right[1],dz=left[2]-right[2];
  const float yy=dy*dy,zz=dz*dz;const float value=std::fma(dx,dx,yy)+zz;
  if(!std::isfinite(value))return false;
  q.floatingResult=bits(value);return true;
 }
 for(float value:out)if(!std::isfinite(value))return false;
 for(unsigned i=0;i<out.size();++i)if(!memory_.writeWord(destination+i*WordBytes,bits(out[i])))return false;
 return true;
}
}
