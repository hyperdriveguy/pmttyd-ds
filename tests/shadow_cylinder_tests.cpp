#include "shadow_display.hpp"
#include "native_shadow_program.hpp"
#include "native_display_state.hpp"
#include "native_camera_draw.hpp"
#include "field_air_shadow.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {
    ShadowWork work;ShadowRecord record;CameraState camera;ShadowCylinderCursor cursor;ShadowDisplayGlobals globals;
    NativeShadowProgram program;NativeDisplayState display;
    uint32_t pending=0,hold=0;unsigned remaining=0;
    std::vector<ShadowGXRequest> requests;std::vector<uint32_t> lists,bounds;std::vector<NativeDisplayPipeline> pipelines;
    std::vector<std::array<uint32_t,3>> drawStates;
    Fixture(){
        record.flags=3;record.radius=20;record.height=20;record.camera=4;record.type=2;record.alpha=255;
        record.scale=1;record.inner=.2f;record.outer=.4f;record.collision=-1;
        record.matrix={-20,0,0,-100,0,20,0,1,0,0,-20,-5};
        camera.matrices.view={{{1,0,0,17},{0,1,0,23},{0,0,1,31}}};
        program.afterDummy();display.afterDummy();program.material=0x6e;program.alpha=NativeShadowProgram::RegisterRaster;
        program.chans=1;program.texGens=0;
        NativeDisplaySubmit inherited{nullptr,[](void*,const NativeDisplayPipeline&){return true;}};
        assert(display.request({DisplayGXState::AlphaCompare,{7,0,0,7,0}},inherited));
        assert(display.request({DisplayGXState::ZCompareLocation,{1}},inherited));
        program.rasterRGB=true;program.descriptors={2,2,0};program.formats={4,5,0};program.arrays={0x80304ce0,0x80415140,0};
    }
    bool blocked(uint32_t id){if(id==hold && remaining){--remaining;return true;}return false;}
    ShadowDisplayServices services(){
        ShadowDisplayServices s;s.user=this;s.cylinder.enabled=true;s.cylinder.user=this;
        s.gx=[](void* user,const ShadowGXRequest& r){
            auto& f=*static_cast<Fixture*>(user);if(f.blocked(uint32_t(r.kind)))return false;
            DisplayStateRequest state{};bool pipeline=true;
            switch(r.kind){
            case ShadowGX::Depth:state.kind=DisplayGXState::ZMode;break;
            case ShadowGX::Blend:state.kind=DisplayGXState::BlendMode;break;
            case ShadowGX::ColorUpdate:state.kind=DisplayGXState::ColorUpdate;break;
            case ShadowGX::AlphaUpdate:state.kind=DisplayGXState::AlphaUpdate;break;
            case ShadowGX::PositionMatrix:pipeline=false;break;
            default:{pipeline=false;const auto next=f.program.request(r);assert(next);f.program=*next;}
            }
            if(pipeline){for(unsigned i=0;i<5;++i)state.values[i]=r.values[i];
                NativeDisplaySubmit submit{&f,[](void* user,const NativeDisplayPipeline& p){static_cast<Fixture*>(user)->pipelines.push_back(p);return true;}};
                assert(f.display.request(state,submit));
            }
            f.requests.push_back(r);return true;
        };
        s.cylinder.list=[](void* user,uint32_t address,uint32_t bytes){
            auto& f=*static_cast<Fixture*>(user);if(f.blocked(0x802b9fa0))return false;assert(bytes==64);
            f.lists.push_back(address);f.drawStates.push_back({uint32_t(f.display.state()->logicOr),uint32_t(f.program.materialVertex),f.program.arrays[1]});return true;
        };
        s.cylinder.bounds=[](void* user,ShadowCylinderBounds& b){
            auto& f=*static_cast<Fixture*>(user);if(f.blocked(b.function))return false;f.bounds.push_back(b.function);
            if(b.function==0x802b6a08)b.values={1,2,3,4};
            if(b.function==0x80072f38){assert(b.slot==-1 && (b.values==std::array<uint16_t,4>{1,2,3,4}));}
            return true;
        };
        return s;
    }
    bool run(uint32_t cameraId=4){return shadowCylinder(work,cameraId,camera,record,cursor,globals,services(),pending);}
};
int main(){
    Fixture f;assert(f.run() && f.requests.size()==24 && f.lists.size()==12 && f.bounds.empty());
    const auto& matrix=f.requests[0];assert(matrix.kind==ShadowGX::PositionMatrix);
    assert(matrix.matrix[0][0]==-20 && matrix.matrix[1][1]==20 && matrix.matrix[2][2]==-20);
    assert(matrix.matrix[0][3]==-83 && matrix.matrix[1][3]==24 && matrix.matrix[2][3]==26);
    assert(f.requests[1].kind==ShadowGX::TevColor && f.requests[1].color==255);
    const std::vector<uint32_t> expected{0x80304c20,0x80304c60,0x80304ba0,0x80304be0};
    for(unsigned pass=0;pass<3;++pass)for(unsigned i=0;i<4;++i)assert(f.lists[pass*4+i]==expected[i]);
    assert(f.drawStates[0][0]==1 && f.drawStates[0][1]==1);
    assert(f.drawStates[4][0]==0 && f.drawStates[4][1]==0);
    assert(f.display.state()->color && !f.display.state()->alpha && !f.display.state()->depthWrite);
    assert(f.display.state()->sourceBlend==6 && f.display.state()->destinationBlend==7 && !f.display.state()->logicOr);
    assert(f.program.registerAlpha==255 && f.program.material==0x6e && f.work.counter==0);
    // Held matrix/TEV/pass/palette/list operations never replay complete calls.
    for(uint32_t id:{0x802ba2fcu,0x802b8ab8u,0x802b93d8u,0x802b5c8cu,0x802b932cu,0x802b93acu,0x802b9380u,0x802b6f78u,0x802b4db0u,0x802b9fa0u}){
        Fixture held;held.hold=id;held.remaining=1;assert(!held.run() && held.pending==id);
        assert(held.run() && held.requests.size()==f.requests.size() && held.lists==f.lists);
    }
    Fixture captured;captured.hold=uint32_t(ShadowGX::PositionMatrix);captured.remaining=1;
    assert(!captured.run());captured.record.matrix[3]=900;captured.camera.matrices.view[0][3]=1000;captured.record.alpha=73;
    assert(captured.run() && captured.requests[0].matrix[0][3]==-83 && captured.requests[1].color==73);
    // A completed pass plan persists during a list hold; later passes dispatch
    // and capture flags afresh. No-decay keeps geometry even at global alpha0.
    Fixture mutation;mutation.hold=0x802b9fa0;mutation.remaining=1;assert(!mutation.run());
    mutation.record.flags|=4;mutation.work.color=0;
    assert(mutation.run() && mutation.lists.size()==10);
    assert(mutation.lists[0]==0x80304c20 && mutation.lists[4]==0x80304b60 && mutation.lists[7]==0x80304b60);
    Fixture transparent;transparent.work.color=0;assert(transparent.run() && transparent.lists.empty() && transparent.requests.size()==21);
    Fixture noDecay;noDecay.record.flags|=4;noDecay.work.color=0;assert(noDecay.run() && noDecay.lists.size()==9);
    // Bounds surround all three passes once; they run even with zero lists.
    Fixture bounded;bounded.work.color=0;assert(bounded.run(1));
    assert((bounded.bounds==std::vector<uint32_t>{0x80005ce4,0x802b69d0,0x80005ce4,0x802b6a08,0x80072f38}));
    Fixture boundHold;boundHold.hold=0x80072f38;boundHold.remaining=1;assert(!boundHold.run(1));
    boundHold.record.collision=99;assert(boundHold.run(1)); // Captured signed slot and read bounds survive hold.
    // Actual imported arrays retain all source pairs/fan/strip triangles.
    FieldShadowGeometry geometry;
    if(geometry.load("romfs/shadows.pmshadow")){
        unsigned triangles=0;for(unsigned i:{3u,4u,1u,2u})triangles+=unsigned(geometry.lists[i].pairs.size()-2);
        assert(triangles==84 && geometry.palettes[0][0][3]==110 && geometry.palettes[0][2][3]==22);
    }
    // Position cache publishes only after submit and restores that exact matrix.
    NativeCameraDraw camera;std::vector<NativeCameraPacket> packets;
    NativeCameraSubmit submit{&packets,[](void* user,const NativeCameraPacket& p){static_cast<std::vector<NativeCameraPacket>*>(user)->push_back(p);return true;}};
    CameraProjectionMatrix projection{};for(unsigned i=0;i<4;++i)projection[i][i]=1;
    assert(!camera.position(matrix.matrix,submit));assert(camera.projection(projection,submit));assert(camera.position(matrix.matrix,submit));
    assert(camera.restore(submit) && packets.back().transform==nativeCameraTransform(projection,matrix.matrix));
}
