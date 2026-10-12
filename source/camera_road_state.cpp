#include "camera_road_state.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
namespace {
constexpr float Pi=3.141592025756836f,WrapPi=3.1415927410125732f,TwoPi=6.2831854820251465f;
float elapsed(float rate,uint64_t before,uint64_t now){return rate*60*float(now-before)/1000;}
float magnitude(float x,float y,float z){return std::sqrt(x*x+y*y+z*z);}
float mix(float a,float b,float w){return std::fma(w,b-a,a);}
bool finite(CameraPoint p){return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);}
bool blendAdmission(const CameraRoadAsset& asset,const CameraRoadState& a,const CameraRoadState& b) {
    if(a.flags || b.flags)return true;
    for(const auto* s:{&a,&b})if(s->road &&
        ((asset.versionCode()==101 && *asset.roadWord(*s->road,0x20)!=0) || *asset.roadFloat(*s->road,0x7c)!=0))return true;
    return false;
}
bool calculateCandidate(const CameraRoadAsset& asset,CameraRoadWork& work,CameraPoint player,CameraPoint look,float shift,uint64_t now) {
    const auto selected=selectCameraRoadZone(asset,look);
    CameraCurveOutput curve;std::optional<uint32_t> road;
    if(selected) {
        const auto status=solveStraightCameraPath(asset,selected->road,player,curve);
        if(status==CameraPathStatus::Unsupported)return false;
        if(status==CameraPathStatus::Found)road=selected->road;
    }
    const bool zone=bool(road);
    const auto& previous=work.destination;
    if(!road) {
        if(!previous.active || !previous.road){work.failedPlayer=player;work.failedLook=look;return true;}
        road=previous.road;
    }
    CameraRoadState next=previous;
    next.active=true;next.zone=zone;next.flags=zone?curve.flags:0;next.road=road;
    next.player=player;next.look=look;next.shift=shift;next.milliseconds=now;
    next.shiftRate=*asset.roadFloat(*road,0x7c);
    if(*asset.roadWord(*road,0xb4)!=0)return false;
    if(zone) {
        const float length=magnitude(curve.tangent.x,0,curve.tangent.z);
        if(length==0 || !std::isfinite(length))return false;
        const float tx=curve.tangent.x/length,tz=curve.tangent.z/length;
        next.anchor=curve.anchor;next.direction={-tz,0,tx};next.segment=float(curve.segment);
        next.distance=*asset.roadFloat(*road,0x58);next.pitch=*asset.roadFloat(*road,0x64);next.secondary=*asset.roadFloat(*road,0x70);
        if(shift!=0 && previous.active) {
            if(now<previous.milliseconds)return false;
            const float theta=Pi*((*asset.lensMetadata())[0]*.5f/180);
            float k=next.distance*std::sin(theta);k/=std::cos(theta);k=640*k;k/=480;k=2*k;
            const auto query=[&](float value,CameraCurveOutput& out) {
                const float offset=.01f*(value*k);
                return solveStraightCameraPath(asset,*road,{player.x+offset*tx,1000000,player.z+offset*tz},out);
            };
            CameraCurveOutput a,b;const auto statusA=query(previous.shift,a),statusB=query(shift,b);
            if(statusA==CameraPathStatus::Unsupported || statusB==CameraPathStatus::Unsupported)return false;
            const auto pa=statusA==CameraPathStatus::Found?a.anchor:curve.anchor;
            const auto pb=statusB==CameraPathStatus::Found?b.anchor:curve.anchor;
            if(statusB==CameraPathStatus::Found) {
                next.flags=b.flags;
                if(!(b.flags&1) && (previous.flags&1))work.tangentWeight=0;
                if(!(b.flags&2) && (previous.flags&2))work.normalWeight=0;
            }
            const float travel=magnitude(pa.x-previous.anchor.x,0,pa.z-previous.anchor.z)+elapsed(.005f,previous.milliseconds,now);
            const float dx=pb.x-previous.anchor.x,dz=pb.z-previous.anchor.z;
            float alpha=tx*dx+tz*dz,beta=-tz*dx+tx*dz;
            const float tangentBudget=travel*work.tangentWeight,normalBudget=travel*work.normalWeight;
            if(std::fabs(alpha)>tangentBudget && std::fabs(alpha)>0)alpha=alpha<0?-tangentBudget:tangentBudget;
            if(std::fabs(beta)>normalBudget && std::fabs(beta)>0)beta=beta<0?-normalBudget:normalBudget;
            next.anchor.x=previous.anchor.x+alpha*tx-beta*tz;
            next.anchor.z=previous.anchor.z+alpha*tz+beta*tx;
        }
    } else {next.anchor.x=player.x;next.anchor.z=player.z;}
    next.anchor.y=asset.versionCode()==101 && *asset.roadWord(*road,0x20)!=0?*asset.roadFloat(*road,0x24):player.y;
    if(!finite(next.anchor) || !finite(next.direction) || !std::isfinite(next.distance) || !std::isfinite(next.pitch) || !std::isfinite(next.secondary))return false;
    work.tangentWeight=std::min(1.f,work.tangentWeight+.004f);
    work.normalWeight=std::min(1.f,work.normalWeight+.008f);
    work.candidate=next;return true;
}
}
uint32_t compareCameraRoad(const CameraRoadAsset& asset,const CameraRoadState& a,const CameraRoadState& b,float& rate) {
    if(!b.active)return 0;
    uint32_t mask=0;
    if(a.road==b.road && !((a.zone&&!b.zone)||(!a.zone&&b.flags==1)) && a.road) {
        const int k=std::abs(int(a.segment)-int(b.segment));
        const int span=int(*asset.roadWord(*a.road,0x44))-int(*asset.roadWord(*a.road,0x40));
        if(k<=1 || k>=span-1)return 0;
        if(k>=span-2)mask=0x20;
    }
    if(a.road!=b.road)mask|=1;
    if(a.zone&&!b.zone)mask|=2;
    if(!a.zone&&b.flags==1)mask|=4;
    if(std::fabs(double(a.segment)-b.segment)>1)mask|=8;
    const auto angleCost=[&](float angle,float factor) {
        if(angle>180)angle=360-angle;
        return ((TwoPi*a.distance)*angle/360)*factor;
    };
    const float dot=std::min(1.f,a.direction.x*b.direction.x+a.direction.z*b.direction.z);
    float cost=angleCost(std::fabs(std::acos(dot)*57.29578018f),2);
    cost=std::max(cost,angleCost(std::fabs(a.pitch-b.pitch),1));
    cost=std::max(cost,angleCost(std::fabs(a.secondary-b.secondary),1));
    cost=std::max(cost,std::fabs(a.distance-b.distance)*3);
    const float travel=magnitude(a.anchor.x-b.anchor.x,a.anchor.y-b.anchor.y,a.anchor.z-b.anchor.z);
    if(cost<.01f && travel<.3f)return 0;
    cost=std::max(cost,travel);rate=1/std::clamp(cost,20.f,90.f);
    return mask?mask:0x10;
}
void blendCameraRoad(bool anchorBlend,const CameraRoadState& o,const CameraRoadState& d,CameraRoadState& c,float progress) {
    const float w=std::min(1.f,.5000010132789612f*(1-std::cos(Pi*progress)));
    c.anchor=anchorBlend?CameraPoint{mix(o.anchor.x,d.anchor.x,w),mix(o.anchor.y,d.anchor.y,w),mix(o.anchor.z,d.anchor.z,w)}:d.anchor;
    float old=std::atan2(o.direction.x,o.direction.z),next=std::atan2(d.direction.x,d.direction.z);
    while(old-next>WrapPi)old-=TwoPi;
    while(old-next<-WrapPi)old+=TwoPi;
    const float angle=mix(old,next,w);c.direction={std::sin(angle),mix(o.direction.y,d.direction.y,w),std::cos(angle)};
    c.distance=mix(o.distance,d.distance,w);c.pitch=mix(o.pitch,d.pitch,w);c.secondary=mix(o.secondary,d.secondary,w);c.shiftRate=mix(o.shiftRate,d.shiftRate,w);
}
bool CameraRoadWork::update(const CameraRoadAsset& asset,CameraPoint player,CameraPoint look,float shift,uint64_t now) {
    if(!finite(player)||!finite(look)||!std::isfinite(shift)||now<transitionMilliseconds)return false;
    auto work=*this;
    player={player.x/10,player.y/10,player.z/10};look={look.x/10,look.y/10,look.z/10};
    if(!calculateCandidate(asset,work,player,look,shift,now))return false;
    const auto start=[&](uint32_t mask,const CameraRoadState& origin) {
        work.changeMask=mask;work.transitioning=true;work.transitionMilliseconds=now;work.progress=0;
        work.origin=origin;work.destination=work.candidate;
        work.anchorBlend=blendAdmission(asset,work.origin,work.destination);
    };
    if(work.transitioning) {
        const auto mask=compareCameraRoad(asset,work.destination,work.candidate,work.rate);
        if(mask)start(mask,work.current);else work.destination=work.candidate;
    }
    if(!work.transitioning) {
        const auto mask=compareCameraRoad(asset,work.candidate,work.destination,work.rate);
        if(mask)start(mask,work.destination);
    }
    if(work.transitioning) {
        work.progress=std::min(1.f,work.progress+elapsed(work.rate,work.transitionMilliseconds,now));
        work.transitionMilliseconds=now;
        blendCameraRoad(work.anchorBlend,work.origin,work.destination,work.current,work.progress);
        work.current.flags=work.anchorBlend?1:0;
        if(work.progress==1){work.current=work.destination;work.transitioning=false;}
    } else {work.destination=work.candidate;work.current=work.destination;}
    *this=work;return true;
}
}
