#include "source_icon_position.hpp"
#include "source_icon_resources.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {uint32_t bits(float v){uint32_t n;std::memcpy(&n,&v,4);return n;}}
struct Fixture {
    SourceIconResources resources;std::map<uint32_t,uint32_t> words;std::vector<uint32_t> reads,halfReads;
    SourceIconPositionRequest request{0x10010,0x10194,0x12345,.5f,{1,2,3}};
    unsigned cameras=0,matrices=0,draws=0;bool holdRotation=false,holdDraw=false,lateUnload=false;
    BattleActMatrix output{};SourceMatrixIcon icon;
    Fixture(){assert(resources.load("romfs/source-icons"));words[0x80415fd0]=0x7000;words[0x701c]=0;words[0x5114]=bits(90);
        const BattleActMatrix view{1,0,0,1,0,1,0,2,0,0,1,3};for(unsigned i=0;i<12;++i)words[0x511c+4*i]=bits(view[i]);}
    SourceIconMatrixServices services(){SourceIconMatrixServices s;s.user=this;
        s.currentCamera=[](void* u)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);++f.cameras;f.request.position={10,20,-100};f.request.scale=.9f;return 0x5000;};
        s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);f.reads.push_back(a);const auto p=f.words.find(a);return p==f.words.end()?f.resources.word(a):std::optional<uint32_t>(p->second);};
        s.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& f=*static_cast<Fixture*>(u);f.halfReads.push_back(a);return f.resources.half(a);};
        s.matrix=[](void* u,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(u);if(r.function==0x8029dff0 && f.holdRotation)return false;
            BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);if(!math.matrix(nullptr,r))return false;
            ++f.matrices;if(f.lateUnload && f.matrices==6)f.words[0x8041e638]=0;return true;};
        s.draw=[](void* u,const BattleActMatrix& m,const SourceMatrixIcon& i){auto& f=*static_cast<Fixture*>(u);if(f.holdDraw)return false;++f.draws;f.output=m;f.icon=i;return true;};return s;}
};
int main(){
    Fixture f;SourceIconPositionCursor c;f.holdRotation=true;
    assert(!resumeSourceIconPosition(f.request,f.services(),c) && c.pending==0x8029dff0 && f.matrices==2 && f.cameras==1);
    f.words[0x5114]=bits(0);f.words[0x5128]=bits(7);f.words[0x5138]=bits(8);f.words[0x5148]=bits(9);f.holdRotation=false;f.holdDraw=true;
    assert(!resumeSourceIconPosition(f.request,f.services(),c) && c.pending==0x800197f8 && f.matrices==6 && f.cameras==1 && f.draws==0);
    f.words[0x5128]=bits(999);f.request.scale=100;f.holdDraw=false;
    assert(resumeSourceIconPosition(f.request,f.services(),c) && f.matrices==6 && f.draws==1 && !c.active);
    assert(f.icon.id==404 && f.icon.flags==16 && f.icon.rgba==0xffffff45 && f.icon.width==0 && f.icon.height==0 && f.icon.texture==418 && f.icon.delay==6);
    assert(f.output[3]==17 && f.output[7]==28 && f.output[11]==-91 && f.output[2]<-.4999f && f.output[8]>.4999f);
    // Current camera still runs before the initial genuine resource-null return.
    Fixture early;early.words[0x8041e638]=0;assert(resumeSourceIconPosition(early.request,early.services(),c) && early.cameras==1 && early.matrices==0 && early.draws==0);
    // The SECOND resource-null path dereferences pointer0, rather than silently skipping an icon.
    Fixture late;late.lateUnload=true;assert(!resumeSourceIconPosition(late.request,late.services(),c) && c.descriptor==0 && late.halfReads.back()==2 && late.draws==0);
    assert(late.reads[late.reads.size()-1]==0x8041e634);
}
