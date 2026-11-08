#include "native_harbor_star_point_presentation.hpp"
#include <cmath>
namespace ttyd {
namespace {
constexpr unsigned SourceIconId=404,SourceFrameCount=4,SourceFrameDelay=6,SourceIconExtent=40;
constexpr float SourceWorldIconScale=.6f,NativeUnitsPerSourceUnit=.01f;
bool frame(const MessageIconFrame&f){
 return f.width==SourceIconExtent&&f.height==SourceIconExtent&&f.delay==SourceFrameDelay&&
  std::isfinite(f.u0)&&std::isfinite(f.u1)&&std::isfinite(f.v0)&&std::isfinite(f.v1)&&
  f.u0>=0&&f.u1<=1&&f.u0<f.u1&&f.v1>=0&&f.v0<=1&&f.v1<f.v0;
}
bool finiteClip(const BattleSceneMatrix&m,float x,float y,float z){
 const float vertex[]={x,y,z,1};
 for(const auto&row:m){float sum=0;for(unsigned column=0;column<4;++column){
  if(!std::isfinite(row[column]))return false;
  volatile float product=row[column]*vertex[column];if(!std::isfinite(product))return false;
  volatile float accumulated=sum+product;if(!std::isfinite(accumulated))return false;sum=accumulated;
 }}return true;
}
}
bool validNativeHarborStarPointIcon(const MessageIcon&icon,uint32_t width,uint32_t height){
 if(icon.id!=SourceIconId||icon.loop!=1||icon.frames.size()!=SourceFrameCount||width!=512||height!=1024)return false;
 for(const auto&f:icon.frames)if(!frame(f))return false;
 return true;
}
std::optional<NativeHarborStarPointGeometry>nativeHarborStarPointGeometry(
 const std::array<NativeHarborStarPointVisual::Instance,NativeHarborStarPointVisual::Capacity>&instances,
 const MessageIcon&icon,uint64_t ticks,uint32_t rgba,const BattleSceneMatrix&matrix){
 if(!validNativeHarborStarPointIcon(icon,512,1024))return {};
 const auto&image=icon.frame(ticks);NativeHarborStarPointGeometry out;
 const float left=-float(image.width)*.5f*SourceWorldIconScale,
  right=left+float(image.width)*SourceWorldIconScale,
  top=float(image.height)*SourceWorldIconScale;
 const float corners[4][2]={{left,top},{right,top},{right,0},{left,0}};
 const float uv[4][2]={{image.u0,image.v0},{image.u1,image.v0},{image.u1,image.v1},{image.u0,image.v1}};
 const std::array<float,4>color{{float(rgba>>24)/255,float((rgba>>16)&255)/255,float((rgba>>8)&255)/255,float(rgba&255)/255}};
 constexpr unsigned Triangles[]={0,1,2,0,2,3};
 for(size_t slot=0;slot<instances.size();++slot){const auto&i=instances[slot];if(!i.id)continue;
  if(i.request.subtype!=0||i.request.amount<0||size_t(i.request.amount)!=i.particles.size()||i.particles.size()>NativeHarborStarPointVisual::MaximumParticles)return {};
  for(float value:i.request.position)if(!std::isfinite(value))return {};
  const auto first=out.vertices.size();
  for(const auto&p:i.particles){if(!p.alpha)continue;
   for(float value:p.position)if(!std::isfinite(value))return {};
   std::array<NativeHarborStarPointVertex,4>quad;
   for(unsigned corner=0;corner<4;++corner){
    const float x=(i.request.position[0]+p.position[0]+corners[corner][0])*NativeUnitsPerSourceUnit;
    const float y=(i.request.position[1]+p.position[1]+corners[corner][1])*NativeUnitsPerSourceUnit;
    const float z=(i.request.position[2]+p.position[2])*NativeUnitsPerSourceUnit;
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(z)||!finiteClip(matrix,x,y,z))return {};
    quad[corner]={x,y,z,uv[corner][0],uv[corner][1],color};
   }
   for(unsigned index:Triangles)out.vertices.push_back(quad[index]);
  }
  out.ranges[slot]={unsigned(first),unsigned(out.vertices.size()-first)};
 }
 return out;
}
}
