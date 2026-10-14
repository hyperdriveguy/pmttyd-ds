#include "map_material_color.hpp"
#include "native_shadow_program.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Trace {
    MapMaterialGroup group;CameraState first,second;const CameraState* current=&first;
    bool groupKnown=true,hold=false;std::vector<uint32_t> calls;std::vector<ShadowGXRequest> gx;
    static const MapMaterialGroup* get(void* u){auto& t=*static_cast<Trace*>(u);t.calls.push_back(1);return t.groupKnown?&t.group:nullptr;}
    static const CameraState* camera(void* u){auto& t=*static_cast<Trace*>(u);t.calls.push_back(0x8000f2e4);return t.current;}
    static bool output(void* u,const ShadowGXRequest& r){auto& t=*static_cast<Trace*>(u);t.calls.push_back(uint32_t(r.kind));t.gx.push_back(r);return !t.hold;}
};
int main(){
    MapMaterialGroup g;
    assert(mapMaterialLastStageColor(0,0,0xff000000,g)==0xffffffff);
    assert(mapMaterialLastStageColor(0,0,0x800000ff,g)==0xffffff80); // RED, not alpha.
    assert(mapMaterialLastStageColor(0x40,0x12345678,0xff000000,g)==0x12345678);
    // Exhaustive comparison with the recovered multiply-high expression,
    // rather than a second /255 implementation. h uses defined floor shift.
    for(uint32_t a=0;a<256;++a)for(uint32_t b=0;b<256;++b){
        int64_t v=int64_t(-2139062143)*int64_t(a*b);
        int64_t h=v>=0?v/4294967296LL:-((-v+4294967295LL)/4294967296LL);
        int64_t sum=h+a*b;int64_t q=sum>=0?sum/128:-((-sum+127)/128);
        q+=uint32_t(q)>>31;
        assert(uint8_t(mapMaterialLastStageColor(0x40,a, b<<24,g))==uint8_t(q));
    }
    g.flags=14;g.colors={0x80808080,0x80808080,0x80808080};
    assert(mapMaterialLastStageColor(0,0,0xff000000,g)==0x20202020);
    assert(mapMaterialLastStageColor(0x800,0,0xff000000,g)==0x80808080);
    assert(mapMaterialLastStageColor(0x1000,0,0xff000000,g)==0x40404040);
    assert(mapMaterialLastStageColor(0x1800,0,0xff000000,g)==0xffffffff);
    g.flags=2;g.colors[0]=0xfefefefe;
    assert(mapMaterialLastStageColor(0x40,0x80808080,0x800000ff,g)==0x7f7f7f3f); // Every stage truncates.
    Trace t;MapMaterialColorServices s{&t,Trace::get,Trace::camera,Trace::output};MapMaterialColorCursor c;
    t.hold=true;assert(!resumeMapMaterialColor(0x80021860,0x40,0x12345678,0x800000ff,s,c));
    assert(c.pending==0x802b8b7c && t.gx.back().color==0x1234563c && t.calls==std::vector<uint32_t>({1,0x802b8b7c}));
    t.group.flags=14;t.group.colors={0,0,0};t.hold=false;
    assert(resumeMapMaterialColor(0x80021860,0,0,0,s,c) && t.gx.back().color==0x1234563c && t.calls.size()==3);
    t={};t.current=&t.first;t.groupKnown=false;assert(!resumeMapMaterialColor(0x80021860,0x40,0x12345678,0xff000000,s,c));
    t.groupKnown=true;assert(resumeMapMaterialColor(0x80021860,0,0,0,s,c) && t.gx.back().color==0x12345678); // Source args retained before getter wait.
    t={};t.current=&t.first;assert(resumeMapMaterialColor(0x800217c8,0,0,0,s,c));
    assert((t.calls==std::vector<uint32_t>({0x8000f2e4,1,0x802b8ff4}) && t.gx.back().values[0]==0 && t.gx.back().color==0xffffffff && t.gx.back().floats==std::array<float,4>{}));
    const int types[]{2,4,5,6,7};
    for(unsigned i=0;i<5;++i){t={};t.current=&t.first;t.group.flags=1;t.group.fogType=i;t.group.fogColor=0x12345678;t.group.fogStart=30;t.group.fogEnd=900;t.first.nearPlane=10;t.first.farPlane=10000;t.hold=true;
        assert(!resumeMapMaterialColor(0x800217c8,0,0,0,s,c));const auto held=t.gx.back();assert(held.values[0]==types[i] && held.color==0x12345678 && held.floats[2]==10 && held.floats[3]==10000);
        t.group={};t.first.nearPlane=999;t.current=&t.second;t.hold=false;assert(resumeMapMaterialColor(0x800217c8,0,0,0,s,c));assert(t.gx.back().floats==held.floats && t.gx.back().color==held.color && t.calls.size()==4);
    }
    t={};t.current=&t.first;t.groupKnown=false;assert(!resumeMapMaterialColor(0x800217c8,0,0,0,s,c));t.groupKnown=true;t.group.flags=1;t.first.nearPlane=42;t.current=&t.second;
    assert(resumeMapMaterialColor(0x800217c8,0,0,0,s,c) && t.gx.back().floats[2]==42 && t.calls.size()==4); // Camera pointer retained, near read after group.
    t={};t.current=&t.first;t.group.flags=1;t.group.fogType=5;assert(!resumeMapMaterialColor(0x800217c8,0,0,0,s,c) && t.gx.empty());c={};
    NativeShadowProgram p;ShadowGXRequest r;r.kind=ShadowGX::TevKColor;r.color=0x12345678;assert(!p.request(r));p.afterDummy();auto n=p.request(r);assert(n && n->tevKColor0==0x12345678 && !p.tevKColor0);n->afterDummy();assert(n->tevKColor0==0x12345678);r.values[0]=1;assert(!n->request(r));
    r={};r.kind=ShadowGX::Fog;r.values[0]=2;assert(!n->request(r)); // Enabled fog needs actual native equation/LUT support.
}
