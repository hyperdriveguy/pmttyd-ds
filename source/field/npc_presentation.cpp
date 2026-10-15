#include "npc_presentation.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
const ActorAnimationPose* selectNpcAnimation(const ActorAnimationPack& pack,const char* name) {
    const auto* found=name?pack.pose(name):nullptr;return found?found:pack.firstPose();
}
// Clock regression resets retained requests. Base changes restart; a new
// message pose request uses its authored request time, not this draw visit.
// Repeated serials must not continuously restart the animation.
float NpcPoseClock::update(const std::string& base,const std::string& talk,const MessagePlayback* message,uint64_t now) {
    if(now<poseAt)*this={};
    if(basePose!=base){basePose=pose=base;poseAt=now;}
    if(message && !talk.empty() && message->poseRequestSerial && message->poseRequestedAt<=now &&
       (messageSerial!=message->serial || requestSerial!=message->poseRequestSerial)) {
        messageSerial=message->serial;requestSerial=message->poseRequestSerial;poseAt=message->poseRequestedAt;
        pose=message->poseRequest==MessagePlayback::PoseRequest::Talk?talk:base;
    }
    return 60.f*float(now-poseAt)/1000.f;
}
// PMNANG01 private LE pack: 91 finite positive coefficients<=2, exact EOF.
// Bounds validate the retained original table, not an inferred atan formula.
bool NpcAngleTable::load(const char* path) {
    if(!path)return false;
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    char magic[8];std::array<float,91> next{};
    bool ok=std::fread(magic,8,1,file)==1 && !std::memcmp(magic,"PMNANG01",8) &&
        std::fread(next.data(),sizeof(next),1,file)==1 && std::fgetc(file)==EOF;std::fclose(file);
    for(auto f:next)if(!std::isfinite(f) || f<=0 || f>2)ok=false;
    if(!ok)return false;
    values=next;return true;
}
// Minor/major ratio*45, rounded ratio*2 -> sample0..90; sign/quadrant maps
// to degrees with source -Z zero. Coincident roots return0. Do not replace
// this approximation with atan2 in collision/talk direction comparisons.
float NpcAngleTable::angle(float fromX,float fromZ,float toX,float toZ) const {
    const float dx=toX-fromX,dz=toZ-fromZ,ax=std::fabs(dx),az=std::fabs(dz);
    assert(std::isfinite(dx) && std::isfinite(dz));
    if(dx==0 && dz==0)return 0;
    const bool wide=ax>az;
    const float ratio=(wide?az/ax:ax/az)*45.f;
    const unsigned index=unsigned(double(ratio*2.f)+.5);assert(index<91);
    const float minor=ratio*values[index];
    if(wide)return dx>=0?(dz>=0?90.f+minor:90.f-minor):(dz>=0?270.f-minor:270.f+minor);
    const float a=dz>=0?180.f-minor:minor;return dx>=0?a:360.f-a;
}
// fmod uses double then narrows; NaN becomes0 and negative wraps once.
// This differs from finite-only while-loop heading helpers elsewhere.
float npcReviseAngle(float value) {
    float result=float(std::fmod(double(value),360.));
    if(std::isnan(result))result=0;
    if(result<0){result+=360;if(result>=360)result=0;}
    return result;
}
namespace {
float heading(const NpcCamera& camera) {
    float k=float(std::atan2(-(camera.at.x-camera.eye.x),-(camera.at.z-camera.eye.z)))*57.29579162597656f;
    while(k<-360)k+=360;
    while(k>=360)k-=360;
    if(k<0)k+=360;
    return k;
}
float difference(float value) {while(value<-360)value+=360;while(value>=360)value-=360;return value;}
// Rear half-plane keeps asymmetric boundaries: 90 excluded, 270 included
// (and corresponding negative bounds). Exact endpoints affect reflection.
bool rear(float d){return (d>90 && d<=270) || (d<-90 && d>=-270);}
ActorMatrix rotate(unsigned axis,float degrees) {
    ActorMatrix out;const float radians=degrees*.01745329238474369f;
    const float s=std::sin(radians),c=std::cos(radians);const int a=(axis+1)%3,b=(axis+2)%3;
    out.v[a*4+a]=out.v[b*4+b]=c;out.v[a*4+b]=-s;out.v[b*4+a]=s;return out;
}
}
// Validate before publication. Flags02000000/08000000 skip projection
// correction, not later paper Main camera requests. Reverse Z scale when
// additional Y rotation lies in inclusive[90, 270]. Matrix order is pivot,
// Z/X/Y additions, root, retained facing/reflection, uniform draw scalar.
bool NpcPresentation::prepare(const NpcMatrixInput& input,const NpcAngleTable& table,const NpcCameraService& service) {
    if(!service.read)return false;
    for(auto p:{input.root,input.pivot,input.scale,input.rotation})for(float f:p)if(!std::isfinite(f))return false;
    if(!std::isfinite(input.yaw) || !std::isfinite(input.drawScale))return false;
    float correction=0;
    if(!(input.flags&(0x02000000u|0x08000000u))) {
        const auto camera=service.read(service.user);
        correction=npcReviseAngle(table.angle(camera.eye.x,camera.eye.z,camera.at.x,camera.at.z)-
            table.angle(camera.eye.x,camera.eye.z,input.root[0],input.root[2]));
    }
    auto scale=input.scale;const float extra=npcReviseAngle(npcReviseAngle(input.rotation[1]));
    if(extra>=90 && extra<=270)scale[2]=-scale[2];
    ActorMatrix base=ActorMatrix::scale(scale.data());
    const float negative[]={-input.pivot[0],-input.pivot[1],-input.pivot[2]};
    base=ActorMatrix::translation(negative)*base;
    base=rotate(2,input.rotation[2])*base;base=rotate(0,input.rotation[0])*base;
    base=rotate(1,input.rotation[1]+correction)*base;
    base=ActorMatrix::translation(input.pivot.data())*base;base=ActorMatrix::translation(input.root.data())*base;
    // Ordinary npcDisp calls Main before the admitted draw. Camera requests in
    // Main and both Inner branches stay separate and preserve their order.
    if(flags&8) {
        sign=rear(difference(heading(service.read(service.user))-retained))?-1.f:1.f;
        incoming=npcReviseAngle(-input.yaw)-90.f;
        const bool back=rear(difference(heading(service.read(service.user))-incoming));
        target=heading(service.read(service.user));
        if(back)target=difference(target+180);
        while(target-retained>180)retained+=360;
        while(target-retained<-180)retained-=360;
        // Adjust old retained angle to within180deg before source .1 easing.
        // Reflection sign was computed from PREVIOUS retained facing; do not
        // derive it again from the just-updated target.
        retained=(flags&0x100)?target:std::fma(.10000000149011612f,target-retained,retained);
        base=base*rotate(1,retained);
        if(sign==-1){const float reflection[]={1,1,-1};base=base*ActorMatrix::scale(reflection);}
    } else base=base*rotate(1,npcReviseAngle(-input.yaw));
    const float uniform[]={input.drawScale,input.drawScale,input.drawScale};matrix=base*ActorMatrix::scale(uniform);return true;
}
}
