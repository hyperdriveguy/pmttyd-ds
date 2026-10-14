#pragma once
#include "shadow_display.hpp"
namespace ttyd {
enum class LetterboxPass : uint32_t {ReadViewport=0x802ba5b4,ReadProjection=0x802ba2b4,ReadScissor=0x802ba6bc,
    Projection=0x802ba184,Viewport=0x802ba56c,ProjectionVector=0x802ba228,Scissor=0x802ba644};
struct LetterboxPassRequest {
    LetterboxPass kind=LetterboxPass::ReadViewport;std::array<float,6> viewport{};
    std::array<float,7> vector{};std::array<uint32_t,4> scissor{};CameraProjectionMatrix projection{};uint32_t projectionKind=0;
};
struct CameraLetterboxDrawServices {
    void* user=nullptr;
    std::optional<uint32_t> (*gp)(void*)=nullptr;
    const CameraState* (*camera)(void*,uint32_t)=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<int16_t> (*half)(void*,const CameraState&,uint32_t)=nullptr;
    bool (*pass)(void*,LetterboxPassRequest&)=nullptr;
    bool (*gx)(void*,const ShadowGXRequest&)=nullptr;
    bool (*begin)(void*)=nullptr;
    bool (*vertex)(void*,uint32_t,const std::array<int16_t,3>&)=nullptr;
};
struct CameraLetterboxDrawCursor {
    bool active=false,queued=false;uint32_t gp=0,phase=0,step=0,vertex=0,pending=0,color=0;
    const CameraState* camera=nullptr;std::array<float,6> viewport{};std::array<float,7> projection{};std::array<uint32_t,4> scissor{};
    LetterboxPassRequest pass;ShadowGXRequest gx;std::array<int16_t,3> point{};
};
using CameraLetterboxVertices=std::array<std::array<int16_t,3>,8>;
struct LetterboxVertexSubmit {void* user=nullptr;bool (*submit)(void*,const CameraLetterboxVertices&)=nullptr;};
class CameraLetterboxVertexStream {
public:
    void begin(){active_=true;count_=0;}
    bool vertex(uint32_t,const std::array<int16_t,3>&,const LetterboxVertexSubmit&);
    uint32_t count()const{return count_;}
    bool active()const{return active_;}
private:CameraLetterboxVertices vertices_{};uint32_t count_=0;bool active_=false;
};
bool drawCameraLetterbox(const CameraLetterboxDrawServices&,CameraLetterboxDrawCursor&);
}
