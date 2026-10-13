#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace ttyd {
// Fit original 640x480 artwork into a centered320x240 area without stretching.
constexpr float MessageWindowScale=.5f;
struct MessageWindowGeometry {float x=-280,y=225,width=560,height=176;};
struct MessageWindowPlan {
    MessageWindowGeometry box;
    float textX=0,textY=0,tailX=0,tailY=0,tipX=0,tipY=0,tailWidth=0;
    bool tail=false,textVisible=false,tailStretched=false;
};
struct MessageTailPoint {float x=0,y=0;};
// Preserve horizontal quad edges; shear toward the speaker instead of rotating.
inline MessageTailPoint messageTailPoint(const MessageWindowPlan& plan,float u,float v) {
    return {plan.tailX+(plan.tipX-plan.tailX)*v+(u-.5f)*plan.tailWidth,
            plan.tailY+(plan.tipY-plan.tailY)*v};
}
// User-requested handheld adaptation: the visible tip ends four screen pixels
// before the talk anchor, rather than at the original fixed window-relative Y.
inline void stretchMessageTail(MessageWindowPlan& plan,float anchorX,float anchorY) {
    if(!plan.tail)return;
    const float dx=anchorX-plan.tailX,dy=anchorY-plan.tailY,distance=std::hypot(dx,dy);
    constexpr float gap=4/MessageWindowScale;
    if(distance<=gap){plan.tail=false;return;}
    const float t=(distance-gap)/distance;
    plan.tipX=plan.tailX+dx*t;plan.tipY=plan.tailY+dy*t;plan.tailStretched=true;
}
inline MessageTailPoint messageTailMaskPoint(const MessageWindowPlan& plan,float u,float v,bool shadow) {
    if(!plan.tailStretched)return messageTailPoint(plan,u,v);
    // Original48x64 masks' final opaque texel centers: (22.5,47.5)/(22.5,59.5).
    // Compensate transparent margins independently so neither pass overshoots.
    constexpr float tipU=22.5f/48;
    const float t=v/((shadow?59.5f:47.5f)/64);
    return {plan.tailX+(plan.tipX-plan.tailX-(tipU-.5f)*plan.tailWidth)*t+(u-.5f)*plan.tailWidth,
            plan.tailY+(plan.tipY-plan.tailY)*t};
}
struct MessageGlyphCrop {
    float x=0,y=0,width=0,height=0,u0=0,v0=0,u1=1,v1=1;
    bool visible=false;
};
inline MessageGlyphCrop messageGlyphCrop(float x,float y,float width,float height,float top,float bottom) {
    MessageGlyphCrop out;
    if(width<=0 || height<=0)return out;
    out.x=std::max(0.f,x);out.y=std::max(top,y);
    out.width=std::min(400.f,x+width)-out.x;out.height=std::min(bottom,y+height)-out.y;
    out.visible=out.width>0 && out.height>0;
    if(out.visible){out.u0=(out.x-x)/width;out.v0=(out.y-y)/height;out.u1=(out.x+out.width-x)/width;out.v1=(out.y+out.height-y)/height;}
    return out;
}
// Source centered640x480 coordinates, Y up. Projection is supplied by the
// backend; original constructor/draw/print constants are kept in this space.
inline unsigned messageSpeakerMode(float speakerX,float marioX) {
    return std::fabs(speakerX)>=300?2:speakerX<marioX?3:7;
}
inline MessageWindowPlan messageWindowPlan(MessageWindowGeometry geometry,unsigned mode,
                                           float anchorX,float anchorY,uint8_t alpha,int type=0) {
    MessageWindowPlan out;out.box=geometry;
    out.textX=geometry.x+62;out.textY=geometry.y-36;
    out.textVisible=mode==0 || alpha==255;
    if(type==1 && mode) {
        geometry.x=anchorX-geometry.width*.5f;
        geometry.y=anchorY+((mode&8)?-80:60);
        out.textX=geometry.x+28;out.textY=geometry.y-6;
    }
    if(mode) {
        const float a=alpha/255.f;
        const float cx=anchorX+a*(geometry.x+geometry.width*.5f-anchorX);
        const float cy=anchorY+a*(geometry.y-geometry.height*.5f-anchorY);
        const float w=10+a*(geometry.width-10),h=10+a*(geometry.height-10);
        out.box={cx-w*.5f,cy+h*.5f,w,h};
    } else out.box=geometry;
    if(mode&1) {
        const auto b=out.box;
        out.tailX=b.x+((mode&4)?.7f:.38f)*b.width;
        const float edge=b.y-((mode&8)?.175f:.8f)*b.height;
        out.tailY=b.y+((mode&8)?.225f:-1.2f)*b.height;
        if((mode&8)?edge<anchorY:edge>anchorY) {
            out.tipX=out.tailX-(out.tailX-anchorX)*(edge-out.tailY)/(edge-anchorY);
            out.tipX=anchorX>=out.tailX?std::min(out.tipX,anchorX):std::max(out.tipX,anchorX);
            out.tipY=out.tailY;out.tailY=edge;out.tailWidth=.1f*b.width;out.tail=true;
        }
    }
    return out;
}
}
