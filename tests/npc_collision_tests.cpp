#include "npc_collision.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
uint32_t bits(float v){uint32_t b;std::memcpy(&b,&v,4);return b;}
bool close(float a,float b){return std::fabs(a-b)<.0001f;}
struct Wall {
    unsigned calls=0;NpcSidePlayer* mario=nullptr;
    FieldWallVector before;NpcSideWallRequest request;bool mutate=false;
    NpcSideServices service(){return {this,[](void* p,FieldWallVector& root,const NpcSideWallRequest& request){
        auto& wall=*static_cast<Wall*>(p);++wall.calls;wall.before=root;wall.request=request;
        assert(wall.mario && wall.mario->hitNpc==17 && &root==&wall.mario->root);
        if(wall.mutate)root.x+=1;
    }};}
};
int main() {
    NpcAngleTable angles;assert(angles.load("romfs/npc-angle.bin"));
    assert(npcCompAngle(0,180)==-180 && npcCompAngle(180,0)==180);
    assert(npcCompAngle(350,10)==20 && npcCompAngle(10,350)==-20 && npcCompAngle(0,720)==360);
    // Independent exact-rational binary32 evaluation of the source instruction graph.
    // These are static deductions, not original-runtime measurements or SDK sqrt parity.
    const uint32_t golden[][2]={{0,0x3f800000},{0x3f350cbd,0x3f352c5c},{0x3f7fffd7,0x36a0c212},
        {0x3f350cbe,0xbf352c5c},{0,0xbf800000},{0xbf350cbc,0xbf352c5f},
        {0xbf7fffd7,0x36a0c212},{0xbf350cbf,0x3f352c5a},{0x34000000,0x3f800000}};
    for(unsigned n=0;n<9;++n){const auto trig=npcSideTrig(float(n*45));assert(bits(trig.sine)==golden[n][0] && bits(trig.cosine)==golden[n][1]);}
    NpcSideBody npc{17,{0,0,0},{0,0,0},30,30,0x80603,0};float distance=999;
    assert(npcHitSphere(npc,{0,15,0},0,distance) && distance==0);
    assert(!npcHitSphere(npc,{15,0,0},0,distance) && distance==15); // Strict sphere equality misses.
    assert(npcHitSphere(npc,{0,-14,0},0,distance) && distance==14);
    assert(!npcHitSphere(npc,{0,45,0},0,distance) && distance==15);
    assert(npcHitSphere(npc,{0,44,0},0,distance) && distance==14);
    assert(npcHitSphere(npc,{15,30,0},1,distance) && distance==15);
    npc.flags=0;assert(npcHitSphere(npc,{},0,distance)); // Pure sphere has no caller flags.
    for(uint32_t flags:{0u,1u,2u,0x100003u}){npc.flags=flags;npc.state=0;assert(!npcSideUpdateAdmitted(npc));}
    npc.flags=0x100003;npc.state=0x10;assert(npcSideUpdateAdmitted(npc));
    for(uint32_t mask:{8u,0x20u,0x4000u}){npc.flags=3|mask;npc.state=0;assert(!npcSideUpdateAdmitted(npc) && (npc.state&0x10));}
    npc.flags=0x80603;npc.state=0;assert(npcSideUpdateAdmitted(npc) && npc.state==0x10);
    npc.flags|=0x80;assert(npcSideUpdateAdmitted(npc)); // Draw hiding is not collision mask8.
    NpcSidePlayer mario;Wall wall;wall.mario=&mario;
    npc.root=npc.previous={0,0,0};mario.root={27.501f,0,0};
    assert(!respondNpcSide(npc,mario,angles,wall.service()) && !mario.hitNpc && !wall.calls);
    mario.root={27.5f,0,0};mario.travelHeading=270;
    assert(respondNpcSide(npc,mario,angles,wall.service()) && mario.hitNpc==17 && wall.calls==1);
    assert(mario.root.x==27.5f && wall.request.heading==430 && wall.request.mode==0 && wall.request.speed==0 && wall.request.width==20 && wall.request.height==37);
    npc.root={0,0,0};npc.previous={100,-100,0};mario.hitNpc=0;mario.root={0,0,0};wall.calls=0;
    assert(!respondNpcSide(npc,mario,angles,wall.service()) && npc.root.x==100 && npc.root.y==0 && !mario.hitNpc && !wall.calls);
    npc.root={100,0,0};npc.previous={0,0,0};assert(!respondNpcSide(npc,mario,angles,wall.service()) && npc.root.x==100);
    npc.root=npc.previous={0,0,0};mario.root={26,30.001f,0};
    assert(!respondNpcSide(npc,mario,angles,wall.service()));
    mario.root.y=30;assert(respondNpcSide(npc,mario,angles,wall.service()) && mario.root.y==30);
    npc.root=npc.previous={0,37,0};mario.root={26,0,0};assert(respondNpcSide(npc,mario,angles,wall.service()));
    npc.root.y=npc.previous.y=37.001f;assert(!respondNpcSide(npc,mario,angles,wall.service()));
    npc.root=npc.previous={0,0,0};mario.root={26,0,0};mario.travelHeading=309;
    assert(respondNpcSide(npc,mario,angles,wall.service()) && mario.root.z>.9f); // <40 uses travel heading.
    mario.root={26,0,0};mario.travelHeading=310;
    assert(respondNpcSide(npc,mario,angles,wall.service()) && std::fabs(mario.root.z)<.0002f); // Exactly40 uses radial theta; source polynomial cosine is not exactly zero.
    mario.root={25.75f,0,0};mario.travelHeading=270;mario.collisionWidth=4;
    assert(respondNpcSide(npc,mario,angles,wall.service()) && close(mario.root.x-25.75f,2)); // Low-width cap can enlarge1.75 to2.
    mario.root={25.75f,0,0};mario.collisionWidth=4.001f;
    assert(respondNpcSide(npc,mario,angles,wall.service()) && close(mario.root.x-25.75f,1.75f));
    mario.root={0,7,0};mario.travelHeading=0;mario.collisionWidth=20;wall.mutate=true;
    assert(respondNpcSide(npc,mario,angles,wall.service()) && close(wall.before.z,2) && mario.root.x==1 && mario.root.y==7 && close(mario.root.z,2));
    assert(wall.request.heading==160 && wall.request.width==20);
}
