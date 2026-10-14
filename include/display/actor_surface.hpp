#pragma once
#include "actor_mesh.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>
namespace ttyd {
using ActorColor=std::array<float,4>;
// Transform lanes=(S offset,T offset,S scale,T scale,rotation degrees).
// Wrap0 clamp,1 repeat,2 mirrored repeat; pass1/2/3 and cull0..3 retain
// original draw classifications, separate from DisplayQueue class IDs.
struct ActorSurfaceState {
    std::array<float,5> transform{{0,0,1,1,0}};
    uint32_t wrapS=0,wrapT=0;
};
struct ActorSurfaceBatch {uint32_t start=0,count=0,pass=2,cull=3;};
struct ActorSurfaceTriangle {uint32_t pass=2,cull=3;};
struct ActorSurfaceVertex {ActorMeshVertex vertex;ActorColor color;};
// Source R*M*S, positive/negative half pivots and literal translation gate.
// Host/ARM trig/rounding is an adapter to the recovered SDK matrix requests.
struct ActorTextureTransform {
    std::array<float,5> t;
    float sine=0,cosine=1;
    explicit ActorTextureTransform(const std::array<float,5>& transform):t(transform) {
        if(t[4]!=0) {
            const float angle=-t[4]*.01745329611003399f;
            sine=std::sin(angle);cosine=std::cos(angle);
        }
    }
    // Source translation gate checks T SCALE!=0 (not rotation); keep literal
    // gate. Rotation uses negative angle around normalized half pivots.
    // Different source degree coefficient here is intentionally retained.
    std::array<float,2> point(float u,float v) const {
        float x=u*t[2],y=v*t[3];
        if(t[0]!=0 || t[1]!=0 || t[3]!=0){x+=t[0];y+=(1-t[1])-t[3];}
        if(t[4]!=0) {
            x-=.5f;y-=.5f;const float rx=x*cosine-y*sine,ry=x*sine+y*cosine;x=rx+.5f;y=ry+.5f;
        }
        return {x,y};
    }
};
inline std::array<float,2> actorTexturePoint(const std::array<float,5>& t,float u,float v) {
    return ActorTextureTransform(t).point(u,v);
}
namespace actor_surface_detail {
struct Bin {float low,high;int32_t tile=0;};
// Native clipped-polygon workspace16 vertices; excessive clipping rejects
// instead of overflowing. Intersections lerp geometry and all RGBA channels.
struct Polygon {std::array<ActorSurfaceVertex,16> vertices{};unsigned count=0;};
inline ActorSurfaceVertex interpolate(const ActorSurfaceVertex& a,const ActorSurfaceVertex& b,float q) {
    ActorSurfaceVertex out{};
    auto lerp=[&](float x,float y){return std::fma(y-x,q,x);};
    out.vertex={lerp(a.vertex.x,b.vertex.x),lerp(a.vertex.y,b.vertex.y),lerp(a.vertex.z,b.vertex.z),lerp(a.vertex.u,b.vertex.u),lerp(a.vertex.v,b.vertex.v)};
    for(unsigned i=0;i<4;++i)out.color[i]=std::fma(b.color[i]-a.color[i],q,a.color[i]);
    return out;
}
inline bool clip(Polygon& polygon,unsigned axis,float plane,bool lower) {
    if(!polygon.count)return true;
    Polygon next;
    auto coordinate=[&](const ActorSurfaceVertex& p){return axis?p.vertex.v:p.vertex.u;};
    auto append=[&](const ActorSurfaceVertex& p){if(next.count==next.vertices.size())return false;next.vertices[next.count++]=p;return true;};
    auto previous=polygon.vertices[polygon.count-1];float last=coordinate(previous);
    bool lastInside=lower?last>=plane:last<=plane;
    for(unsigned i=0;i<polygon.count;++i) {
        const auto current=polygon.vertices[i];const float value=coordinate(current);const bool inside=lower?value>=plane:value<=plane;
        if(inside!=lastInside) {
            auto intersection=interpolate(previous,current,(plane-last)/(value-last));
            if(axis)intersection.vertex.v=plane;else intersection.vertex.u=plane;
            if(!append(intersection))return false;
        }
        if(inside && !append(current))return false;
        previous=current;last=value;lastInside=inside;
    }
    polygon=next;return true;
}
// Native finite-domain guards: UV magnitude<=2^20,at most34 bins per axis.
// Clamping partitions below0/[0,1]/above1; repeats use integer tile cells.
// ceil(high)-1 avoids adding an empty tile at an exact positive boundary.
inline unsigned bins(float low,float high,uint32_t wrap,std::array<Bin,34>& out) {
    if(!std::isfinite(low) || !std::isfinite(high) || std::fabs(low)>1048576 || std::fabs(high)>1048576 || wrap>2)return 0;
    if(!wrap) {
        unsigned n=0;
        if(low<0)out[n++]={low,0,0};
        if(high>=0 && low<=1)out[n++]={0,1,0};
        if(high>1)out[n++]={1,high,0};
        return n;
    }
    const int32_t first=int32_t(std::floor(low)),last=std::max(first,int32_t(std::ceil(high))-1);
    if(last-first>=int32_t(out.size()))return 0;
    for(int32_t i=first;i<=last;++i)out[unsigned(i-first)]={float(i),float(i+1),i};
    return unsigned(last-first+1);
}
// Mirrored repeat reverses fractional UV on ODD tiles, including negative
// odd tiles. Clamp AFTER subtracting the tile so seams retain0/1 endpoints.
inline float normalized(float value,const Bin& bin,uint32_t wrap) {
    if(!wrap)return std::clamp(value,0.f,1.f);
    const float fraction=std::clamp(value-float(bin.tile),0.f,1.f);
    return wrap==2 && bin.tile%2!=0?1-fraction:fraction;
}
}
// Partition source UV planes before atlas remapping. Clamping/wrapping ONLY
// vertices changes interpolation inside a triangle; this retains that surface.
// Per-axis34-bin/output limits bound native work; unsupported packs reject.
// Atlas ABI:64 cells in8x8,128-pixel pitch,2-pixel border,124 usable
// pixels in1024-square texture. V inversion happens once after remapping.
// Native output cap65536 vertices; emit complete triangles (three vertices).
// Failure may follow earlier appends; caller owns candidate/rollback policy.
inline bool appendActorSurface(const std::array<ActorSurfaceVertex,3>& triangle,const ActorSurfaceState& state,uint16_t cell,
                               std::vector<ActorMeshVertex>& vertices,std::vector<ActorColor>& colors) {
    using namespace actor_surface_detail;
    if(cell>=64)return false;
    std::array<Bin,34> s{},t{};
    float lowS=INFINITY,lowT=INFINITY,highS=-INFINITY,highT=-INFINITY;
    for(const auto& p:triangle){lowS=std::min(lowS,p.vertex.u);highS=std::max(highS,p.vertex.u);lowT=std::min(lowT,p.vertex.v);highT=std::max(highT,p.vertex.v);}
    const unsigned ns=bins(lowS,highS,state.wrapS,s),nt=bins(lowT,highT,state.wrapT,t);if(!ns || !nt)return false;
    // If every original vertex survives the sole bin's four planes, clipping
    // only copies the same three vertices. Explicit ordered comparisons leave
    // NaN coordinates on the original clipping path.
    if(ns==1 && nt==1 && std::all_of(triangle.begin(),triangle.end(),[&](const auto& p) {
        return p.vertex.u>=s[0].low && p.vertex.u<=s[0].high &&
               p.vertex.v>=t[0].low && p.vertex.v<=t[0].high;
    })) {
        if(vertices.size()>65536-3)return false;
        for(const auto& p:triangle) {
            auto v=p.vertex;
            const float u=normalized(v.u,s[0],state.wrapS),tv=normalized(v.v,t[0],state.wrapT);
            v.u=(float(cell%8)*128+2+u*124)/1024;v.v=1-(float(cell/8)*128+2+tv*124)/1024;
            vertices.push_back(v);colors.push_back(p.color);
        }
        return true;
    }
    for(unsigned i=0;i<ns;++i)for(unsigned j=0;j<nt;++j) {
        Polygon p;p.count=3;std::copy(triangle.begin(),triangle.end(),p.vertices.begin());
        if(!clip(p,0,s[i].low,true) || !clip(p,0,s[i].high,false) || !clip(p,1,t[j].low,true) || !clip(p,1,t[j].high,false))return false;
        for(unsigned n=1;n+1<p.count;++n) {
            if(vertices.size()>65536-3)return false;
            for(unsigned k:{0u,n,n+1}) {
                auto v=p.vertices[k].vertex;
                const float u=normalized(v.u,s[i],state.wrapS),tv=normalized(v.v,t[j],state.wrapT);
                v.u=(float(cell%8)*128+2+u*124)/1024;v.v=1-(float(cell/8)*128+2+tv*124)/1024;
                vertices.push_back(v);colors.push_back(p.vertices[k].color);
            }
        }
    }
    return true;
}
}
