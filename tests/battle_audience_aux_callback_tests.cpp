#include "battle_audience_aux_callback.hpp"
#include "native_shadow_program.hpp"
#include "native_display_state.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    uint32_t battle=0x10000;CameraState camera;NativeShadowProgram program;NativeDisplayState pipeline;
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    unsigned baseReads=0,cameraReads=0,flagsReads=0;
    int holdGX=-1,holdMath=-1;bool iconAvailable=false;
    std::vector<BattleAudienceGXRequest> gx;std::vector<BattleAudienceMatrixRequest> math;
    std::vector<BattleAudienceIconRequest> icons;
    Fixture(){program.afterDummy();pipeline.afterDummy();real(0x80424988,0);real(0x80424990,1);real(0x80424a10,-20);real(0x80424a18,20);words[0x80424a14]=0x3c8efa35;words[0x80424978]=0xffffff00;}
    uint32_t owner()const{return battle+0x2858;}
    uint32_t item(unsigned i)const{return owner()+0xf25c+0x48*i;}
    uint32_t ap(unsigned i)const{return owner()+0x10e7c+0x60*i;}
    void real(uint32_t p,float v){uint32_t bits;std::memcpy(&bits,&v,4);words[p]=bits;}
    BattleAudienceAuxServices services(){BattleAudienceAuxServices s;s.user=this;s.common.user=this;s.common.actors.user=this;s.common.actors.memory.user=this;
        s.common.actors.memory.word=[](void* p,uint32_t addr){auto& f=*static_cast<Fixture*>(p);if(addr==0x8041eb90){++f.baseReads;return f.battle;}
            if(addr<0x80000000)++f.flagsReads;return f.words[addr];};
        s.common.actors.memory.half=[](void* p,uint32_t addr){return uint16_t(static_cast<Fixture*>(p)->words[addr&~3u]>>(addr&2?0:16));};
        s.common.actors.byte=[](void* p,uint32_t addr){return static_cast<Fixture*>(p)->bytes[addr];};
        s.common.camera=[](void* p,uint32_t index)->const CameraState*{auto& f=*static_cast<Fixture*>(p);assert(index==8);++f.cameraReads;return &f.camera;};
        s.common.request=[](void* p,const BattleAudienceGXRequest& r){auto& f=*static_cast<Fixture*>(p);if(int(f.gx.size())==f.holdGX)return false;
            // Shader requests genuinely update a retained logical native owner;
            // AP pipeline tuples are applied to the actual native state adapter.
            if(r.function!=0x802b932c && r.function!=0x802b940c && r.function!=0x802b8d60 && r.function!=0x802b93d8){ShadowGXRequest q;q.kind=static_cast<ShadowGX>(r.function);q.values=r.arguments;auto next=f.program.request(q);assert(next);f.program=*next;}
            else {DisplayStateRequest state{};
                state.kind=r.function==0x802b932c?DisplayGXState::BlendMode:r.function==0x802b940c?DisplayGXState::ZCompareLocation:r.function==0x802b8d60?DisplayGXState::AlphaCompare:DisplayGXState::ZMode;
                for(unsigned i=0;i<5;++i)state.values[i]=r.arguments[i];
                NativeDisplaySubmit submit{nullptr,[](void*,const NativeDisplayPipeline&){return true;}};assert(f.pipeline.request(state,submit));}
            f.gx.push_back(r);return true;};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(p);if(int(f.math.size())==f.holdMath)return false;
            BattleAudienceAuxServices adapter;bindAudienceAuxMathAdapter(adapter);if(!adapter.matrix(nullptr,r))return false;f.math.push_back(r);return true;};
        s.icon=[](void* p,const BattleAudienceIconRequest& r){auto& f=*static_cast<Fixture*>(p);if(!f.iconAvailable)return false;f.icons.push_back(r);return true;};return s;
    }
};
int main(){
    Fixture inactive;BattleAudienceAuxCursor c;
    assert(resumePackedBattleAudienceAux(0x801a37f0,inactive.services(),c));
    assert(inactive.gx.size()==18 && inactive.flagsReads==100 && inactive.baseReads==2 && inactive.cameraReads==0 && inactive.math.empty() && inactive.icons.empty());
    assert(inactive.program.textureRGB && inactive.program.materialVertex && !inactive.program.audienceColor && !inactive.program.texture && !inactive.program.material);
    Fixture sources;BattleAudienceAuxCursor a;assert(resumePackedBattleAudienceAux(0x801a36c8,sources.services(),a));
    assert(sources.gx.size()==22 && sources.flagsReads==100 && sources.baseReads==2 && sources.cameraReads==1);
    assert(sources.gx[18].function==0x802b932c && sources.gx[19].function==0x802b940c && sources.gx[20].function==0x802b8d60 && sources.gx[21].function==0x802b93d8);
    assert(sources.pipeline.state()->sourceBlend==4 && sources.pipeline.state()->destinationBlend==5 && sources.pipeline.state()->beforeAlpha && !sources.pipeline.state()->alphaReject && sources.pipeline.state()->depth && sources.pipeline.state()->compare==3 && !sources.pipeline.state()->depthWrite);

    // Fresh ItemGetPtr AFTER held GXInit; AP instead scans its original owner.
    Fixture fresh;fresh.holdGX=5;BattleAudienceAuxCursor h;assert(!resumePackedBattleAudienceAux(0x801a37f0,fresh.services(),h) && fresh.baseReads==1);
    fresh.battle=0x50000;fresh.holdGX=-1;fresh.words[fresh.item(99)]=3;fresh.words[fresh.item(99)+0x10]=7;fresh.words[0x803108a8+7*40+0x20]=42u<<16;
    fresh.holdMath=0;assert(!resumePackedBattleAudienceAux(0x801a37f0,fresh.services(),h) && h.index==99 && h.record==fresh.item(99) && h.pending==0x8029e230 && fresh.gx.size()==18 && fresh.baseReads==2);
    const auto pinned=h.matrix;fresh.real(0x80424a10,-99);assert(!resumePackedBattleAudienceAux(0x801a37f0,fresh.services(),h) && h.matrix.xyz==pinned.xyz && fresh.gx.size()==18);
    Fixture captured;const auto oldOwner=captured.owner();captured.holdGX=5;BattleAudienceAuxCursor q;
    assert(!resumePackedBattleAudienceAux(0x801a36c8,captured.services(),q));captured.battle=0x50000;captured.holdGX=-1;captured.words[oldOwner+0x10e7c]=1;captured.holdMath=0;
    assert(!resumePackedBattleAudienceAux(0x801a36c8,captured.services(),q) && q.record==oldOwner+0x10e7c && captured.baseReads==2 && captured.cameraReads==1);
    // Exact six-call item transform; held ScaleApply pins size, later position stays fresh.
    Fixture item;const auto p=item.item(0);item.words[p]=3;item.words[p+0x10]=7;item.words[0x803108a8+7*40+0x20]=42u<<16;
    item.real(p+0x20,2);item.real(p+0x24,90);item.real(p+0x14,10);item.real(p+0x18,20);item.real(p+0x1c,30);item.holdMath=4;BattleAudienceAuxCursor i;
    assert(!resumePackedBattleAudienceAux(0x801a37f0,item.services(),i) && i.pending==0x8029e2d8 && item.math.size()==4 && i.icon.icon==42);
    uint32_t radians;std::memcpy(&radians,&item.math[1].radians,4);assert(radians==0x3fc90fdb && item.words[0x80424a14]==0x3c8efa35);
    item.real(p+0x20,9);item.real(p+0x14,100);item.words[0x803108a8+7*40+0x20]=77u<<16;item.holdMath=-1;
    assert(!resumePackedBattleAudienceAux(0x801a37f0,item.services(),i) && i.pending==0x8001a6e0 && item.math.size()==6 && i.icon.icon==42);
    assert(std::fabs(i.icon.matrix[3]-140)<.0001 && std::fabs(i.icon.matrix[7]-60)<.0001 && i.icon.matrix[11]==30);
    const auto frozen=i.icon;item.real(p+0x14,999);item.iconAvailable=true;assert(resumePackedBattleAudienceAux(0x801a37f0,item.services(),i));
    assert(item.icons.size()==1 && item.icons[0].matrix==frozen.matrix && item.icons[0].flags==0 && item.math.size()==6);
    // Item flags1 or2 alone reject; AP bit1 alone admits even alpha0.
    Fixture ap;ap.words[ap.ap(0)]=2;ap.words[ap.ap(1)]=1;ap.real(ap.ap(1)+0x38,2);ap.real(ap.ap(1)+0x3c,3);ap.real(ap.ap(1)+0x40,4);ap.real(ap.ap(1)+8,10);ap.real(ap.ap(1)+12,20);
    BattleAudienceAuxCursor b;assert(!resumePackedBattleAudienceAux(0x801a36c8,ap.services(),b) && b.index==1 && b.pending==0x8001a4bc && ap.math.size()==5);
    assert(b.icon.icon==442 && b.icon.flags==16 && b.icon.rgba==0xffffff00 && b.icon.matrix[0]==2 && b.icon.matrix[5]==3 && b.icon.matrix[10]==4 && b.icon.matrix[3]==10 && b.icon.matrix[7]==20);
    ap.bytes[ap.ap(1)+0x5c]=123;ap.iconAvailable=true;assert(resumePackedBattleAudienceAux(0x801a36c8,ap.services(),b) && ap.icons[0].rgba==0xffffff00);
    Fixture flags;flags.words[flags.item(0)]=1;flags.words[flags.item(1)]=2;BattleAudienceAuxCursor f;assert(resumePackedBattleAudienceAux(0x801a37f0,flags.services(),f) && flags.math.empty());
    // Preserve original Concat's +0 term for Z-column, including signed zero.
    BattleAudienceAuxServices adapter;bindAudienceAuxMathAdapter(adapter);BattleAudienceMatrixRequest zero;zero.function=0x8029dd64;
    zero.left={-0.f,-0.f,-0.f,1,0,0,0,0,0,0,0,0};zero.right={0,0,1,0,0,0,1,0,0,0,1,0};
    assert(adapter.matrix(nullptr,zero) && zero.output[2]==0 && !std::signbit(zero.output[2]));
    inactive.program.afterDummy();assert(!inactive.program.textureRGB);
}
