#include "actor_animation.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstring>
using namespace ttyd;
namespace {
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
    for(uint32_t s=0;s<3;++s)for(uint32_t t=0;t<3;++t)surfaceCases(s,t);
    const auto translated=actorTexturePoint({.25f,.5f,2,3,0},.2f,.4f);
    assert(std::fabs(translated[0]-.65f)<.000001f && std::fabs(translated[1]+1.3f)<.000001f);
    const auto zeroScale=actorTexturePoint({0,0,1,0,0},.2f,.4f);assert(zeroScale[0]==.2f && zeroScale[1]==0);
    const auto rotated=actorTexturePoint({0,0,1,1,90},.5f,.6f);assert(std::fabs(rotated[0]-.6f)<.000001f && std::fabs(rotated[1]-.5f)<.000001f);
    if(argc>1) {
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
        for(auto fault:std::array<std::pair<size_t,uint32_t>,3>{{{surfaceOffset,0x7fc00000},{surfaceOffset+20,3},{surfaceOffset+idle->surfaces.size()*28+idle->colors.size()*4,1}}}) {
            auto bad=bytes;std::memcpy(bad.data()+fault.first,&fault.second,4);FILE* f=std::tmpfile();assert(f);
            assert(std::fwrite(bad.data(),1,bad.size(),f)==bad.size());std::rewind(f);assert(!pack.read(f));std::fclose(f);
            assert(pack.pose("S_1")==idle && pack.pose("A_1")==hit);
        }
        std::cout<<"Original save-block colored repeat surfaces, discrete scroll, source passes and nonzero start passed; peak sampled vertices="<<peak<<"\n";
    }
    std::cout<<"clamp/repeat/mirror surface interpolation and colors preserved across atlas partitions\n";
}
