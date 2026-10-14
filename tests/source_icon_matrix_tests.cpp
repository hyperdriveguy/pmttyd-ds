#include "source_icon_matrix.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint16_t> halves;
    std::vector<uint32_t> reads;std::vector<SourceMatrixIcon> icons;
    uint32_t camera=0x10000;unsigned cameraCalls=0,matrixCalls=0;
    bool holdCamera=false,holdMatrix=false,holdDraw=false;
    BattleActMatrix emitted{};
    Fixture(){words[0x8041e638]=0x20000;words[0x8041e63c]=0x30000;words[0x8041e634]=0x40000;
        words[0x8041f864]=0xffffffff;words[0x80415fd0]=0x50000;words[0x5001c]=0;words[0x5016c]=0;
        view(camera,10);descriptor(42,{40,32},{{7,2},{8,3},{9,1}});}
    void view(uint32_t addr,float x){BattleActMatrix m{1,0,0,x,0,1,0,0,0,0,1,0};
        for(unsigned i=0;i<12;++i)std::memcpy(&words[addr+0x11c+4*i],&m[i],4);}
    void descriptor(uint16_t id,std::array<uint16_t,2> size,std::vector<std::array<uint16_t,2>> frames){
        uint32_t d=0x41000+id*0x100;halves[0x40000+2*(uint32_t(id)+1)]=uint16_t(d-0x40000);
        halves[d]=0;halves[d+2]=size[0];halves[d+4]=size[1];halves[d+6]=uint16_t(frames.size());
        for(unsigned i=0;i<frames.size();++i){halves[d+8+4*i]=frames[i][0];halves[d+10+4*i]=frames[i][1];}}
    SourceIconMatrixServices services(){SourceIconMatrixServices s;s.user=this;
        s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto i=f.words.find(a);return i==f.words.end()?std::optional<uint32_t>{}:i->second;};
        s.half=[](void* p,uint32_t a)->std::optional<uint16_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);auto i=f.halves.find(a);return i==f.halves.end()?std::optional<uint16_t>{}:i->second;};
        s.currentCamera=[](void* p)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.cameraCalls;return f.holdCamera?std::optional<uint32_t>{}:f.camera;};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(p);++f.matrixCalls;if(f.holdMatrix)return false;
            BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,r);};
        s.draw=[](void* p,const BattleActMatrix& m,const SourceMatrixIcon& icon){auto& f=*static_cast<Fixture*>(p);if(f.holdDraw)return false;f.emitted=m;f.icons.push_back(icon);return true;};return s;}
};
BattleAudienceIconRequest request(uint32_t function=0x8001a6e0){BattleAudienceIconRequest r;r.function=function;r.icon=0x1002a;r.flags=0x10014;r.rgba=0x12345678;r.matrix={2,0,0,3,0,2,0,4,0,0,1,5};return r;}
int main(){
    // Source frame selection always loops by signed GP.1c, irrespective of the
    // descriptor's loop word. Boundaries are strict '<', not '<='.
    for(const auto row:std::vector<std::array<int32_t,2>>{{-1,7},{0,7},{1,7},{2,8},{4,8},{5,9},{6,7},{11,9}}){
        Fixture f;f.words[0x5001c]=uint32_t(row[0]);SourceIconMatrixCursor c;assert(resumeSourceIconMatrix(request(),f.services(),c));
        assert(f.cameraCalls==1 && f.matrixCalls==1 && f.icons.size()==1 && !c.active && !c.pending);
        const auto& icon=f.icons[0];assert(icon.texture==row[1] && icon.delay==2 && icon.width==40 && icon.height==32);
        assert(icon.id==42 && icon.flags==20 && icon.rgba==0xffffffff);
        assert(f.emitted[0]==2 && f.emitted[3]==13 && f.emitted[7]==4 && f.emitted[11]==5);
    }
    // Source's unrolled summation (>8 entries) and zero-delay frames obey
    // the same weighted interval rule; descriptor loop never clamps the end.
    {Fixture f;f.descriptor(42,{24,48},{{10,0},{11,1},{12,2},{13,3},{14,4},{15,5},{16,6},{17,7},{18,8},{19,9}});
        for(int32_t tick=0;tick<90;++tick){f.words[0x5001c]=uint32_t(tick);int32_t remainder=tick%45;uint16_t expected=11;
            for(int32_t duration=1;duration<=9;++duration){if(remainder<duration){expected=uint16_t(10+duration);break;}remainder-=duration;}
            SourceIconMatrixCursor c;assert(resumeSourceIconMatrix(request(),f.services(),c));assert(f.icons.back().texture==expected && f.icons.back().delay==0);}}
    // Actual missing buffers return after camera lookup; no metadata, matrix
    // or primitive is requested. Missing word provider is not that branch.
    for(unsigned which:{0u,1u}){Fixture f;f.words[which?0x8041e63c:0x8041e638]=0;SourceIconMatrixCursor c;
        assert(resumeSourceIconMatrix(request(),f.services(),c));assert(f.icons.empty() && !f.matrixCalls && f.cameraCalls==1);
        assert(f.reads== (which?std::vector<uint32_t>{0x8041e638,0x8041e63c}:std::vector<uint32_t>{0x8041e638}));}
    {Fixture f;f.words.erase(0x8041e638);SourceIconMatrixCursor c;assert(!resumeSourceIconMatrix(request(),f.services(),c));assert(c.active && f.icons.empty());}
    // Single/zero-count source descriptors skip GP clock and retain frame0.
    for(uint16_t count:{0,1}){Fixture f;f.descriptor(42,{0,0},{{99,0}});f.halves[0x43a00+6]=count;f.words.erase(0x5001c);SourceIconMatrixCursor c;
        assert(resumeSourceIconMatrix(request(0x8001a4bc),f.services(),c));assert(f.icons[0].texture==99 && f.icons[0].rgba==0x12345678 && f.icons[0].width==0);}
    // Both source special IDs resolve by GP.16c, but the stack icon ID remains
    // the original low16 request, not the selected descriptor's alias.
    for(uint16_t id:{73,103}){Fixture f;f.words[0x5016c]=2;f.halves[(id==73?0x80415040:0x8041504c)+4]=42;
        auto r=request();r.icon=id;SourceIconMatrixCursor c;assert(resumeSourceIconMatrix(r,f.services(),c));assert(f.icons[0].id==id && f.icons[0].texture==7);}
    // Camera is captured before metadata work. Concat reads its view after
    // frame selection and pins both arguments across the linked SDK wait.
    {Fixture f;f.holdMatrix=true;SourceIconMatrixCursor c;auto r=request();assert(!resumeSourceIconMatrix(r,f.services(),c));
        assert(c.pending==0x8029dd64 && c.icon.texture==7);f.camera=0x60000;f.view(0x10000,99);f.words[0x5001c]=5;r.matrix[3]=999;
        f.holdMatrix=false;f.holdDraw=true;assert(!resumeSourceIconMatrix(r,f.services(),c));assert(c.pending==0x800197f8 && c.matrix.output[3]==13);
        f.halves[0x43a00+8]=999;const auto reads=f.reads.size();const auto calls=f.matrixCalls;f.holdDraw=false;
        assert(resumeSourceIconMatrix(r,f.services(),c));assert(f.icons[0].texture==7 && f.cameraCalls==1 && f.matrixCalls==calls && f.reads.size()==reads);}
    {Fixture f;f.holdCamera=true;SourceIconMatrixCursor c;assert(!resumeSourceIconMatrix(request(),f.services(),c));assert(c.pending==0x8000f2e4 && f.reads.empty());
        f.holdCamera=false;assert(resumeSourceIconMatrix(request(),f.services(),c));assert(f.cameraCalls==2);}
    {Fixture f;f.descriptor(42,{40,40},{{1,0},{2,0}});SourceIconMatrixCursor c;assert(!resumeSourceIconMatrix(request(),f.services(),c));assert(f.icons.empty());}
    {Fixture f;auto s=f.services();s.draw=nullptr;SourceIconMatrixCursor c;assert(!resumeSourceIconMatrix(request(),s,c));assert(c.pending==0x800197f8);}
    {Fixture f;auto r=request(123);SourceIconMatrixCursor c;assert(!resumeSourceIconMatrix(r,f.services(),c) && !c.active && !f.cameraCalls);}
    // Actual containing audience callback uses this source wrapper rather than
    // acknowledging IconMtx directly. It retains the selected frame while the
    // real iconGX provider waits, then continues the remaining99-record scan.
    {Fixture f;uint32_t battle=0x60000,audience=battle+0x2858,item=audience+0xf25c;
        f.words[0x8041eb90]=battle;f.words[item]=3;f.words[item+0x10]=237;
        f.halves[0x803108a8+40*237+0x20]=42;
        auto put=[&](uint32_t address,float value){std::memcpy(&f.words[address],&value,4);};
        put(0x80424988,0);put(0x80424990,1);put(0x80424a10,-20);put(0x80424a18,20);f.words[0x80424a14]=0x3c8efa35;
        put(item+0x20,1);put(item+0x24,0);put(item+0x14,3);put(item+0x18,4);put(item+0x1c,5);
        auto original=f.services();BattleAudienceAuxServices aux;aux.sourceIcon=&original;aux.common.actors.memory.user=&f;
        aux.common.actors.memory.word=[](void* p,uint32_t address){return static_cast<Fixture*>(p)->words[address];};
        aux.common.actors.memory.half=[](void* p,uint32_t address){return static_cast<Fixture*>(p)->halves[address];};
        aux.common.request=[](void*,const BattleAudienceGXRequest&){return true;};bindAudienceAuxMathAdapter(aux);
        BattleAudienceAuxCursor c;f.holdDraw=true;assert(!resumePackedBattleAudienceAux(0x801a37f0,aux,c));
        assert(c.pending==0x800197f8 && c.index==0 && c.iconCursor.icon.texture==7);
        f.words[0x5001c]=5;f.holdDraw=false;assert(resumePackedBattleAudienceAux(0x801a37f0,aux,c));
        assert(f.icons.size()==1 && f.icons[0].texture==7 && f.emitted[3]==13 && f.emitted[7]==4 && !c.active);
    }

}
