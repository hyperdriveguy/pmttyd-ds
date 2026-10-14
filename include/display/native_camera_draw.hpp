#pragma once
#include "camera_manager.hpp"
namespace ttyd {
struct NativeCameraRect {uint32_t x=0,y=0,width=0,height=0;};
enum class NativeCameraCommand {Viewport,Scissor,Projection,DummyDraw,DummyStateRestore};
struct NativeCameraPacket {
    NativeCameraCommand kind;NativeCameraRect rect;
    float depthScale=-1,depthOffset=0;
    CameraProjectionMatrix transform{};
};
struct NativeCameraSubmit {void* user=nullptr;bool (*submit)(void*,const NativeCameraPacket&)=nullptr;};
// Exact integer-endpoint domain of the declared608x480 -> tilted240x400 adapter.
std::optional<NativeCameraRect> nativeCameraRect(float x,float y,float w,float h);
// Declared raster adapter for shadow setup: nearest physical endpoints.
std::optional<NativeCameraRect> nativeCameraRoundedRect(float x,float y,float w,float h);
CameraProjectionMatrix nativeCameraTransform(const CameraProjectionMatrix&,const CameraViewMatrix&);
class NativeCameraDraw {
public:
    bool request(const CameraDrawRequest&,const NativeCameraSubmit&);
    bool viewport(std::array<float,6>,const NativeCameraSubmit&,bool rounded=false);
    bool scissor(std::array<uint32_t,4>,const NativeCameraSubmit&,bool rounded=false);
    bool projection(const CameraProjectionMatrix&,const NativeCameraSubmit&);
    const std::optional<std::array<float,6>>& viewport()const{return logicalViewport_;}
    const std::optional<std::array<uint32_t,4>>& scissor()const{return logicalScissor_;}
    // Reapply backend state only; never resubmit the source dummy vertices.
    bool restore(const NativeCameraSubmit&) const;
private:
    std::optional<NativeCameraPacket> viewport_,scissor_;
    std::optional<std::array<float,6>> logicalViewport_;
    std::optional<std::array<uint32_t,4>> logicalScissor_;
    std::optional<CameraProjectionMatrix> projection_;
    CameraViewMatrix view_{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
    bool dummyState_=false;
};
}
