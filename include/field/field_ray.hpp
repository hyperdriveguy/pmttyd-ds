#pragma once
#include "field_wall.hpp"
#include <array>
#include <algorithm>
#include <cmath>
#include <limits>

namespace ttyd {
// Native acceleration only. Retain the original narrow-phase request/order.
struct FieldRayBounds {FieldWallVector low,high;};
inline FieldRayBounds fieldRayBounds(const float* v) {
    float magnitude=1;
    for(unsigned i=0;i<9;++i)magnitude=std::max(magnitude,std::fabs(v[i]));
    const float margin=.01f+64.f*std::numeric_limits<float>::epsilon()*magnitude;
    FieldRayBounds bounds;
    bounds.low={std::min({v[0],v[3],v[6]})-margin,std::min({v[1],v[4],v[7]})-margin,std::min({v[2],v[5],v[8]})-margin};
    bounds.high={std::max({v[0],v[3],v[6]})+margin,std::max({v[1],v[4],v[7]})+margin,std::max({v[2],v[5],v[8]})+margin};
    return bounds;
}
struct FieldRaySegment {FieldRayBounds bounds;bool finite=false;};
inline FieldRaySegment fieldRaySegment(FieldWallVector origin,FieldWallVector direction,float maximum) {
    const FieldWallVector end{origin.x+direction.x*maximum,origin.y+direction.y*maximum,origin.z+direction.z*maximum};
    const bool finite=std::isfinite(origin.x) && std::isfinite(origin.y) && std::isfinite(origin.z) &&
        std::isfinite(direction.x) && std::isfinite(direction.y) && std::isfinite(direction.z) &&
        std::isfinite(maximum) && maximum>=0 && std::isfinite(end.x) && std::isfinite(end.y) && std::isfinite(end.z);
    return {{{std::min(origin.x,end.x),std::min(origin.y,end.y),std::min(origin.z,end.z)},
             {std::max(origin.x,end.x),std::max(origin.y,end.y),std::max(origin.z,end.z)}},finite};
}
inline bool fieldRayMayReach(const FieldRayBounds& b,const FieldRaySegment& ray) {
    if(!ray.finite)return true; // Leave unsupported requests to the existing narrow phase.
    return !(ray.bounds.low.x>b.high.x || ray.bounds.high.x<b.low.x ||
             ray.bounds.low.y>b.high.y || ray.bounds.high.y<b.low.y ||
             ray.bounds.low.z>b.high.z || ray.bounds.high.z<b.low.z);
}
inline bool fieldRayMayReach(const FieldRayBounds& b,FieldWallVector origin,
                             FieldWallVector direction,float maximum) {
    return fieldRayMayReach(b,fieldRaySegment(origin,direction,maximum));
}
// Native finite-triangle geometry service for the source wall policy. This is
// an explicit numerical adaptation, not original hitCheckVecFilter emulation.
inline bool intersectFieldRay(const std::array<float,9>& v,uint32_t cull,
                              FieldWallVector origin,FieldWallVector direction,
                              float maximum,FieldWallHit& hit) {
    const FieldWallVector a{v[3]-v[0],v[4]-v[1],v[5]-v[2]};
    const FieldWallVector b{v[6]-v[0],v[7]-v[1],v[8]-v[2]};
    const FieldWallVector normal{a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};
    const float dot=normal.x*direction.x+normal.y*direction.y+normal.z*direction.z;
    if(dot==0 || (cull==1 && dot>=0))return false;
    const FieldWallVector cross{direction.y*b.z-direction.z*b.y,
        direction.z*b.x-direction.x*b.z,direction.x*b.y-direction.y*b.x};
    const float determinant=a.x*cross.x+a.y*cross.y+a.z*cross.z;
    if(determinant==0)return false;
    const FieldWallVector offset{origin.x-v[0],origin.y-v[1],origin.z-v[2]};
    const float u=(offset.x*cross.x+offset.y*cross.y+offset.z*cross.z)/determinant;
    if(u<0 || u>1)return false;
    const FieldWallVector q{offset.y*a.z-offset.z*a.y,offset.z*a.x-offset.x*a.z,offset.x*a.y-offset.y*a.x};
    const float w=(direction.x*q.x+direction.y*q.y+direction.z*q.z)/determinant;
    if(w<0 || u+w>1)return false;
    const float distance=(b.x*q.x+b.y*q.y+b.z*q.z)/determinant;
    if(!std::isfinite(distance) || distance<0 || distance>maximum)return false;
    const float length=std::sqrt(normal.x*normal.x+normal.y*normal.y+normal.z*normal.z);
    if(length==0 || !std::isfinite(length))return false;
    hit.distance=distance;
    // Source general and axis ray paths orient a back hit toward its origin
    // before normalization (80016460–94; downward80015d20–7c).
    const float orientedLength=dot>0?-length:length;
    hit.normal={normal.x/orientedLength,normal.y/orientedLength,normal.z/orientedLength};
    return true;
}
inline bool ordinaryWallAttributes(uint32_t flags) {
    // High-bit named NPC owners need a provider; static Rogueport packs have no
    // such records. This helper implements the preceding ordinary bit branches.
    return (flags&0x00080080) || !(flags&4) || (flags&0x00800000);
}
}
