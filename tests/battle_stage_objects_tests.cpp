#include "battle_stage_objects.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,D=0x40000,R=B+0x1715c;
    using P=BattleStageObjectServices::Point;
    struct Call {char kind;uint32_t name;P point;};
    std::map<uint32_t,uint32_t> words{{0x8041eb90,B}};
    std::map<uint32_t,uint16_t> halves;
    std::map<uint32_t,uint32_t> flags;
    std::vector<Call> calls;
    bool available=true,mutate=false;
    static uint32_t bits(float x){uint32_t u;std::memcpy(&u,&x,4);return u;}
    void vector(uint32_t a,P p){for(unsigned i=0;i<3;++i)words[a+4*i]=bits(p[i]);}
    void entry(unsigned i,uint16_t layer) {
        const auto r=R+0x7c*i,d=D+0x18*i;words[r]=i+1;words[r+0x64]=d;words[d]=0x50000+i;halves[d+6]=layer;
        for(unsigned j=0;j<12;++j)words[r+0x34+4*j]=bits(999);
    }
    BattleStageObjectServices services() {
        BattleStageObjectServices s;s.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        s.actors.memory.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->halves[a];};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;};
        s.translate=[](void* p,uint32_t name,const P& value){auto& f=*static_cast<Fixture*>(p);f.calls.push_back({'T',name,value});
            if(f.mutate){f.words[D]=0x60000;f.vector(R+0x1c,{4,5,6});f.words[R+0x68]|=1;}
            return f.available;};
        s.rotate=[](void* p,uint32_t name,const P& value){auto& f=*static_cast<Fixture*>(p);f.calls.push_back({'R',name,value});return f.available;};
        s.groupFlags=[](void* p,uint32_t name)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.calls.push_back({'G',name,{}});return f.flags[name];};
        s.groupFlag=[](void* p,uint32_t name,uint32_t mask,bool on){auto& f=*static_cast<Fixture*>(p);assert(mask==1);f.calls.push_back({on?'H':'V',name,{}});if(on)f.flags[name]|=mask;else f.flags[name]&=~mask;return f.available;};return s;
    }
};
int main() {
    using F=Fixture;using P=F::P;
    Fixture empty;auto s=empty.services();assert(packedBattleStageObjectMain(s) && empty.calls.empty());
    for(uint16_t layer:{0u,1u,2u,3u,4u,5u,6u,7u,8u}) {
        Fixture f;f.entry(0,layer);f.vector(F::R+4,{10,20,30});f.vector(F::R+0x10,{1,2,3});f.vector(F::R+0x28,{4,5,6});f.vector(F::R+0x1c,{7,8,9});f.vector(F::B+0x180e0,{40,50,60});
        s=f.services();assert(packedBattleStageObjectMain(s));
        if(layer>=8)assert(f.calls.size()==1 && f.calls[0].kind=='G');
        else {
            assert(f.calls[0].kind=='T' && (f.calls[0].point==P{15,27,39}));
            if(layer==2 || layer==3) {
                assert(f.calls.size()==5 && f.calls[1].kind=='T' && (f.calls[1].point==P{0,layer==2?-2.f:-20.f,0}));
                assert(f.calls[2].kind=='R' && (f.calls[2].point==(layer==2?P{40,50,60}:P{7,8,9})));
                assert(f.calls[3].kind=='T' && (f.calls[3].point==P{0,layer==2?2.f:20.f,0}));
            } else assert(f.calls.size()==3 && f.calls[1].kind=='R' && (f.calls[1].point==P{7,8,9}));
        }
        f.calls.clear();assert(packedBattleStageObjectMain(s) && f.calls.size()==1 && f.calls[0].kind=='G');
        f.words[F::R+0x68]=0x83;f.calls.clear();assert(packedBattleStageObjectMain(s) && f.words[F::R+0x68]==0x81 && f.flags[0x50000]==1 && f.calls.back().kind=='H');
        f.words[F::R+0x68]=0x80;f.calls.clear();assert(packedBattleStageObjectMain(s) && f.calls.back().kind=='V');
    }
    Fixture all;for(unsigned i=0;i<32;++i)all.entry(i,1);s=all.services();assert(packedBattleStageObjectMain(s) && all.calls.size()==96);
    assert(all.calls.back().name==0x50000+31);
    Fixture signedRecord;signedRecord.entry(0,1);signedRecord.words[F::R]=0xffffffff;s=signedRecord.services();assert(packedBattleStageObjectMain(s) && signedRecord.calls.empty());
    Fixture failure;failure.entry(0,1);failure.available=false;failure.words[F::R+0x68]=2;s=failure.services();
    assert(!packedBattleStageObjectMain(s) && failure.words[F::R+0x68]==0 && failure.words[F::R+0x34]==0 && failure.calls.size()==1);
    Fixture callback;callback.entry(0,1);callback.mutate=true;s=callback.services();assert(packedBattleStageObjectMain(s));
    assert(callback.calls[1].name==0x60000 && (callback.calls[1].point==P{4,5,6}) && callback.calls.back().kind=='H');
    Fixture equality;equality.entry(0,1);s=equality.services();assert(packedBattleStageObjectMain(s));equality.calls.clear();
    equality.words[F::R+4]=F::bits(-0.f);assert(packedBattleStageObjectMain(s) && equality.calls.size()==1 && equality.words[F::R+0x34]==0);
    equality.vector(F::R+4,{std::numeric_limits<float>::quiet_NaN(),0,0});equality.calls.clear();assert(packedBattleStageObjectMain(s) && equality.calls.size()==3);
    Fixture shared;shared.entry(0,2);shared.entry(1,2);s=shared.services();assert(packedBattleStageObjectMain(s));shared.calls.clear();
    shared.vector(F::B+0x180e0,{1,0,0});assert(packedBattleStageObjectMain(s));
    assert(shared.calls.size()==6 && shared.calls[0].kind=='T' && shared.calls.back().kind=='G'); // Shared global cache admits first layer2 only.
}
