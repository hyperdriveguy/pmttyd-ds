#include "shadow_display.hpp"
#include "native_shadow_program.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {
    ShadowWork work;ShadowDisplayCursor cursor;ShadowDisplayGlobals globals;
    CameraState camera;DisplayRecord record{1,9,0,0,0x8002d3d8,0};
    NativeShadowProgram program;
    bool cameraReady=true,textureReady=true,gxReady=true,geometryReady=false;
    unsigned cameras=0,lookups=0,geometries=0;
    std::vector<ShadowGXRequest> requests;
    Fixture(){program.afterDummy();}
    ShadowDisplayServices services(){return {this,
        [](void* p)->const DisplayRecord*{return &static_cast<Fixture*>(p)->record;},
        [](void* p)->const CameraState*{auto& f=*static_cast<Fixture*>(p);++f.cameras;return f.cameraReady?&f.camera:nullptr;},
        [](void* p,const ShadowGXRequest& r){auto& f=*static_cast<Fixture*>(p);if(!f.gxReady)return false;
            switch(r.kind){case ShadowGX::Blend:case ShadowGX::Depth:case ShadowGX::DepthLocation:case ShadowGX::Alpha:break;
            default:{auto next=f.program.request(r);assert(next);f.program=*next;}}
            f.requests.push_back(r);return true;},
        [](void* p,ShadowTexture& t){auto& f=*static_cast<Fixture*>(p);++f.lookups;if(!f.textureReady)return false;t=ShadowTexture::N64Texture44;return true;},
        [](void* p,ShadowGeometry,uint32_t,const CameraState&,ShadowRecord&){auto& f=*static_cast<Fixture*>(p);++f.geometries;return f.geometryReady;},
        {}};}
    bool run(){return shadowDisplay(work,1,0,cursor,globals,services());}
};
int main(){
    Fixture f;assert(f.work.enter({1,2,3},4));
    assert(f.run() && f.requests.size()==73 && f.cameras==4 && f.lookups==1 && f.geometries==0);
    // Original three independent prefixes and both overwritten cylinder arrays
    // are submitted; unconditional texture load precedes texture record gates.
    assert(f.requests[0].kind==ShadowGX::NumChans && f.requests[21].kind==ShadowGX::NumChans && f.requests[49].kind==ShadowGX::NumChans);
    assert(f.requests[52].kind==ShadowGX::TextureBind && f.requests[53].kind==ShadowGX::NumTexGens && f.requests[54].kind==ShadowGX::TexCoordGen);
    assert(f.requests[17].kind==ShadowGX::Array && uint32_t(f.requests[17].values[1])==0x80304b00);
    assert(f.requests[47].kind==ShadowGX::Array && uint32_t(f.requests[47].values[1])==0x80304ce0);
    assert(f.requests[48].kind==ShadowGX::Array && uint32_t(f.requests[48].values[1])==0x80415140);
    assert(f.program.alpha==NativeShadowProgram::TextureRaster && f.program.registerAlpha==129 && f.program.material==0x6e);
    assert((f.program.descriptors==std::array<uint32_t,3>{1,0,1}) && f.program.arrays[0]==0x80304ce0 && f.program.fogOff && f.program.cull==0);
    // A held material request pins its copied RGBA even if the owner changes.
    Fixture held;held.gxReady=false;held.cameraReady=false;
    assert(!held.run() && held.cursor.pool==held.work.record(0) && held.requests.empty());
    held.cameraReady=true;assert(!held.run() && held.cursor.pending==uint32_t(ShadowGX::NumChans));
    auto services=held.services();
    services.gx=[](void* p,const ShadowGXRequest& r){auto& f=*static_cast<Fixture*>(p);
        if(r.kind==ShadowGX::ChanMaterial)return false;f.requests.push_back(r);return true;};
    assert(!shadowDisplay(held.work,1,0,held.cursor,held.globals,services));
    assert(held.cursor.request.color==0x6e);held.work.color=0x11223344;
    services.gx=[](void* p,const ShadowGXRequest& r){static_cast<Fixture*>(p)->requests.push_back(r);return true;};
    assert(shadowDisplay(held.work,1,0,held.cursor,held.globals,services));
    assert(held.requests[2].color==0x6e && held.requests[23].color==0x11223344 && held.requests[51].color==0x11223344);
    // Real texture query hold retains complete earlier setup; no record type1
    // is necessary for the lookup and its actual binding.
    Fixture tex;tex.textureReady=false;assert(!tex.run());
    assert(tex.requests.size()==52 && tex.lookups==1 && tex.cursor.pending==0x801d78b0);
    tex.textureReady=true;assert(tex.run() && tex.requests.size()==73 && tex.lookups==2);
    // An admitted record cannot be acknowledged without its full geometry/
    // bounding provider; changes during the hold do not restart admission.
    Fixture geometry;geometry.work.type=0;assert(geometry.work.enter({0,0,0},1));
    auto* r=geometry.work.record(0);r->flags=3;r->camera=1;
    assert(!geometry.run() && geometry.requests.size()==21 && geometry.geometries==1);
    r->radius=0;r->flags=0;geometry.geometryReady=true;
    assert(geometry.run() && geometry.geometries==2 && geometry.requests.size()==73);
    // Setup domain excludes unknown inherited state and unsupported literals.
    NativeShadowProgram unknown;ShadowGXRequest req;req.values[0]=1;
    assert(!unknown.request(req));unknown.afterDummy();assert(unknown.request(req));
    req.values[0]=2;assert(!unknown.request(req));
    auto known=f.program;known.afterDummy();
    assert(known.registerAlpha==129 && known.alpha==NativeShadowProgram::Register && known.material==0x6e && known.fogOff && known.texture==ShadowTexture::N64Texture44);
    // Camera2/class5 first executes independent Collision, never image success.
    Fixture preparation;preparation.record.camera=2;preparation.record.drawClass=5;
    assert(preparation.work.enter({1,2,3},4));auto ss=preparation.services();
    ss.collision.currentCamera=[](void*){return true;};
    assert(!shadowDisplay(preparation.work,2,0,preparation.cursor,preparation.globals,ss));
    assert(preparation.cursor.pending==0x800158a4 && preparation.work.record(0)->flags==1);
    ss.collision.ray=[](void*,const ShadowCollisionRay&,ShadowCollisionHit&){return ShadowRayResult::Miss;};
    assert(!shadowDisplay(preparation.work,2,0,preparation.cursor,preparation.globals,ss));
    assert(preparation.cursor.pending==0x8002d3d8 && preparation.work.record(0)->flags==1);
}
