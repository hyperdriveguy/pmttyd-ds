#include "npc_interaction.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
struct Rays {
    unsigned calls=0;bool blocked=false;
    FieldWallVector origin,direction;float length=0;
    FieldWallServices service(){return {this,[](void* p,FieldWallVector origin,FieldWallVector direction,float length){
        auto& r=*static_cast<Rays*>(p);++r.calls;r.origin=origin;r.direction=direction;r.length=length;
        FieldWallHit hit;hit.token=r.blocked?1:0;return hit;
    }};}
};
int main() {
    NpcAngleTable angles;assert(angles.load("romfs/npc-angle.bin"));
    NpcTalkPlayer player;player.displayTurn=90;
    assert(player.talkable() && player.displayDirection()==0);
    player.displayTurn=-271;player.cameraSnapshot=1;assert(player.displayDirection()==0);
    player.displayTurn=810;player.cameraSnapshot=0;assert(player.displayDirection()==0);player.displayTurn=90;
    for(unsigned motion=0;motion<34;++motion){player.motion=motion;assert(player.talkable()==(motion<=2));}player.motion=0;
    player.partyType=4;player.partyFlags=0x100;assert(!player.talkable());player.partyType=1;assert(player.talkable());player.partyFlags=0;
    player.flags=0x01000000;assert(!player.talkable());player.flags=0;
    player.viewFlags=1;assert(!player.talkable());player.viewFlags=0;
    player.backgroundMode=1;assert(!player.talkable());player.backgroundMode=0;
    player.controlMode=1;assert(!player.talkable());player.controlMode=0;
    NpcTalkBody npc{1,{35,0,0},30,30,3,true};Rays rays;NpcTalkScan scan;
    scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1 && scan.distance==35 && rays.calls==1);
    assert(rays.origin.x==0 && rays.origin.y==27.75f && rays.origin.z==0 && rays.length==35);
    assert(rays.direction.x==1 && std::fabs(rays.direction.z)<.000001f);
    npc.root.x=35.001f;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate && !scan.nearActor);
    npc.root={30,37,0};scan={};scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1);
    npc.root.y=37.001f;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);
    npc.root.y=-30;scan={};scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1);
    npc.root.y=-30.001f;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);npc.root.y=0;
    for(uint32_t flags:{0u,1u,8u,11u}){npc.flags=flags;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);}npc.flags=3;
    npc.talkEvent=false;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);npc.talkEvent=true;
    player.keyDepth=1;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);player.keyDepth=0;
    npc.root.x=-30;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate && scan.nearActor==1);
    player.displayTurn=270;scan={};scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1); // Exact180 admits both halves.
    player.displayTurn=269;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);player.displayTurn=90;
    npc.root={-5,0,30};scan={};scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1); // Width edge crosses halfplane.
    npc.width=0;scan={};scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);npc.width=30;
    npc.root={30,0,0};scan={};scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1);
    npc.token=2;scan.consider(npc,player,angles,rays.service());assert(scan.candidate==1); // First equal-distance candidate wins.
    npc.root.x=29;scan.consider(npc,player,angles,rays.service());assert(scan.candidate==2);
    rays.blocked=true;npc.token=3;npc.root.x=20;scan.consider(npc,player,angles,rays.service());assert(scan.candidate==2 && scan.nearActor==3);
    npc.token=4;npc.root.x=25;scan.consider(npc,player,angles,rays.service());assert(scan.candidate==2 && scan.nearActor==4); // r25 is not nearest.
    scan={};scan.consider(npc,player,angles,{});assert(!scan.candidate && scan.nearActor==4);
    // Separate live camera and rounded snapshot inputs survive subtraction.
    rays.blocked=false;player.cameraSnapshot=0;player.viewYaw=90;npc.root={30,0,0};scan={};
    scan.consider(npc,player,angles,rays.service());assert(!scan.candidate);
    player.cameraSnapshot=270;scan={};scan.consider(npc,player,angles,rays.service());assert(scan.candidate==4);
}
