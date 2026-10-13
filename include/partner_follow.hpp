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
};
struct PartnerFollow {
    enum class Motion { Ground, Jump, Fall };
    struct Mark {CameraPoint point;bool jump=false;};
    std::vector<Mark> trail;
    Motion motion=Motion::Ground;
    float velocity=0,acceleration=0;
    unsigned blocked=0;
    int jumpCount=0;
    bool sourceAir=false,initialized=false,warped=false;
    CameraPoint lastSource;
    static float distance(CameraPoint a,CameraPoint b){return std::hypot(a.x-b.x,a.z-b.z);}
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
        // downward offsets. Width is the native actor's .2-unit diameter.
        CameraPoint candidate=root;
        for(unsigned k=0;k<=101;++k) {
            const auto q=s.project(candidate);
            if(q.valid && q.x>=-100 && q.x<=700) {root=candidate;reset(root,owner);warped=true;return true;}
            if(k==101)break;
            const float d=distance(candidate,owner);
            if(d>0){candidate.x+=(owner.x-candidate.x)/d*.2f;candidate.z+=(owner.z-candidate.z)/d*.2f;}
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
    void tick(CameraPoint& root,CameraPoint owner,bool ownerAir,float speed,const PartnerFollowServices& s) {
        warped=false;if(!initialized)reset(root,owner);
        // Append actual post-collision XYZ, suppress consecutive airborne marks.
        // The native vector retains an air edge at equal XYZ to avoid losing
        // short jumps; this differs from the original raw ring append guard.
        if(trail.size()<512 && !(sourceAir && ownerAir) && (owner.x!=lastSource.x || owner.y!=lastSource.y || owner.z!=lastSource.z || ownerAir!=sourceAir)) {
            if(ownerAir && !sourceAir && !trail.empty() && distance(trail.back().point,owner)<.001f) {
                trail.back().jump=true;trail.back().point=owner;
            } else trail.push_back({owner,ownerAir && !sourceAir});
            lastSource=owner;
        }
        sourceAir=ownerAir;
        if(root.y<-20) {root=owner;reset(root,owner);warped=true;return;}
        if(motion==Motion::Ground && recover(root,owner,s))return;
        while(trail.size()>1 && distance(root,trail.front().point)<.001f && !trail.front().jump)trail.erase(trail.begin());
        float remaining=trail.empty()?0:distance(root,trail.front().point);
        for(size_t n=1;n<trail.size();++n)remaining+=distance(trail[n-1].point,trail[n].point);
        if(motion==Motion::Ground) {
            float lead=0;CameraPoint previous=root;
            for(auto& mark:trail) {
                lead+=distance(previous,mark.point);previous=mark.point;
                if(mark.jump) {
                    if(lead<=.50001f){mark.jump=false;beginJump(root,mark.point);return;}
                    break;
                }
            }
        }
        if(motion!=Motion::Ground) {
            const float oldY=root.y;
            if(motion==Motion::Fall && acceleration==0)acceleration=-.014f;
            velocity+=acceleration;if(motion==Motion::Fall)velocity=std::max(velocity,-.16f);
            root.y+=velocity;
            float h=root.y;
            if(velocity<=0 && s.floor(root,root.y,oldY+.001f,h) && s.free({root.x,h,root.z})) {
                root.y=h;velocity=acceleration=0;motion=Motion::Ground;
                return;
            }
        }
        float budget=std::min(motion==Motion::Jump?.0225f:speed,std::max(0.f,remaining-.5f));
        if(motion==Motion::Jump && budget==0) {
            // Preserve following space for an in-place hop, but do not stop
            // over a gap merely because Mario is close to the landing edge.
            float floor=root.y;
            if(!s.floor(root,root.y-3.f,root.y,floor))budget=.0225f;
        }
        // Fall still travels toward the trail; a missing floor never blocks XZ.
        if(motion==Motion::Fall)budget=speed;
        const auto before=root;
        while(budget>0 && !trail.empty()) {
            const auto target=trail.front().point;const float d=distance(root,target);
            if(d<.001f){if(trail.front().jump && motion==Motion::Ground)break;trail.erase(trail.begin());continue;}
            const float step=std::min(d,budget);auto next=move(root,target,step,s);
            const float travelled=distance(root,next);root=next;budget-=step;
            if(travelled<step*.5f)break;
            if(distance(root,target)<.001f){if(trail.front().jump && motion==Motion::Ground)break;trail.erase(trail.begin());}
        }
        if(motion==Motion::Ground) {
            float h=root.y;
            if(s.floor(root,root.y-.15f,root.y+.11f,h) && s.free({root.x,h,root.z}))root.y=h;
            else {motion=Motion::Fall;velocity=0;acceleration=-.015f;}
        }
        if(motion==Motion::Ground && remaining>.5f && distance(root,before)<.001f)++blocked;else blocked=0;
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
