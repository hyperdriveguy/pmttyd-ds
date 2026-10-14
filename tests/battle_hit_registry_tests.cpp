#include "battle_hit_registry.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>
using namespace ttyd;
using Triangle=std::array<std::array<float,3>,3>;
struct Object {uint32_t parts=1,attributes=0;uint16_t flags=0,cull=1;std::array<float,3> center{};float radius=100;std::vector<Triangle> triangles;};
template<class T> void write(FILE* f,const T& v){assert(std::fwrite(&v,sizeof(v),1,f)==1);}
void name(FILE* f,const char* text){char bytes[64]{};std::strncpy(bytes,text,63);assert(std::fwrite(bytes,64,1,f)==1);}
void fixture(const char* path,const std::vector<Object>& objects){
    FILE* f=std::fopen(path,"wb");assert(f);assert(std::fwrite("PMBHIT01",8,1,f)==1);write(f,uint32_t(1));name(f,"test-map");name(f,"test-root");write(f,uint32_t(objects.size()));
    for(unsigned i=0;i<objects.size();++i){const auto& o=objects[i];char text[32];std::snprintf(text,sizeof(text),"node%u",i);name(f,text);write(f,uint32_t(i+1));write(f,o.parts);write(f,o.flags);write(f,o.cull);write(f,o.attributes);write(f,o.center);write(f,o.radius);write(f,uint32_t(o.triangles.size()));for(const auto& t:o.triangles)write(f,t);}
    std::fclose(f);
}
int main(){
    const char* path="artifacts/battle-hit-test.bin";
    // Original collision winding yields N.y>0 for this floor triangle.
    const Triangle floor{{{-10,0,-10},{0,0,10},{10,0,-10}}};
    Object o;o.triangles={floor};fixture(path,{o,o});BattleHitRegistry registry;
    assert(registry.load(path) && registry.known() && registry.count()==2);
    ShadowCollisionRay ray;ray.origin={0,11,0};ShadowCollisionHit hit{{71,72,73},{74,75,76}};BattleHitWinner winner;
    assert(registry.ray(ray,hit,&winner)==ShadowRayResult::Hit && winner.object==0 && winner.triangle==0 && winner.distance==11);
    assert((hit.point==std::array<float,3>{0,0,0}) && std::fabs(hit.normal[1]-1)<1e-6f);
    // Stable object/triangle ties; runtime flags change published admission only
    // on the explicit hitMain-style chain publication, not an implicit ray scan.
    assert(registry.flags(0,1));assert(registry.ray(ray,hit,&winner)==ShadowRayResult::Hit && winner.object==0);
    registry.publish();assert(registry.ray(ray,hit,&winner)==ShadowRayResult::Hit && winner.object==1);
    assert(registry.flags(1,0x80));registry.publish();
    hit.point={71,72,73};assert(registry.ray(ray,hit)==ShadowRayResult::Miss && hit.point[0]==71);
    assert(registry.flags(0,0));registry.publish();
    ray.length=11;assert(registry.ray(ray,hit)==ShadowRayResult::Miss); // Strict far endpoint.
    ray.length=12;assert(registry.ray(ray,hit)==ShadowRayResult::Hit);
    ray.origin={0,0,0};assert(registry.ray(ray,hit)==ShadowRayResult::Hit && hit.point[1]==0); // One-sided plane equality.
    o.cull=0;fixture(path,{o});assert(registry.load(path));
    assert(registry.ray(ray,hit)==ShadowRayResult::Miss); // Two-sided Q0 rejection.
    auto reversed=floor;std::swap(reversed[1],reversed[2]);o.triangles={reversed};fixture(path,{o});assert(registry.load(path));
    ray.origin={0,11,0};assert(registry.ray(ray,hit)==ShadowRayResult::Hit && hit.normal[1]>0); // Raw negative normal is oriented before normalize.
    o.cull=1;fixture(path,{o});assert(registry.load(path));assert(registry.ray(ray,hit)==ShadowRayResult::Miss);
    o.triangles={floor};o.attributes=4;fixture(path,{o});assert(registry.load(path));assert(registry.ray(ray,hit)==ShadowRayResult::Hit); // No Mario attr4 filter.
    o.attributes=0x80000000;fixture(path,{o});assert(registry.load(path));
    assert(registry.ray(ray,hit)==ShadowRayResult::Unavailable);
    assert(registry.mobjFlags(0,0x800));assert(registry.ray(ray,hit)==ShadowRayResult::Miss);
    assert(registry.mobjFlags(0,0));assert(registry.ray(ray,hit)==ShadowRayResult::Hit);
    // Source sphere gate uses original midpoint/length, not triangle bounds.
    ray.length=150;o.attributes=0;o.center={0,10000,0};o.radius=0;fixture(path,{o});assert(registry.load(path));
    assert(registry.ray(ray,hit)==ShadowRayResult::Miss);
    o.center={0,-64,0};fixture(path,{o});assert(registry.load(path));assert(registry.ray(ray,hit)==ShadowRayResult::Hit);
    // Hierarchy-only slots never enter the published geometry chain.
    Object parent;parent.parts=0;fixture(path,{parent});assert(registry.load(path));assert(registry.ray(ray,hit)==ShadowRayResult::Miss);
    registry.invalidate();assert(registry.ray(ray,hit)==ShadowRayResult::Unavailable);
    assert(registry.load(path));ray.direction={0,1,0};assert(registry.ray(ray,hit)==ShadowRayResult::Unavailable);
    ray.direction={0,-1,0};ray.origin[0]=std::numeric_limits<float>::quiet_NaN();assert(registry.ray(ray,hit)==ShadowRayResult::Unavailable);
    assert(!registry.load("/missing-hit-pack") && registry.known());
    FILE* f=std::fopen(path,"ab");assert(f);write(f,uint8_t(1));std::fclose(f);assert(!registry.load(path) && registry.known());
    std::remove(path);
    // Actual read-only-ISO imported registry, source map/slot order and each
    // independent finishing query; never reads field globals or B.floor.
    assert(registry.load("romfs/battle-hit.bin") && registry.count()==5);
    assert(registry.object(1)->name=="A_bti_1_00" && registry.object(1)->map==0 && registry.object(1)->triangles.size()==4);
    assert(registry.object(4)->map==1 && registry.object(4)->triangles.size()==160);
    ray.origin={-100,11,-5};ray.length=150;
    assert(registry.ray(ray,hit,&winner)==ShadowRayResult::Hit && winner.object==1 && winner.triangle==1 && winner.distance==11);
    assert((hit.point==std::array<float,3>{-100,0,-5}) && std::fabs(hit.normal[1]-1)<1e-6f);
    for(auto p:{std::array<float,3>{-140,11,5},std::array<float,3>{-60,11,-30}}){ray.origin=p;assert(registry.ray(ray,hit,&winner)==ShadowRayResult::Hit && winner.object==1 && hit.point[1]==0);}
    assert(registry.flags(1,1));registry.publish();ray.origin={0,11,0};
    assert(registry.ray(ray,hit,&winner)==ShadowRayResult::Hit && winner.object==4 && hit.point[1]==-20); // Genuine secondary map winner.
}
