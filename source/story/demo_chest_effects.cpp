#include "demo_chest_effects.hpp"
#include "actor_animation.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
namespace ttyd {
namespace {
struct Assets {std::array<DemoEffectVertex,11> vertices;std::array<std::array<int32_t,5>,16> rays;std::array<float,10> crystal;std::array<float,15> radiation;std::array<uint32_t,2> colors;};
static_assert(sizeof(Assets)==692,"Original effect data export layout");
Assets assets{};bool ready=false;
ActorMatrix rotate(unsigned axis,float degrees) {
    const float angle=degrees*assets.crystal[0],s=std::sin(angle),c=std::cos(angle);
    ActorMatrix m;const unsigned a=(axis+1)%3,b=(axis+2)%3;m.v[a*4+a]=m.v[b*4+b]=c;m.v[a*4+b]=-s;m.v[b*4+a]=s;return m;
}
DemoEffectDraw geometry(const DemoEffectVertex* source,unsigned count,const ActorMatrix& matrix,int texture,bool textureColor,std::array<float,4> color) {
    DemoEffectDraw draw;draw.texture=texture;draw.textureColor=textureColor;draw.count=count==4?6:3;
    static constexpr unsigned quad[]={0,1,2,0,2,3};
    for(unsigned n=0;n<draw.count;++n){const auto& v=source[count==4?quad[n]:n];auto& out=draw.vertices[n];out.xyz=matrix.point(v.xyz);out.uv=v.uv;out.color=color;
        if(texture<0){out.color[3]=float(v.color[3])/255;}
    }return draw;
}
int fade(int32_t remaining,uint32_t age) {int value=age<16?int(age*16)-1:255;if(remaining<16)value=value*remaining/16;return value;}
}
bool loadDemoChestEffectAssets(const char* root) {
    if(!root)return false;
    FILE* f=std::fopen((std::string(root)+"/demo-effects.bin").c_str(),"rb");if(!f)return false;
    Assets next{};char magic[8];bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMDEFX01",8) && std::fread(&next,sizeof(next),1,f)==1 && std::fgetc(f)==EOF;std::fclose(f);
    for(const auto& v:next.vertices)for(float x:v.xyz)ok=ok && std::isfinite(x);
    for(const auto& v:next.vertices)for(float x:v.uv)ok=ok && std::isfinite(x);
    for(float x:next.crystal)ok=ok && std::isfinite(x);
    for(float x:next.radiation)ok=ok && std::isfinite(x);
    if(!ok)return false;
    assets=next;ready=true;return true;
}
void DemoChestEffects::requestCrystals(uint64_t now,std::array<float,3> point) {
    if(!ready || started){failed=true;return;}started=true;tick=now;
    for(unsigned i=0;i<2;++i){auto& c=crystals[i];c.active=true;c.type=i?6:2;c.remaining=i?60:100;c.position=point;
        requests.push_back({i?0x39d4u:0x3994u,c.type,5,now,point,.5f,i?60:0});}
    // Source outer order is evtmgrMain before effMain. Fresh admitted slots
    // receive their first Main in the creation update, not one update later.
    updateCrystals();
}
void DemoChestEffects::resetRay(unsigned i) {
    auto& r=rays[i];const auto& row=assets.rays[i];r.remaining=32+2*int(i);r.age=0;r.alpha=0;
    for(unsigned k=0;k<3;++k)r.angle[k]=float(row[k]);
    r.zVelocity=assets.radiation[13]*float(row[4]);
}
void DemoChestEffects::requestRadiation(uint64_t now,std::array<float,3> point) {
    if(!ready || !started || radiationActive){failed=true;return;}advance(now);radiationActive=true;radiationPosition=point;
    for(unsigned i=0;i<16;++i)resetRay(i);
    requests.push_back({0x3a24,3,5,now,point,.5f,0});
    updateRadiation();
}
void DemoChestEffects::updateCrystals() {
    for(auto& c:crystals){if(!c.active)continue;if(c.remaining<100)--c.remaining;++c.age;
        if(c.remaining<0){c.active=false;continue;}const int f=fade(c.remaining,c.age);
        if(c.type==2){float angle=(float(c.age*2)*assets.crystal[4])/assets.crystal[5];float sine=float(std::sin(double(angle)));const float wave=assets.crystal[7]*sine;c.alpha=(215+int(wave))*f/255;}
        else {c.alpha=160*f/255;c.scale=c.scale*assets.crystal[8];}
    }
}
void DemoChestEffects::updateRadiation() {
    if(radiationActive)for(unsigned i=0;i<16;++i){auto& r=rays[i];--r.remaining;++r.age;
        if(r.age<5)r.alpha=r.age*50;
        if(r.remaining<5)r.alpha=r.remaining*50;
        if(r.remaining<=0)resetRay(i);
        r.angle[2]=r.angle[2]+r.zVelocity;
    }
}
void DemoChestEffects::update() {updateCrystals();updateRadiation();}
void DemoChestEffects::advance(uint64_t now) {if(!started || now<tick)return;while(tick<now){++tick;update();}}
std::vector<DemoEffectDraw> DemoChestEffects::draw(float cameraYaw) const {
    std::vector<DemoEffectDraw> out;if(!ready || failed)return out;out.reserve(19);
    for(const auto& c:crystals){if(!c.active || !c.age)continue;const float scale[]={c.scale,c.scale,c.scale};const auto base=(ActorMatrix::translation(c.position.data())*rotate(1,-cameraYaw))*ActorMatrix::scale(scale);
        const std::array<float,4> color=c.type==2?std::array<float,4>{235/255.f,235/255.f,200/255.f,c.alpha/255.f}:std::array<float,4>{180/255.f,178/255.f,174/255.f,c.alpha/255.f};
        if(c.type==2){for(unsigned layer=0;layer<2;++layer){auto d=geometry(assets.vertices.data()+layer*4,4,base*rotate(2,layer?-8.f*float(c.age):float(c.age)),109,true,color);
            for(unsigned n=0;n<d.count;++n){d.vertices[n].uv[0]*=assets.crystal[1];d.vertices[n].uv[1]*=assets.crystal[2];}out.push_back(d);}}
        else {auto d=geometry(assets.vertices.data()+4,4,base,110,false,color);for(unsigned n=0;n<d.count;++n)for(float& v:d.vertices[n].uv)v*=assets.crystal[2];out.push_back(d);}
    }
    if(radiationActive){const auto t=ActorMatrix::translation(radiationPosition.data());
        const uint32_t word=assets.colors[0];const std::array<float,4> color={float(word>>24)/255,float((word>>16)&255)/255,float((word>>8)&255)/255,1};
        for(unsigned i=0;i<16;++i){const auto& r=rays[i];const float fraction=float(r.alpha)/assets.radiation[2];float width=fraction;
            switch((i+1)%4){case 0:width*=assets.radiation[6];break;case 1:width*=assets.radiation[7];break;case 2:width*=assets.radiation[8];break;default:break;}
            const float scale[]={assets.radiation[4]*(assets.radiation[5]+fraction),width,assets.radiation[4]*(assets.radiation[5]+fraction)};
            const auto matrix=(((t*rotate(0,r.angle[0]))*rotate(2,r.angle[2]+45))*rotate(1,r.angle[1]))*ActorMatrix::scale(scale);
            auto d=geometry(assets.vertices.data()+8,3,matrix,-1,false,color);out.push_back(d);
        }
    }return out;
}
}
