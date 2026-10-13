#include "npc_presentation.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
bool near(float a,float b){return std::fabs(a-b)<.0001f;}
struct Camera {
    NpcCamera camera{{0,0,100},{0,0,0}};unsigned calls=0;
    NpcCameraService service(){return {this,[](void* p){auto& c=*static_cast<Camera*>(p);++c.calls;return c.camera;}};}
};
int main() {
    NpcAngleTable table;assert(table.load("romfs/npc-angle.bin"));
    assert(!table.load("/nonexistent/npc-angle.bin"));
    assert(table.angle(0,0,0,0)==0 && table.angle(0,0,0,-1)==0 && table.angle(0,0,1,0)==90);
    assert(table.angle(0,0,0,1)==180 && table.angle(0,0,-1,0)==270);
    for(int x:{-10,-3,-1,0,1,3,10})for(int z:{-10,-3,-1,0,1,3,10})if(x || z) {
        const float a=table.angle(0,0,float(x),float(z));
        const float expected=npcReviseAngle(float(std::atan2(float(x),float(-z)))*57.29577951308232f);
        assert(std::fabs(a-expected)<.2f); // Approximation bound, not original SDK numeric parity.
        assert(near(table.angle(10,20,float(10+x),float(20+z)),a));
    }
    assert(npcReviseAngle(-90)==270 && npcReviseAngle(720)==0 && npcReviseAngle(std::numeric_limits<float>::quiet_NaN())==0);
    Camera camera;NpcMatrixInput input;input.flags=0x02000000;input.yaw=270;
    NpcPresentation left;assert(left.prepare(input,table,camera.service()) && camera.calls==3);
    assert(left.incoming==0 && left.retained==0 && left.target==0 && left.sign==1);
    const ActorMeshVertex v{10,20,5,0,0};auto p=left.point(v);
    assert(near(p[0],.2f) && near(p[1],.4f) && near(p[2],.1f));
    NpcPresentation right;input.yaw=90;assert(right.prepare(input,table,camera.service()));
    assert(right.incoming==180 && right.target==180 && near(right.retained,18) && right.sign==1);
    for(unsigned i=0;i<200;++i)assert(right.prepare(input,table,camera.service()));
    assert(near(right.retained,180) && right.sign==-1);
    p=right.point(v);assert(near(p[0],-.2f) && near(p[1],.4f) && near(p[2],.1f));
    NpcPresentation snapped;snapped.flags|=0x100;assert(snapped.prepare(input,table,camera.service()) && snapped.retained==180);
    const float previous=snapped.sign;assert(previous==1); // Main sign precedes the first snap.
    assert(snapped.prepare(input,table,camera.service()) && snapped.sign==-1);
    input.flags=0;input.yaw=270;input.root={50,0,0};camera.calls=0;
    NpcPresentation corrected;assert(corrected.prepare(input,table,camera.service()) && camera.calls==4);
    p=corrected.point({0,0,0,0,0});assert(near(p[0],.5f) && near(p[1],0) && near(p[2],0));
    input.pivot={3,5,7};input.scale={2,3,4};input.rotation={30,120,-10};
    assert(corrected.prepare(input,table,camera.service()));
    for(auto f:corrected.point(v))assert(std::isfinite(f));
    const auto anchor=NpcPresentation::talkAnchor({150,10,-400},30);
    assert(near(anchor.x,1.5f) && near(anchor.y,.4f) && near(anchor.z,-4));
    input.yaw=std::numeric_limits<float>::infinity();assert(!corrected.prepare(input,table,camera.service()));
    NpcPoseClock clock;MessagePlayback message;
    assert(clock.update("Stay","Talk",nullptr,100)==0 && clock.pose=="Stay");
    message.serial=1;message.poseRequestSerial=1;message.poseRequestedAt=120;
    message.poseRequest=MessagePlayback::PoseRequest::Talk;
    assert(near(clock.update("Stay","Talk",&message,137),1.02f) && clock.pose=="Talk");
    assert(near(clock.update("Stay","Talk",&message,154),2.04f) && clock.pose=="Talk" && clock.poseAt==120);
    ++message.poseRequestSerial;message.poseRequestedAt=160;
    assert(clock.update("Stay","Talk",&message,160)==0); // Same-name restart.
    ++message.poseRequestSerial;message.poseRequestedAt=180;message.poseRequest=MessagePlayback::PoseRequest::Stay;
    assert(clock.update("Stay","Talk",&message,180)==0 && clock.pose=="Stay");
    assert(clock.update("Run","Talk",&message,190)==0 && clock.pose=="Run");
    assert(clock.update("Stay","Talk",nullptr,0)==0 && clock.pose=="Stay" && !clock.requestSerial);
}
