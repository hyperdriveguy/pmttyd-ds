#include "shadow_display.hpp"
#include <initializer_list>
namespace ttyd {
namespace {
ShadowGXRequest request(ShadowGX kind,std::initializer_list<int32_t> args){
    ShadowGXRequest r;r.kind=kind;unsigned i=0;for(auto value:args)r.values[i++]=value;return r;
}
// Prefix and suffix retain source ordering, including subsequently replaced
// arrays/descriptors. No record gate runs until the whole setup completes.
std::optional<ShadowGXRequest> setup(unsigned helper,unsigned step,const ShadowWork& work,const ShadowDisplayGlobals& globals,ShadowTexture texture){
    if(step==0)return request(ShadowGX::NumChans,{1});
    if(step==1)return request(ShadowGX::ChanCtrl,{4,0,0,0,0,2,2});
    if(step==2){auto r=request(ShadowGX::ChanMaterial,{4});r.color=work.color;return r;}
    const bool textured=helper==2;
    unsigned n=step-3;
    if(textured){
        // n0 is the separate retained texture lookup, handled by caller.
        if(n==1){auto r=request(ShadowGX::TextureBind,{0});r.texture=texture;return r;}
        if(n==2)return request(ShadowGX::NumTexGens,{1});
        if(n==3)return request(ShadowGX::TexCoordGen,{0,1,4,60,0,125});
        n-=4;
    }else{
        if(n==0)return request(ShadowGX::NumTexGens,{0});
        --n;
    }
    switch(n){
    case 0:return request(ShadowGX::NumTevStages,{1});
    case 1:return request(ShadowGX::TevOrder,{0,textured?0:255,textured?0:255,4});
    case 2:return request(ShadowGX::TevColorOp,{0,0,0,0,1,0});
    case 3:return request(ShadowGX::TevAlphaOp,{0,0,0,0,1,0});
    case 4:return request(ShadowGX::TevColorIn,{0,15,15,15,10});
    case 5:return request(ShadowGX::TevAlphaIn,{0,7,textured?4:1,5,7});
    case 6:{auto r=request(ShadowGX::Fog,{0});r.color=textured?globals.textureFog:globals.polygonFog;return r;}
    case 7:return request(ShadowGX::Blend,{1,4,5,0});
    case 8:return request(ShadowGX::Depth,{1,3,0});
    case 9:return request(ShadowGX::DepthLocation,{1});
    case 10:return request(ShadowGX::Alpha,{7,0,0,7,0});
    case 11:return request(ShadowGX::CurrentMatrix,{0});
    case 12:return request(ShadowGX::Cull,{0});
    }
    if(textured){
        switch(n){
        case 13:return request(ShadowGX::ClearDesc,{});
        case 14:return request(ShadowGX::Desc,{9,1});
        case 15:return request(ShadowGX::Desc,{13,1});
        case 16:return request(ShadowGX::Format,{0,9,1,4,0});
        case 17:return request(ShadowGX::Format,{0,13,1,4,0});
        }
    }else{
        switch(n){
        case 13:return request(ShadowGX::Array,{9,int32_t(0x80304b00u),12});
        case 14:return request(ShadowGX::ClearDesc,{});
        case 15:return request(ShadowGX::Desc,{9,2});
        case 16:return request(ShadowGX::Format,{0,9,1,4,0});
        }
        if(helper==1)switch(n){
        case 17:return request(ShadowGX::ClearDesc,{});
        case 18:return request(ShadowGX::Desc,{9,2});
        case 19:return request(ShadowGX::Desc,{11,2});
        case 20:return request(ShadowGX::Format,{0,9,1,4,0});
        case 21:return request(ShadowGX::Format,{0,11,1,5,0});
        case 22:return request(ShadowGX::Array,{9,int32_t(0x80304ce0u),12});
        case 23:return request(ShadowGX::Array,{11,int32_t(0x80415140u),4});
        }
    }
    return {};
}
bool scan(ShadowWork& work,ShadowDisplayCursor& c,unsigned helper,const ShadowDisplayGlobals& globals,const ShadowDisplayServices& s){
    constexpr uint8_t types[]{0,2,1};
    constexpr ShadowGeometry kinds[]{ShadowGeometry::Polygon,ShadowGeometry::Cylinder,ShadowGeometry::Texture};
    constexpr uint32_t functions[]{0x8002c818,0x8002bf84,0x8002c484};
    while(c.geometryPending || c.index<work.count()){
        if(c.index>=64)return false;
        auto& record=c.pool[c.index];
        if(!c.geometryPending){
            if(record.radius==0 || record.type!=types[helper] || !(record.flags&2) || record.camera!=int32_t(c.camera)){++c.index;continue;}
            c.geometryPending=true;
        }
        c.pending=functions[helper];
        if(helper==1 && s.cylinder.enabled){
            if(!shadowCylinder(work,c.camera,*c.view,record,c.cylinder,globals,s,c.pending))return false;
        }else if(!s.geometry || !s.geometry(s.user,kinds[helper],c.camera,*c.view,record))return false;
        c.geometryPending=false;++c.index;
    }
    return true;
}
}
bool shadowDisplay(ShadowWork& work,uint32_t camera,uint32_t userdata,ShadowDisplayCursor& c,ShadowDisplayGlobals& globals,const ShadowDisplayServices& s){
    if(!c.active){c={};c.active=true;c.camera=camera;c.userdata=userdata;c.owner=&work;}
    if(c.camera!=camera || c.userdata!=userdata || c.owner!=&work)return false;
    for(;;){
        if(c.phase==0){
            c.pending=0x80010864;
            const auto* current=s.currentRecord?s.currentRecord(s.user):nullptr;if(!current)return false;
            c.drawClass=current->drawClass;c.phase=1;
        }
        if(camera==2){
            if(c.drawClass!=5){
                if(c.drawClass!=0 && c.drawClass!=2 && c.drawClass!=7){c.active=false;c.pending=0;return true;}
                if(c.phase==1){
                    c.auxiliary=&work.auxiliary[c.drawClass==7?1:0];
                    c.auxiliarySetupStarted=c.drawClass==0;
                    c.phase=c.drawClass==0?3:18;
                }
                return shadowProjectionPass(work,c,globals,s);
            }
            if(c.phase==1){
                c.pending=0x8002d16c;
                if(!shadowPrepareCollision(work,c.collision,s.collision)){
                    c.pending=c.collision.rayPending?0x800158a4:0x8000f2e4;return false;
                }
                c.phase=2;
            }
            if(c.phase==2){
                // Pin the selected mutable row across a held camGetPtr(4).
                // Source has no clamp; outside the owned two-row domain hold.
                if(!c.profile){
                    c.profile=work.selectedProfile();
                    c.profileTracking=c.profile && c.profile->followCamera4==1;
                }
                if(!c.profile){c.pending=0x8002d3d8;return false;}
                if(c.profileTracking){
                    c.pending=0x8000f2fc;
                    const auto* camera4=s.cameraByIndex?s.cameraByIndex(s.user,4):nullptr;
                    if(!camera4)return false;
                    // Offsets18/1c/20 are camera4.at, NOT camera4.eye.
                    // Preserve float addition/store and reread target after
                    // the eye/up stores, matching the source capture order.
                    c.profile->eye[0]=camera4->at[0];
                    c.profile->eye[1]=float(1000.f+camera4->at[1]);
                    c.profile->eye[2]=camera4->at[2];
                    c.profile->up={0.f,0.f,-1.f};
                    c.profile->at[0]=camera4->at[0];
                    c.profile->at[1]=camera4->at[1];
                    c.profile->at[2]=camera4->at[2];
                }
                c.phase=3;
            }
            return shadowProjectionPass(work,c,globals,s);
        }
        if(camera!=1 && camera!=4 && camera!=5 && camera!=7){c.pending=0x8002d3d8;return false;}
        // phase1/6/8 capture helper pool/view; phase4 unused outer camera query.
        if(c.phase==1 || c.phase==6 || c.phase==8){
            if(!c.pool)c.pool=work.record(0); // Captured before a held camera call.
            c.pending=0x8000f2e4;
            c.view=s.currentCamera?s.currentCamera(s.user):nullptr;if(!c.view)return false;
            c.index=0;c.step=0;c.phase=c.phase==1?2:c.phase==6?7:9;
        }
        if(c.phase==4){
            c.pending=0x8000f2e4;if(!s.currentCamera || !s.currentCamera(s.user))return false;
            c.step=0;c.phase=5;
        }
        if(c.phase==2 || c.phase==5 || c.phase==9){
            const unsigned helper=c.phase==2?0:c.phase==5?1:2;
            while(true){
                if(helper==2 && c.step==3){
                    c.pending=0x801d78b0;if(!s.texture44 || !s.texture44(s.user,c.texture))return false;
                    ++c.step;continue;
                }
                if(!c.requestPending){auto next=setup(helper,c.step,work,globals,c.texture);if(!next)break;c.request=*next;c.requestPending=true;}
                c.pending=uint32_t(c.request.kind);
                if(!s.gx || !s.gx(s.user,c.request))return false;
                c.requestPending=false;++c.step;
            }
            c.phase=helper==0?3:helper==1?6:10;
            if(helper==1)c.pool=nullptr;
        }
        if(c.phase==3 || c.phase==7 || c.phase==10){
            const unsigned helper=c.phase==3?0:c.phase==7?1:2;
            if(!scan(work,c,helper,globals,s))return false;
            if(helper==2){c.active=false;c.pending=0;return true;}
            c.phase=helper==0?4:8;c.pool=nullptr;
        }
    }
}
}
