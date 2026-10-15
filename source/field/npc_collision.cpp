#include "npc_collision.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float literal(uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
struct Reduced {float u;bool negativeSine,negativeCosine;};
// Exact IEEE source constants: tau40c90fdb, pi40490fdb, halfPi3fc90fdb,
// threeHalfPi4096cbe4(hex). Keep single quadrant reduction and boundaries.
Reduced reduce(float heading) {
    const float t=(literal(0x40c90fdb)*heading)/360.f;
    const float pi=literal(0x40490fdb),half=literal(0x3fc90fdb),threeHalf=literal(0x4096cbe4);
    if(t<=pi) {
        if(t<=half)return {t,false,false};
        const float d=t-half;return {half-d,false,true};
    }
    if(t<threeHalf)return {t-pi,true,true};
    const float d=t-threeHalf;return {half-d,true,false};
}
float horizontalDistance(FieldWallVector a,FieldWallVector b) {
    const float dx=a.x-b.x,dz=a.z-b.z;
    return std::sqrt(std::fma(dx,dx,dz*dz)); // SDK/Gekko norm numerical adapter.
}
}
float npcCompAngle(float from,float to) {
    if(std::fabs(to-from)>=180.f)to+=to<from?360.f:-360.f;
    return to-from; // Exactly one adjustment; not a generic wrap loop.
}
// Odd fifth-degree sine/even fourth-degree cosine coefficients are original
// bit patterns below. Independent reductions/FMA graphs avoid std::sin/cos
// substituting different response vectors; full Gekko norm parity is unclaimed.
NpcSideTrig npcSideTrig(float heading) {
    // Independent finite source reduction/rounding graphs for the two polynomials.
    const auto s=reduce(heading);const float sv=s.u*s.u;
    float sine=std::fma(std::fma(literal(0x3bf95d4f),sv,-literal(0x3e2a0903)),sv,1.f)*s.u;
    const auto c=reduce(heading);const float cv=c.u*c.u;
    float cosine=std::fma(std::fma(literal(0x3d17c1be),cv,-literal(0x3efe4f76)),cv,1.f);
    if(s.negativeSine)sine=-sine;
    if(c.negativeCosine)cosine=-cosine;
    return {sine,cosine};
}
bool npcHitSphere(const NpcSideBody& npc,FieldWallVector point,float extraRadius,float& distance) {
    float dy=0;
    if(point.y<npc.root.y)dy=point.y-npc.root.y;
    else if(point.y>npc.root.y+npc.height)dy=point.y-(npc.root.y+npc.height);
    const float dx=point.x-npc.root.x,dz=point.z-npc.root.z;
    distance=std::sqrt(std::fma(dz,dz,std::fma(dx,dx,dy*dy)));
    return distance<std::fma(.5f,npc.width,extraRadius);
}
// Active flags low 2 must equal3. Without100000(hex), publish state 10;
// flags 8/20/4000 then suppress side handling. Do not undo the state write.
bool npcSideUpdateAdmitted(NpcSideBody& npc) {
    if((npc.flags&3)!=3)return false;
    if(!(npc.flags&0x100000))npc.state|=0x10;
    return (npc.state&0x10) && !(npc.flags&(8|0x20|0x4000));
}
// Source engine-space vertical interval/radius tests. NPC XZ rolls back to
// previous before the second range test and remains rolled back even on miss.
// Combined radius=(MarioWidth+NpcWidth+5)/2, heading near-window strict40deg.
// Correction threshold 3(or1.5 narrow Mario) triggers scaling to length 2,
// then mandatory wall request uses heading theta+160, width 20.
bool respondNpcSide(NpcSideBody& npc,NpcSidePlayer& mario,const NpcAngleTable& angles,const NpcSideServices& services) {
    if(npc.root.y>mario.root.y+mario.height || mario.root.y>npc.root.y+npc.height)return false;
    const float radius=((mario.width+npc.width)+5.f)*.5f;
    if(horizontalDistance(mario.root,npc.root)>radius)return false;
    npc.root.x=npc.previous.x;npc.root.z=npc.previous.z;
    const float distance=horizontalDistance(mario.root,npc.root);
    if(distance>radius)return false;
    assert(services.wall);
    mario.hitNpc=npc.token;
    const float theta=angles.angle(mario.root.x,mario.root.z,npc.root.x,npc.root.z);
    const bool near=std::fabs(npcCompAngle(theta,mario.travelHeading))<40.f;
    const auto trig=npcSideTrig(near?mario.travelHeading:theta);
    float dx,dz;
    if(near){const float penetration=radius-distance;dx=penetration*-trig.sine;dz=penetration*trig.cosine;}
    else {dx=-std::fma(radius,trig.sine,-npc.root.x)-mario.root.x;dz=std::fma(radius,trig.cosine,npc.root.z)-mario.root.z;}
    const float norm=std::sqrt(std::fma(dx,dx,dz*dz));
    const float threshold=mario.collisionWidth>4.f?3.f:1.5f;
    if(norm>threshold){const float factor=2.f/norm;dx*=factor;dz*=factor;}
    mario.root.x+=dx;mario.root.z+=dz;
    services.wall(services.user,mario.root,{0,0,theta+160.f,20,mario.height});
    return true;
}
}
