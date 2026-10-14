#include "shadow_display.hpp"
#include <initializer_list>
namespace ttyd {
namespace {
ShadowGXRequest gx(ShadowGX kind,std::initializer_list<int32_t> args){
    ShadowGXRequest r;r.kind=kind;unsigned i=0;for(auto v:args)r.values[i++]=v;return r;
}
ShadowGXRequest prefix(unsigned step,const ShadowWork& w,const ShadowDisplayGlobals& g,uint32_t color){
    switch(step){
    case 0:return gx(ShadowGX::NumChans,{1});
    case 1:return gx(ShadowGX::ChanCtrl,{4,0,0,0,0,2,2});
    case 2:{auto r=gx(ShadowGX::ChanMaterial,{4});r.color=w.color;return r;}
    case 3:return gx(ShadowGX::NumTexGens,{0});
    case 4:return gx(ShadowGX::NumTevStages,{1});
    case 5:return gx(ShadowGX::TevOrder,{0,255,255,4});
    case 6:return gx(ShadowGX::TevColorOp,{0,0,0,0,1,0});
    case 7:return gx(ShadowGX::TevAlphaOp,{0,0,0,0,1,0});
    case 8:return gx(ShadowGX::TevColorIn,{0,15,15,15,10});
    case 9:return gx(ShadowGX::TevAlphaIn,{0,7,1,5,7});
    case 10:{auto r=gx(ShadowGX::Fog,{0});r.color=g.projectionFog;return r;}
    case 11:return gx(ShadowGX::Blend,{1,4,5,0});
    case 12:return gx(ShadowGX::Depth,{1,3,0});
    case 13:return gx(ShadowGX::DepthLocation,{1});
    case 14:return gx(ShadowGX::Alpha,{7,0,0,7,0});
    case 15:return gx(ShadowGX::CurrentMatrix,{0});
    case 16:return gx(ShadowGX::Cull,{0});
    case 17:return gx(ShadowGX::Array,{9,int32_t(0x80304b00u),12});
    case 18:return gx(ShadowGX::ClearDesc,{});
    case 19:return gx(ShadowGX::Desc,{9,2});
    case 20:return gx(ShadowGX::Format,{0,9,1,4,0});
    case 21:return gx(ShadowGX::Blend,{0,4,5,0});
    case 23:{auto r=gx(ShadowGX::ChanMaterial,{4});r.color=color;return r;}
    default:return gx(ShadowGX::Depth,{0,3,0});
    }
}
bool submit(ShadowDisplayCursor& c,const ShadowDisplayServices& s){
    c.pending=uint32_t(c.passRequest.kind);
    auto next=c.passRequest;
    if(!s.pass || !s.pass(s.user,next))return false;
    c.passRequest=next;c.passPending=false;return true;
}
}
bool shadowProjectionPass(ShadowWork& w,ShadowDisplayCursor& c,ShadowDisplayGlobals& g,const ShadowDisplayServices& s){
    auto selectedProfile=[&]()->ShadowProfile*{
        const auto index=c.auxiliary?c.auxiliary->profileIndex:w.profileIndex;
        return index<w.profiles.size()?&w.profiles[index]:nullptr;
    };
    for(;;){
        // Class2 switches from restoring pswp to setting up dswp. References
        // must therefore be selected at each operation, not captured forever.
        switch(c.phase){
        case 3:
            if(!c.passPending){c.passRequest={};c.passRequest.kind=ShadowPassKind::InvalidateTextures;c.passPending=true;}
            if(!submit(c,s))return false;
            ++c.phase;break;
        case 4:{
            // Read size only AFTER invalidate returns, from retained owner.
            const auto* row=selectedProfile();c.pending=0x8002d3d8;
            if(!row || row->sizeIndex>=w.profileSizes.size())return false;
            c.size=w.profileSizes[row->sizeIndex];c.phase=c.drawClass==2?23:5;break;
        }
        case 5:case 6:
            if(!c.passPending){c.passRequest={};c.passRequest.kind=c.phase==5?ShadowPassKind::ReadViewport:ShadowPassKind::ReadScissor;c.passPending=true;}
            if(!submit(c,s))return false;
            if(c.phase==5)g.savedViewport=c.passRequest.viewport;else g.savedScissor=c.passRequest.scissor;
            ++c.phase;break;
        case 7:case 8:
            if(!c.passPending){
                c.passRequest={};c.passPending=true;
                if(c.phase==7){c.passRequest.kind=ShadowPassKind::Viewport;c.passRequest.viewport={0,0,float(c.size),float(c.size),0,1};}
                else{c.passRequest.kind=ShadowPassKind::Scissor;c.passRequest.scissor={1,1,c.size-2,c.size-2};}
            }
            if(!submit(c,s))return false;
            ++c.phase;break;
        case 9:
            c.pending=0x8002d3d8;c.profile=selectedProfile();if(!c.profile)return false;
            c.capturedEye=c.profile->eye;++c.phase;break;
        case 10:{
            c.pending=0x8029e330;
            const auto view=cameraLookAtAdapter(c.capturedEye,c.profile->at,c.profile->up);
            if(!view)return false;
            (c.auxiliary?c.auxiliary->view:w.view)=*view;
            const auto& row=*c.profile;
            (c.auxiliary?c.auxiliary->projection:w.projection)=row.projectionKind==0?
                cameraFrustum(row.top,-row.top,row.left,-row.left,row.nearPlane,row.farPlane):
                cameraOrthographic(row.top,-row.top,row.left,-row.left,row.nearPlane,row.farPlane);
            ++c.phase;break;
        }
        case 11:
            if(!c.passPending){
                c.pending=0x8002d3d8;const auto* row=selectedProfile();if(!row)return false;
                c.passRequest={};c.passRequest.kind=ShadowPassKind::Projection;
                c.passRequest.projection=c.auxiliary?c.auxiliary->projection:w.projection;c.passRequest.projectionKind=row->projectionKind;c.passPending=true;
            }
            if(!submit(c,s))return false;
            ++c.phase;break;
        case 12:{
            c.pending=0x8000f2e4;const auto* camera=s.currentCamera?s.currentCamera(s.user):nullptr;
            if(!camera)return false;
            g.savedView=camera->matrices.view;++c.phase;break;
        }
        case 13:{
            c.pending=0x8000f2e4;auto* camera=s.mutableCurrentCamera?s.mutableCurrentCamera(s.user):nullptr;
            if(!camera)return false;
            camera->matrices.view=c.auxiliary?c.auxiliary->view:w.view;
            if(c.auxiliary){c.active=false;c.pending=0;return true;}
            ++c.phase;break;
        }
        case 14:
            // Projection's pool/profile captures precede its current query.
            if(!c.pool){c.pool=w.record(0);c.profile=w.selectedProfile();}
            c.pending=0x8000f2e4;c.view=s.currentCamera?s.currentCamera(s.user):nullptr;
            if(!c.view || !c.profile)return false;
            c.projectionAlpha=uint8_t(w.color);c.step=0;c.index=0;++c.phase;break;
        case 15:
            while(c.step<25){
                if(c.step==22){
                    c.pending=0x80415fd0;const auto gp=s.gp14?s.gp14(s.user):std::nullopt;
                    if(!gp)return false;
                    const uint32_t a=c.projectionAlpha;c.gpColor=*gp?0xffffffffu:(a<<24|a<<16|a<<8|255);
                    ++c.step;continue;
                }
                if(!c.requestPending){c.request=prefix(c.step,w,g,c.gpColor);c.requestPending=true;}
                c.pending=uint32_t(c.request.kind);
                if(!s.gx || !s.gx(s.user,c.request))return false;
                c.requestPending=false;++c.step;
            }
            ++c.phase;break;
        case 16:
            while(c.index<w.count()){
                auto& record=c.pool[c.index];
                if(!c.geometryPending){
                    if(record.radius==0 || record.type!=3 || !(record.flags&2)){++c.index;continue;}
                    c.geometryPending=true;
                }
                c.pending=0x8002caf4;
                if(!s.geometry || !s.geometry(s.user,ShadowGeometry::Projection,c.camera,*c.view,record))return false;
                // Only complete six-index geometry increments retained owner.
                ++w.counter;c.geometryPending=false;++c.index;
            }
            ++c.phase;break;
        case 17:
            if(w.counter){c.pending=0x8029e8b8;return false;} // Nonzero compositing suffix remains unbound.
            ++c.phase;break;
        case 18:{
            c.pending=0x8000f2e4;auto* camera=s.mutableCurrentCamera?s.mutableCurrentCamera(s.user):nullptr;
            if(!camera)return false;
            camera->matrices.view=g.savedView;c.phase=c.drawClass==7?23:19;break;
        }
        case 19:case 20:
            if(!c.passPending){
                c.passRequest={};c.passPending=true;
                if(c.phase==19){c.passRequest.kind=ShadowPassKind::Viewport;c.passRequest.viewport=g.savedViewport;}
                else{c.passRequest.kind=ShadowPassKind::Scissor;c.passRequest.scissor=g.savedScissor;}
            }
            if(!submit(c,s))return false;
            ++c.phase;break;
        case 21:
            if(c.auxiliary?c.auxiliary->counter:w.counter){c.pending=c.drawClass==7?0x8002b4d8:0x8002b834;return false;}
            if(c.drawClass==2 && !c.auxiliarySetupStarted){
                c.auxiliary=&w.auxiliary[1];c.auxiliarySetupStarted=true;c.phase=3;break;
            }
            // Source End does not restore projection or reset request pool.
            c.active=false;c.pending=0;return true;
        case 23:
            if(!c.requestPending){c.request=gx(ShadowGX::ColorUpdate,{c.drawClass==7?1:0});c.requestPending=true;}
            c.pending=uint32_t(c.request.kind);
            if(!s.gx || !s.gx(s.user,c.request))return false;
            c.requestPending=false;c.phase=c.drawClass==7?19:5;break;
        default:return false;
        }
    }
}
}
