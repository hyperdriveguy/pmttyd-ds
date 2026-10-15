#include "camera_manager.hpp"
#include <cmath>
namespace ttyd {
// Original13-camera bank names/order are retained. Source NTSC projections
// use608x480 bounds,25deg FOV,near1/far32768 (engine units). Supplied
// render width/height set viewport/scissor, not those authored lens bounds.
void CameraManager::initializeBoot(uint16_t width,uint16_t height){
    if(!renderDimensionsPublished_){sourceRenderDimensions={uint8_t(width>>8),uint8_t(width),uint8_t(height>>8),uint8_t(height)};renderDimensionsPublished_=true;} // Native admission of original marioStInit render-descriptor fields.
    cameras_={}; // Original full record clears; current draw index is independent.
    constexpr const char* names[]{"off","off2","bg","shadow","3d","3deff_A","3dimg","3deff_B","2d","fade","fade2","dbg","dbg3d"};
    // .21816612780094147 radians=half25deg FOV. Aspect constants608/480
    // and4/3 are original binary32 values;400/240 divided by tangent positions
    // selected image/UI cameras, not native 3DS physical screen dimensions.
    const float angleTangent=float(std::tan(0.21816612780094147));
    for(uint32_t i=0;i<cameras_.size();++i){
        auto& c=cameras_[i];c.name=names[i];c.eye={0,0,1000};c.up={0,1,0};
        c.nearPlane=1;c.farPlane=32768;c.fov=25;c.aspect=1.2666666507720947f;
        c.distance=1000;c.drawCamera=8;c.viewport={0,0,float(width),float(height),0,1};
        c.scissor={0,0,width,height};
        if(i<=3 || i==11){
            c.projectionKind=1;
            c.matrices.projection=i==0?cameraOrthographic(240,-240,-304,304,1,32768):cameraOrthographic(0,480,0,608,1,32768);
        }else{
            // Lens overrides:effects5/7 and UI8 near100/far10000; depth slices
            //5/7=[.2,.3],8=[.1,.2],fade9/10=[0,.1]. Others retain[0,1].
            // Callback tokens:4 battle/field camera,5/7 effect,6 image providers.
            if(i==5 || i==7 || i==8){c.nearPlane=100;c.farPlane=10000;}
            if(i==6 || i==9 || i==10){c.aspect=1.3333333730697632f;c.eye[2]=400.f/angleTangent;}
            if(i==8)c.eye[2]=240.f/angleTangent;
            if(i==5 || i==7){c.viewport[4]=.2f;c.viewport[5]=.3f;}
            if(i==8){c.viewport[4]=.1f;c.viewport[5]=.2f;}
            if(i==9 || i==10)c.viewport[5]=.1f;
            c.callback=i==4?0x8000ea68u:i==5 || i==7?0x8000e974u:i==6?0x8000ea30u:0;
            c.matrices.projection=cameraPerspective(c.fov,c.aspect,c.nearPlane,c.farPlane);
        }
    }
}
// Main stages0 callback,1 LookAt+roll/translation,2 field camera4 zoom,
//3 projection-extra publication. Missing providers retain stage/partial stores;
// capture callback once so resumed work does not reread a mutated identity.
bool CameraManager::main(bool field,const CameraMainServices& s,CameraMainCursor& cursor){
    if(!cursor.active)cursor={0,0,true};
    while(cursor.index<cameras_.size()){
        auto& c=cameras_[cursor.index]; // Drawing-disabled cameras still update.
        if(cursor.stage==0){
            if(!cursor.callbackCaptured){cursor.callback=c.callback;cursor.callbackCaptured=true;}
            if(cursor.callback && (!s.callback || !s.callback(s.user,cursor.callback,cursor.index,c)))return false;
            cursor.stage=1;
        }
        if(cursor.stage==1){
            const auto view=s.lookAt?s.lookAt(s.user,c):std::nullopt;if(!view)return false;
            // Roll/offset precede the field-only zoom; extra must wait until after it.
            auto transform=c.publication;transform.flags=uint16_t(transform.flags&~8u);
            c.matrices=publishCameraMatrices({*view,c.matrices.projection},transform);c.viewPublished=true;
            cursor.stage=2;
        }
        if(cursor.stage==2){
            if(cursor.index==4 && field && (!s.fieldZoom || !s.fieldZoom(s.user,c)))return false;
            cursor.stage=3;
        }
        if(cursor.stage==3){
            publishCameraExtra(c.matrices.projection,c.publication);
            ++cursor.index;cursor.stage=0;cursor.callbackCaptured=false;
        }
    }
    cursor.active=false;return true;
}
// Draw stages0 letterbox,1 sort,2 camera gate/current index,3 viewport,
//4 scissor,5 captured near/far branch,6 recompute,7 projection,8 callbacks,
//9 dummy draw. Cameras dispatch ascending0..12. Flag1 skips drawing but not
// Main updates; flag0x40+perspective-kind0 admits lens refresh. Reset queue LAST.
bool CameraManager::draw(DisplayQueue& queue,const CameraDrawServices& s,CameraDrawCursor& cursor){
    if(!cursor.active)cursor={0,0,true,{}};
    if(cursor.stage==0){if(!s.letterbox || !s.letterbox(s.user,queue))return false;cursor.stage=1;}
    if(cursor.stage==1){queue.sort();cursor.stage=2;}
    while(cursor.index<cameras_.size()){
        auto& c=cameras_[cursor.index];
        if(cursor.stage==2){
            if(c.publication.flags&1){++cursor.index;continue;}
            current_=cursor.index;cursor.stage=3;
        }
        auto request=[&](CameraDrawRequestKind kind){return s.request && s.request(s.user,{kind,cursor.index,&c});};
        if(cursor.stage==3){if(!request(CameraDrawRequestKind::Viewport))return false;cursor.stage=4;}
        if(cursor.stage==4){if(!request(CameraDrawRequestKind::Scissor))return false;cursor.stage=5;}
        if(cursor.stage==5){
            // Capture this source branch before invoking a suspending provider.
            cursor.stage=(c.publication.flags&0x40) && c.projectionKind==0?6:7;
        }
        if(cursor.stage==6){
            if(!s.nearFar || !s.nearFar(s.user,c))return false;
            c.matrices.projection=cameraPerspective(c.fov,c.aspect,c.nearPlane,c.farPlane);cursor.stage=7;
        }
        if(cursor.stage==7){if(!request(CameraDrawRequestKind::Projection))return false;cursor.stage=8;}
        if(cursor.stage==8){if(!queue.dispatch(cursor.index,s.display,cursor.display))return false;cursor.stage=9;}
        if(cursor.stage==9){if(!request(CameraDrawRequestKind::DummyDraw))return false;++cursor.index;cursor.stage=2;}
    }
    queue.reinitialize();cursor.active=false;return true;
}
}
