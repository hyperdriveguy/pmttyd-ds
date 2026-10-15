#include "hit_n64_effect_draw.hpp"
#include "battle_audience_aux_callback.hpp"
#include "battle_stage_display.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Effect=0x2000,Payload=0x3000,Camera=0x4000;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
bool same(const SourceIconGXCall& a,const SourceIconGXCall& b){return a.function==b.function && a.integers==b.integers && a.reals==b.reals && a.matrix==b.matrix;}
struct Fixture {
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    std::vector<SourceIconGXCall> calls;std::vector<BattleAudienceMatrixRequest> math;
    std::vector<std::array<uint16_t,18>> geometry;
    int holdCall=-1,holdMath=-1,holdIndices=-1,holdTruncate=-1;unsigned held=0,truncations=0;
    HitN64DrawServices s;BattleAudienceAuxServices adapter;BattleActMatrix* mutateBase=nullptr;
    HitN64DrawCursor* recursive=nullptr;bool recursed=false;
    Fixture(uint32_t kind=0,float progress=.25f,uint32_t age=1){
        words[Effect+0xc]=Payload;words[Effect+8]=2;words[Payload]=kind;words[Payload+0x28]=bits(1.5f);
        words[Payload+4]=bits(1);words[Payload+8]=bits(2);words[Payload+12]=bits(3);
        words[Payload+0x2c]=0x123;words[Payload+0x34]=age;words[Payload+0x38]=bits(progress);
        const uint8_t color[]={255,128,64,32,16,8};for(unsigned i=0;i<6;++i)bytes[Payload+0x3c+i]=color[i];
        bytes[0x804198ac]=255;bytes[0x804198ad]=255;
        for(unsigned i=0;i<3;++i)words[0x802fb0ac+i*4]=bits(1);
        words[0x802fb0b8]=0x43300000;words[0x802fb0bc]=0x80000000;
        const float constants[]={5,4,1.f/32,1.f/128,0,96,.01745329238474369f,256,1.f/256,1};
        for(unsigned i=0;i<10;++i)words[0x80425458+i*4]=bits(constants[i]);
        words[Camera+0x114]=bits(0);
        const BattleActMatrix view={1,0,0,0,0,1,0,0,0,0,1,0};for(unsigned i=0;i<12;++i)words[Camera+0x11c+4*i]=bits(view[i]);
        words[Payload+0x44+4]=bits(10);words[Payload+0x44+8]=0;words[Payload+0x44+12]=0;
        words[Payload+0x44+0x1c]=bits(360);words[Payload+0x44+0x24]=bits(17);
        bindAudienceAuxMathAdapter(adapter);s.user=this;s.memory.user=this;
        s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        s.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.multiply=[](void*,float a,float b){volatile float n=a*b;return n;};
        s.truncate=[](void* p,float n)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);
            if(int(f.truncations)==f.holdTruncate && f.held++<2)return {};
            ++f.truncations;if(!std::isfinite(n) || n<-2147483648.f || n>=2147483648.f)return {};return int32_t(n);};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(p);
            if(int(f.math.size())==f.holdMath && f.held++<2)return false;
            if(f.mutateBase){(*f.mutateBase)[3]=37;f.mutateBase=nullptr;}
            f.math.push_back(r);
            if(r.function==0x8029e1c0){BattleStageMath q;q.function=r.function;q.axis=r.xyz;q.angle=r.radians;if(!nativeBattleStageMath(q))return false;r.output=q.matrix;return true;}
            return f.adapter.matrix(f.adapter.user,r);};
        s.call=[](void* p,const SourceIconGXCall& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);
            if(f.recursive && !f.recursed){f.recursed=true;assert(!resumeHitN64Draw(4,Effect,f.s,*f.recursive));}
            if(int(f.calls.size())==f.holdCall && f.held++<2)return {};
            f.calls.push_back(q);if(q.function==0x8000f2fc)return Camera;
            if(q.function==0x801d78b0){assert(q.integers[0]==74);return 0x7788;} // Explicit recorded fixture object.
            return 0;};
        s.indices=[](void* p,const std::array<uint16_t,18>& v){auto& f=*static_cast<Fixture*>(p);
            if(int(f.geometry.size())==f.holdIndices && f.held++<2)return false;
            f.geometry.push_back(v);return true;};
    }
    void run(HitN64DrawCursor& c){unsigned waits=0;while(!resumeHitN64Draw(4,Effect,s,c))assert(++waits<=3);assert(!c.active());}
    void equal(const Fixture& base){assert(calls.size()==base.calls.size() && math.size()==base.math.size() && geometry==base.geometry && truncations==base.truncations);for(size_t i=0;i<calls.size();++i)assert(same(calls[i],base.calls[i]));for(size_t i=0;i<math.size();++i){assert(math[i].function==base.math[i].function && math[i].axis==base.math[i].axis && math[i].xyz==base.math[i].xyz && math[i].radians==base.math[i].radians && math[i].left==base.math[i].left && math[i].right==base.math[i].right);}}
};
void completeAndEveryHold(){
    Fixture base;HitN64DrawCursor c;base.recursive=&c;base.run(c);assert(base.recursed);
    assert(base.geometry.size()==3 && base.truncations==5);
    const std::array<uint16_t,18> expected={0,0,0,1,1,1,2,2,2,0,0,0,2,2,2,3,3,3};assert(base.geometry[0]==expected);
    unsigned color=0,bind=0,arrays=0,begin=0;
    for(const auto& q:base.calls){
        if(q.function==0x802b8ab8){if(color==0)assert(q.integers[0]==1 && q.integers[1]==0xff804023);else assert(q.integers[0]==2 && q.integers[1]==0x20100840);++color;}
        if(q.function==0x802b7880){assert(q.integers[0]==0x7788 && q.integers[1]==bind);++bind;}
        if(q.function==0x802b4db0){const auto lane=arrays%3;assert(q.integers[0]==(lane==0?9u:lane==1?11u:13u));assert(q.integers[1]==0x803a0458+56+(lane==0?0:lane==1?10:6) && q.integers[2]==14);++arrays;}
        if(q.function==0x802b5a6c){assert((q.integers==std::array<uint32_t,8>{144,0,6}));++begin;}
    }
    assert(color==2 && bind==2 && arrays==9 && begin==3);
    for(size_t i=0;i<base.calls.size();++i){Fixture f;f.holdCall=int(i);HitN64DrawCursor held;f.run(held);f.equal(base);}
    for(size_t i=0;i<base.math.size();++i){Fixture f;f.holdMath=int(i);HitN64DrawCursor held;f.run(held);f.equal(base);}
    for(size_t i=0;i<base.geometry.size();++i){Fixture f;f.holdIndices=int(i);HitN64DrawCursor held;f.run(held);f.equal(base);}
    for(size_t i=0;i<base.truncations;++i){Fixture f;f.holdTruncate=int(i);HitN64DrawCursor held;f.run(held);f.equal(base);}
}
void branchesAndBorrow(){
    for(uint32_t kind:{1u,2u})for(uint32_t age:{1u,2u,14u}){Fixture f(kind,.25f,age);HitN64DrawCursor c;f.run(c);
        for(const auto& q:f.calls)if(q.function==0x802b8ab8 && q.integers[0]==1){
            const auto color=q.integers[1];if(kind==1){const auto fire=age<=2?255:0;assert(color==(0xff000023u|uint32_t((128*fire)>>8)<<16|uint32_t((64*fire)>>8)<<8));}
            else assert(color==(age&1?0xff804023u:0x23u));}}
    Fixture hidden(0,1,1);HitN64DrawCursor c;hidden.run(c);assert(hidden.calls.size()==1 && hidden.math.empty() && hidden.geometry.empty());
    Fixture last(0,.9f,1);HitN64DrawCursor lastCursor;last.run(lastCursor);unsigned collapsed=0;
    for(const auto& q:last.calls)if(q.function==0x802ba3d0 && q.integers[0]==33 && q.matrix[0]==0 && q.matrix[5]==0 && q.matrix[10]==0)++collapsed;
    assert(collapsed==3);
    Fixture f;BattleActMatrix base={1,0,0,0,0,1,0,0,0,0,1,0};f.mutateBase=&base;HitN64DrawCursor particle;
    assert(resumeHitN64ParticleDraw(Effect,base,f.s,particle));bool captured=false;
    for(const auto& m:f.math)if(m.function==0x8029dd64 && m.left[3]==37)captured=true;
    assert(captured && f.geometry.size()==1);
    Fixture held;held.holdCall=0;HitN64DrawCursor waiting;assert(!resumeHitN64Draw(4,Effect,held.s,waiting));assert(!resumeHitN64Draw(8,Effect,held.s,waiting));
    auto changed=held.s;changed.indices=nullptr;assert(!resumeHitN64Draw(4,Effect,changed,waiting));held.run(waiting);
}
}
int main(){completeAndEveryHold();branchesAndBorrow();std::cout<<"PASS complete original HitN64 Display/main_dl/index helpers and every service hold\n";}
