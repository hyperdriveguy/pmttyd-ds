#include "actor_animation.hpp"
#include "actor_surface_reference.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstring>
#include <filesystem>
#include <limits>
using namespace ttyd;
namespace {
void compareSurface(const std::array<ActorSurfaceVertex,3>& triangle,const ActorSurfaceState& state,
                    uint16_t cell,size_t prefix=0) {
    const ActorMeshVertex sentinel{1,2,3,4,5};const ActorColor color{{.1f,.2f,.3f,.4f}};
    std::vector<ActorMeshVertex> actual(prefix,sentinel),expected=actual;
    std::vector<ActorColor> actualColors(prefix,color),expectedColors=actualColors;
    const bool oldResult=actor_surface_reference::appendActorSurface(triangle,state,cell,expected,expectedColors);
    const bool newResult=appendActorSurface(triangle,state,cell,actual,actualColors);
    assert(oldResult==newResult && expected.size()==actual.size() && expectedColors.size()==actualColors.size());
    if(!expected.empty())assert(std::memcmp(expected.data(),actual.data(),expected.size()*sizeof(ActorMeshVertex))==0);
    if(!expectedColors.empty())assert(std::memcmp(expectedColors.data(),actualColors.data(),expectedColors.size()*sizeof(ActorColor))==0);
}
void clippingDifferentialCases() {
    std::array<ActorSurfaceVertex,3> triangle{{{{1,2,3,.1f,.2f},{1,0,0,.25f}},
        {{4,5,6,.7f,.2f},{0,1,0,.5f}},{{7,8,9,.1f,.8f},{0,0,1,.75f}}}};
    for(uint32_t ws=0;ws<4;++ws)for(uint32_t wt=0;wt<4;++wt) {
        ActorSurfaceState state;state.wrapS=ws;state.wrapT=wt;
        for(float shift:{-3.f,-1.f,-0.f,0.f,1.f,2.f}) {
            auto shifted=triangle;for(auto& p:shifted){p.vertex.u+=shift;p.vertex.v+=shift;}
            for(uint16_t cell:{0,10,63,64})compareSurface(shifted,state,cell);
        }
        // Include edges, signed zeros, nonfinite UVs, and bin/output rejection.
        const float nan=std::numeric_limits<float>::quiet_NaN();
        for(float value:{-INFINITY,-1048577.f,-35.f,-1.f,-0.f,0.f,1.f,35.f,1048577.f,INFINITY,nan}) {
            for(unsigned vertex=0;vertex<3;++vertex)for(unsigned axis=0;axis<2;++axis) {
                auto changed=triangle;if(axis)changed[vertex].vertex.v=value;else changed[vertex].vertex.u=value;
                compareSurface(changed,state,10);
            }
            auto point=triangle;for(auto& p:point){p.vertex.u=value;p.vertex.v=value;}
            compareSurface(point,state,10);
        }
        for(size_t prefix:{size_t(1),size_t(65532),size_t(65533),size_t(65534),size_t(65536)}) {
            compareSurface(triangle,state,10,prefix);
            auto crossing=triangle;crossing[0].vertex.u=-1.2f;crossing[1].vertex.u=2.4f;
            compareSurface(crossing,state,10,prefix); // Retain fallback's partial output on overflow.
        }
        auto boundary=triangle;boundary[0].vertex.u=-0.f;boundary[0].vertex.v=0;
        boundary[1].vertex.u=1;boundary[2].vertex.v=1;
        compareSurface(boundary,state,10);
        for(float value:{nan,INFINITY,-INFINITY}) {
            auto nonfinite=triangle;nonfinite[0].vertex.x=value;nonfinite[1].color[2]=value;
            compareSurface(nonfinite,state,10); // Positions/colors are copied, not eligibility inputs.
        }
    }
    // Deterministic mixed interior, outside-clamp, mirrored and crossing bins.
    uint32_t seed=0x19371;
    auto random=[&](){seed=seed*1664525u+1013904223u;return float(int(seed%4097)-2048)/1024.f;};
    for(unsigned n=0;n<12000;++n) {
        auto changed=triangle;
        const float ox=random()*4,oy=random()*4,span=n%2?.2f:2.f;
        for(auto& p:changed){p.vertex.u=ox+random()*span;p.vertex.v=oy+random()*span;}
        ActorSurfaceState state;state.wrapS=n%3;state.wrapT=(n/3)%3;
        compareSurface(changed,state,uint16_t(n%64));
    }
}
// Retain the pre-hoist arithmetic as an independent exact numerical oracle.
std::array<float,2> referenceTexturePoint(const std::array<float,5>& t,float u,float v) {
    float x=u*t[2],y=v*t[3];
    if(t[0]!=0 || t[1]!=0 || t[3]!=0){x+=t[0];y+=(1-t[1])-t[3];}
    if(t[4]!=0) {
        const float angle=-t[4]*.01745329611003399f,s=std::sin(angle),c=std::cos(angle);
        x-=.5f;y-=.5f;const float rx=x*c-y*s,ry=x*s+y*c;x=rx+.5f;y=ry+.5f;
    }
    return {x,y};
}
void preparedTextureCases() {
    for(float rotation:{0.f,-0.f,1.f,-37.25f,90.f,180.f,359.99f,720.f})
        for(float scale:{0.f,-0.f,1.f,-2.f,.125f}) {
            const std::array<float,5> t{{.125f,-.375f,scale,scale,rotation}};
            const ActorTextureTransform prepared(t);
            for(float u:{-3.f,-0.f,0.f,.125f,.5f,1.f,4.25f})
                for(float v:{-2.f,-0.f,0.f,.25f,1.f,3.75f}) {
                    const auto expected=referenceTexturePoint(t,u,v),actual=prepared.point(u,v);
                    assert(std::memcmp(expected.data(),actual.data(),sizeof(expected))==0);
                }
        }
    ActorAnimationPose pose;pose.end=2;
    pose.positions={{{0,0,0}},{{1,0,0}},{{0,1,0}}};
    pose.nodes.resize(24);pose.nodes[3]=pose.nodes[4]=pose.nodes[5]=1;
    pose.groups.push_back({-1,-1,0,0,1});
    pose.bindings={{0,0,0,-.2f,.1f},{0,1,0,1.2f,.1f},{0,2,0,.3f,1.1f}};
    pose.textures.resize(1);pose.surfaces.push_back({{{.1f,.2f,1,1,37.25f}},1,2});
    pose.colors={{{1,0,0,1}},{{0,1,0,.5f}},{{0,0,1,1}}};pose.triangles.push_back({2,3});
    pose.keys.resize(2);pose.keys[1].time=1;
    pose.keys[1].textures.push_back({0,0,{0,0,0}});pose.keys[1].shifts.push_back({.25f,-.125f});
    ActorAnimationSample sample;
    // Reuse the sample across forward, repeated and backward discrete shifts.
    for(float frame:{0.f,1.f,1.f,0.f,2.f}) {
        assert(pose.evaluate(frame,sample));
        assert(sample.surfaces[0].transform[0]==(frame>=1?.35f:.1f));
        std::array<ActorSurfaceVertex,3> triangle;
        for(unsigned i=0;i<3;++i) {
            const auto& b=pose.bindings[i];const auto& p=pose.positions[b.position];
            const auto uv=referenceTexturePoint(sample.surfaces[0].transform,b.u,b.v);
            triangle[i]={{p[0],p[1],p[2],uv[0],uv[1]},pose.colors[i]};
        }
        std::vector<ActorMeshVertex> expected;std::vector<ActorColor> colors;
        assert(appendActorSurface(triangle,sample.surfaces[0],0,expected,colors));
        assert(expected.size()==sample.vertices.size() && colors==sample.colors);
        assert(std::memcmp(expected.data(),sample.vertices.data(),expected.size()*sizeof(ActorMeshVertex))==0);
        assert(sample.batches.size()==1 && sample.batches[0].count==expected.size());
    }
}
void originalPackDifferentialCases() {
    size_t packs=0,frames=0;
    for(const auto& file:std::filesystem::directory_iterator("romfs")) {
        if(file.path().extension()!=".pmanim")continue;
        ActorAnimationPack pack;assert(pack.load(file.path().c_str()));++packs;
        const auto* pose=pack.firstPose();assert(pose);
        for(float frame:{0.f,17.f,45.f}) {
            ActorAnimationSample sample;assert(pose->evaluate(frame,sample));
            if(pose->surfaces.empty())continue;
            std::vector<ActorMeshVertex> vertices;std::vector<ActorColor> colors;
            for(size_t index=0;index<pose->bindings.size();index+=3) {
                const auto& first=pose->bindings[index];if(!sample.active[first.group])continue;
                const auto cell=pose->textures[first.texture].cells[sample.textures[first.texture]];
                std::array<ActorSurfaceVertex,3> triangle;
                for(unsigned i=0;i<3;++i) {
                    const auto& b=pose->bindings[index+i];const auto p=sample.world[b.group].point(sample.positions[b.position]);
                    const auto uv=referenceTexturePoint(sample.surfaces[b.texture].transform,b.u,b.v);
                    triangle[i]={{p[0],p[1],p[2],uv[0],uv[1]},pose->colors[index+i]};
                }
                assert(actor_surface_reference::appendActorSurface(triangle,sample.surfaces[first.texture],cell,vertices,colors));
            }
            assert(vertices.size()==sample.vertices.size() && colors.size()==sample.colors.size());
            if(!vertices.empty())assert(std::memcmp(vertices.data(),sample.vertices.data(),vertices.size()*sizeof(ActorMeshVertex))==0);
            if(!colors.empty())assert(std::memcmp(colors.data(),sample.colors.data(),colors.size()*sizeof(ActorColor))==0);
            ++frames;
        }
    }
    assert(packs>0 && frames>0);
    std::cout<<"Original pack clipping differential: "<<packs<<" packs, "<<frames<<" surface frames\n";
}
float area(const ActorMeshVertex& a,const ActorMeshVertex& b,const ActorMeshVertex& c){return std::fabs((b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x))*.5f;}
float wrap(float v,uint32_t mode) {
    if(!mode)return std::clamp(v,0.f,1.f);
    const int tile=int(std::floor(v));const float q=v-float(tile);
    return mode==2 && tile%2!=0?1-q:q;
}
void surfaceCases(uint32_t ws,uint32_t wt) {
    const std::array<ActorSurfaceVertex,3> source{{{{-1.2f,-.4f,0,-1.2f,-.4f},{1,0,0,.2f}},{{2.4f,.2f,0,2.4f,.2f},{0,1,0,.6f}},{{.3f,2.2f,0,.3f,2.2f},{0,0,1,1}}}};
    ActorSurfaceState state;state.wrapS=ws;state.wrapT=wt;
    std::vector<ActorMeshVertex> vertices;std::vector<ActorColor> colors;
    constexpr uint16_t cell=10;
    assert(appendActorSurface(source,state,cell,vertices,colors));assert(vertices.size()==colors.size() && vertices.size()%3==0);
    double total=0;for(size_t i=0;i<vertices.size();i+=3)total+=area(vertices[i],vertices[i+1],vertices[i+2]);
    assert(std::fabs(total-area(source[0].vertex,source[1].vertex,source[2].vertex))<.00002);
    for(unsigned a=0;a<16;++a)for(unsigned b=0;b<16-a;++b) {
        const float q=(a+.31f)/16,r=(b+.23f)/16,p=1-q-r;
        const float x=p*source[0].vertex.x+q*source[1].vertex.x+r*source[2].vertex.x;
        const float y=p*source[0].vertex.y+q*source[1].vertex.y+r*source[2].vertex.y;
        bool found=false;
        for(size_t i=0;i<vertices.size();i+=3) {
            const auto& v0=vertices[i];const auto& v1=vertices[i+1];const auto& v2=vertices[i+2];
            const float denominator=(v1.x-v0.x)*(v2.y-v0.y)-(v1.y-v0.y)*(v2.x-v0.x);
            if(denominator==0)continue;
            const float s=((x-v0.x)*(v2.y-v0.y)-(y-v0.y)*(v2.x-v0.x))/denominator;
            const float t=((v1.x-v0.x)*(y-v0.y)-(v1.y-v0.y)*(x-v0.x))/denominator;
            if(s<-.000001f || t<-.000001f || s+t>1.000001f)continue;
            const float u=((1-s-t)*v0.u+s*v1.u+t*v2.u)*1024;
            const float v=(1-((1-s-t)*v0.v+s*v1.v+t*v2.v))*1024;
            assert(std::fabs((u-(cell%8)*128-2)/124-wrap(x,ws))<.00002f);
            assert(std::fabs((v-(cell/8)*128-2)/124-wrap(y,wt))<.00002f);
            for(unsigned channel=0;channel<4;++channel) {
                const float actual=(1-s-t)*colors[i][channel]+s*colors[i+1][channel]+t*colors[i+2][channel];
                const float expected=p*source[0].color[channel]+q*source[1].color[channel]+r*source[2].color[channel];
                assert(std::fabs(actual-expected)<.00002f);
            }
            found=true;break;
        }
        assert(found);
    }
}
}
int main(int argc,char**) {
    clippingDifferentialCases();
    preparedTextureCases();
    for(uint32_t s=0;s<3;++s)for(uint32_t t=0;t<3;++t)surfaceCases(s,t);
    const auto translated=actorTexturePoint({.25f,.5f,2,3,0},.2f,.4f);
    assert(std::fabs(translated[0]-.65f)<.000001f && std::fabs(translated[1]+1.3f)<.000001f);
    const auto zeroScale=actorTexturePoint({0,0,1,0,0},.2f,.4f);assert(zeroScale[0]==.2f && zeroScale[1]==0);
    const auto rotated=actorTexturePoint({0,0,1,1,90},.5f,.6f);assert(std::fabs(rotated[0]-.6f)<.000001f && std::fabs(rotated[1]-.5f)<.000001f);
    if(argc>1) {
        originalPackDifferentialCases();
        ActorAnimationPack pack;assert(pack.load("romfs/field-saveblock.pmanim"));
        const auto* idle=pack.pose("S_1");const auto* hit=pack.pose("A_1");assert(idle && hit);
        assert(idle->start==1 && idle->end==119 && idle->loop==1 && hit->start==0 && hit->end==15 && hit->loop==0);
        assert(idle->surfaces.size()==3 && idle->surfaces[0].wrapS==1 && idle->surfaces[0].wrapT==1 && idle->surfaces[1].wrapS==1 && idle->surfaces[1].wrapT==1);
        assert(idle->bindings.size()==264 && idle->colors.size()==264);
        bool translucent=false;for(const auto& c:idle->colors)translucent|=c[3]<1;assert(translucent);
        ActorAnimationSample sample;size_t peak=0;
        for(float frame:{0.f,1.f,1.5f,2.f,2.5f,60.f,118.f,119.f,120.f}) {
            assert(idle->evaluate(frame,sample));peak=std::max(peak,sample.vertices.size());
            assert(sample.colors.size()==sample.vertices.size() && sample.vertices.size()%3==0 && sample.vertices.size()<12000);
            bool opaque=false,glow=false;for(const auto& b:sample.batches){opaque|=b.pass==2;glow|=b.pass==3;assert(b.start+b.count<=sample.vertices.size());}assert(opaque && glow);
        }
        assert(idle->evaluate(2,sample));const auto at2=sample.surfaces[1].transform;
        assert(at2[0]==.00847576279193163f && at2[1]==.01695152558386326f);
        assert(idle->evaluate(2.5f,sample) && sample.surfaces[1].transform==at2);
        assert(idle->evaluate(119,sample) && sample.selectedTime==1 && sample.surfaces[1].transform[0]==0);
        for(float frame=0;frame<=15;frame+=.25f)assert(hit->evaluate(frame,sample) && sample.vertices.size()<12000);
        std::ifstream file("romfs/field-saveblock.pmanim",std::ios::binary);
        const std::vector<uint8_t> bytes{std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
        const size_t surfaceOffset=12+76+idle->positions.size()*12+idle->normals.size()*12+idle->nodes.size()*4+idle->visibility.size()+
            idle->groups.size()*20+idle->bindings.size()*20+idle->textures.size()*516;
        for(auto fault:std::array<std::pair<size_t,uint32_t>,3>{{{surfaceOffset,0x7fc00000},{surfaceOffset+20,3},{surfaceOffset+idle->surfaces.size()*28+idle->colors.size()*4,4}}}) {
            auto bad=bytes;std::memcpy(bad.data()+fault.first,&fault.second,4);FILE* f=std::tmpfile();assert(f);
            assert(std::fwrite(bad.data(),1,bad.size(),f)==bad.size());std::rewind(f);assert(!pack.read(f));std::fclose(f);
            assert(pack.pose("S_1")==idle && pack.pose("A_1")==hit);
        }
        std::cout<<"Original save-block colored repeat surfaces, discrete scroll, source passes and nonzero start passed; peak sampled vertices="<<peak<<"\n";
    }
    std::cout<<"clamp/repeat/mirror surface interpolation and colors preserved across atlas partitions\n";
}
