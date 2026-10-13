#include "npc_wander.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::vector<int32_t> values,bounds,frameCalls;
    unsigned next=0,readyCalls=0,soundCalls=0;
    bool ready=true;float distance=0,angle=90;
    std::vector<bool> readiness;unsigned readyIndex=0;
    uintptr_t selectedSound=0;uint32_t soundMask=0;FieldWallVector soundRoot;
    NpcWanderServices services() {
        return {this,[](void* p,int32_t ms){auto& f=*static_cast<Fixture*>(p);f.frameCalls.push_back(ms);return ms*60/1000;},
            [](void* p,int32_t upper){auto& f=*static_cast<Fixture*>(p);f.bounds.push_back(upper);assert(f.next<f.values.size());const auto value=f.values[f.next++];assert(value>=0 && value<upper);return value;},
            [](void* p,const NpcWanderActor&){auto& f=*static_cast<Fixture*>(p);++f.readyCalls;if(f.readiness.empty())return f.ready;assert(f.readyIndex<f.readiness.size());return bool(f.readiness[f.readyIndex++]);},
            [](void* p,FieldWallVector,FieldWallVector){return static_cast<Fixture*>(p)->angle;},
            [](void* p,FieldWallVector,FieldWallVector){return static_cast<Fixture*>(p)->distance;},
            [](void*,float r){return float(std::sin(double(r)));},[](void*,float r){return float(std::cos(double(r)));},
            [](void* p,uintptr_t sound,uint32_t mask,FieldWallVector root){auto& f=*static_cast<Fixture*>(p);++f.soundCalls;f.selectedSound=sound;f.soundMask=mask;f.soundRoot=root;}};
    }
};
bool close(float a,float b){return std::fabs(a-b)<.00001f;}
int main() {
    assert(npcWanderFrames(6000,60)==360 && npcWanderFrames(9000,50)==450);
    assert(npcWanderFrames(-1500,60)==-90 && npcWanderFrames(0x40000000,4)==0);
    uint32_t seed=1;assert(npcWanderRandom(seed,0)==0 && seed==1);
    assert(npcWanderRandom(seed,60)==38 && seed==0x41c67ea6);
    assert(npcWanderRandom(seed,1000)==758 && seed==0x967eb0e7);
    Fixture f;auto s=f.services();NpcWanderWork w;NpcWanderActor a;a.token=7;a.root={10,12,20};a.yaw=270;
    initializeNpcWander(w,a,100,200,70,30,0x10000,s);
    assert((f.frameCalls==std::vector<int32_t>{6000,9000,1500}));
    assert(w.phase==0 && w.count==0 && w.flips==0 && w.anchor.y==12 && w.anchor.x==100 && w.radius==70);
    assert(a.speed==30 && (a.flags&0x00400000));
    f.values={19,59,270,0,999,45};NpcWanderMario mario;
    assert(tickNpcWander(w,a,mario,s)==0 && w.phase==1 && w.count==180 && a.yaw==299 && a.moveHeading==299);
    assert((f.bounds==std::vector<int32_t>{181,60})); // 199 rounded down to180, not remainder19.
    w.phase=2;assert(tickNpcWander(w,a,mario,s)==0 && w.phase==3 && w.count==540 && w.flips==2);
    assert(a.yaw==269 && a.moveHeading==269);
    assert((f.bounds==std::vector<int32_t>{181,60,271,60,1000}));
    w.count=1;tickNpcWander(w,a,mario,s);assert(w.phase==3 && w.count==90 && w.flips==1);
    assert(a.yaw==89 && a.moveHeading==269); // Idle turn never changes movement heading.
    w.count=1;w.flips=0;f.values.push_back(0);tickNpcWander(w,a,mario,s);assert(w.phase==0 && w.count==45);
    w={};w.phase=1;w.count=2;w.radius=70;a.dt=1.f/60;a.moveHeading=90;a.components={7,8,9};
    f.ready=false;f.readyCalls=0;tickNpcWander(w,a,mario,s);assert(w.count==1 && a.components.x==7 && a.components.z==9 && f.readyCalls==4);
    f.readyCalls=0;tickNpcWander(w,a,mario,s);assert(w.phase==2 && a.components.x==7 && f.readyCalls==3); // Count expires before final readiness.
    w.phase=1;w.count=0;tickNpcWander(w,a,mario,s);assert(w.count==0xffffffff && w.phase==1);
    w.count=20;f.ready=true;f.distance=71;w.flags=0;a.dt=.1f;
    tickNpcWander(w,a,mario,s);assert(w.flags==1 && a.yaw==90 && close(a.components.x,.6f)); // MIN cap.
    a.dt=.01f;tickNpcWander(w,a,mario,s);assert(close(a.components.x,.3f));
    a.dt=.02f;tickNpcWander(w,a,mario,s);assert(close(a.components.x,.6f)); // Equality retains the cap.
    f.distance=70;tickNpcWander(w,a,mario,s);assert(!(w.flags&3)); // Inclusive radius-return admission.
    f.distance=69;mario.contact=7;mario.travelHeading=90;a.moveHeading=90;w.flags=3;
    tickNpcWander(w,a,mario,s);assert(a.yaw==270 && a.moveHeading==270 && !(w.flags&3));
    a.moveHeading=180; // Exactly90 heading difference rejects the contact reversal.
    tickNpcWander(w,a,mario,s);assert(a.moveHeading==180);
    a.wall=1;f.angle=45;tickNpcWander(w,a,mario,s);assert(a.moveHeading==45 && a.yaw==45);a.wall=0;
    a.sounds[0]=11;a.sounds[1]=22;a.stepPeriod=12000;a.stepElapsed=400;a.stepToggle=0;
    a.dt=.016f;a.soundFlags=0x100;a.materialFlags=0;
    tickNpcWander(w,a,mario,s);assert(f.soundCalls==0 && a.stepElapsed==416);
    tickNpcWander(w,a,mario,s);assert(f.soundCalls==1 && f.selectedSound==22 && f.soundMask==0x10000 && a.stepToggle==1 && a.stepElapsed==16);
    assert(f.soundRoot.y==12);
    a.stepElapsed=401;a.materialFlags=0x1800;
    tickNpcWander(w,a,mario,s);assert(f.soundCalls==2 && f.selectedSound==11 && f.soundMask==0 && a.stepToggle==0);
    a.sounds[1]=0;a.stepElapsed=401;a.stepToggle=0;
    tickNpcWander(w,a,mario,s);assert(f.soundCalls==2 && a.stepToggle==1 && a.stepElapsed==16); // Missing sound still resets/toggles.
    a.sounds[0]=0;a.stepElapsed=401;
    tickNpcWander(w,a,mario,s);assert(a.stepToggle==0 && a.stepElapsed==0);
    for(int16_t toggle:{int16_t(-1),int16_t(127),int16_t(128)}) {
        a.stepToggle=toggle;a.stepElapsed=401;tickNpcWander(w,a,mario,s);
        assert(a.stepToggle==(toggle==-1?2:toggle==127?-126:-127));
    }
    Fixture ordered;ordered.readiness={false,true,false,true};ordered.distance=100;
    auto orderedServices=ordered.services();NpcWanderWork orderedWork;orderedWork.phase=1;orderedWork.count=5;orderedWork.radius=70;
    NpcWanderActor orderedActor;orderedActor.speed=30;orderedActor.dt=.02f;orderedActor.wall=1;
    tickNpcWander(orderedWork,orderedActor,mario,orderedServices);
    assert(ordered.readyIndex==4 && orderedWork.flags==0 && close(orderedActor.components.z,-.6f));
    assert(npcWanderTurnReady(0,0,100) && !npcWanderTurnReady(8,0,10) && npcWanderTurnReady(8,0,9.9f));
    assert(!npcWanderTurnReady(8,359,1)); // Absolute difference, not angle wrap.
    assert(!npcWanderTurnReady(8,0,std::numeric_limits<float>::quiet_NaN()));
}
