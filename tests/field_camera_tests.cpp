#include "field_camera_state.hpp"
#include "field_camera.hpp"
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
void bottomlessCases() {
    auto frame=adaptedFieldCameraFrame(0,0,0);
    const auto height=bottomlessProbeHeight({0,0,0},frame);CHECK(height);
    auto outside=projectCameraRoot({0,*height,0},frame.view,frame.projection,frame.viewport);
    auto inside=projectCameraRoot({0,*height-.1f,0},frame.view,frame.projection,frame.viewport);
    CHECK(!outside.inScreen() && inside.inScreen());
    const auto side=bottomlessProbeHeight({100,0,0},frame);CHECK(side && *side==1); // Initial outside needs no increments.
    CHECK(!bottomlessProbeHeight({0,-20.01f,0},frame));
    FieldCameraFrame invalid;CHECK(!bottomlessProbeHeight({0,0,0},invalid));
    WaterRecovery recovery;recovery.beginVoid({0,2,0},{.03f,0,0});
    CHECK(recovery.counter==0 && recovery.position[0]==0);int hp=7;recovery.tick(hp);
    CHECK(recovery.position[0]==.08f && recovery.state==20 && hp==7);
    CHECK(recovery.fallVelocity==.34f && recovery.fallAcceleration==-.002f && recovery.fallJerk==-.01f && recovery.fallJerkRate==0.f); // Recovered mot_bottomless drop-back lanes, DOL 0x80091bd4.
    recovery.beginVoid({0,2,0},{0,0,0});recovery.tick(hp);
    CHECK(recovery.position[2]==-.08f && recovery.state==20);
    clearCollision();Game game;game.restore(game.progress);
    game.water.beginVoid({0,2,0},{.8f,0,0});game.fieldCameraDisplayFlags=0x10000;game.marioCamera.retained={123,0,456};
    game.tick({});CHECK(close(game.fieldCamera.point.x,123.01f));
    CHECK(close(game.marioCamera.retained.x,8) && close(game.marioCamera.retained.z,0));
    game.tick({});CHECK(close(game.fieldCamera.point.x,8.01f));
    CHECK(game.fieldCameraShift.previousMilliseconds==33);
    game.fieldCameraDisplayFlags=0;game.tick({});
    CHECK(close(game.marioCamera.retained.y,game.water.position[1]*100)); // An unlocked recovery permits the ordinary writer.
    CHECK(game.marioCamera.retained.y!=0);
    Game wet;wet.restore(wet.progress);wet.fieldCameraDisplayFlags=0x10000;
    wet.water.begin({0,.4f,0},{0,.1f,0});wet.marioCamera.retained={100,70,200};
    wet.water.state=11;wet.water.position[1]=5.4f;wet.water.velocity=.1f;
    wet.tick({});CHECK(wet.water.state==14 && wet.water.counter==10);
    CHECK(close(wet.water.followDelta,-.03f) && wet.marioCamera.retained.y==70);
    wet.tick({});CHECK(close(wet.marioCamera.retained.y,67) && wet.marioCamera.retained.x==0 && wet.marioCamera.retained.z==0);
    wet.water.state=32;wet.water.counter=1;wet.tick({});
    CHECK(!wet.water.active() && !(wet.fieldCameraDisplayFlags&0x10000));
}
void eventCases() {
    const auto near=[](float a,float b){return std::fabs(a-b)<.001f;};
    CHECK(cameraInterpolation(99,123,0,4,9)==9);
    CHECK(!cameraInterpolation(-1,1,10,0,1));CHECK(!cameraInterpolation(99,1,10,0,1));
    CHECK(cameraInterpolation(0,15,10,0,1)==1.5f); // No source step/output clamp.
    CHECK(cameraInterpolation(1,5,10,0,1)==.25f);
    CHECK(cameraInterpolation(2,5,10,0,1)==.125f);
    CHECK(cameraInterpolation(3,5,10,0,1)==.0625f);
    CHECK(cameraInterpolation(4,5,10,0,1)==.75f);
    CHECK(cameraInterpolation(5,5,10,0,1)==.875f);
    CHECK(cameraInterpolation(6,5,10,0,1)==.9375f);
    CHECK(near(*cameraInterpolation(7,5,10,0,1),.75f));
    CHECK(*cameraInterpolation(7,1,4,0,1)>1.5f); // Original overshoot, not clamped ease.
    CHECK(*cameraInterpolation(8,2,10,0,1)>1);
    CHECK(cameraInterpolation(9,0,10,0,1)==1);
    CHECK(*cameraInterpolation(9,2,10,0,1)>1);
    CHECK(near(*cameraInterpolation(10,5,10,0,-2),-2));
    CHECK(cameraEventCurve(0,.0000099f)==0); // fctiwz scale truncates, not rounds.

    CHECK(near(*cameraInterpolation(10,0,10,0,-2),0));
    CHECK(near(*cameraInterpolation(11,5,10,0,1),.5f));
    CHECK(near(*cameraInterpolation(12,5,10,0,1),.70710678f));
    CHECK(*cameraInterpolation(13,10,10,0,1)<1); // Approximate source HalfPi endpoint.
    for(int mode=0;mode<=13;++mode)CHECK(std::isfinite(*cameraInterpolation(mode,3,10,2,5)));
    CameraBlendTimer timer;timer.state=1;timer.start=100;timer.duration=100;
    CHECK(timer.update(200) && timer.state==1 && timer.progress==0);
    CHECK(timer.update(201) && timer.state==0 && timer.progress==0);
    timer.state=3;CHECK(timer.update(200) && timer.state==3 && timer.progress==1);
    CHECK(timer.update(201) && timer.state==0 && timer.progress==1);
    timer.state=2;timer.duration=0;CHECK(timer.update(201) && timer.state==0 && timer.progress==1);
    timer.state=0;timer.progress=.7f;CHECK(timer.update(202) && timer.progress==.7f);
    CHECK(!timer.update(99) && timer.progress==.7f);
    FieldCameraEvents events;CHECK(events.prepare({100,20,300},{100,5,300},0));
    CHECK(events.follow.y==20 && events.look.y==5 && events.dolly==0);
    CameraPoint eye,at;CHECK(events.finish({0,100,500},{0,0,0},eye,at));CHECK(eye.z==500 && at.z==0);
    CHECK(events.setTarget({400,50,600},50,100,11,0));
    CHECK(events.prepare({900,900,900},{800,800,800},50));
    CHECK(near(events.follow.x,250) && events.look.x==events.follow.x && near(events.dolly,25));
    CHECK(events.finish({0,100,500},{0,0,0},eye,at));CHECK(near(eye.z,375) && near(eye.y,75));
    CHECK(events.finish({0,100,500},{0,0,0},eye,at,false));CHECK(eye.z==500); // No-road branch bypasses dolly.
    CHECK(events.prepare({900,900,900},{800,800,800},100));CHECK(events.target.state==3 && events.follow.x==400);
    CHECK(events.prepare({900,900,900},{800,800,800},101));CHECK(events.target.state==0 && events.follow.x==400);
    CHECK(events.targetOff(100,11,101));CHECK(events.prepare({900,20,900},{800,5,800},201));
    CHECK(events.target.state==1 && events.follow.x==900 && events.look.x==800 && events.dolly==0);
    events.dolly=150;CHECK(events.finish({0,0,500},{0,0,0},eye,at));CHECK(eye.z==-250); // No source percent clamp.
    CHECK(events.setView({400,100,600},{400,0,0},{0,100,500},{0,0,0},100,11,201));
    CHECK(events.prepare({}, {},251));CHECK(events.finish({999,999,999},{999,999,999},eye,at));
    CHECK(near(eye.x,200) && near(eye.z,550));
    CHECK(events.prepare({}, {},302));CHECK(events.view.state==0 && events.view.progress==1);
    CHECK(events.finish({999,999,999},{999,999,999},eye,at));CHECK(eye.x==400 && eye.z==600); // Updated state switches branch but full stored view retained.
    events.target.state=3;events.target.start=77;events.target.duration=123;events.target.progress=.5f;
    events.anchor={1,2,3};events.previousAnchor={4,5,6};events.endPercent=60;events.previousPercent=30;
    events.roomLoad();CHECK(events.target.state==3 && events.target.start==77 && events.target.duration==123 && events.target.progress==0);
    CHECK(events.anchor.y==2 && events.previousAnchor.y==5 && events.endPercent==60 && events.previousPercent==30);
    CHECK(events.view.state==0 && events.view.progress==0 && events.storedEye.x==400);
    const auto retained=events;CHECK(!events.setTarget({},0,-1,11,0));CHECK(!events.setTarget({},0,100,99,0));CHECK(events.anchor.x==retained.anchor.x && events.target.start==77);
    clearCollision();Game game;game.restore(game.progress);game.tick({});
    game.fieldCameraFlags|=2;const auto stamp=game.fieldCameraShift.previousMilliseconds;
    const auto follow=game.fieldCamera.point;game.marioCamera.retained={999,999,999};game.tick({});
    CHECK(game.fieldCameraShift.previousMilliseconds==stamp && game.fieldCamera.point.x==follow.x);
    game.fieldCameraEvents=retained;game.restore(game.progress);
    CHECK(game.fieldCameraEvents.target.state==3 && game.fieldCameraEvents.target.start==77 && game.fieldCameraEvents.previousAnchor.y==5);
}
int main() {
    shiftCases();bottomlessCases();eventCases();
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
