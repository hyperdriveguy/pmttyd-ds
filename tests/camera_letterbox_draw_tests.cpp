#include "camera_letterbox_draw.hpp"
#include "native_camera_draw.hpp"
#include "native_shadow_program.hpp"
#include "native_display_state.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {
    CameraState camera;NativeCameraDraw backend;NativeShadowProgram program;NativeDisplayState display;CameraLetterboxVertexStream stream;
    uint32_t gp=0x90000000,flags=0,color=0x11223344,holdGX=0;bool holdBegin=false,ready=true,changeGp=false;
    int holdVertex=-1;unsigned alphaReads=0,topReads=0,bottomReads=0,begins=0,submits=0;
    std::vector<uint32_t> calls;std::vector<NativeCameraPacket> packets;std::vector<CameraLetterboxVertices> primitives;
    std::array<int16_t,2> tops{{49,49}},bottoms{{44,44}};
    NativeCameraSubmit sink(){return {this,[](void* u,const NativeCameraPacket& p){static_cast<Fixture*>(u)->packets.push_back(p);return true;}};}
    Fixture(){camera.viewPublished=true;camera.letterboxAlpha=255;camera.matrices.view={{{1,0,0,7},{0,1,0,8},{0,0,1,9}}};
        camera.matrices.projection=cameraPerspective(25,1.26666665f,100,10000);camera.projectionKind=0;
        assert(backend.viewport({0,0,608,480,.1f,.2f},sink()) && backend.scissor({0,0,608,480},sink()));
        assert(backend.projection(cameraOrthographic(240,-240,-304,304,1,32768),sink(),1));program.afterDummy();display.afterDummy();}
    CameraLetterboxDrawServices services(){
        CameraLetterboxDrawServices s;s.user=this;
        s.gp=[](void* u)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);f.calls.push_back(1);return f.gp;};
        s.camera=[](void* u,uint32_t i)->const CameraState*{auto& f=*static_cast<Fixture*>(u);assert(i==8);f.calls.push_back(2);if(f.changeGp)f.gp+=0x100;return &f.camera;};
        s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);f.calls.push_back(a);
            if(a==0x90000000)return f.flags;if(a==f.gp)return 0x8000;if(a==0x80429510)return f.color;if(a==0x8041f6b0)return 0xffffffff;return std::nullopt;};
        s.half=[](void* u,const CameraState& c,uint32_t a)->std::optional<int16_t>{auto& f=*static_cast<Fixture*>(u);f.calls.push_back(a);
            if(a==0x1e0){++f.alphaReads;return c.letterboxAlpha;}if(a==0x1e2)return f.tops[std::min(f.topReads++,1u)];if(a==0x1e4)return f.bottoms[std::min(f.bottomReads++,1u)];return std::nullopt;};
        s.pass=[](void* u,LetterboxPassRequest& r){auto& f=*static_cast<Fixture*>(u);f.calls.push_back(uint32_t(r.kind));switch(r.kind){
            case LetterboxPass::ReadViewport:r.viewport=*f.backend.viewport();return true;
            case LetterboxPass::ReadProjection:r.vector=*f.backend.projectionVector();return true;
            case LetterboxPass::ReadScissor:r.scissor=*f.backend.scissor();return true;
            case LetterboxPass::Projection:return f.backend.projection(r.projection,f.sink(),r.projectionKind);
            case LetterboxPass::ProjectionVector:return f.backend.projectionVector(r.vector,f.sink());
            case LetterboxPass::Viewport:return f.backend.viewport(r.viewport,f.sink(),true);
            case LetterboxPass::Scissor:return f.backend.scissor(r.scissor,f.sink(),true);
            }return false;};
        s.gx=[](void* u,const ShadowGXRequest& r){auto& f=*static_cast<Fixture*>(u);if(uint32_t(r.kind)==f.holdGX)return false;f.calls.push_back(uint32_t(r.kind));
            if(r.kind==ShadowGX::NumChans)f.camera.letterboxAlpha=-2; // Fresh material alpha differs from the admission read.
            if(r.kind==ShadowGX::PositionMatrix)return f.backend.position(r.matrix,f.sink());
            if(r.kind==ShadowGX::Depth || r.kind==ShadowGX::Blend || r.kind==ShadowGX::Alpha || r.kind==ShadowGX::DepthLocation){DisplayStateRequest q;
                q.kind=r.kind==ShadowGX::Depth?DisplayGXState::ZMode:r.kind==ShadowGX::Blend?DisplayGXState::BlendMode:r.kind==ShadowGX::Alpha?DisplayGXState::AlphaCompare:DisplayGXState::ZCompareLocation;
                for(unsigned i=0;i<5;++i)q.values[i]=r.values[i];return f.display.request(q,{nullptr,[](void*,const NativeDisplayPipeline&){return true;}});}
            const auto next=f.program.request(r);assert(next);f.program=*next;return true;};
        s.begin=[](void* u){auto& f=*static_cast<Fixture*>(u);if(f.holdBegin)return false;f.calls.push_back(0x802b5a6c);++f.begins;f.stream.begin();return true;};
        s.vertex=[](void* u,uint32_t i,const std::array<int16_t,3>& p){auto& f=*static_cast<Fixture*>(u);if(int(i)==f.holdVertex)return false;
            return f.stream.vertex(i,p,{u,[](void* u,const CameraLetterboxVertices& v){auto& f=*static_cast<Fixture*>(u);if(!f.ready)return false;++f.submits;f.primitives.push_back(v);return true;}});};return s;
    }
};
int main(){
    Fixture f;CameraLetterboxDrawCursor c;f.flags=0x8000;assert(drawCameraLetterbox(f.services(),c) && f.alphaReads==0 && f.packets.size()==3);
    assert(f.calls==std::vector<uint32_t>({1,2,0x90000000}));
    f=Fixture{};c={};f.camera.letterboxAlpha=0;assert(drawCameraLetterbox(f.services(),c) && f.alphaReads==1 && f.calls.size()==4);
    f=Fixture{};c={};f.changeGp=true;f.holdGX=uint32_t(ShadowGX::ChanMaterial);
    const auto viewport=*f.backend.viewport();const auto projection=*f.backend.projectionVector();const auto scissor=*f.backend.scissor();
    assert(!drawCameraLetterbox(f.services(),c) && c.pending==f.holdGX && c.gx.color==0x112233fe && f.alphaReads==2);
    // Admission retained the original GP pointer. Material captures RGB before
    // fresh alpha, and a held setter does not recapture either operand.
    f.color=0xaabbccdd;f.camera.letterboxAlpha=7;f.holdGX=0;f.holdBegin=true;
    assert(!drawCameraLetterbox(f.services(),c) && c.pending==0x802b5a6c && *f.program.material==0x112233fe);
    assert(f.topReads==0 && f.bottomReads==0 && f.begins==0);
    f.tops={-32768,49};f.bottoms={32767,44};f.holdBegin=false;f.holdVertex=3;
    assert(!drawCameraLetterbox(f.services(),c) && c.pending==0xcc008000 && f.stream.count()==3 && f.topReads==2);
    f.tops={1000,2000};f.holdVertex=-1;f.ready=false;
    assert(!drawCameraLetterbox(f.services(),c) && f.stream.count()==8 && f.begins==1 && f.submits==0 && f.topReads==2 && f.bottomReads==2);
    const auto before=f.calls.size();f.camera.letterboxBottom=999;f.ready=true;
    assert(drawCameraLetterbox(f.services(),c) && f.begins==1 && f.submits==1 && !f.stream.active());
    assert(f.calls.size()==before+3 && f.calls[before]==uint32_t(LetterboxPass::Viewport) && f.calls[before+1]==uint32_t(LetterboxPass::ProjectionVector) && f.calls[before+2]==uint32_t(LetterboxPass::Scissor));
    const auto& v=f.primitives[0];assert((v[0]==std::array<int16_t,3>{-304,240,0}) && v[2][1]==-32528 && v[3][1]==191 && v[4][1]==32527 && v[5][1]==-196 && v[7][1]==-240);
    assert(*f.backend.viewport()==viewport && *f.backend.projectionVector()==projection && *f.backend.scissor()==scissor);
    assert(f.packets.back().kind==NativeCameraCommand::Scissor && f.packets[f.packets.size()-2].transform==nativeCameraTransform(cameraGxProjectionMatrix(projection),f.camera.matrices.view));
    assert(f.program.formats[0]==3 && f.program.descriptors[1]==0 && f.program.alpha==NativeShadowProgram::Raster && !f.program.materialVertex);
    // Stream owns final retry admission: a changed vertex7 cannot submit the
    // captured prefix, nor can an already completed packet be emitted twice.
    CameraLetterboxVertexStream stream;stream.begin();LetterboxVertexSubmit unavailable;
    for(unsigned i=0;i<7;++i)assert(stream.vertex(i,{int16_t(i),0,0},unavailable));
    assert(!stream.vertex(7,{7,0,0},unavailable) && stream.count()==8);
    unsigned submitted=0;LetterboxVertexSubmit submit{&submitted,[](void* u,const CameraLetterboxVertices& v){++*static_cast<unsigned*>(u);assert(v[6][0]==6);return true;}};
    assert(!stream.vertex(7,{8,0,0},submit) && submitted==0);
    assert(stream.vertex(7,{7,0,0},submit) && submitted==1 && !stream.vertex(7,{7,0,0},submit));
}
