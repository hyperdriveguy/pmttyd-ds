#include "battle_break_slot_draw.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}}
struct Fixture {
    static constexpr uint32_t B=0x100000,R=B+0x16184,GP=0x500000;
    std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{0x80415fd0,GP},{0x80426884,0xffffffd0},{0x80426888,0xffffff00},{0x8042688c,0xffffff00},{0x804268a0,0x3c8efa35}};
    std::map<uint32_t,uint16_t> halves{{GP+0x170,608},{GP+0x172,480}};
    std::map<uint32_t,uint8_t> bytes;
    std::vector<uint32_t> reads;std::vector<std::array<uint32_t,4>> scissors;std::vector<BattleBreakSlotPlaneRequest> planes;
    unsigned getters=0,matrices=0,icons=0,gx=0;bool holdScissor=false,holdPlane=false,pivotMutation=false,freshOwners=false;
    Fixture(){const uint16_t small[]{65,67,69,68,66},big[]{70,72,74,73,71};for(unsigned i=0;i<5;++i){halves[0x80419f30+2*i]=small[i];halves[0x80419f3c+2*i]=big[i];}}
    BattleBreakSlotDrawServices services(){BattleBreakSlotDrawServices s;s.user=this;
        s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);const auto v=f.words[a];if(a==0x8041eb90){++f.getters;if(f.freshOwners && f.getters==2)f.words[a]=0x300000;}return v;};
        s.half=[](void* p,uint32_t a)->std::optional<uint16_t>{return static_cast<Fixture*>(p)->halves[a];};
        s.byte=[](void* p,uint32_t a)->std::optional<uint8_t>{return static_cast<Fixture*>(p)->bytes[a];};
        s.scissor=[](void* p,const std::array<uint32_t,4>& r){auto& f=*static_cast<Fixture*>(p);f.scissors.push_back(r);return !f.holdScissor;};
        s.gx=[](void* p,const ShadowGXRequest&){++static_cast<Fixture*>(p)->gx;return true;};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(p);BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);if(!math.matrix(nullptr,r))return false;++f.matrices;
            if(f.pivotMutation && r.function==0x8029e230 && r.xyz==std::array<float,3>{2,0,0})f.words[R+0x44]=bits(5);return true;};
        s.icon=[](void* p,const BattleAudienceIconRequest& r){auto& f=*static_cast<Fixture*>(p);assert(r.function==0x8001a4bc && r.flags==16 && r.icon==476 && r.rgba==0xffffffd0);assert(r.matrix[3]==226 && r.matrix[7]==122);++f.icons;if(f.freshOwners)f.words[0x8041eb90]=0x200000;return true;};
        s.plane=[](void* p,const BattleBreakSlotPlaneRequest& r){auto& f=*static_cast<Fixture*>(p);if(f.holdPlane)return false;f.planes.push_back(r);if(f.planes.size()==1){f.words[R+8]=1;f.bytes[R+0x58]=20;}return true;};return s;
    }
};
int main(){
    Fixture idle;BattleBreakSlotDrawCursor c;assert(drawBattleBreakSlot(idle.services(),c));
    assert(idle.getters==3 && idle.matrices==0 && idle.gx==0 && idle.icons==0 && idle.planes.empty());
    assert((idle.scissors==std::vector<std::array<uint32_t,4>>{{0,176,608,128},{0,0,608,480}}));
    assert(idle.reads==std::vector<uint32_t>({0x8041eb90,Fixture::R+4,Fixture::R+0x60,Fixture::R+0xbc,0x8041eb90,Fixture::R+8,0x8041eb90,Fixture::R+4,Fixture::R+0x60,Fixture::R+0xbc,0x80415fd0}));
    // A held initial scissor retains its source-selected rectangle; the third getter is still fresh.
    Fixture held;held.holdScissor=true;assert(!drawBattleBreakSlot(held.services(),c) && c.pending==0x802ba644 && held.getters==2);
    held.words[Fixture::R+8]=11;held.words[0x8041eb90]=0x200000;held.holdScissor=false;assert(drawBattleBreakSlot(held.services(),c) && held.getters==3);
    assert(held.scissors[0]==held.scissors[1] && held.scissors.size()==3);
    // First getter's reels, second getter's state, third getter's flags have distinct retained owners.
    Fixture owners;owners.freshOwners=true;owners.words[Fixture::R+4]=1;owners.words[Fixture::R+8]=1;owners.words[0x21618c]=11;
    assert(drawBattleBreakSlot(owners.services(),c) && owners.getters==3 && owners.icons==1 && owners.scissors.size()==1 && c.base==0x316184);
    // State0 active flag still runs original matrices and Plane2, including fresh negative pivot and state/alpha.
    Fixture active;active.words[Fixture::R+4]=1;active.pivotMutation=true;active.holdPlane=true;
    active.words[Fixture::R+0x20]=bits(10);active.words[Fixture::R+0x24]=bits(20);active.words[Fixture::R+0x28]=bits(30);
    for(unsigned i=0;i<3;++i)active.words[Fixture::R+0x38+4*i]=bits(1);
    active.words[Fixture::R+0x44]=bits(2);active.words[Fixture::R+0x14]=0x6000;active.words[Fixture::R+0x18]=2;active.words[0x6004]=2;active.bytes[Fixture::R+0x58]=10;
    assert(!drawBattleBreakSlot(active.services(),c) && c.pending==0x8010323c && active.matrices==13 && active.icons==0 && active.gx==4);
    active.words[Fixture::R+0x20]=bits(999);active.holdPlane=false;
    assert(drawBattleBreakSlot(active.services(),c) && active.matrices==15 && active.planes.size()==2);
    assert(active.planes[0].matrix[3]==7 && active.planes[0].matrix[7]==20 && active.planes[0].texture==70 && active.planes[0].rgba==0xffffff0a);
    assert(active.planes[1].matrix[3]==7 && active.planes[1].matrix[7]==-12 && active.planes[1].texture==74 && active.planes[1].rgba==0xffffff14);
    auto missing=active.services();missing.plane=nullptr;assert(!drawBattleBreakSlot(missing,c) && c.pending==0x8010323c);
}
