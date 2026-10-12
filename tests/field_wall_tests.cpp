#include "field_wall.hpp"
#include "field_ray.hpp"
#include "game.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace ttyd;
#define CHECK(x) do {if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(0)
bool close(float a,float b,float tolerance=.00002f){return std::fabs(a-b)<tolerance;}
struct Fixture {
    struct Query {FieldWallVector origin,direction;float length;};
    std::vector<FieldWallHit> hits;
    std::vector<Query> calls;
    FieldWallServices services() {
        return {this,[](void* user,FieldWallVector p,FieldWallVector d,float length) {
            auto& f=*static_cast<Fixture*>(user);const size_t n=f.calls.size();f.calls.push_back({p,d,length});
            return n<f.hits.size() && f.hits[n].token?f.hits[n]:FieldWallHit{0,length,{}};
        }};
    }
};
FieldWallHit hit(uintptr_t token,float distance,FieldWallVector normal={1,0,0},uint32_t attributes=0) {
    return {token,distance,normal,attributes};
}
int main(int argc,char**) {
    if(argc>1) {
        struct Triangle {std::array<float,9> vertices;GroundContact owner;uint32_t cull;};
        static_assert(sizeof(Triangle)==48,"Original converted triangle record");
        int checked=0;
        for(const char* name:{"map","gor_00","gor_02","gor_03","gor_04"}) {
            const std::string path=std::string("romfs/")+name+".head";
            std::ifstream in(path,std::ios::binary);char magic[4];uint32_t count=0;
            in.read(magic,4);in.read(reinterpret_cast<char*>(&count),4);CHECK(in && count>0);
            std::vector<Triangle> triangles(count);in.read(reinterpret_cast<char*>(triangles.data()),count*48);
            CHECK(in && loadHead(path.c_str()));
            for(int n=0;n<160;++n) {
                const auto& v=triangles[(n*1229+13)%count].vertices;
                const float angle=n*.37f;FieldWallVector direction{std::cos(angle),0,std::sin(angle)};
                FieldWallVector start{(v[0]+v[3]+v[6])/3-direction.x*.15f,
                    (v[1]+v[4]+v[7])/3+(n%3)*.001f,(v[2]+v[5]+v[8])/3-direction.z*.15f};
                FieldWallVector source{start.x*100,start.y*100,start.z*100};
                // Use the native adapter's scale/rounding for the reference scan.
                start={source.x*.01f,source.y*.01f,source.z*.01f};
                const float maximum=40.f*.01f;FieldWallHit expected;expected.distance=maximum;
                for(size_t row=0;row<triangles.size();++row) {
                    const auto& tri=triangles[row];CHECK(!(tri.owner.attributes&0x80000000));
                    FieldWallHit h;
                    if(ordinaryWallAttributes(tri.owner.attributes) &&
                       intersectFieldRay(tri.vertices,tri.cull,start,direction,maximum,h) &&
                       (!expected.token || h.distance<expected.distance)) {
                        h.token=row+1;h.attributes=tri.owner.attributes;expected=h;
                    }
                }
                const auto actual=fieldWallRay(source,direction,40);
                CHECK(actual.token==expected.token && actual.attributes==expected.attributes);
                CHECK(close(actual.distance,expected.distance*100,.0001f));++checked;
            }
        }
        clearCollision();std::cout<<checked<<" original-mesh directional rays matched full scan\n";return 0;
    }
    CHECK(close(quantizeFieldWall(1.0004f),1));CHECK(close(quantizeFieldWall(-1.0006f),-1.001f));
    CHECK(close(quantizeFieldWall(.0005f),.001f));CHECK(close(quantizeFieldWall(-.0005f),-.001f));
    FieldWallActor actor;
    for(uint32_t attributes:{0u,0x80u,0x80000u,0x880000u,0x800080u})CHECK(!fieldWallPassThrough(attributes,actor));
    CHECK(fieldWallPassThrough(0x800000,actor));
    actor.flags=0x1000000;CHECK(fieldWallPassThrough(0x880000,actor));
    actor.motion=25;CHECK(fieldWallPassThrough(0x80,actor));actor={};
    {
        Fixture f;auto move=searchFieldFrontWall({3,7,9},{2,0,1},actor,f.services());
        CHECK(close(move.position.x,5) && close(move.position.z,10) && !move.forward.token && !move.distanceWritten);
        CHECK(f.calls.size()==4);CHECK(close(f.calls[0].origin.y,33.6400013f) && f.calls[1].origin.y==18);
        CHECK(close(f.calls[0].length,10+std::sqrt(5.f)) && f.calls[2].length==10);
        // Source heading sin(H),-cos(H): H+45 rotates right travel toward +Z.
        CHECK(f.calls[2].direction.z>f.calls[0].direction.z);
        Fixture airborne;actor.counter=1;searchFieldFrontWall({}, {1,0,0},actor,airborne.services());
        CHECK(airborne.calls.size()==6 && close(airborne.calls[2].origin.y,.1f));actor.counter=0;
    }
    {
        Fixture f;f.hits={hit(1,4,{1,0,0}),hit(2,3,{.6f,0,.8f})};
        auto move=searchFieldFrontWall({}, {2,0,1},actor,f.services());
        CHECK(move.forward.token==2 && close(move.position.x,.4f) && close(move.position.z,-.3f));
        CHECK(move.distanceWritten && move.contactDistance==10); // Last failed side query retains radius.
        Fixture tie;tie.hits={hit(1,1.0004f,{1,0,0}),hit(2,1.0003f,{0,0,1})};
        auto same=searchFieldFrontWall({}, {1,0,1},actor,tie.services());CHECK(same.forward.token==1);
        CHECK(close(same.position.x,0) && close(same.position.z,.5f));
    }
    for(bool finalHit:{false,true}) {
        Fixture f;f.hits={hit(1,5),{},hit(3,2),hit(4,3),finalHit?hit(5,3):FieldWallHit{}};
        auto move=searchFieldFrontWall({}, {1,0,1},actor,f.services());
        CHECK(f.calls.size()==5 && f.calls[2].direction.z>0 && f.calls[3].direction.z<.01f);
        CHECK(move.sideBlocked==finalHit && move.contactDistance==(finalHit?0:10));
        CHECK(close(move.position.z,finalHit?0:.5f));
        f.calls.clear();f.hits[0]={};move=searchFieldFrontWall({}, {1,0,1},actor,f.services());
        CHECK(close(move.position.x,finalHit?0:1) && close(move.position.z,finalHit?0:1));
        CHECK(!move.distanceWritten);
    }
    {
        Fixture f;actor.counter=2;f.hits={hit(1,4),{}, {},hit(2,4),hit(3,4),hit(4,4),{}};
        auto move=searchFieldFrontWall({}, {1,0,1},actor,f.services());
        CHECK(f.calls.size()==7 && !move.sideBlocked && close(move.position.z,.5f));actor.counter=0;
    }
    {
        Fixture f;FieldWallVector position{};
        CHECK(!correctFieldWallsAround(position,{1,0,0},0,actor,f.services()));
        CHECK(f.calls.size()==16);CHECK(f.calls[0].origin.y==11 && f.calls[1].origin.y==36);
        CHECK(f.calls[2].direction.x>0 && f.calls[2].direction.z>0);
        Fixture two;actor.viewFlags=1;
        CHECK(!correctFieldWallsAround(position,{1,0,0},0,actor,two.services()));CHECK(two.calls.size()==4);
        actor.viewFlags=0;
    }
    for(int mode:{0,1})for(uint16_t motion:{uint16_t(1),uint16_t(10),uint16_t(11)}) {
        Fixture f;f.hits={hit(1,5),hit(2,2)};actor.motion=motion;
        FieldWallVector position{};CHECK(correctFieldWallsAround(position,{1,0,0},mode,actor,f.services()));
        const float cap=mode==0 && (motion==10 || motion==11)?5:2.5f;
        CHECK(close(position.x,-cap));CHECK(f.calls.size()==32 && close(f.calls[16].origin.x,-cap));
    }
    actor={};
    {
        Fixture f;f.hits.resize(32);f.hits[0]=hit(1,5);f.hits[16]=hit(2,1);
        FieldWallVector p{};CHECK(correctFieldWallsAround(p,{1,0,0},0,actor,f.services()));
        CHECK(close(p.x,-7)); // First cap2.5; second uncapped9, multiplied .5.
        Fixture reversed;reversed.hits.resize(32);reversed.hits[0]=hit(1,5);reversed.hits[24]=hit(2,1);
        p={};CHECK(correctFieldWallsAround(p,{1,0,0},0,actor,reversed.services()));CHECK(close(p.x,-2.5f));
        Fixture pass;pass.hits={hit(1,0,{},0x800000)};p={};
        CHECK(!correctFieldWallsAround(p,{1,0,0},0,actor,pass.services()) && p.x==0 && pass.calls.size()==16);
    }
    {
        std::array<float,9> triangle{0,-1,-1,0,1,-1,0,-1,1};FieldWallHit result;
        CHECK(intersectFieldRay(triangle,1,{1,0,0},{-1,0,0},1,result));CHECK(result.distance==1 && result.normal.x==1);
        CHECK(!intersectFieldRay(triangle,1,{-1,0,0},{1,0,0},2,result));
        CHECK(intersectFieldRay(triangle,3,{-1,0,0},{1,0,0},2,result));
        CHECK(result.normal.x==-1); // Double-sided back hit faces ray origin.
        CHECK(!intersectFieldRay(triangle,3,{1,0,0},{0,0,1},2,result));
        CHECK(!intersectFieldRay(triangle,3,{1,0,0},{-1,0,0},.99f,result));
        triangle={-1,1,-1,1,-1,-1,-1,1,1};
        CHECK(intersectFieldRay(triangle,3,{-1,0,0},{1,0,0},2,result));
        CHECK(close(result.distance,1) && std::fabs(result.normal.y)>.7f); // Not a legacy near-vertical wall.
    }
    // A full source-winding mesh drives native player movement even when the
    // legacy wall pack contains no selected near-vertical triangles.
    clearCollision();char path[]="/tmp/ttyd-wall-rays-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);close(fd);
    {std::ofstream out(path,std::ios::binary);uint32_t count=0;out.write(reinterpret_cast<char*>(&count),4);}
    CHECK(loadCollision(path));
    {std::ofstream out(path,std::ios::binary);uint32_t count=2,cull=3;GroundContact owner{0,777};
        out.write("HEAD",4);out.write(reinterpret_cast<char*>(&count),4);
        for(std::array<float,9> tri:{std::array<float,9>{-.9f,0,1,-.9f,1,1,-.9f,0,3},std::array<float,9>{-.9f,1,1,-.9f,1,3,-.9f,0,3}}) {
            out.write(reinterpret_cast<char*>(tri.data()),36);out.write(reinterpret_cast<char*>(&owner),8);out.write(reinterpret_cast<char*>(&cull),4);
        }}
    CHECK(loadHead(path));auto ray=fieldWallRay({-100,11,200},{1,0,0},12.25f);
    CHECK(ray.token && close(ray.distance,10) && ray.attributes==0);
    Progress initial;initial.room=2;initial.position={-1,2};Game g;g.restore(initial);Input right;right.x=1;
    g.tick(right);CHECK(close(g.progress.position.x,-1) && g.wallContact.sourceNode==777);
    CHECK(close(g.wallContactDistance,.1f));
    // Sideways travel slides along a whole vector, while head-on travel blocks.
    Input diagonal;diagonal.x=diagonal.z=1;const float before=g.progress.position.z;g.tick(diagonal);
    CHECK(g.progress.position.z>before && close(g.progress.position.x,-1,.001f));
    const auto standing=g.progress.position;g.tick({});CHECK(g.progress.position.x==standing.x && g.progress.position.z==standing.z);
    clearCollision();std::remove(path);
    std::cout<<"Original wall policy and native directional rays passed\n";
}
