#include "animation_shape_render.hpp"
#include <cmath>
namespace ttyd {
bool animationShapeNativeMath(AnimationShapeMath& r){
    for(float v:r.input)if(!std::isfinite(v))return false;
    if(r.function==0x8029e6a4){
        for(float v:r.vector)if(!std::isfinite(v))return false;
        ActorVector out;for(unsigned row=0;row<3;++row){const auto i=4*row;
            const float p0=r.input[i]*r.vector[0],p1=r.input[i+1]*r.vector[1];
            const float q0=std::fma(r.input[i+2],r.vector[2],p0),q1=std::fma(r.input[i+3],1.f,p1);out[row]=q0+q1;}
        for(float v:out)if(!std::isfinite(v))return false;
        r.vectorOutput=out;return true;
    }
    if(r.function!=0x8029df28)return false;
    const auto& m=r.input;const float a=m[0],b=m[1],c=m[2],d=m[4],e=m[5],f=m[6],g=m[8],h=m[9],i=m[10];
    const float cof[]{std::fma(e,i,-(h*f)),std::fma(f,g,-(i*d)),std::fma(d,h,-(e*g)),
        std::fma(h,c,-(b*i)),std::fma(i,a,-(c*g)),std::fma(b,g,-(a*h)),
        std::fma(b,f,-(e*c)),std::fma(c,d,-(f*a)),std::fma(a,e,-(b*d))};
    const float determinant=std::fma(g,cof[6],std::fma(d,cof[3],a*cof[0])),zero=b-b;
    if(determinant==zero){r.status=0;return true;} // Source leaves output untouched.
    if(!std::isfinite(determinant))return false;
    const float estimate=1.f/determinant,twice=estimate+estimate,square=estimate*estimate;
    const float reciprocal=std::fma(-determinant,square,twice);
    BattleActMatrix out{};for(unsigned row=0;row<3;++row){for(unsigned col=0;col<3;++col)out[4*row+col]=cof[3*row+col]*reciprocal;out[4*row+3]=zero;}
    for(float v:out)if(!std::isfinite(v))return false;
    r.output=out;r.status=1;return true;
}
bool resumeAnimationShape(BattlePosePool& pool,int32_t handle,const std::optional<uint32_t>& pass,const AnimationShapeRequest& input,const AnimationShapeServices& s,AnimationShapeCursor& c){
    if(!c.active){
        if(input.shape==-1)return true;
        const auto* p=pool.pose(handle);if(!p || !p->resource || !p->resource->draw)return false;
        c=AnimationShapeCursor{};c.active=true;c.handle=handle;c.input=input;c.resource=p->resource;
    }
    if(handle!=c.handle)return false;
    auto emit=[&](uint32_t fn){c.pending=fn;return s.call && s.call(s.user,c.request);};
    auto queue=[&](uint32_t fn){if(!c.queued){c.request={};c.request.function=fn;c.request.resource=c.resource->draw;c.request.draw=c.draw;c.request.pose=pool.pose(handle);c.queued=true;return true;}return false;};
    auto done=[&](uint32_t phase){c.queued=false;c.phase=phase;};
    const auto& d=*c.resource->draw;
    for(;;){
        auto* p=pool.pose(handle);if(!p || p->resource!=c.resource){c.pending=0x8003c884;return false;}
        switch(c.phase){
        case 0:{c.pending=0x8000f2e4;const auto* camera=s.currentCamera?s.currentCamera(s.user):nullptr;if(!camera || !camera->viewPublished)return false;
            c.concat={};c.concat.function=0x8029dd64;c.concat.right=c.input.model;
            for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col)c.concat.left[4*row+col]=camera->matrices.view[row][col];
            c.phase=1;break;}
        case 1:c.pending=0x8029dd64;if(!s.concat || !s.concat(s.user,c.concat))return false;c.viewModel=c.concat.output;c.phase=2;break;
        case 2:if(queue(0x802ba2fc))c.request.matrix=c.viewModel;if(!emit(0x802ba2fc))return false;
            done(3);break;
        case 3:if(!c.queued){c.math={};c.math.function=0x8029df28;c.math.input=c.viewModel;if(c.normal)c.math.output=*c.normal;c.queued=true;}
            c.pending=c.math.function;if(!s.math || !s.math(s.user,c.math))return false;if(c.math.status)c.normal=c.math.output;done(4);break;
        case 4:if(queue(0x802ba34c))c.request.matrix=c.normal;if(!emit(0x802ba34c))return false;
            done(5);break;
        case 5:queue(0x802ba39c);if(!emit(0x802ba39c))return false;
            done(6);break;
        case 6:{c.pending=0x8003c884;if(!pass || c.input.shape<0 || uint32_t(c.input.shape)>=d.word(0xe8))return false;
            const auto h=d.word(0x14c)+168*uint32_t(c.input.shape),m=d.word(h+0xa0);
            if(!((m==0 && *pass==2) || m==*pass || p->state[0xf3]!=255 || p->state[0xf7]!=255)){c.active=false;c.pending=0;return true;}
            c.draw=d.word(0x19c)+108*d.word(h+0x98);c.drawCount=d.word(h+0x9c);
            c.positionBase=d.word(h+0x40);c.normalBase=d.word(h+0x48);c.colorBase=d.word(h+0x50);
            for(unsigned k=0;k<8;++k)c.uvBases[k]=d.word(0x18c)+8*d.word(h+0x58+8*k);
            queue(0x802b5c8c);constexpr uint32_t table[]{2,1,3,0};c.request.arguments[0]=table[d.word(h+0xa4)];c.phase=7;break;}
        case 7:if(!emit(0x802b5c8c))return false;
            done(8);break;
        case 8:if(c.drawIndex>=c.drawCount){c.active=false;c.pending=0;return true;}
            c.slots=d.word(c.draw);c.phase=p->word(0xe4)?18:9;break;
        case 9:if(!c.queued){c.math={};c.math.function=0x8029e6a4;c.math.input=c.input.model;c.queued=true;}
            c.pending=c.math.function;if(!s.math || !s.math(s.user,c.math))return false;
            done(10);break;
        case 10:if(queue(0x800208c0)){c.request.arguments[0]=p->word(0xec);c.request.origin=c.math.vectorOutput;}
            if(!emit(0x800208c0))return false;
            done(11);break;
        case 11:if(queue(0x8003dcfc)){c.request.arguments={d.word(c.draw),c.draw+0x10,d.word(0x194),0,d.word(0x134),d.word(0x198),0};}
            if(!emit(0x8003dcfc))return false;
            done(12);break;
        case 12:if(queue(0x80021860)){const auto c2=p->word(0xf4),c1=p->word(0xf0);c.request.arguments={p->word(0xe8),c1,c2,0,0,0,0};}
            if(!emit(0x80021860))return false;
            done(13);break;
        case 13:c.pending=0x8000f2fc;c.fogCamera=s.cameraByIndex?s.cameraByIndex(s.user,4):nullptr;if(!c.fogCamera)return false;c.phase=14;break;
        case 14:{c.pending=0x8000f2e4;const auto* current=s.currentCamera?s.currentCamera(s.user):nullptr;if(!current)return false;c.phase=current==c.fogCamera?15:16;break;}
        case 15:queue(0x800217c8);if(!emit(0x800217c8))return false;
            done(16);break;
        case 16:if(queue(0x80021b80)){c.request.arguments={c.slots,d.word(c.draw+8),p->word(0xe8),0,0,0,0};c.request.matrix=c.input.model;}
            if(!emit(0x80021b80))return false;
            done(18);break;
        case 18:
            // The native owner has no override objects. Nonzero source P+4
            // still needs the kind scan: an all-kind0 bank skips naturally.
            if(p->word(4))for(uint32_t j=0;j<c.slots;++j){const auto slot=d.word(c.draw+0x10+4*(c.slots-j-1));
                if(slot>=d.word(0x130) || slot>=p->shape.textures.size()){c.pending=0x8003c884;return false;}
                auto index=d.word(d.word(0x194)+8*slot)+p->shape.textures[slot];
                if(int32_t(index)<0 || index>=d.word(0x134)){c.pending=0x8026a2a8;return false;}
                const auto kind=d.word(d.word(0x198)+64*index+8);if(kind==1 || kind==2){c.pending=0x8003c884;return false;}}
            // Independent flag10 rebind needs the work override-pointer owner.
            if(p->word(0)&0x10){c.pending=0x8003c884;return false;}
            if(pool.outputSelector){c.pending=0x8003c884;return false;}
            c.run=d.word(0x150)+8*d.word(c.draw+0x38);c.runCount=d.word(c.draw+0x3c);c.attribute=0;c.attributeStep=0;c.phase=19;break;
        case 19:queue(0x802b4520);if(!emit(0x802b4520))return false;
            done(20);break;
        case 20:{if(c.attribute>=3+c.slots){c.phase=24;break;}
            const uint32_t attr=c.attribute<3?9+c.attribute:13+c.attribute-3;
            if(c.attributeStep==0){if(queue(0x802b3c08))c.request.arguments={attr,3,0,0,0,0,0};if(!emit(0x802b3c08))return false;
            done(20);++c.attributeStep;}
            else if(c.attributeStep==1){if(queue(0x802b4558))c.request.arguments={0,attr,c.attribute==1?0u:1u,c.attribute==2?5u:4u,0,0,0};if(!emit(0x802b4558))return false;
            done(20);++c.attributeStep;}
            else {if(queue(0x802b4db0)){auto& r=c.request;r.uvBases=c.uvBases;
                    if(c.attribute<3)r.arguments={attr,c.attribute==0?c.positionBase:c.attribute==1?c.normalBase:d.word(0x164)+4*c.colorBase,c.attribute==2?4u:12u,0,0,0,0};
                    else {const auto k=uint32_t(d.byte(c.draw+0x30+(c.slots==1?0:c.slots-(c.attribute-3)-1)));r.arguments={attr,c.uvBases[k],8,0,0,0,0};}}
                if(!emit(0x802b4db0))return false;
            done(20);c.attributeStep=0;++c.attribute;}
            break;}
        case 24:if(!c.runCount){c.draw+=108;++c.drawIndex;c.phase=8;break;}
            c.first=d.word(c.run);c.count=d.word(c.run+4)&0xffff;c.vertex=0;c.phase=25;break;
        case 25:if(queue(0x802b5a6c))c.request.arguments={0xa0,0,c.count,0,0,0,0};if(!emit(0x802b5a6c))return false;
            done(26);break;
        case 26:if(c.vertex>=c.count){c.run+=8;--c.runCount;c.phase=24;break;}
            if(queue(0xcc008000)){auto& r=c.request;r.arguments[0]=3+c.slots;
                for(unsigned j=0;j<3;++j){constexpr uint32_t ptr[]{0x158,0x160,0x168};r.indices[j]=uint16_t(d.word(d.word(ptr[j])+4*(c.first+d.word(c.draw+0x40+4*j)+c.vertex)));}
                for(uint32_t j=0;j<c.slots;++j){const auto k=uint32_t(d.byte(c.draw+0x30+(c.slots==1?0:c.slots-j-1)));r.indices[3+j]=uint16_t(d.word(d.word(0x16c+4*k)+4*(c.first+d.word(c.draw+0x4c+4*k)+c.vertex)));}}
            if(!emit(0xcc008000))return false;
            done(26);++c.vertex;break;
        default:c.pending=0x8003c884;return false;
        }
    }
}
}
