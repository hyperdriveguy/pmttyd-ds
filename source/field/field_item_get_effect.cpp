#include "field_item_get_effect.hpp"
#include <cmath>
#include "camera_road_view.hpp"
namespace ttyd {
std::optional<FieldCameraFrame> itemGetCamera5Frame(CameraPoint eye,CameraPoint up,CameraPoint target) {
 auto frame=fieldCameraFrameFromView(eye,target,25,1,100);if(!frame)return std::nullopt;
 for(float value:{up.x,up.y,up.z})if(!std::isfinite(value))return std::nullopt;
 const float dx=eye.x-target.x,dy=eye.y-target.y,dz=eye.z-target.z,length=std::sqrt(dx*dx+dy*dy+dz*dz);
 const CameraPoint back{dx/length,dy/length,dz/length};
 CameraPoint right{up.y*back.z-up.z*back.y,up.z*back.x-up.x*back.z,up.x*back.y-up.y*back.x};
 const float side=std::sqrt(right.x*right.x+right.y*right.y+right.z*right.z);
 if(side==0 || !std::isfinite(side))return std::nullopt;
 right={right.x/side,right.y/side,right.z/side};
 const CameraPoint vertical{back.y*right.z-back.z*right.y,back.z*right.x-back.x*right.z,back.x*right.y-back.y*right.x};
 const auto row=[&](CameraPoint v){return std::array<float,4>{v.x,v.y,v.z,-v.x*eye.x-v.y*eye.y-v.z*eye.z};};
 frame->view={row(right),row(vertical),row(back)};
 frame->viewport.nearDepth=.2f;frame->viewport.farDepth=.3f;return frame;
}

void FieldItemGetEffect6::begin(ActorVector root) {
 root_=root;active_=true;softDelete_=false;age_=0;lifetime_=1000;alpha_=255;scale_=0;camera_=4;
}
bool FieldItemGetEffect6::tick() {
 if(!active_)return false;
 if(softDelete_){softDelete_=false;lifetime_=16;}
 if(lifetime_<1000)--lifetime_;
 if(lifetime_<16)alpha_=uint8_t(uint32_t(lifetime_)<<4);
 if(lifetime_<0){active_=false;return false;}
 ++age_;
 constexpr uint8_t curve[]={0,18,38,59,78,96,110,120,121,114,105,100};
 if(uint32_t(age_)<12)scale_=float(curve[age_])/100.f;
 if(age_>1000)age_=1000;
 return true;
}
ItemGetEffectDraw FieldItemGetEffect6::draw(float yaw,uint32_t shadow) const {
 ItemGetEffectDraw out;
 const float radians=-(yaw*.01745329238474369f),s=std::sin(radians),c=std::cos(radians);
 ActorMatrix rotate;rotate.v[0]=rotate.v[10]=c;rotate.v[2]=s;rotate.v[8]=-s;
 const float size=.75f*(12.f/10.f),scales[]={scale_*size,scale_*size,scale_*size};
 auto model=ActorMatrix::translation(root_.data())*rotate*ActorMatrix::scale(scales);
 const float turn=(yaw*6.2831854820251465f)/360.f;
 const float shift[]={2.f*std::sin(turn),0,-2.f*std::cos(turn)};
 model=ActorMatrix::translation(shift)*model;
 // Source projects POS into texture44 independently of camera view and the
 // later shadow-only local offset. Texture42 retains direct TEX0 coordinates.
 ActorMatrix textureView;textureView.v[0]=textureView.v[10]=c;
 textureView.v[2]=-s;textureView.v[8]=s;textureView.v[11]=-1000;
 ActorMatrix light;light.v[0]=1.f/6.f;light.v[5]=-1.f/6.f;light.v[7]=1;
 light.v[10]=0;light.v[11]=1;
 out.textureProjection=light*textureView*model;

 const float offset[]={2,-2,0};
 out.passes[0].model=model*ActorMatrix::translation(offset);
 out.passes[0].alpha=uint8_t((128u*alpha_)/255u);out.passes[0].color=(shadow&0xffffff00u)|out.passes[0].alpha;
 out.passes[1].model=model;out.passes[1].alpha=alpha_;out.passes[1].color=0xffffff00u|alpha_;
 return out;
}
}
