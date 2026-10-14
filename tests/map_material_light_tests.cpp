#include "map_material_light.hpp"
#include "native_shadow_program.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
static void flags(MapLightRecord& r,uint16_t f){r.bytes[0]=uint8_t(f>>8);r.bytes[1]=uint8_t(f);}
struct Trace {
    MapMaterialLightWork* work;std::vector<uint32_t> queries;std::vector<ShadowGXRequest> gx;
    bool hold=false;MapLightRecord preset;
    static bool query(void* u,MapLightQuery& q,uint32_t& pending){auto& t=*static_cast<Trace*>(u);t.queries.push_back(q.function);
        if(q.function==0x8001b944 || q.function==0x8001b938){q.record=&t.preset;return true;}return mapLightBankQuery(*t.work,q,pending);}
    static bool output(void* u,const ShadowGXRequest& r){auto& t=*static_cast<Trace*>(u);if(t.hold && r.kind==ShadowGX::ChanCtrl && r.values[0]==2)return false;t.gx.push_back(r);return true;}
};
int main(){
    MapMaterialLightWork w;assert(w.banks[1].count==32 && w.banks[1].known && !w.banks[0].known);
    assert(w.loadBattle("romfs/material-light.bin") && w.selector==1 && !w.battleFog[0].enabled && w.battleFog[1].enabled && w.battleFog[1].start==300 && w.paperAmbient==0x707070ff);
    Trace t{&w};MapMaterialLightServices s;s.user=&t;s.query=Trace::query;s.gx=Trace::output;MapMaterialLightCursor c;
    t.hold=true;assert(!resumeMapMaterialLight(w,0,{1,2,3},s,c) && c.pending==0x802b6f78);
    assert(t.queries==std::vector<uint32_t>{0x8001b9f0} && t.gx.size()==4);
    assert(t.gx[0].kind==ShadowGX::Fog && t.gx[0].color==0xffffffff && t.gx[1].kind==ShadowGX::NumChans);
    const std::array<int32_t,7> rgb{0,0,0,1,0,2,2},alpha{2,0,0,1,0,2,2};
    assert(t.gx[2].kind==ShadowGX::ChanAmbient && t.gx[2].color==0xff && t.gx[3].values==rgb);
    t.hold=false;assert(resumeMapMaterialLight(w,4,{99,99,99},s,c) && t.queries.size()==1 && t.gx.size()==5 && t.gx.back().values==alpha);
    t.queries.clear();t.gx.clear();assert(resumeMapMaterialLight(w,4,{},s,c));assert(t.gx.size()==6 && t.gx[3].kind==ShadowGX::ChanMaterial && t.gx[3].color==0xffffffff && t.gx[4].values[3]==0 && t.gx[5].values[3]==0);
    // Actual bank scan: active/character ambient is source-first, excludes
    // ambient from near results; <=8 active remains array order, not distance.
    flags(w.banks[1].records[1],0x81);flags(w.banks[1].records[2],0x8081);flags(w.banks[1].records[3],1);flags(w.banks[1].records[4],0x81);
    w.banks[1].records[2].bytes[0x48]=0x12;w.banks[1].records[2].bytes[0x4b]=0x34;
    MapLightQuery q;q.function=0x8001bad0;q.maximum=8;q.mask=0x80;uint32_t pending=0;
    assert(mapLightBankQuery(w,q,pending) && q.count==2 && q.output[0]==&w.banks[1].records[1] && q.output[1]==&w.banks[1].records[4] && !q.output[2]);
    q.maximum=0;assert(mapLightBankQuery(w,q,pending) && q.count==1);q.maximum=-3;assert(mapLightBankQuery(w,q,pending) && q.count==1);
    q.function=0x8001b9f0;assert(mapLightBankQuery(w,q,pending) && q.record==&w.banks[1].records[2]);
    t.queries.clear();t.gx.clear();assert(resumeMapMaterialLight(w,2,{},s,c) && c.count==2 && c.mask==3 && c.ambient==0x12000034);
    assert(t.queries==std::vector<uint32_t>({0x8001b984,0x8001bad0,0x8001b9f0}));
    // A map load never erases the light owner. Explicit reInit preserves count.
    assert(w.loadBattle("romfs/material-light.bin") && w.banks[1].records[1].flags()==0x81);w.resetSelected();assert(w.banks[1].count==32 && !w.banks[1].records[1].flags());
    t.queries.clear();t.gx.clear();assert(resumeMapMaterialLight(w,1,{},s,c) && c.count==1 && c.mask==1 && c.ambient==0x707070ff);
    t.queries.clear();assert(resumeMapMaterialLight(w,9,{},s,c) && c.count==0 && t.queries==std::vector<uint32_t>({0x8001b944,0x8001b938,0x8001bad0,0x8001b9f0}));
    // A real typed light needs its own SDK provider; no silent unlit fallback.
    flags(w.banks[1].records[0],0x1081);assert(!resumeMapMaterialLight(w,2,{},s,c) && c.pending==0x800208c0 && c.index==0);
    c={};for(unsigned i=0;i<9;++i)flags(w.banks[1].records[i],0x81);q={};q.function=0x8001bad0;q.maximum=8;assert(!mapLightBankQuery(w,q,pending) && pending==0x800064fc);
    w.banks[1].count=2;flags(w.banks[1].records[3],0x81);w.resetSelected();assert(w.banks[1].count==2 && w.banks[1].records[3].flags()==0x81);
    NativeShadowProgram program;program.afterDummy();ShadowGXRequest r;r.kind=ShadowGX::ChanCtrl;r.values={4,0,0,0,0,2,2};auto state=program.request(r);assert(state && !state->materialVertex && !state->alphaMaterialVertex);
    r.values={0,0,0,1,0,2,2};state=state->request(r);assert(state && state->materialVertex && !state->alphaMaterialVertex && state->channelControl[0]->at(0)==0);
    r.values={2,0,0,1,0,2,2};state=state->request(r);assert(state && state->materialVertex && state->alphaMaterialVertex && state->channelControl[1]->at(0)==2);
    r.values[1]=1;assert(!state->request(r)); // Lit GPU state is not acknowledged by passthrough.
}
