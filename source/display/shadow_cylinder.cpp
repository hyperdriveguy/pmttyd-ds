#include "shadow_display.hpp"
#include <cmath>
#include <initializer_list>
namespace ttyd {
namespace {
float single(float v){volatile float f=v;return f;}
ShadowGXRequest request(ShadowGX kind,std::initializer_list<int32_t> args){ShadowGXRequest r;r.kind=kind;unsigned i=0;for(auto v:args)r.values[i++]=v;return r;}
// Three source passes: OR back-face alpha mask, zero front-face alpha, then
// destination-alpha RGB blend with work color. Depth writes stay disabled.
// Per-pass setter order matters even when later setters replace earlier state.
std::optional<ShadowGXRequest> state(unsigned pass,unsigned step,const ShadowWork& w,const ShadowDisplayGlobals& g){
    if(pass==0)switch(step){
    case 0:return request(ShadowGX::Depth,{1,3,0});case 1:return request(ShadowGX::Cull,{2});
    case 2:return request(ShadowGX::Blend,{1,1,1,0});case 3:return request(ShadowGX::Blend,{2,1,1,7});
    case 4:return request(ShadowGX::AlphaUpdate,{1});case 5:return request(ShadowGX::ColorUpdate,{0});
    case 6:return request(ShadowGX::ChanCtrl,{4,0,0,1,0,2,2});
    }
    if(pass==1)switch(step){
    case 0:return request(ShadowGX::Depth,{1,3,0});case 1:return request(ShadowGX::Cull,{1});
    case 2:return request(ShadowGX::Blend,{0,4,5,0});case 3:return request(ShadowGX::AlphaUpdate,{1});
    case 4:return request(ShadowGX::ColorUpdate,{0});
    case 5:{auto r=request(ShadowGX::ChanMaterial,{4});r.color=g.cylinderZeroMaterial;return r;}
    case 6:return request(ShadowGX::ChanCtrl,{4,0,0,0,0,2,2});
    }
    if(pass==2)switch(step){
    case 0:return request(ShadowGX::Cull,{2});case 1:return request(ShadowGX::Blend,{1,6,7,0});
    case 2:return request(ShadowGX::AlphaUpdate,{0});case 3:return request(ShadowGX::ColorUpdate,{1});
    case 4:{auto r=request(ShadowGX::ChanMaterial,{4});r.color=w.color;return r;}
    }
    return {};
}
bool submit(ShadowCylinderCursor& c,const ShadowDisplayServices& s,uint32_t& pending){
    pending=uint32_t(c.request.kind);
    if(!s.gx || !s.gx(s.user,c.request))return false;
    c.requestPending=false;return true;
}
bool bound(ShadowCylinderCursor& c,const ShadowDisplayServices& s,uint32_t function,uint32_t& pending){
    c.bounds.function=function;pending=function;
    auto next=c.bounds;
    if(!s.cylinder.bounds || !s.cylinder.bounds(s.cylinder.user,next))return false;
    c.bounds=next;return true;
}
}
// Borrow exact record/view/camera across holds. Camera 1 alone performs draw
// sync, bounding-box clear/read and collision-bound publication. Other cameras
// must not inherit those calls or their results.
bool shadowCylinder(ShadowWork& w,uint32_t camera,const CameraState& view,ShadowRecord& record,
                    ShadowCylinderCursor& c,const ShadowDisplayGlobals& g,const ShadowDisplayServices& s,uint32_t& pending){
    if(!c.active){c={};c.active=true;c.camera=camera;c.record=&record;c.view=&view;c.stage=camera==1?0:2;}
    if(c.record!=&record || c.view!=&view || c.camera!=camera)return false;
    for(;;)switch(c.stage){
    case 0:if(!bound(c,s,0x80005ce4,pending))return false;++c.stage;break; // sysWaitDrawSync
    case 1:if(!bound(c,s,0x802b69d0,pending))return false;++c.stage;break; // GXClearBoundingBox
    case 2:{
        // Native FMA concat adapter; borrowed view and prepared matrix are
        // captured here, before a held GXLoadPosMtxImm. Never rescale/reray.
        c.request=request(ShadowGX::PositionMatrix,{0});
        const auto& a=c.view->matrices.view;const auto& b=c.record->matrix;
        for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j){
            float v=single(a[i][0]*b[j]);v=std::fma(a[i][1],b[4+j],v);v=std::fma(a[i][2],b[8+j],v);
            if(j==3)v=single(v+a[i][3]);c.request.matrix[i][j]=single(v);
        }
        c.requestPending=true;++c.stage;break;
    }
    case 3:if(!submit(c,s,pending))return false;++c.stage;break;
    case 4:
        if(!c.requestPending){c.request=request(ShadowGX::TevColor,{1});c.request.color=(g.cylinderTevRGB&0xffffff00u)|c.record->alpha;c.requestPending=true;}
        if(!submit(c,s,pending))return false;c.step=0;c.pass=0;++c.stage;break;
    case 5:
        for(;;){
            if(!c.requestPending){const auto next=state(c.pass,c.step,w,g);if(!next)break;c.request=*next;c.requestPending=true;}
            if(!submit(c,s,pending))return false;++c.step;
        }
        ++c.stage;break;
        // Each pass captures variant/flags/opacity anew, then pins palette/list
        // plan across holds. Variant1 uses cube resources; flag 4 profile uses
        // fixed cylinder lists. Ordinary radius=clamp(1-outer, 0, 1); alpha derives
        // from low 8(workColor)*scale/255*inner with source float/truncation order.
    case 6:{
        // Fresh dispatch/flags/opacity captures for EACH pass. Retain this plan
        // before palette/list holds, while later passes can observe mutations.
        pending=0x8002c2ec;c.listIndex=0;c.listCount=0;
        if(c.record->variant==1){
            c.request=request(ShadowGX::Array,{11,int32_t(0x80415180u),4});
            c.lists={0x80304ca0,0x80415100,0x80415120,0};c.bytes={64,32,32,0};c.listCount=3;
        }else if(c.record->flags&4){
            c.request=request(ShadowGX::Array,{11,int32_t(0x80415160u),4});
            c.lists={0x80304b60,0x80304ba0,0x80304be0,0};c.bytes={64,64,64,0};c.listCount=3;
        }else{
            const float scale=c.record->scale,outer=c.record->outer,inner=c.record->inner;
            if(!std::isfinite(scale) || !std::isfinite(outer) || !std::isfinite(inner))return false;
            const float product=single(float(uint8_t(w.color))*scale);
            float radius=single(1.f-outer);
            const float a=single(product/255.f);float b=single(a*inner);
            if(!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(radius))return false;
            if(radius<0)radius=0;else if(radius>1)radius=1;
            if(a<=0){c.stage=9;break;}
            if(b<=0 || b>=1)b=0;
            const uint8_t alpha=uint8_t(int32_t(std::trunc(single(255.f*b)))&255);
            c.request=request(ShadowGX::Array,{11,int32_t(0x80415140u),4});
            if(radius!=0)c.lists[c.listCount++]=0x80304c20;
            if(radius!=1 && alpha!=0)c.lists[c.listCount++]=0x80304c60;
            c.lists[c.listCount++]=0x80304ba0;c.lists[c.listCount++]=0x80304be0;c.bytes={64,64,64,64};
        }
        c.requestPending=true;++c.stage;break;
    }
    case 7:if(!submit(c,s,pending))return false;++c.stage;break;
    // GXCallDisplayList identities/byte lengths are immutable original data.
    // Advance only after full list acknowledgement; do not replay prior lists.
    case 8:
        while(c.listIndex<c.listCount){
            pending=0x802b9fa0;
            if(!s.cylinder.list || !s.cylinder.list(s.cylinder.user,c.lists[c.listIndex],c.bytes[c.listIndex]))return false;
            ++c.listIndex;
        }
        ++c.stage;break;
    case 9:
        if(++c.pass<3){c.step=0;c.stage=5;break;}
        c.stage=c.camera==1?10:13;break;
    case 10:if(!bound(c,s,0x80005ce4,pending))return false;++c.stage;break;
    case 11:if(!bound(c,s,0x802b6a08,pending))return false;c.bounds.slot=c.record->collision;++c.stage;break; // GXReadBoundingBox
    case 12:
        if(!bound(c,s,0x80072f38,pending))return false;++c.stage;break;
    case 13:c.active=false;return true;
    default:return false;
    }
}
}
