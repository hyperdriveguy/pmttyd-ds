#include "camera_letterbox_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {int16_t half(int32_t n){const auto bits=uint16_t(uint32_t(n));int16_t out;std::memcpy(&out,&bits,2);return out;}}
bool CameraLetterboxVertexStream::vertex(uint32_t index,const std::array<int16_t,3>& point,const LetterboxVertexSubmit& s){
    if(!active_ || index>=8)return false;
    if(count_<8){if(index!=count_)return false;vertices_[index]=point;++count_;}
    else if(index!=7 || vertices_[7]!=point)return false;
    if(count_<8)return true;
    if(!s.submit || !s.submit(s.user,vertices_))return false;
    active_=false;return true;
}
bool drawCameraLetterbox(const CameraLetterboxDrawServices& s,CameraLetterboxDrawCursor& c){
    if(!c.active){c={};c.active=true;}
    auto done=[&](uint32_t phase){c.queued=false;c.phase=phase;};
    auto word=[&](uint32_t address){return s.word?s.word(s.user,address):std::nullopt;};
    auto scalar=[&](uint32_t offset){return s.half?s.half(s.user,*c.camera,offset):std::nullopt;};
    auto pass=[&](LetterboxPass kind){
        if(!c.queued){c.pass={};c.pass.kind=kind;
            if(kind==LetterboxPass::Projection){c.pass.projection=c.camera->matrices.projection;c.pass.projectionKind=c.camera->projectionKind;}
            if(kind==LetterboxPass::Viewport)c.pass.viewport=c.viewport;
            if(kind==LetterboxPass::ProjectionVector)c.pass.vector=c.projection;
            if(kind==LetterboxPass::Scissor)c.pass.scissor=c.scissor;
            c.queued=true;}
        c.pending=uint32_t(kind);return s.pass && s.pass(s.user,c.pass);
    };
    auto gx=[&](ShadowGX kind,std::array<int32_t,7> values){
        if(!c.queued){c.gx={};c.gx.kind=kind;c.gx.values=values;c.queued=true;}
        c.pending=uint32_t(kind);return s.gx && s.gx(s.user,c.gx);
    };
    for(;;)switch(c.phase){
    case 0:{c.pending=0x8000e564;const auto gp=s.gp?s.gp(s.user):std::nullopt;if(!gp)return false;c.gp=*gp;done(1);break;}
    case 1:c.pending=0x8000f2fc;if(!s.camera || !(c.camera=s.camera(s.user,8)))return false;done(2);break;
    case 2:{c.pending=0x8000e564;const auto flags=word(c.gp);if(!flags)return false;done(*flags&0x8000?19:3);break;}
    case 3:{const auto alpha=scalar(0x1e0);if(!alpha)return false;done(*alpha==0?19:4);break;}
    case 4:if(!pass(LetterboxPass::ReadViewport))return false;c.viewport=c.pass.viewport;done(5);break;
    case 5:if(!pass(LetterboxPass::ReadProjection))return false;c.projection=c.pass.vector;done(6);break;
    case 6:if(!pass(LetterboxPass::ReadScissor))return false;c.scissor=c.pass.scissor;done(7);break;
    case 7:if(!pass(LetterboxPass::Projection))return false;done(8);break;
    case 8:{static const ShadowGXRequest setup[]={
            {ShadowGX::Blend,{1,4,5,0}},{ShadowGX::DepthLocation,{1}},{ShadowGX::Alpha,{7,0,0,7,0}},
            {ShadowGX::Depth,{0,3,0}},{ShadowGX::NumChans,{1}}};
        while(c.step<5){const auto& r=setup[c.step];if(!gx(r.kind,r.values))return false;c.queued=false;++c.step;}
        c.step=0;done(9);break;}
    case 9:{const auto color=word(0x80429510);if(!color)return false;c.color=*color;done(10);break;}
    case 10:if(!c.queued){const auto alpha=scalar(0x1e0);if(!alpha)return false;c.gx={};c.gx.kind=ShadowGX::ChanMaterial;c.gx.values={4};c.gx.color=(c.color&0xffffff00)|uint8_t(*alpha);c.queued=true;}
        c.pending=uint32_t(c.gx.kind);if(!s.gx || !s.gx(s.user,c.gx))return false;done(11);break;
    case 11:if(!gx(ShadowGX::ChanCtrl,{4,0,0,0,0,0,2}))return false;done(12);break;
    case 12:if(!c.queued){const auto color=word(0x8041f6b0);if(!color)return false;c.gx={};c.gx.kind=ShadowGX::Fog;c.gx.color=*color;c.queued=true;}
        c.pending=uint32_t(c.gx.kind);if(!s.gx || !s.gx(s.user,c.gx))return false;done(13);break;
    case 13:{static const ShadowGXRequest setup[]={
            {ShadowGX::NumTexGens,{0}},{ShadowGX::NumTevStages,{1}},{ShadowGX::TevOrder,{0,255,255,4}},
            {ShadowGX::TevOp,{0,4}},{ShadowGX::Cull,{0}},{ShadowGX::ClearDesc,{}},{ShadowGX::Desc,{9,1}},
            {ShadowGX::Format,{0,9,1,3,0}},{ShadowGX::PositionMatrix,{}},{ShadowGX::CurrentMatrix,{0}}};
        while(c.step<10){if(!c.queued){c.gx=setup[c.step];
                if(c.gx.kind==ShadowGX::PositionMatrix){c.pending=0x802ba2fc;if(!c.camera->viewPublished)return false;c.gx.matrix=c.camera->matrices.view;}c.queued=true;}
            c.pending=uint32_t(c.gx.kind);if(!s.gx || !s.gx(s.user,c.gx))return false;c.queued=false;++c.step;}
        done(14);break;}
    case 14:c.pending=0x802b5a6c;if(!s.begin || !s.begin(s.user))return false;done(15);break;
    case 15:while(c.vertex<8){
            c.pending=0xcc008000;
            if(!c.queued){int32_t y=c.vertex<4?240:-240;
                if(c.vertex==2 || c.vertex==3){const auto top=scalar(0x1e2);if(!top)return false;y=240-int32_t(*top);}
                if(c.vertex==4 || c.vertex==5){const auto bottom=scalar(0x1e4);if(!bottom)return false;y=-240+int32_t(*bottom);}
                const bool right=c.vertex==1 || c.vertex==2 || c.vertex==5 || c.vertex==6;
                c.point={int16_t(right?304:-304),half(y),0};c.queued=true;}
            if(!s.vertex || !s.vertex(s.user,c.vertex,c.point))return false;
            c.queued=false;++c.vertex;
        }done(16);break;
    case 16:if(!pass(LetterboxPass::Viewport))return false;done(17);break;
    case 17:if(!pass(LetterboxPass::ProjectionVector))return false;done(18);break;
    case 18:if(!pass(LetterboxPass::Scissor))return false;done(19);break;
    case 19:c.active=false;c.pending=0;return true;
    default:return false;
    }
}
}
