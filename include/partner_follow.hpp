#pragma once
#include "field_camera_state.hpp"
#include <functional>
#include <vector>

namespace ttyd {
// Ordinary ground partner adapter. Source numeric owners are specified in the
// partner jump/fall contracts; trail/terrain composition is explicitly scoped
// in FIELD_PARTNER_NATIVE_FOLLOW.md. Coordinates use native scale .01.
struct PartnerFollowServices {
    std::function<bool(CameraPoint)> free;
    std::function<bool(CameraPoint,float,float,float&)> floor; // min/max Y
    std::function<CameraScreenCache(CameraPoint)> project;
    CameraPoint ownerDirection{0,0,-1};
};
struct PartnerFollow {
    enum class Motion { Ground, Jump, Fall };
    struct Mark {CameraPoint point;bool jump=false;};
    std::vector<Mark> trail;
    Motion motion=Motion::Ground;
    float velocity=0,acceleration=0;
    float horizontalSpeed=0,headingX=0,headingZ=-1;
    unsigned blocked=0;
    int jumpCount=0;
    bool sourceJumping=false,initialized=false,warped=false,homing=false;
    CameraPoint lastSource;
    static float distance(CameraPoint a,CameraPoint b){return std::hypot(a.x-b.x,a.z-b.z);}
    // These are speed-control thresholds, not a required resting gap.
    static float nearThreshold(float targetActivity){return targetActivity==0?.45f:.5f;}
    static float historyReach(float speed){return .5f*.26f+speed;}
    static float acquisitionDistance(float ownerSpeed){return ownerSpeed==0?1.f:.5f;}
    static float adjustedSpeed(float current,float target) {
        float s=current*100,t=target*100;
        if(t>0)s=std::min(t,std::fma(.05000000074505806f,(.5f+t)-s,s));
        else s=std::max(0.f,std::fma(.10000000149011612f,-1.f-s,s));
        return s*.01f;
    }
    static float catchupSpeed(float ownerSpeed,float separation) {
        float extra=std::min(1.5f,(separation*100.f-45.f)*.10000000149011612f);
        if(extra<1.f)extra=0;
        return (ownerSpeed*100.f+extra)*.01f;
    }
    void correctOverlap(CameraPoint& root,CameraPoint owner,float speed,const PartnerFollowServices& s) {
        const float dx=root.x-owner.x,dz=root.z-owner.z;
        if(std::fabs(dx)>.4f*(.26f+.2f) || std::fabs(dz)>.3f*(.26f+.2f) ||
           std::fabs(root.y-owner.y)>.4f*(.24f+.37f))return;
        auto angle=[](float x,float z){float a=std::atan2(x,-z)*57.29577951308232f;return a<0?a+360:a;};
        float bearing=angle(dx,dz);
        if(speed!=0) {
            const float delta=bearing-angle(s.ownerDirection.x,s.ownerDirection.z);
            if(std::fabs(delta)<=4)bearing+=delta>=0?90:-90;
        }
        const float magnitude=speed>.01f?.800000011920929f*speed:speed==0?.04f:.01f;
        const float radians=bearing*.01745329238474369f;
        root=move(root,{root.x+std::sin(radians),root.y,root.z-std::cos(radians)},magnitude,s);
    }
    // partySearchGround's five probes and finite ordinary Goombella domain.
    // The terrain callback is the native static-room ray adapter.
    bool support(CameraPoint root,float depth,float dropLimit,float& height,const PartnerFollowServices& s) const {
        constexpr float width=.26f,bodyHeight=.24f;
        const float a=.375f*width*headingX,b=.375f*width*headingZ;
        const CameraPoint offsets[]={{0,0,0},{a,0,b},{-b,0,a},{b,0,-a},{-a,0,-b}};
        bool found=false;float best=-100.f;
        for(auto offset:offsets) {
            CameraPoint p{root.x+offset.x,root.y,root.z+offset.z};float h=root.y;
            if(!s.floor(p,root.y-std::fabs(depth)-.001f,root.y+.75f*bodyHeight,h))continue;
            const float engine=h*100.f;
            h=std::trunc(std::fma(engine,1000.f,engine>=0?.5f:-.5f))*.00001f;
            const float drop=root.y-h;
            const bool admitted=drop>=.02f?drop<=std::fabs(dropLimit):std::fabs(drop)<=.11f;
            if(admitted && h>best){best=h;found=true;}
        }
        if(found)height=best;
        return found;
    }
    void reset(CameraPoint root,CameraPoint owner) {
        *this=PartnerFollow{};initialized=true;lastSource=owner;
        trail.push_back({root});trail.push_back({owner});
    }
    void beginJump(CameraPoint root,CameraPoint target) {
        const float d=distance(root,target)*100;
        float r=(d/2.25f)*.9f;
        jumpCount=std::clamp(int(std::fma(10.f,r,.5f)/10.f),12,40);
        const float a=.5f*jumpCount,delta=(target.y-root.y)*100;
        const float h=std::fabs(delta)<300?25:delta>=0?delta:10;
        const float c=(h+h)/std::fma(a,a,-a);
        velocity=std::clamp(c*a,3.f,13.f)*.01f;acceleration=-c*.01f;
        horizontalSpeed=.0225f;
        motion=Motion::Jump;blocked=0;
    }
    // Sweep, then slide along each world axis. This native finite-radius
    // adapter prevents passing through thin walls and repairs corner contacts.
    static CameraPoint move(CameraPoint root,CameraPoint target,float budget,const PartnerFollowServices& s) {
        const float d=distance(root,target);if(d==0)return root;
        const float step=std::min(d,budget),dx=(target.x-root.x)/d*step,dz=(target.z-root.z)/d*step;
        const int count=std::max(1,int(std::ceil(step/.025f)));
        for(int n=0;n<count;++n) {
            auto next=root;next.x+=dx/count;next.z+=dz/count;
            if(s.free(next)){root=next;continue;}
            next=root;next.x+=dx/count;if(s.free(next))root=next;
            next=root;next.z+=dz/count;if(s.free(next))root=next;
        }
        return root;
    }
    static bool groundRoute(CameraPoint root,CameraPoint target,const PartnerFollowServices& s) {
        const float d=distance(root,target);
        const int steps=std::max(1,int(std::ceil(d/.025f)));
        // Native bounded lookahead domain, not a source pathfinding constant.
        if(steps>320)return false;
        for(int n=1;n<=steps;++n) {
            CameraPoint p{root.x+(target.x-root.x)*n/steps,root.y,root.z+(target.z-root.z)*n/steps};
            float h=root.y;
            if(!s.free(p) || !s.floor(p,root.y-.02f,root.y+.11f,h) || std::fabs(h-root.y)>.11f)return false;
            p.y=h;if(!s.free(p))return false;
        }
        return true;
    }
    bool recover(CameraPoint& root,CameraPoint owner,const PartnerFollowServices& s) {
        if(!s.project)return false;
        const auto screen=s.project(root);if(!screen.valid)return false;
        if(screen.x>=-100 && screen.x<=700 && screen.y<=650)return false;
        // Recovered ground recovery's candidate projection, 101 moves, and
        // downward offsets. Goombella's initialized source width is26 units.
        CameraPoint candidate=root;
        for(unsigned k=0;k<=101;++k) {
            const auto q=s.project(candidate);
            if(q.valid && q.x>=-100 && q.x<=700) {root=candidate;reset(root,owner);warped=true;return true;}
            if(k==101)break;
            const float d=distance(candidate,owner);
            if(d>0){candidate.x+=(owner.x-candidate.x)/d*.26f;candidate.z+=(owner.z-candidate.z)/d*.26f;}
            float chosen=owner.y;
            for(int n=0;n<5;++n) {
                const float top=candidate.y+2.f-.5f*n;
                float h=chosen;if(s.floor(candidate,top-3.f,top,h))chosen=h;
                const auto p=s.project({candidate.x,chosen,candidate.z});
                if(p.valid && p.y>100) {
                    if(candidate.y+.74f>chosen && s.floor(candidate,candidate.y-.26f,candidate.y+.74f,h))chosen=h;
                    break;
                }
            }
            candidate.y=chosen;
        }
        return false;
    }
    void tick(CameraPoint& root,CameraPoint owner,bool ownerJumping,float speed,const PartnerFollowServices& s) {
        warped=false;if(!initialized)reset(root,owner);
        // Append actual post-collision XYZ, suppress consecutive jump marks.
        // The native vector retains an air edge at equal XYZ to avoid losing
        // short jumps; this differs from the original raw ring append guard.
        if(trail.size()<512 && !(sourceJumping && ownerJumping) && (owner.x!=lastSource.x || owner.y!=lastSource.y || owner.z!=lastSource.z || ownerJumping!=sourceJumping)) {
            if(ownerJumping && !sourceJumping && !trail.empty() && distance(trail.back().point,owner)<.001f) {
                trail.back().jump=true;trail.back().point=owner;
            } else trail.push_back({owner,ownerJumping && !sourceJumping});
            lastSource=owner;
        }
        sourceJumping=ownerJumping;
        if(root.y<-20) {root=owner;reset(root,owner);warped=true;return;}
        if(motion==Motion::Ground && recover(root,owner,s))return;
        if(motion==Motion::Ground) {
            const float separation=std::hypot(distance(root,owner),root.y-owner.y);
            if(separation>=acquisitionDistance(speed))homing=true;
            const float target=separation<.45f?.01f:separation>=.5f?.03f:speed>0?speed:.0225f;
            if(homing) {
                horizontalSpeed=adjustedSpeed(horizontalSpeed,target);
                if(separation>=.45f)horizontalSpeed=adjustedSpeed(horizontalSpeed,target);
            } else horizontalSpeed=0;
            if(homing && separation>=.45f)horizontalSpeed=catchupSpeed(speed,separation);
        }
        const float reach=historyReach(horizontalSpeed);
        while(trail.size()>1 && distance(root,trail.front().point)<=reach && !trail.front().jump)trail.erase(trail.begin());
        const float remaining=trail.empty()?0:distance(root,trail.front().point);
        if(motion==Motion::Ground) {
            if(!trail.empty() && trail.front().jump && remaining<=reach) {
                trail.front().jump=false;beginJump(root,trail.front().point);return;
            }
            if(trail.size()<=1 && remaining<=reach){homing=false;horizontalSpeed=0;blocked=0;}
        }
        if(motion==Motion::Fall) {
            // Original fall queries before integration with OLD velocity.
            // Admit nearby floors above the root too: a crossing-only query
            // loses a ledge/slope after horizontal movement or quantization.
            float h=root.y;
            if(support(root,.1f,std::fabs(velocity),h,s)) {
                root.y=h;velocity=0;motion=Motion::Ground;return;
            }
        }
        if(motion!=Motion::Ground) {
            if(motion==Motion::Fall && acceleration==0)acceleration=-.014f;
            velocity+=acceleration;if(motion==Motion::Fall)velocity=std::max(velocity,-.16f);
            root.y+=velocity;
            float h=root.y;
            if(motion==Motion::Jump && velocity<=0 && support(root,std::fabs(velocity),0,h,s)) {
                root.y=h;velocity=acceleration=0;motion=Motion::Ground;
                return;
            }
        }
        float budget=horizontalSpeed;
        if(motion==Motion::Ground && !homing)budget=0;
        // Fall retains heading and damps speed by .2 original units, then moves
        // at1.3*speed. Do not continue homing at full dash speed while falling.
        if(motion==Motion::Fall) {
            horizontalSpeed=std::max(0.f,horizontalSpeed*100.f-.20000000298023224f)*.01f;
            const CameraPoint target{root.x+headingX,root.y,root.z+headingZ};
            root=move(root,target,1.2999999523162842f*horizontalSpeed,s);
            return;
        }
        const auto before=root;
        if(motion==Motion::Ground)correctOverlap(root,owner,speed,s);
        while(budget>0 && !trail.empty()) {
            const auto target=trail.front().point;const float d=distance(root,target);
            if(d<.001f){if(trail.front().jump && motion==Motion::Ground)break;trail.erase(trail.begin());continue;}
            headingX=(target.x-root.x)/d;headingZ=(target.z-root.z)/d;
            const float step=std::min(d,budget);auto next=move(root,target,step,s);
            const float travelled=distance(root,next);root=next;budget-=step;
            if(travelled<step*.5f)break;
            if(distance(root,target)<.001f){if(trail.front().jump && motion==Motion::Ground)break;trail.erase(trail.begin());}
        }
        if(motion==Motion::Ground) {
            float h=root.y;
            if(support(root,homing?.1f:.15f,homing?.1f:.15f,h,s))root.y=h;
            else {motion=Motion::Fall;velocity=0;acceleration=homing?-.01f:-.015f;}
        }
        if(motion==Motion::Ground && homing && horizontalSpeed>0 && remaining>reach && distance(root,before)<.001f)++blocked;else blocked=0;
        if(blocked>8) {
            // Discard only obsolete marks reachable by a supported swept path.
            // Never shortcut a jump marker or cut through a wall/gap.
            for(size_t n=1;n<trail.size();++n) {
                if(trail[n-1].jump)break;
                if(groundRoute(root,trail[n].point,s)) {trail.erase(trail.begin(),trail.begin()+n);blocked=0;break;}
            }
        }
        // Capacity pressure stalls appends; it never teleports or joins distant
        // segments. Original ring capacity80 is a separate index contract.
    }
};
}
