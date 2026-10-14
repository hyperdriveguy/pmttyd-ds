#include "map_material_tev.hpp"
#include "native_shadow_program.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace ttyd;
struct Trace {
    std::vector<ShadowGXRequest> requests;std::vector<uint32_t> branches;
    uint32_t hold=0;bool branchHold=false,native=false;NativeShadowProgram program;
    static bool gx(void* u,const ShadowGXRequest& r){auto& t=*static_cast<Trace*>(u);if(uint32_t(r.kind)==t.hold)return false;
        if(t.native){auto next=t.program.request(r);if(!next)return false;t.program=*next;}
        t.requests.push_back(r);return true;}
    static bool branch(void* u,uint32_t entry,const MapTevInput& in,MapTevCounters& n,uint32_t& pending){auto& t=*static_cast<Trace*>(u);t.branches.push_back(entry);
        assert(in.model[0]==42);if(t.branchHold){pending=0x12345678;return false;}
        if(entry==0x800248a8){assert(n.indirect==0);n.stages=0x102;n.generators=0x101;n.indirect=0x100;n.maps=999;}
        else if(entry==0x800229fc || entry==0x80022b00)n.stages=3;
        return true;}
};
int main(int argc,char** argv){
    MapTevInput input;input.slots=1;input.mode=0;input.flags=0x40;input.model[0]=42;
    Trace t;MapTevServices s{&t,Trace::gx,Trace::branch};MapTevCursor c;
    if(argc==3){input.mode=uint32_t(std::atoi(argv[2]));input.flags=0x10000000;assert(resumeMapMaterialTev(input,s,c));
        std::printf("[");bool first=true;for(const auto& r:t.requests){if(!first)std::printf(",");first=false;std::printf("{\"function\":%u,\"arguments\":[",unsigned(r.kind));for(unsigned j=0;j<7;++j)std::printf("%s%d",j?",":"",int(r.values[j]));std::printf("]}");}std::printf("]\n");return 0;}
    assert(resumeMapMaterialTev(input,s,c) && t.requests.size()==14 && c.counts.stages==2);
    assert(t.requests[0].kind==ShadowGX::TevOrder && t.requests[1].kind==ShadowGX::TevOp && t.requests[2].kind==ShadowGX::TevSwapMode);
    assert(t.requests[3].kind==ShadowGX::TevKColorSel && t.requests[3].values[1]==12 && t.requests[4].values[1]==28);
    assert(t.requests[11].kind==ShadowGX::NumTexGens && t.requests[12].kind==ShadowGX::NumTevStages && t.requests[13].kind==ShadowGX::NumIndStages);
    t={};t.hold=uint32_t(ShadowGX::TevAlphaIn);c={};assert(!resumeMapMaterialTev(input,s,c) && t.requests.size()==9 && c.pending==t.hold);const auto held=c.request;
    input.flags=0xffffffff;input.slots=99;input.mode=9;t.hold=0;assert(resumeMapMaterialTev(input,s,c) && t.requests.size()==14 && t.requests[9].values==held.values && t.branches.empty());
    input={};input.model[0]=42;input.slots=-1;input.mode=8;input.flags=0x10000000;t={};c={};assert(resumeMapMaterialTev(input,s,c) && t.branches.empty() && t.requests.size()==9 && t.requests[6].values[0]==255);
    input.slots=257;input.mode=0;t={};c={};assert(resumeMapMaterialTev(input,s,c) && t.requests[3].values[0]==1 && c.counts.maps==257);
    input.slots=1;input.mode=10;t={};c={};assert((resumeMapMaterialTev(input,s,c) && t.requests[3].values==std::array<int32_t,7>({0,15,10,8,15,0,0})));
    // Every optional boundary is ordered after prior completed work. Callback
    // publication reloads K/C/I only, and truncates each final count to low8.
    input.mode=0;input.flags=4|0x400|0x10000|0x20000|0x40000|0x80100000|0x800000|0x01000000;t={};c={};t.branchHold=true;
    assert(!resumeMapMaterialTev(input,s,c) && c.pending==0x12345678 && t.requests.size()==11);t.branchHold=false;
    assert(resumeMapMaterialTev(input,s,c));const std::vector<uint32_t> expected{0x8002bc7c,0x8002bc7c,0x80023128,0x80023314,0x80023d58,0x80023e9c,0x800244ac,0x80024654,0x800248a8};assert(t.branches==expected);
    assert(c.counts.maps==1 && c.counts.stages==0x102 && t.requests[11].values[0]==1 && t.requests[12].values[0]==2 && t.requests[13].values[0]==0);
    for(unsigned mode:{8,9}){t={};c={};input.mode=mode;input.flags=0;assert(resumeMapMaterialTev(input,s,c) && t.branches.size()==1 && c.counts.stages==4);}
    input.mode=0;input.flags=4;t={};c={};MapTevServices missing{&t,Trace::gx};assert(!resumeMapMaterialTev(input,missing,c) && c.pending==0x8002bc7c);
    // Actual native state admission needs a bound image, known K0 and the
    // completed second-stage setters; dummy/legacy count1 disables that stage.
    t={};c={};input.flags=0x40;t.native=true;t.program.afterDummy();t.program.texture=ShadowTexture::CharacterAtlas;t.program.materialVertex=t.program.alphaMaterialVertex=true;t.program.tevKColor0=0x12345678;
    assert(resumeMapMaterialTev(input,s,c) && t.program.tevStages==2 && t.program.constantStageMask==255 && t.program.alpha==NativeShadowProgram::TextureRaster && t.program.indirectStages==0 && t.program.textureRGB);
    auto old=t.program;ShadowGXRequest q;q.kind=ShadowGX::TevKColor;q.color=0x87654321;auto next=old.request(q);assert(next && next->tevKColor0==0x87654321 && next->tevStages==2);next->afterDummy();assert(next->tevStages==1 && next->tevKColor0==0x87654321 && next->constantStageMask==255);
    q={};q.kind=ShadowGX::NumTevStages;q.values[0]=2;NativeShadowProgram unknown;unknown.afterDummy();assert(!unknown.request(q));q.kind=ShadowGX::NumIndStages;q.values[0]=1;assert(!old.request(q));
    t={};t.program.afterDummy();t.program.materialVertex=t.program.alphaMaterialVertex=true;t.program.tevKColor0=0xffffffff;t.native=true;c={};input.slots=0;assert(resumeMapMaterialTev(input,s,c) && t.program.alpha==NativeShadowProgram::Raster && t.program.tevStages==2);
}
