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
    // The candidate lifecycle is independent of winning nearest selection.
    struct Events {unsigned stops=0,starts=0;} events;
    NpcTalkTurnServices turns{&events,
        [](void* p){++static_cast<Events*>(p)->stops;},
        [](void* p){++static_cast<Events*>(p)->starts;}};
    NpcTalkTurn turn{0x10,0x40,270,0};
    updateNpcTalkTurn(turn,true,0x00400003,{0,0,0},{30,0,0},angles,turns);
    assert(turn.state==0x20000010 && turn.savedYaw==270 && turn.yaw==90 && events.stops==1);
    updateNpcTalkTurn(turn,true,0x00400003,{0,0,0},{0,0,30},angles,turns);
    assert(turn.yaw==90 && turn.savedYaw==270 && events.stops==1);
    updateNpcTalkTurn(turn,false,0x00400003,{},{},angles,turns);
    assert(turn.state==0x10 && turn.yaw==270 && turn.secondaryFlags==0x40 && events.starts==1);
    updateNpcTalkTurn(turn,false,0x00400003,{},{},angles,turns);assert(events.starts==1);
    // Static loss requests +140's consumer rather than directly restoring yaw.
    updateNpcTalkTurn(turn,true,3,{},{30,0,0},angles,turns);
    updateNpcTalkTurn(turn,false,3,{},{},angles,turns);
    assert(turn.yaw==90 && turn.savedYaw==270 && turn.secondaryFlags==0x10000040 && events.starts==1);
    // A source no-turn actor still saves yaw and stops its regular event.
    turn={0,0,180,0};updateNpcTalkTurn(turn,true,0x01000003,{},{30,0,0},angles,turns);
    assert(turn.yaw==180 && turn.savedYaw==180 && (turn.state&0x20000000));
    turn.secondaryFlags=0xfffffaff;turn.state=0x20001010;
    consumeNpcNullInterrupts(turn.secondaryFlags,turn.state);
    assert(turn.secondaryFlags==0xa00 && turn.state==0x20000010 && turn.yaw==180);
    turn={};updateNpcTalkTurn(turn,true,3,{},{-1.e-10f,0,-30},angles,{});
    assert(turn.yaw>=0 && turn.yaw<360 && turn.yaw==0); // Rounded source angle360 is normalized.
}
