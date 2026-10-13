#include "npc_interaction.hpp"
#include <cassert>
#include <cmath>
namespace ttyd {
bool NpcTalkPlayer::talkable() const {
    return !(partyType==4 && (partyFlags&0x100)) && motion<=2 && !(viewFlags&1) &&
        backgroundMode!=1 && !(flags&0x01000000) && !controlMode;
}
float NpcTalkPlayer::displayDirection() const {
    float angle=(displayTurn-90.f)+cameraSnapshot;
    assert(std::isfinite(angle));
    while(angle<0)angle+=360.f;
    while(angle>=360.f)angle-=360.f;
    return angle;
}
void NpcTalkScan::consider(const NpcTalkBody& npc,const NpcTalkPlayer& mario,
                          const NpcAngleTable& angles,const FieldWallServices& rays) {
    if(!mario.talkable() || mario.keyDepth || !npc.token || !(npc.flags&2) ||
       (npc.flags&8) || !npc.talkEvent || npc.root.y>mario.root.y+mario.height ||
       mario.root.y>npc.root.y+npc.height)return;
    const float dx=npc.root.x-mario.root.x,dz=npc.root.z-mario.root.z;
    const float norm=std::sqrt(std::fma(dx,dx,dz*dz)); // Host/ARM SDK norm adapter.
    const float radius=std::fma(.5f,mario.width+npc.width,10.f);
    if(norm>radius)return;
    if(norm<distance)nearActor=npc.token; // Separate r25; the bound changes only on full admission.
    const float facing=npcReviseAngle(mario.displayDirection()-mario.viewYaw);
    bool admitted=false;
    for(float x:{npc.root.x,npc.root.x+.5f*npc.width,npc.root.x-.5f*npc.width}) {
        const float angle=npcReviseAngle(angles.angle(mario.root.x,mario.root.z,x,npc.root.z)-mario.viewYaw);
        if(angle<180?facing<=180:facing>=180){admitted=true;break;}
    }
    if(!admitted || !rays.ray)return; // Unavailable map service cannot prove a clear ray.
    const float heading=angles.angle(mario.root.x,mario.root.z,npc.root.x,npc.root.z)*.01745329238474369f;
    auto origin=mario.root;origin.y=std::fma(.75f,mario.height,origin.y);
    if(rays.ray(rays.user,origin,{std::sin(heading),0,-std::cos(heading)},norm).token)return;
    if(norm<distance){candidate=npc.token;distance=norm;}
}
}
