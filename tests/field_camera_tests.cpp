#include "field_camera_state.hpp"
#include "game.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace ttyd;
#define CHECK(x) do {if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(0)
bool close(float a,float b){return std::fabs(a-b)<.00001f;}
void shiftCases() {
    CHECK(fieldCameraTickMilliseconds(1)==16 && fieldCameraTickMilliseconds(2)==33 && fieldCameraTickMilliseconds(60)==1000);
    FieldCameraShift work;work.previousMilliseconds=123;work.reset();CHECK(work.previousMilliseconds==123);
    CHECK(work.update(1,{1,0,0},0,100,140));CHECK(close(work.accumulation,25.5f) && work.current==0);work.post(140);
    CHECK(work.update(1,{2,0,0},0,100,156));CHECK(close(work.accumulation,49.5f));work.post(156);
    work.accumulation=299;
    CHECK(work.update(1,{3,0,0},0,100,173));
    CHECK(work.accumulation==300 && work.desired==100 && close(work.blend,.00204f) && close(work.current,.20808f));work.post(173);
    const float held=work.current;
    CHECK(work.update(0,{4,0,0},0,100,190));CHECK(work.current==held && work.blend==0 && work.enabled==1);work.post(190);
    CHECK(work.update(1,{4,99,0},0,100,206));CHECK(work.current==held && work.blend==0);work.post(206); // Root Y does not count as XZ movement.
    CHECK(work.update(1,{999,0,999},0x20,100,222));CHECK(work.current==0 && work.desired==0 && work.previousRootX==4);
    FieldCameraShift over;over.current=1;over.previousMilliseconds=0;
    CHECK(over.update(0,{},0x20,100,17));CHECK(close(over.current,-.02f)); // Literal target-sign clamp retains this overshoot.
    FieldCameraShift reverse;reverse.accumulation=300;reverse.desired=100;reverse.previousMilliseconds=0;
    CHECK(reverse.update(-1,{1,0,0},0,100,16));CHECK(close(reverse.accumulation,-24) && reverse.desired==100);
    reverse.accumulation=-299;reverse.blend=.9f;reverse.post(16);
    CHECK(reverse.update(-1,{2,0,0},0,100,32));CHECK(reverse.desired==-100 && reverse.accumulation==-300 && close(reverse.blend,.00192f));
    FieldCameraShift cap;cap.blend=1;cap.enabled=1;cap.desired=1000;cap.accumulation=300;
    CHECK(cap.update(1,{1,0,0},0,1000,16));CHECK(close(cap.current,2.88f)); // Cap3 precedes clock increment .96.
    const auto unchanged=cap;cap.post(16);CHECK(!cap.update(1,{2,0,0},0,1000,15));CHECK(cap.current==unchanged.current && cap.previousRootX==unchanged.previousRootX);
    cap.post(99);cap.reset();CHECK(cap.previousMilliseconds==99 && cap.current==0 && cap.limit==300 && cap.incrementCap==3);
}
int main() {
    shiftCases();
    CameraShift up{0,10,.06f};up.tick();CHECK(close(up.current,.66f));
    CameraShift down{10,0,.03f};down.tick();CHECK(close(down.current,9.67f));
    CameraShift cross{9.99f,10,.06f};cross.tick();CHECK(cross.current==10);
    cross={10.01f,10,.06f};cross.tick();CHECK(cross.current==10);
    cross={10,10,.5f};cross.tick();CHECK(cross.current==10);
    MarioCameraTarget mario;mario.retained={100,20,300};
    FieldCameraTarget camera;camera.point={0,10,0};camera.consume(mario);
    CHECK(close(camera.point.x,100.01f) && close(camera.point.y,12.2f) && camera.point.z==300);
    camera.consume(mario,0x10);CHECK(close(camera.point.y,12.2f));
    // Camera consumes retained state, and only later Mario publishes a new root.
    CHECK(mario.publish({200,30,400},90));CHECK(camera.point.z==300);
    camera.consume(mario);CHECK(camera.point.z==400 && close(camera.point.y,15.96f));
    mario.side={2,2,.03f};mario.vertical={3,3,.06f};mario.depth={4,4,.06f};mario.drawOffset=5;
    CHECK(mario.publish({10,20,30},90));CHECK(mario.retained.x==10 && mario.retained.y==28 && mario.retained.z==28 && mario.depthValue==4);
    mario.side.target=10;const auto saved=mario.retained;
    CHECK(!mario.publish({999,999,999},0,0x10000));CHECK(mario.side.current>2 && mario.retained.x==saved.x && mario.retained.y==saved.y);
    const float shift=mario.side.current;CHECK(!mario.publish({},0,0,29));CHECK(mario.side.current==shift);
    mario.side={2,2,.03f};CHECK(mario.publish({10,999,30},90,0x20000));
    CHECK(mario.retained.x==10 && mario.retained.y==saved.y && mario.retained.z==24 && mario.depthValue==4);
    CHECK(close(cameraHeadingSnapshot(-.0006f),359.999f));CHECK(close(cameraHeadingSnapshot(360.0004f),0));
    CameraView identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
    const CameraProjection projection{2,.25f,3,-.125f,-.1f,-.2f};
    auto cache=projectCameraRoot({1.25f,-.75f,-2},identity,projection,{10,20,100,80,.2f,.9f});
    CHECK(cache.valid && cache.x==110 && cache.y==100 && cache.z==0);
    // Negative W is computed; truncation, not rounding/clipping, owns the cache.
    cache=projectCameraRoot({0,0,2},identity,{1,0,1,0,0,0});CHECK(cache.valid && cache.x==304 && cache.y==240);
    cache=projectCameraRoot({-1.001f,0,-1},identity,{1,0,1,0,0,0});CHECK(cache.x==0 && cache.inScreen());
    CHECK((CameraScreenCache{600,535,-999,true}.inScreen()));
    CHECK(!(CameraScreenCache{601,535,0,true}.inScreen()));CHECK(!(CameraScreenCache{600,536,0,true}.inScreen()));
    CHECK(!(CameraScreenCache{-1,0,0,true}.inScreen()));
    CHECK(!projectCameraRoot({0,0,0},identity,{1,0,1,0,0,0}).valid);
    clearCollision();Game g;g.restore(g.progress);const auto originalX=g.cameraX;
    Input right;right.x=1;g.tick(right);const float movedX=g.progress.position.x;
    CHECK(movedX>originalX && close(g.cameraX,originalX+.0001f));
    CHECK(close(g.marioCamera.retained.x,movedX*100));g.tick({});CHECK(close(g.cameraX,movedX+.0001f));
    CHECK(g.fieldScreen.valid && g.fieldScreen.inScreen());
    Game shifted;shifted.restore(shifted.progress);shifted.fieldCameraFlags|=4;
    shifted.fieldCameraCandidateActive=true;shifted.fieldCameraCandidateShiftRate=100;
    shifted.fieldCameraShift.accumulation=299;shifted.tick(right);
    CHECK(shifted.fieldCameraShift.current==0 && shifted.fieldCameraShift.accumulation==299); // Prior Mario input was neutral.
    shifted.tick(right);CHECK(close(shifted.fieldCameraShift.current,.20808f));
    CHECK(shifted.fieldCameraShift.previousMilliseconds==33); // Road/view post owns this timestamp.
    shifted.restore(shifted.progress);
    CHECK(shifted.fieldCameraShift.previousMilliseconds==33 && shifted.fieldCameraShift.current==0 && shifted.fieldCameraShift.accumulation==0);
    shifted.tick({});CHECK(shifted.fieldCameraShift.previousMilliseconds==50); // Global clock survives the room reset.
    // Explicit supplied camera frame can replace the provisional view adapter.
    g.fieldCameraViewAdapter=false;g.fieldCameraFrame.view=identity;
    g.fieldCameraFrame.projection={1,0,1,0,0,0};g.fieldCameraFrame.yaw=90;
    g.fieldCameraFrame.viewport={};g.tick(right);
    CHECK(close(g.fieldTravel().headingOffsetDegrees,90));CHECK(g.fieldTravel().headingZ>.99f);
    Game falling;falling.restore(falling.progress);Input launch;launch.x=.5f;launch.confirm=true;falling.tick(launch);
    for(int n=0;n<100 && falling.fieldAirMotion().motion==3;++n)falling.tick({});
    CHECK(falling.airborne() && falling.fieldAirMotion().pendingEntry);
    falling.fieldCameraViewAdapter=false;
    falling.fieldCameraFrame.view={{{0,0,0,0},{0,1,0,0},{0,0,0,-1}}};
    falling.fieldCameraFrame.projection={1,0,1,0,0,0};
    bool changed=false;
    for(int n=0;n<3;++n) {
        const float beforeY=falling.groundHeight+falling.jumpHeight;falling.tick({});
        CHECK(falling.fieldScreen.y==int32_t(240-beforeY*240));
        if(beforeY!=falling.groundHeight+falling.jumpHeight)changed=true;
    }
    CHECK(changed); // Cached move root precedes this tick's falling Y integration.
    std::cout<<"Camera target, projection and native lifecycle contracts passed\n";
}
