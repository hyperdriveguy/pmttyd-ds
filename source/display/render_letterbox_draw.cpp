#include "platform.hpp"
namespace ttyd {
// Read/write the acknowledged logical camera domain; native temporary passes
// must preserve the source viewport/projection/scissor across provider holds.
bool Renderer::finishingLetterboxPass(LetterboxPassRequest& r){
    NativeCameraSubmit submit{this,[](void* u,const NativeCameraPacket& p){return static_cast<Renderer*>(u)->finishingCameraPacket(p);}};
    switch(r.kind){
    case LetterboxPass::ReadViewport:if(!finishingCameraDraw.viewport())return false;r.viewport=*finishingCameraDraw.viewport();return true;
    case LetterboxPass::ReadProjection:{const auto v=finishingCameraDraw.projectionVector();if(!v)return false;r.vector=*v;return true;}
    case LetterboxPass::ReadScissor:if(!finishingCameraDraw.scissor())return false;r.scissor=*finishingCameraDraw.scissor();return true;
    case LetterboxPass::Projection:return finishingCameraDraw.projection(r.projection,submit,r.projectionKind);
    case LetterboxPass::ProjectionVector:return finishingCameraDraw.projectionVector(r.vector,submit);
    case LetterboxPass::Viewport:return finishingCameraDraw.viewport(r.viewport,submit,true);
    case LetterboxPass::Scissor:return finishingCameraDraw.scissor(r.scissor,submit,true);
    }
    return false;
}
namespace {
// Unlit material-color-only admission: one channel/stage, no texgens,
// position DIRECT(format 3=S16), no color/UV descriptors. Do not acknowledge
// an unrelated physical pipeline just because its output would be black.
bool letterboxProgram(const NativeShadowProgram& p,const NativeDisplayPipeline* state){
    return state && state->rasterAdmitted() && p.seeded && p.chans==1 && !p.texGens && p.tevStages==1 &&
       p.descriptors==std::array<uint32_t,3>{1,0,0} && p.formats[0]==3 && p.material && !p.materialVertex && !p.alphaMaterialVertex &&
       p.channelControl[0] && !(*p.channelControl[0])[1] && p.rasterRGB && !p.textureRGB && !p.audienceColor && p.alpha==NativeShadowProgram::Raster;
}
}
bool Renderer::finishingLetterboxBegin(){
    if(!letterboxProgram(finishingShadowProgram,finishingDisplayState.state()))return false;
    finishingLetterboxVertices.begin();return true; // CPU staging; no physical GPU Begin is left open.
}
// Stage eight signed16 vertices on CPU before emitting two four-vertex fans.
// A hold between source vertices must never leave a GPU primitive open.
bool Renderer::finishingLetterboxVertex(uint32_t index,const std::array<int16_t,3>& point){
    LetterboxVertexSubmit submit{this,[](void* u,const CameraLetterboxVertices& vertices){
        auto& r=*static_cast<Renderer*>(u);const auto& p=r.finishingShadowProgram;
        if(!letterboxProgram(p,r.finishingDisplayState.state()) || !r.finishingShadowGpu(p))return false;
        for(unsigned quad=0;quad<2;++quad){
            C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
            for(unsigned i=0;i<4;++i){const auto& v=vertices[4*quad+i];
                C3D_ImmSendAttrib(v[0],v[1],v[2],1);C3D_ImmSendAttrib(0,0,0,1);C3D_ImmSendAttrib(0,0,0,0);
            }
            C3D_ImmDrawEnd();
        }
        return true;
    }};
    return finishingLetterboxVertices.vertex(index,point,submit);
}
}
