#include "shadow_display.hpp"
#include "native_camera_draw.hpp"
#include "native_shadow_program.hpp"
#include <cassert>
#include <cmath>
#include <vector>
using namespace ttyd;
struct Fixture {
    ShadowWork work;ShadowDisplayCursor cursor;ShadowDisplayGlobals globals;
    CameraState camera,camera4;DisplayRecord record{2,5,0,0,0x8002d3d8,0};
    NativeCameraDraw backend;NativeShadowProgram program;
    std::vector<uint32_t> calls;std::vector<NativeCameraPacket> packets;
    std::vector<ShadowGXRequest> gxCalls;
    uint32_t hold=0;unsigned remaining=0,geometryCalls=0,cameraQueries=0,mutableQueries=0;
    std::optional<uint32_t> gp=1;
    bool changeColor=false,changeAfterInvalidate=false,distinctCameras=false;
    std::array<CameraState,4> cameraTokens{};
    Fixture(){
        camera.matrices.view={{{1,0,0,17},{0,1,0,23},{0,0,1,31}}};
        camera.viewport={0,0,608,480,.1f,1};camera.scissor={0,0,608,480};
        camera4.eye={99,98,97};camera4.at={10,20,30};program.afterDummy();
        const auto s=submit();assert(backend.request({CameraDrawRequestKind::Viewport,2,&camera},s));
        assert(backend.request({CameraDrawRequestKind::Scissor,2,&camera},s));packets.clear();
    }
    NativeCameraSubmit submit(){return {this,[](void* user,const NativeCameraPacket& p){static_cast<Fixture*>(user)->packets.push_back(p);return true;}};}
    bool blocked(uint32_t id){if(hold==id && remaining){--remaining;return true;}return false;}
    ShadowDisplayServices services(){
        ShadowDisplayServices s;s.user=this;
        s.currentRecord=[](void* p)->const DisplayRecord*{return &static_cast<Fixture*>(p)->record;};
        s.cameraByIndex=[](void* p,uint32_t id)->const CameraState*{assert(id==4);return &static_cast<Fixture*>(p)->camera4;};
        s.currentCamera=[](void* p)->const CameraState*{auto& f=*static_cast<Fixture*>(p);++f.cameraQueries;return f.distinctCameras?&f.cameraTokens[f.cameraQueries==1?0:2]:&f.camera;};
        s.mutableCurrentCamera=[](void* p)->CameraState*{auto& f=*static_cast<Fixture*>(p);++f.mutableQueries;return f.distinctCameras?&f.cameraTokens[f.mutableQueries==1?1:3]:&f.camera;};
        s.collision.currentCamera=[](void*){return true;};
        s.pass=[](void* p,ShadowPassRequest& r){
            auto& f=*static_cast<Fixture*>(p);const auto id=uint32_t(r.kind);if(f.blocked(id))return false;
            f.calls.push_back(id);
            switch(r.kind){
            case ShadowPassKind::InvalidateTextures:
                if(f.changeAfterInvalidate){f.work.profileIndex=0;f.work.profiles[0].sizeIndex=2;}return true;
            case ShadowPassKind::ReadViewport:r.viewport=*f.backend.viewport();return true;
            case ShadowPassKind::ReadScissor:r.scissor=*f.backend.scissor();return true;
            case ShadowPassKind::Viewport:return f.backend.viewport(r.viewport,f.submit(),true);
            case ShadowPassKind::Scissor:return f.backend.scissor(r.scissor,f.submit(),true);
            case ShadowPassKind::Projection:return f.backend.projection(r.projection,f.submit());
            }
            return false;
        };
        s.gx=[](void* p,const ShadowGXRequest& r){
            auto& f=*static_cast<Fixture*>(p);if(f.blocked(uint32_t(r.kind)))return false;
            switch(r.kind){case ShadowGX::Blend:case ShadowGX::Depth:case ShadowGX::DepthLocation:case ShadowGX::Alpha:case ShadowGX::ColorUpdate:break;
            default:{const auto next=f.program.request(r);assert(next);f.program=*next;}}
            f.gxCalls.push_back(r);f.calls.push_back(uint32_t(r.kind));
            if(f.changeColor && r.kind==ShadowGX::Blend)f.work.color=0x11223344;
            return true;
        };
        s.gp14=[](void* p)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(0x80415fd0);return f.gp;};
        s.geometry=[](void* p,ShadowGeometry kind,uint32_t camera,const CameraState&,ShadowRecord&){
            auto& f=*static_cast<Fixture*>(p);assert(kind==ShadowGeometry::Projection && camera==2);++f.geometryCalls;return !f.blocked(0x8002caf4);
        };
        return s;
    }
    bool run(){return shadowDisplay(work,2,0,cursor,globals,services());}
};
int main(){
    for(uint8_t cls:{1,3,4,6,8,255}){
        Fixture skipped;skipped.record.drawClass=cls;assert(skipped.run() && skipped.calls.empty());
    }
    Fixture f;const auto enteringView=f.camera.matrices.view;
    assert(f.work.enter({0,0,0},20));f.work.record(0)->flags=2; // Prepared type2, mandatory setup still runs.
    assert(f.run() && !f.cursor.active && f.cursor.pending==0);
    assert(f.work.count()==1 && f.work.counter==0 && f.geometryCalls==0 && f.work.flags==0);
    assert(f.camera.matrices.view==enteringView && f.cameraQueries==2 && f.mutableQueries==2);
    assert(f.backend.viewport()==std::optional<std::array<float,6>>(f.camera.viewport));
    assert((f.backend.scissor()==std::optional<std::array<uint32_t,4>>({0,0,608,480})));
    assert(f.gxCalls.size()==24 && f.gxCalls[2].color==0x6e && f.gxCalls[22].color==0xffffffff);
    assert(f.gxCalls[10].kind==ShadowGX::Fog && f.gxCalls[10].color==f.globals.projectionFog);
    assert(f.gxCalls[21].kind==ShadowGX::Blend && f.gxCalls[21].values[0]==0);
    assert(f.gxCalls.back().kind==ShadowGX::Depth && f.gxCalls.back().values[0]==0);
    assert(f.packets.size()==5 && f.packets[0].rect.x==40 && f.packets[0].rect.y==137 && f.packets[0].rect.width==200 && f.packets[0].rect.height==263);
    assert(f.packets[3].kind==NativeCameraCommand::Viewport && f.packets[4].kind==NativeCameraCommand::Scissor);
    assert(f.calls.front()==uint32_t(ShadowPassKind::InvalidateTextures));
    assert(f.calls[1]==uint32_t(ShadowPassKind::ReadViewport) && f.calls[2]==uint32_t(ShadowPassKind::ReadScissor));
    // The three const/mutable current-camera reads are independent tokens.
    Fixture tokens;tokens.distinctCameras=true;
    for(unsigned i=0;i<4;++i)tokens.cameraTokens[i].matrices.view[0][3]=float(100+i);
    const auto originalBackup=tokens.cameraTokens[0].matrices.view;
    assert(tokens.run() && tokens.cameraTokens[1].matrices.view==tokens.work.view);
    assert(tokens.cameraTokens[2].matrices.view[0][3]==102 && tokens.cameraTokens[3].matrices.view==originalBackup);
    // Auxiliary source owners share the same backup buffers. Class0 and
    // class2 still set up their projection at counter0; class7 re-enables color.
    Fixture aux;const auto originalAuxView=aux.camera.matrices.view;assert(aux.run());
    assert(aux.work.auxiliary[0].flags==2 && aux.work.auxiliary[1].profileIndex==0);
    aux.record.drawClass=0;const auto class0Start=aux.calls.size();assert(aux.run());
    assert(aux.calls.size()==class0Start+6 && aux.camera.matrices.view==aux.work.auxiliary[0].view);
    assert(aux.globals.savedView==originalAuxView && aux.gxCalls.size()==24);
    aux.globals.savedView[0][3]=321;aux.record.drawClass=2;
    const auto class2Start=aux.calls.size();assert(aux.run());
    assert(aux.calls.size()==class2Start+9 && aux.camera.matrices.view==aux.work.auxiliary[1].view);
    assert(aux.globals.savedView[0][3]==321 && aux.gxCalls.back().kind==ShadowGX::ColorUpdate && aux.gxCalls.back().values[0]==0);
    aux.record.drawClass=7;aux.hold=uint32_t(ShadowGX::ColorUpdate);aux.remaining=1;
    assert(!aux.run() && aux.cursor.pending==uint32_t(ShadowGX::ColorUpdate) && aux.camera.matrices.view[0][3]==321);
    const auto restored=aux.calls.size();assert(aux.run());assert(aux.calls.size()==restored+3);
    assert(aux.gxCalls.back().values[0]==1 && aux.work.auxiliary[1].flags==2);
    // A stale auxiliary counter still restores before admitting image work.
    Fixture staleAux;assert(staleAux.run());staleAux.record.drawClass=0;assert(staleAux.run());
    staleAux.work.auxiliary[0].counter=1;staleAux.record.drawClass=2;
    const auto staleCalls=staleAux.calls.size();assert(!staleAux.run());
    assert(staleAux.calls.size()==staleCalls+2 && staleAux.cursor.pending==0x8002b834 && staleAux.camera.matrices.view==staleAux.globals.savedView);
    // Every GPU/readback hold resumes without repeating an acknowledged call.
    for(uint32_t id:{0x802b7b54u,0x802ba5b4u,0x802ba6bcu,0x802ba56cu,0x802ba644u,0x802ba184u,0x802b6f3cu,0x802b6e54u,0x802b932cu,0x802b93d8u}){
        Fixture held;held.hold=id;held.remaining=1;
        assert(!held.run() && held.cursor.pending==id);const auto completed=held.calls;
        assert(held.run());assert(held.calls==f.calls); // Includes mandatory prefix/restoration on empty pool.
        assert(held.calls.size()>=completed.size());
    }
    // Retain alpha captured before prefix even when material/global color changes.
    Fixture gray;gray.gp=0;gray.changeColor=true;assert(gray.run());
    assert(gray.gxCalls[2].color==0x6e && gray.gxCalls[22].color==0x6e6e6eff);
    Fixture gpWait;gpWait.gp.reset();assert(!gpWait.run() && gpWait.cursor.pending==0x80415fd0);
    const auto prefixCount=gpWait.gxCalls.size();assert(prefixCount==22);
    gpWait.gp=0;gpWait.work.color=0xff;assert(gpWait.run());
    assert(gpWait.gxCalls.size()==24 && gpWait.gxCalls[22].color==0x6e6e6eff);
    // Select size after invalidation; row0 takes the recovered frustum branch.
    Fixture reselect;reselect.changeAfterInvalidate=true;assert(reselect.run());
    assert(reselect.cursor.size==128 && std::fabs(reselect.work.projection[0][0]-17.8f)<.00001f);
    assert(reselect.work.projection[3][2]==-1 && reselect.work.projection[3][3]==0);
    assert(std::fabs(reselect.work.projection[2][2]-(-356.f/385.f))<.000001f);
    // Type3 has no camera filter. Increment counter only after complete geometry.
    Fixture volume;volume.work.type=3;assert(volume.work.enter({0,0,0},1));volume.work.record(0)->flags=2;volume.work.record(0)->camera=99;
    volume.hold=0x8002caf4;volume.remaining=1;assert(!volume.run() && volume.work.counter==0);
    assert(!volume.run() && volume.work.counter==1 && volume.cursor.pending==0x8029e8b8);
    const auto emitted=volume.calls.size();assert(!volume.run() && volume.calls.size()==emitted && volume.work.counter==1);
    Fixture wrapped;wrapped.work.counter=0xffffffffu;wrapped.work.type=3;
    assert(wrapped.work.enter({0,0,0},1));wrapped.work.record(0)->flags=2;
    assert(wrapped.run() && wrapped.work.counter==0 && wrapped.geometryCalls==1);
    Fixture invalidSize;invalidSize.work.profiles[1].sizeIndex=4;
    assert(!invalidSize.run() && invalidSize.calls.size()==1 && invalidSize.cursor.pending==0x8002d3d8);
    Fixture stale;stale.work.counter=1;assert(!stale.run() && stale.gxCalls.size()==24 && stale.cursor.pending==0x8029e8b8);
    // End restores view before an unavailable viewport, then scissor without
    // restoring source projection or publishing an image completion flag.
    Fixture end;
    // Drive to GP wait, then arrange a viewport hold only in restoration.
    end.gp.reset();assert(!end.run());end.gp=1;
    end.hold=uint32_t(ShadowPassKind::Viewport);end.remaining=1;
    assert(!end.run() && end.cursor.phase==19 && end.camera.matrices.view==end.globals.savedView);
    assert(end.run() && end.work.flags==0 && end.work.counter==0);
    // A held projection setter keeps captured matrix/kind while live rows mutate.
    Fixture matrix;matrix.hold=uint32_t(ShadowPassKind::Projection);matrix.remaining=1;
    assert(!matrix.run());const auto captured=matrix.cursor.passRequest;
    matrix.work.profiles[1].top=120;matrix.work.profiles[1].projectionKind=0;
    assert(matrix.run() && matrix.work.projection==captured.projection && captured.projectionKind==1);
}
