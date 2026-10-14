#include "npc_map_move.hpp"
#include <cassert>
#include <cmath>
#include <vector>
using namespace ttyd;
struct Probe {FieldWallVector origin,direction;float extent;};
struct Harness {std::vector<FieldWallHit> hits;std::vector<Probe> probes;int directions=0,angles=0;float slope=45;uintptr_t published=0;int publications=0;};
NpcMapMoveServices services(Harness& h) {
    NpcMapMoveServices s;s.user=&h;
    s.direction=[](void* p,float,float){++static_cast<Harness*>(p)->directions;return FieldWallVector{.125f,0,-.875f};};
    s.length=[](void*,float x,float z){return std::sqrt(std::fma(x,x,z*z));};
    s.normalLength=[](void*,float x,float z){return std::sqrt(x*x+z*z);};
    s.angle=[](void* p,float x,float z){auto& h=*static_cast<Harness*>(p);++h.angles;assert(x==1 && z==0);return h.slope;};
    s.ray=[](void* p,FieldWallVector origin,FieldWallVector direction,float length){auto& h=*static_cast<Harness*>(p);const auto i=h.probes.size();if(h.publications)assert(h.published==41);h.probes.push_back({origin,direction,length});return i<h.hits.size()?h.hits[i]:FieldWallHit{};};
    s.publishContact=[](void* p,const FieldWallHit& hit){auto& h=*static_cast<Harness*>(p);assert(h.publications++==0);h.published=hit.token;};
    return s;
}
bool close(float x,float y){return std::fabs(x-y)<.00001f;}
int main() {
    const FieldWallVector root{100,20,-50};Harness h;auto s=services(h);
    auto zero=checkNpcMapSide(root,30,30,0,-0.f,0x80603,{});assert(zero && zero->root.x==100 && zero->root.z==-50 && !zero->contact.token);
    auto miss=checkNpcMapSide(root,30,30,10,8,0x80603,s);
    assert(miss && miss->root.x==110 && miss->root.y==20 && miss->root.z==-42 && !miss->contact.token);
    assert(h.directions==1 && h.angles==0 && h.probes.size()==3);
    for(unsigned i=0;i<3;++i){assert(h.probes[i].origin.x==100 && h.probes[i].origin.z==-50);assert(h.probes[i].direction.x==.125f && h.probes[i].direction.z==-.875f);assert(close(h.probes[i].extent,std::fma(.75f,30.f,std::sqrt(164.f))));}
    assert(close(h.probes[0].origin.y,26) && close(h.probes[1].origin.y,32) && close(h.probes[2].origin.y,42.5f));
    h={};s=services(h);h.hits={{17,.001f,{1,0,0}}};
    auto hit=checkNpcMapSide(root,30,100,10,8,0x80603,s);
    assert(hit && hit->contact.token==17 && hit->root.x==100 && hit->root.z==-46 && h.probes.size()==1 && h.probes[0].origin.y==30);
    for(float slope:{44.999f,45.f,45.001f}) {
        h={};s=services(h);h.slope=slope;h.hits={{},{29,999,{1,0,0}},{}};
        auto middle=checkNpcMapSide(root,30,30,10,8,0x80603,s);assert(middle && h.angles==1);
        if(slope<45){assert(h.probes.size()==3 && !middle->contact.token && middle->root.x==110 && middle->root.z==-42);}
        else {assert(h.probes.size()==2 && middle->contact.token==29 && middle->root.x==100 && middle->root.z==-46);}
    }
    h={};s=services(h);h.hits={{},{},{31,0,{1,0,0}}};
    auto third=checkNpcMapSide(root,30,30,10,8,0x80603,s);assert(third && third->contact.token==31 && h.probes.size()==3);
    for(bool secondHit:{false,true}) {
        h={};s=services(h);h.hits={{41,0,{1,0,0}},secondHit?FieldWallHit{99,0,{1,0,0}}:FieldWallHit{}};
        auto moved=advanceNpcMapSide(root,30,30,10,8,0x80603,s);
        assert(moved && moved->contact.token==41 && moved->root.x==100 && moved->root.z==(secondHit?-48:-46) && h.published==41 && h.publications==1);
        assert(h.directions==2 && h.probes.size()==(secondHit?2u:4u));
        assert(h.probes[1].origin.x==root.x && h.probes[1].origin.z==root.z && h.probes[1].extent==26.5f);
    }
    h={};s=services(h);h.hits={{44,0,{1,0,0}}};
    auto stopped=advanceNpcMapSide(root,30,30,10,0,0x80603,s);assert(stopped && stopped->contact.token==44 && h.probes.size()==1 && h.directions==1);
    h={};s=services(h);assert(!checkNpcMapSide(root,30,30,1,0,0x82003,s));assert(!checkNpcMapSide(root,30,30,1,0,0x90603,s));assert(h.probes.empty());
    s.ray=nullptr;assert(!checkNpcMapSide(root,30,30,1,0,0x80603,s));
}
