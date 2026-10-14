#pragma once
#include "camera_depth.hpp"
#include "display_queue.hpp"
namespace ttyd {
// Semantic retained record, not packed original memory. projectionKind0/1
// means perspective/orthographic; viewport=(x,y,width,height,minZ,maxZ),
// scissor=(x,y,width,height). Source draw-camera index defaults8 independently
// of this record's slot. Letterbox fields are signed16 source scalars.
struct CameraState {
    CameraMatrixPublication publication;
    uint16_t mode=0;
    CameraDepthPoint eye{},at{},up{};
    float nearPlane=0,farPlane=0,fov=0,aspect=0,distance=0,yaw=0;
    CameraDepthSnapshot matrices;
    bool viewPublished=false; // Native availability metadata; boot matrices contain no view.
    uint32_t projectionKind=0,callback=0,drawCamera=0;
    const char* name=nullptr;
    int16_t letterboxAlpha=0,letterboxTop=0,letterboxBottom=0;
    std::array<float,6> viewport{};
    std::array<uint16_t,4> scissor{};
};
// Cursors survive unavailable providers; starting a fresh cursor replays a
// whole pass. Keep Main and Draw continuations separate from scene state.
struct CameraMainCursor {uint8_t index=0,stage=0;bool active=false,callbackCaptured=false;uint32_t callback=0;};
struct CameraDrawCursor {uint8_t index=0,stage=0;bool active=false;DisplayDispatchCursor display;};
struct CameraMainServices {
    void* user=nullptr;
    bool (*callback)(void*,uint32_t identity,uint32_t index,CameraState&)=nullptr;
    std::optional<CameraViewMatrix> (*lookAt)(void*,const CameraState&)=nullptr;
    bool (*fieldZoom)(void*,CameraState&)=nullptr;
};
enum class CameraDrawRequestKind {Viewport,Scissor,Projection,DummyDraw};
struct CameraDrawRequest {CameraDrawRequestKind kind;uint32_t index;const CameraState* camera;};
struct CameraDrawServices {
    void* user=nullptr;
    bool (*letterbox)(void*,DisplayQueue&)=nullptr;
    bool (*nearFar)(void*,CameraState&)=nullptr;
    bool (*request)(void*,const CameraDrawRequest&)=nullptr;
    DisplayDispatchServices display;
};
// Retained source bank; callback and GPU availability are explicit providers.
class CameraManager {
public:
    uint32_t letterboxColor=0; // Original startup-cleared camdrv sbss2 word80429510, live across room/battle ownership.

    // NTSC dimensions are explicit caller inputs. Native double trig substitutes
    // source SDK/libm numerics; boot must run once, independently of battle entry.
    void initializeBoot(uint16_t renderWidth,uint16_t renderHeight);
    CameraState* camera(uint32_t index){return index<cameras_.size()?&cameras_[index]:nullptr;}
    const CameraState* camera(uint32_t index)const{return index<cameras_.size()?&cameras_[index]:nullptr;}
    std::optional<uint32_t> currentIndex()const{return current_;}
    const CameraState* current()const{return current_?camera(*current_):nullptr;}
    bool main(bool fieldContext,const CameraMainServices&,CameraMainCursor&);
    bool draw(DisplayQueue&,const CameraDrawServices&,CameraDrawCursor&);
private:
    std::array<CameraState,13> cameras_{};
    std::optional<uint32_t> current_;
};
}
