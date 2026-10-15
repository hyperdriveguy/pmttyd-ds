#include "battle_stage_display.hpp"
#include "animation_shape_render.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
bool normalize(const std::array<float,3>& v,std::array<float,3>& out){
    for(float x:v)if(!std::isfinite(x))return false;
    const float px=v[0]*v[0],py=v[1]*v[1],zpx=std::fma(v[2],v[2],px),length=zpx+py;
    const float seed=1.f/std::sqrt(length),square=seed*seed,half=seed*.5f;
    const float refined=-std::fma(square,length,-3.f),factor=refined*half;
    for(unsigned i=0;i<3;++i){out[i]=v[i]*factor;if(!std::isfinite(out[i]))return false;}
    return true;
}
float real(const EventMemoryServices& m,uint32_t address){const auto bits=m.word(m.user,address);float value;std::memcpy(&value,&bits,4);return value;}
}
// SDK axis-angle/vector math uses native sin/cos and reciprocal-sqrt seed.
// Preserve visible FMA/store graph without claiming Gekko estimate/FPSCR parity.
bool nativeBattleStageMath(BattleStageMath& r){
    if(r.function==0x8029ebe4)return normalize(r.vector,r.output);
    if(r.function==0x8029e6a4){AnimationShapeMath point;point.function=r.function;point.input=r.matrix;point.vector=r.vector;
        if(!animationShapeNativeMath(point))return false;r.output=point.vectorOutput;return true;}
    if(r.function!=0x8029e1c0 || !std::isfinite(r.angle))return false;
    const float sine=std::sin(r.angle),cosine=std::cos(r.angle),k=1.f-cosine;
    std::array<float,3> axis;if(!normalize(r.axis,axis))return false;
    const auto x=axis[0],y=axis[1],z=axis[2];
    const float xk=x*k,yk=y*k,zk=z*k,xt=x*sine,yt=y*sine;
    const float xxk=xk*x,xyk=xk*y,yyk=yk*y,xzk=xk*z,yzk=yk*z,zzk=zk*z;
    r.matrix={xxk+cosine,-std::fma(z,sine,-xyk),xzk+yt,0,
        std::fma(z,sine,xyk),cosine+yyk,-xt+yzk,0,xzk+(-yt),xt+yzk,zzk+cosine,0};
    for(float x:r.matrix)if(!std::isfinite(x))return false;return true;
}
// Original dark80138da8/light801389a0(hex); stage work=BattleWork+163fc.
// Each request/primitive latches before provider hold. Source camera 4 must
// publish its view before the position matrix is admitted.
bool resumePackedBattleStageDisplay(uint32_t callback,const BattleStageDisplayServices& s,BattleStageDisplayCursor& c){
    if(callback!=0x80138da8 && callback!=0x801389a0)return false;
    const bool light=callback==0x801389a0;const auto& a=s.actors;const auto& m=a.memory;
    if(!c.active){if(!m.word || !a.byte)return false;c={};c.active=true;c.callback=callback;c.owner=currentBattleAddress(a)+0x163fc;}
    if(c.callback!=callback)return false;
    auto done=[&](uint32_t phase){c.queued=false;c.phase=phase;};
    auto emit=[&](ShadowGX kind,std::array<int32_t,7> values){
        if(!c.queued){c.request={};c.request.kind=kind;c.request.values=values;c.queued=true;}
        c.pending=uint32_t(kind);return s.gx && s.gx(s.user,c.request);
    };
    // Intentional start+2*(end-start), not midpoint. Source light records use
    // stridee8(hex), start+58, end+88, direction+b8, width+40, color+28.
    auto center=[&](uint32_t r){for(unsigned i=0;i<3;++i){c.start[i]=real(m,r+0x58+4*i);const auto end=real(m,r+0x88+4*i);c.center[i]=std::fma(2.f,end-c.start[i],c.start[i]);}};
    for(;;)switch(c.phase){
    case 0:c.pending=0x8000f2fc;if(!s.camera || !(c.camera=s.camera(s.user,4)))return false;done(1);break;
    case 1:{
        // The two source prefixes differ at the position-matrix call and light
        // depth setup. Keep every setter acknowledged separately and in order.
        static const ShadowGXRequest setup[]={
            {ShadowGX::Cull,{2}},{ShadowGX::DepthLocation,{1}},{ShadowGX::Alpha,{7,0,0,7,0}},
            {ShadowGX::Blend,{1,4,5,5}},{ShadowGX::Depth,{1,3,0}},
            {ShadowGX::ClearDesc,{}},{ShadowGX::Desc,{9,1}},{ShadowGX::Desc,{11,1}},
            {ShadowGX::Format,{0,9,1,4,0}},{ShadowGX::Format,{0,11,1,5,0}},
            {ShadowGX::NumChans,{1}},{ShadowGX::ChanCtrl,{4,0,1,1,0,2,2}},
            {ShadowGX::NumTexGens,{0}},{ShadowGX::PositionMatrix,{}},
            {ShadowGX::NumTevStages,{1}},{ShadowGX::TevColorOp,{0,0,0,0,1,0}},
            {ShadowGX::TevColorIn,{0,15,15,15,10}},{ShadowGX::TevAlphaOp,{0,0,0,0,1,0}},
            {ShadowGX::TevAlphaIn,{0,7,7,7,5}},{ShadowGX::TevOrder,{0,255,255,4}},
            {ShadowGX::PositionMatrix,{}}
        };
        while(c.step<21){
            if((!light && (c.step==4 || c.step==20)) || (light && c.step==13)){++c.step;continue;}
            if(!c.queued){c.request=setup[c.step];
                if(light){if(c.step==3)c.request.values={1,2,1,5};if(c.step==11)c.request.values[0]=2;
                    if(c.step==18)c.request.values={0,7,7,7,7};if(c.step==19)c.request.values={0,0,0,4};}
                if(c.request.kind==ShadowGX::PositionMatrix){c.pending=0x802ba2fc;if(!c.camera->viewPublished)return false;
                    for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)c.request.matrix[row][col]=c.camera->matrices.view[row][col];}
                c.queued=true;}
            c.pending=uint32_t(c.request.kind);if(!s.gx || !s.gx(s.user,c.request))return false;c.queued=false;++c.step;
        }
        done(light?5:2);break;}
    case 2:if(!emit(ShadowGX::Depth,{1,c.index?1:6,0}))return false;done(3);break;
    // Three dark quads span +/-640 atZ105; second reverses winding. Alpha
    // bytes+ae8/+b08(hex) and depth compare sequence remain source state.
    case 3:if(!c.queued){
            c.primitive={};c.primitive.mode=0x80;c.primitive.count=4;
            const auto alpha=a.byte(a.user,c.owner+(c.index==2?0xae8:0xb08));
            const float x0=c.index==1?640.f:-640.f,x1=-x0;
            const std::array<std::array<float,3>,4> points{{{x0,640,105},{x1,640,105},{x1,-640,105},{x0,-640,105}}};
            for(unsigned i=0;i<4;++i)c.primitive.vertices[i]={points[i],{0,0,0,alpha}};c.queued=true;}
        c.pending=0x802b5a6c;if(!s.primitive || !s.primitive(s.user,c.primitive))return false;
        ++c.index;done(c.index==3?12:2);break;
    case 5:if(c.index==12){done(12);break;}
        if(!(m.word(m.user,c.owner+0xe8*c.index+4)&1)){++c.index;break;}
        {const auto r=c.owner+0xe8*c.index;
            for(unsigned i=0;i<3;++i){c.axis[i]=real(m,r+0x88+4*i)-real(m,r+0x58+4*i);c.vector[i]=real(m,r+0xb8+4*i);}
            c.width=real(m,r+0x40);
            c.segment=0;done(6);break;}
    case 6:center(c.owner+0xe8*c.index);
        for(unsigned i=0;i<3;++i)c.previous[i]=std::fma(c.vector[i],c.width,c.center[i]);done(7);break;
    // Up to 12 active light records; 18 segments of20-degree axis rotation
    // (.3490658402442932 radians), transform, normalize, then triangle. This is
    // authored spotlight geometry, not native screen-resolution tessellation.
    case 7:case 8:case 9:{
        const auto phase=c.phase;
        if(!c.queued){
            if(phase==7){c.math={};c.math.function=0x8029e1c0;c.math.axis=c.axis;c.math.angle=.3490658402442932f;}
            if(phase==8){c.math.function=0x8029e6a4;c.math.vector=c.vector;}
            if(phase==9){c.math.function=0x8029ebe4;c.math.vector=c.math.output;}
            c.queued=true;}
        c.pending=c.math.function;if(!s.math || !s.math(s.user,c.math))return false;
        if(phase==9)c.vector=c.math.output;done(phase+1);break;}
    case 10:if(!c.queued){
            c.primitive={};c.primitive.mode=0x90;c.primitive.count=3;
            std::array<float,3> current;for(unsigned i=0;i<3;++i)current[i]=std::fma(c.vector[i],c.width,c.center[i]);
            const auto r=c.owner+0xe8*c.index;const std::array<uint8_t,4> rgba{a.byte(a.user,r+0x28),a.byte(a.user,r+0x29),a.byte(a.user,r+0x2a),0};
            c.primitive.vertices[0]={c.start,rgba};c.primitive.vertices[1]={c.previous,rgba};c.primitive.vertices[2]={current,rgba};c.queued=true;}
        c.pending=0x802b5a6c;if(!s.primitive || !s.primitive(s.user,c.primitive))return false;
        ++c.segment;
        if(c.segment==18){++c.index;done(5);}else done(6);break;
    case 12:c.active=false;c.pending=0;return true;
    default:return false;
    }
}
}
