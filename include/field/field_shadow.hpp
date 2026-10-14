#pragma once
#include "field_wall.hpp"
#include <array>
#include <cstdint>
namespace ttyd {
struct FieldShadowRecord {
    uint8_t type=0,camera=4;
    uint32_t flags=1,color=0x6e000000u; // Native packed RGBA: black,alpha110.
    FieldWallVector root;
    float size=0,height=20;
    float opacity=1,innerOpacity=.2f,decay=.4f;
    uint8_t alpha=255;
    std::array<float,12> matrix{};
};
inline FieldShadowRecord marioShadowRequest(FieldWallVector root,float retainedGround,bool support,
                                           uint32_t displayFlags=0,uint32_t mainFlags=0) {
    FieldShadowRecord r;r.root=root;
    if((displayFlags&0x1001) || (mainFlags&0x02000000))return r;
    constexpr float size=13.333333015441895f;
    if(support){r.size=size;return r;}
    const float h=root.y-retainedGround;
    if(h<-2)return r;
    r.type=2;r.size=size-h/size;
    if(r.size<.10000000149011612f)r.size=0;
    return r;
}
inline bool fieldShadowAttributeAdmitted(uint32_t attributes,uint32_t objectFlags) {
    return !(attributes&0x80000000u) || !(objectFlags&0x800);
}
struct FieldShadowHit {bool found=false,unresolvedObject=false;FieldWallVector point,normal;};
struct FieldShadowServices {
    void* user=nullptr;
    FieldShadowHit (*ray)(void*,FieldWallVector,FieldWallVector,float)=nullptr;
};
inline bool prepareFieldShadow(FieldShadowRecord& r,const FieldShadowServices& services) {
    if(r.size==0)return false;
    if(r.flags&2)return true;
    if(r.type!=3 && (r.flags&1)) {
        const auto hit=services.ray(services.user,{r.root.x,r.root.y+1,r.root.z},{0,-1,0},150);
        if(!hit.found)return false;
        const auto p=hit.point,n=hit.normal;const float s=r.size,h=r.height;
        r.matrix={{-s*(n.y*n.y+n.z*n.z),s*n.x*n.y,s*n.x*n.z,p.x+n.x,
                   s*n.x,h*n.y,s*n.z,p.y+n.y,0,s*n.z,-s*n.y,p.z+n.z}};
    } else {
        r.matrix={{r.size,0,0,r.root.x,0,r.height,0,r.root.y+1.5f,0,0,r.size,r.root.z}};
    }
    r.flags|=2;return true;
}
inline bool fieldShadowPolygonAdmitted(const FieldShadowRecord& r,uint8_t camera=4) {
    return r.size!=0 && r.type==0 && (r.flags&2) && r.camera==camera;
}
inline std::array<FieldWallVector,6> fieldShadowPolygon(const FieldShadowRecord& r) {
    constexpr std::array<FieldWallVector,6> original{{{0,0,-1},{.8999999761581421f,0,-.5f},
        {-.8999999761581421f,0,-.5f},{.8999999761581421f,0,.5f},{-.8999999761581421f,0,.5f},{0,0,1}}};
    auto points=original;
    for(auto& v:points) {
        const auto p=v;const auto& m=r.matrix;
        v={m[0]*p.x+m[1]*p.y+m[2]*p.z+m[3],m[4]*p.x+m[5]*p.y+m[6]*p.z+m[7],m[8]*p.x+m[9]*p.y+m[10]*p.z+m[11]};
    }
    return points;
}
}
