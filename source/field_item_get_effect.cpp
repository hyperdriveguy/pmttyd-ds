#include "field_item_get_effect.hpp"
#include <cmath>
namespace ttyd {
void FieldItemGetEffect6::begin(ActorVector root) {
 root_=root;active_=true;softDelete_=false;age_=0;lifetime_=1000;alpha_=255;scale_=0;layer_=4;
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
 const float offset[]={2,-2,0};
 out.passes[0].model=model*ActorMatrix::translation(offset);
 out.passes[0].alpha=uint8_t((128u*alpha_)/255u);out.passes[0].color=(shadow&0xffffff00u)|out.passes[0].alpha;
 out.passes[1].model=model;out.passes[1].alpha=alpha_;out.passes[1].color=0xffffff00u|alpha_;
 return out;
}
}
