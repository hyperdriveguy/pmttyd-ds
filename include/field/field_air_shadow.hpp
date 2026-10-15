#pragma once
#include "field_shadow.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
namespace ttyd {
struct FieldShadowList {uint32_t primitive=0;std::vector<std::array<uint8_t,2>> pairs;};
// PMSHDW1 is native LE geometry: 60 XYZ float3, 24 palette bytes(two banks
// of3 RGBA colors), five lists. Load validates candidate/EOF before publication.
class FieldShadowGeometry {
public:
    std::array<FieldWallVector,60> positions{};
    std::array<std::array<std::array<uint8_t,4>,3>,2> palettes{};
    std::array<FieldShadowList,5> lists;
    bool loaded=false;
    bool load(const char* path) {FILE* f=std::fopen(path,"rb");if(!f)return false;bool ok=read(f);std::fclose(f);return ok;}
    bool read(FILE* file) {
        FieldShadowGeometry next;std::array<char,8> magic{};uint32_t count=0,listCount=0;
        if(std::fread(magic.data(),8,1,file)!=1 || std::memcmp(magic.data(),"PMSHDW1",8) ||
           std::fread(&count,4,1,file)!=1 || count!=60 || std::fread(&listCount,4,1,file)!=1 || listCount!=5)return false;
        if(std::fread(next.positions.data(),sizeof(FieldWallVector),60,file)!=60 ||
           std::fread(next.palettes.data(),24,1,file)!=1)return false;
        for(const auto& p:next.positions)if(!std::isfinite(p.x) || !std::isfinite(p.y) || !std::isfinite(p.z))return false;
        // Source fana0/strip98(hex), INDEX8 position/color pairs. Top/bottom/
        // inside fans16 points; side/outside strips30. These are resource IDs,
        // not adaptive tessellation counts.
        const char* names[]={"top","bottom","side","top_inside","top_outside"};
        constexpr uint32_t primitives[]={0xa0,0xa0,0x98,0xa0,0x98};
        constexpr uint32_t sizes[]={16,16,30,16,30};
        for(size_t i=0;i<5;++i) {
            std::array<char,16> name{};auto& list=next.lists[i];uint32_t size=0;
            if(std::fread(name.data(),16,1,file)!=1 || !std::memchr(name.data(),0,16) || std::strcmp(name.data(),names[i]) ||
               std::fread(&list.primitive,4,1,file)!=1 || list.primitive!=primitives[i] ||
               std::fread(&size,4,1,file)!=1 || size!=sizes[i])return false;
            list.pairs.resize(size);if(std::fread(list.pairs.data(),2,size,file)!=size)return false;
            for(auto p:list.pairs)if(p[0]>=60 || p[1]>=3)return false;
        }
        if(std::fgetc(file)!=EOF)return false;
        next.loaded=true;*this=std::move(next);return true;
    }
};
struct FieldAirShadowPlan {std::vector<uint8_t> lists;bool noDecay=false;};
// Native ABGR high byte supplies opacity. Flag 4 selects no-decay lists0/1/2;
// otherwise inner/outside list3/4 admission follows clamped decay and truncated
// alpha. The original palette owns colors; do not bake this into vertex alpha.
inline FieldAirShadowPlan fieldAirShadowPlan(const FieldShadowRecord& r,uint8_t camera=4) {
    FieldAirShadowPlan plan;
    if(r.type!=2 || !(r.flags&2) || r.size==0 || r.camera!=camera)return plan;
    if(r.flags&4){plan.noDecay=true;plan.lists={0,1,2};return plan;}
    float a=float(uint8_t(r.color>>24))*r.opacity;a/=255.f;
    float b=a*r.innerOpacity;float inner=1-r.decay;
    inner=std::max(0.f,std::min(1.f,inner));
    if(a<=0)return plan;
    if(b<=0 || b>=1)b=0;
    const uint8_t alpha=uint8_t(int32_t(std::trunc(255.f*b))&255);
    if(inner!=0)plan.lists.push_back(3);
    if(inner!=1 && alpha!=0)plan.lists.push_back(4);
    plan.lists.push_back(1);plan.lists.push_back(2);return plan;
}
struct FieldAirShadowVertex {FieldWallVector point;std::array<uint8_t,4> color;};
inline FieldWallVector fieldShadowPoint(const FieldShadowRecord& r,FieldWallVector p) {
    const auto& m=r.matrix;
    return {m[0]*p.x+m[1]*p.y+m[2]*p.z+m[3],m[4]*p.x+m[5]*p.y+m[6]*p.z+m[7],m[8]*p.x+m[9]*p.y+m[10]*p.z+m[11]};
}
// Preserve fan and alternating strip winding while triangulating into native
// CPU vertices. This expands geometry only; simulation/source requests stay put.
inline std::vector<FieldAirShadowVertex> fieldAirShadowVertices(const FieldShadowGeometry& geometry,
                                                              const FieldShadowRecord& r,const FieldAirShadowPlan& plan) {
    std::vector<FieldAirShadowVertex> out;
    if(!geometry.loaded)return out;
    for(auto id:plan.lists) {
        const auto& list=geometry.lists[id];
        auto emit=[&](size_t index){const auto pair=list.pairs[index];out.push_back({fieldShadowPoint(r,geometry.positions[pair[0]]),geometry.palettes[plan.noDecay?1:0][pair[1]]});};
        for(size_t i=2;i<list.pairs.size();++i) {
            if(list.primitive==0xa0){emit(0);emit(i-1);emit(i);}
            else if(i%2==0){emit(i-2);emit(i-1);emit(i);}
            else {emit(i-1);emit(i-2);emit(i);}
        }
    }
    return out;
}
}
