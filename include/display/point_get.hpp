#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
namespace ttyd {
// Original US PointGet payload/update/layout. Host cosine is a numeric adapter;
// GPU camera/dispatcher and shared effect-pool ownership remain native services.
struct PointGetEffect {
    float x=0,y=0,z=0,scale=1;
    int32_t amount=0,duration=0,timer=0,phase=0;
    bool active=false,softDelete=false;
    std::array<float,3> depthPosition{};
    // Original input offset X-2000/Y-30,phase0 incoming,1 hold,2 outgoing.
    // Coordinates/layout sizes stay in the source effect space, not native world.
    void enter(int32_t count,float inputX,float inputY,float inputZ,int32_t ticks) {
        x=inputX-2000;y=inputY-30;z=inputZ;scale=1;amount=count;duration=ticks;timer=phase=0;active=true;softDelete=false;
    }
    static float interpolate(float start,float end,int32_t tick,int32_t total) {
        if(!total)return end;
        const float angle=(3.141592025756836f*float(tick))/float(total);
        const float cosine=float(std::cos(double(angle)));
        const float product=(end-start)*(1.f-cosine);
        return std::fma(product,.5f,start);
    }
    // Capture depth BEFORE current motion. Incoming45/outgoing60 invocations
    // use cosine easing; duration0 holds until softDelete,positive duration
    // counts from hold entry. Soft request is consumed when exit starts.
    void tick() {
        if(!active)return;
        depthPosition={x,y,z};++timer;
        if(phase==0){x=interpolate(-1000,0,timer,45);if(timer>=45){timer=0;phase=1;}}
        else if(phase==1){if((duration==0 && softDelete) || (duration!=0 && timer>=duration)){softDelete=false;timer=0;phase=2;}}
        else if(phase==2){x=interpolate(0,1000,timer,60);if(timer>=60)active=false;}
    }
};
struct PointGetQuad {uint32_t texture=0;float centerX=0,bottomY=0,width=0,height=0,u0=0,u1=1;bool shadow=false;};
// Original effect IDs124 phrase,126 ten-digit strip,128 singular/130 plural;
// shadow is the paired +1 ID. Phrase widths296/328,height48; leading word200,
// digit advance48 with20/14 glyph scaling,10-unit gaps. Fit half-width to200;
// shadow shifts(+4,-4),digit UV width.1. Layout emits shadow pass before main.
inline std::vector<PointGetQuad> pointGetQuads(const PointGetEffect& effect) {
    std::vector<PointGetQuad> out;if(!effect.active)return out;
    const std::string digits=std::to_string(effect.amount);
    const float phraseWidth=effect.amount>=2?328.f:296.f;
    const float halfWidth=100.f+5.f+float(digits.size())*24.f+5.f+phraseWidth*.5f;
    const float scale=effect.scale*(halfWidth>200?200.f/halfWidth:1.f);
    for(unsigned pass=0;pass<2;++pass){const bool shadow=pass==0;const float dx=shadow?4.f:0.f,dy=shadow?-4.f:0.f;float cursor=0;
        auto quad=[&](uint32_t texture,float width,float height,float glyphScale,float u0,float u1){
            cursor+=width*.5f;out.push_back({texture+(shadow?1u:0u),effect.x+scale*(dx+cursor-halfWidth),effect.y+scale*dy,scale*width*glyphScale,scale*height*glyphScale,u0,u1,shadow});cursor+=width*.5f;
        };
        quad(124,200,48,1,0,1);cursor+=10;
        for(char digit:digits){const float u=float(digit-'0')*.10000000149011612f;quad(126,48,48,1.4285714626312256f,u,u+.10000000149011612f);}
        cursor+=10;quad(effect.amount>=2?130:128,phraseWidth,48,1,0,1);
    }
    return out;
}
}
