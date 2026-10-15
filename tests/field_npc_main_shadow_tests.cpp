#include "field_npc_main_shadow.hpp"
#include "shadow_work.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <memory>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A=0x10000, D=0x20000, OtherD=0x30000;
uint32_t bits(float f) { uint32_t w;std::memcpy(&w,&f,4);return w; }
struct Fixture {
  using Effect=std::tuple<char,uint32_t,uint32_t>;
  std::vector<Effect> trace;
  std::vector<FieldNpcShadowRequest> calls;
  std::vector<FieldNpcDisplayRequest> draws;
  std::map<uint32_t,uint32_t> words;
  std::unique_ptr<ShadowWork> shadows=std::make_unique<ShadowWork>();
  std::unique_ptr<DisplayQueue> queue=std::make_unique<DisplayQueue>();
  std::array<float,3> vivian{};
  unsigned attempts=0, fail=~0u;
  uint8_t alpha=255;
  bool mutate=false, hit=false, incompleteHit=false;
  float normalized=90, sine=1;
  Fixture() {
    words={{A,0},{A+0x28,0},{A+0x8c,bits(10)},{A+0x90,bits(20)},
      {A+0x94,bits(30)},{A+0x144,bits(7)},{A+0x14c,bits(6)},
      {A+0x154,bits(4)},{A+0xe0,bits(2)},{A+0xe4,bits(3)},
      {A+0xe8,bits(4)},{A+0x110,0xffffffff},{A+0x104,255},{A+0x1f4,4},
      {0x8041fc7c,0},{0x8041fc94,bits(1)},{0x8041fcf0,bits(100)},
      {0x8041fcd8,bits(-1)},{0x8041fccc,bits(180)},
      {0x8041fcb0,bits(90)},{0x8041fcc4,bits(270)},
      {0x8041fce0,bits(3)},{0x8041fd68,0x3faa3d71},
      {D+0x30,bits(1)},{D+0x34,bits(2)},{D+0x38,bits(3)},
      {OtherD+0x30,bits(5)},{OtherD+0x34,bits(6)},{OtherD+0x38,bits(7)}};
  }
  bool ready(){return attempts++!=fail;}
  FieldNpcShadowServices services() {
    FieldNpcShadowServices s;s.memory.user=this;
    s.memory.word=[](void*p,uint32_t a)->std::optional<uint32_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || !f.words.count(a))return {};
      f.trace.emplace_back('r',a,f.words[a]);return f.words[a];
    };
    s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || a!=A+0x117)return {};
      f.trace.emplace_back('b',a,f.alpha);return f.alpha;
    };
    s.queue=[](void*p,const FieldNpcDisplayRequest&q,FieldNpcDisplayResponse&){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      assert(q.function==0x80010bc4 && q.args[3]==A);
      if(f.queue->enter(q.args[0],q.args[1],q.args[2],q.args[3],q.depth)==DisplayAdmission::Unavailable)return false;
      f.draws.push_back(q);f.trace.emplace_back('q',q.args[2],q.args[1]);
      if(f.mutate){f.alpha=0;f.words[A+0x110]=0xffffffff;++f.words[A+0x1f4];}
      return true;
    };
    s.request=[](void*p,const FieldNpcShadowRequest&q,FieldNpcShadowResponse&r){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      if(q.function==0x80007328){assert(q.scalars[0]==7);r.scalar=f.normalized;}
      else if(q.function==0x80125cd4){
        assert(q.scalars[0]==(f.normalized<180?90:270));r.scalar=f.sine;
        if(f.mutate){f.words[A+0x28]=OtherD;f.words[A+0x8c]=bits(100);}
      } else if(q.function==0x8001577c){
        const std::array<float,7> expected{{10,21,30,0,-1,0,100}};
        assert(q.args[0]==0 && q.scalars==expected);
        r.word=f.hit?0x50000:0;
        if(f.hit && !f.incompleteHit)r.hit[1]=15;
        if(f.mutate){f.words[A+0x8c]=bits(100);f.words[A+0x90]=bits(200);}
      } else if(q.function==0x80039824){
        assert(q.args[0]==255);f.vivian=q.point;
        if(f.mutate)f.words[A+0x94]=bits(300);
      } else if(q.function==0x8002be98){
        auto slot=f.shadows->enter({q.scalars[0],q.scalars[1],q.scalars[2]},q.scalars[3]);
        if(!slot)return false;
        r.word=*slot;
        if(f.mutate)f.words[A]|=0x80000;
      } else if(q.function==0x8002bcfc){
        assert(q.args[0]==0 && q.args[1]==0);
        f.shadows->record(q.args[0])->flags&=~1u;
        if(f.mutate)f.words[A]|=0x100;
      } else assert(false);
      f.calls.push_back(q);f.trace.emplace_back('c',q.function,0);return true;
    };
    return s;
  }
  bool run(FieldNpcShadowCursor&c,FieldNpcDisplayFrontier start=FieldNpcDisplayFrontier::Shadow,float depth=-2500){
    return advanceFieldNpcMainShadow(A,start,depth,services(),c);
  }
};
void ordinaryTrace() {
  Fixture f;FieldNpcShadowCursor c;assert(f.run(c));
  using E=Fixture::Effect;
  const std::vector<E> expected={{'r',A,0},{'r',A+0x28,0},
    {'r',A+0x8c,bits(10)},{'r',A+0x90,bits(20)},{'r',A+0x94,bits(30)},
    {'r',A+0x14c,bits(6)},{'r',0x8041fce0,bits(3)},{'r',A+0xe0,bits(2)},
    {'r',A+0x154,bits(4)},{'r',0x8041fd68,0x3faa3d71},
    {'c',0x8002be98,0},{'r',A,0},{'r',A,0},{'r',A+0x110,0xffffffff}};
  assert(f.trace==expected && f.calls.size()==1 && f.shadows->count()==1);
  assert(f.shadows->record(0)->position==c.root && bits(c.radius)==0x41aa3d71);
  for(unsigned i=0;i<f.attempts;++i){
    Fixture h;h.fail=i;FieldNpcShadowCursor d;assert(!h.run(d));h.fail=~0u;
    assert(h.run(d) && h.trace==expected && h.shadows->count()==1);
    assert(h.run(d) && h.trace==expected);
  }
}
void allBranchesAndHolds(){
  for(unsigned variant=0;variant<7;++variant){
    auto setup=[&](Fixture&f){
      if(variant<2){f.words[A+0x28]=D;f.normalized=variant?270:90;f.sine=variant?-1:1;}
      if(variant==2)f.words[A]=0x40000|0x100;
      if(variant==3 || variant==4){f.words[A]=0x04040000;f.hit=variant==4;}
      if(variant==5)f.words[A]=0x80000|0x100;
      if(variant==6)f.words[A]=0x40000;
      f.words[A+0x110]=0;f.alpha=variant==6?0:255;
      f.mutate=variant!=2 && variant!=6;
    };
    Fixture f;setup(f);FieldNpcShadowCursor c;assert(f.run(c));
    if(variant<2){
      const std::array<float,3> expected{{variant?0.f:20.f,38,58}};
      assert(f.shadows->record(0)->position==expected && f.shadows->record(0)->flags==0);
    }
    if(variant==3 || variant==4){
      const std::array<float,3> copied{{10,variant==4?15.f:20.f,30}};
      const std::array<float,3> fresh{{100,200,300}};
      assert(f.vivian==copied && f.shadows->record(0)->position==fresh && c.radius==16);
    }
    if(variant==2 || variant==6)assert(f.shadows->count()==0);
    for(const auto&q:f.draws){
      assert(q.depth==-2500);
      if(q.args[2]!=0x8004bc3c)assert(q.args[0]==1);
    }
    for(unsigned i=0;i<f.attempts;++i){
      Fixture h;setup(h);h.fail=i;FieldNpcShadowCursor d;
      assert(!h.run(d));h.fail=~0u;
      assert(h.run(d) && h.trace==f.trace && h.shadows->count()==f.shadows->count());
    }
  }
}
void fusedOffset(){
  Fixture f;f.words[A+0x28]=D;f.words[A+0x8c]=0xbf800002;
  f.words[D+0x30]=0x3f800001;f.words[A+0xe0]=bits(1);
  float next;uint32_t w=0x3f800001;std::memcpy(&next,&w,4);f.sine=next;
  FieldNpcShadowCursor c;assert(f.run(c));
  // Exact cancellation: separate multiply/add would round this to zero.
  assert(bits(f.shadows->record(0)->position[0])==0x28800000);
}
void frontiersAndExceptional(){
  Fixture f;FieldNpcShadowCursor c;
  assert(!f.run(c,FieldNpcDisplayFrontier::Pending) && !c.active && f.trace.empty());
  assert(f.run(c,FieldNpcDisplayFrontier::History) && f.trace.empty());
  assert(!f.run(c,FieldNpcDisplayFrontier::Shadow));
  for(auto alpha:{0,255}){
    Fixture h;h.alpha=alpha;h.words[A+0x110]=0;FieldNpcShadowCursor d;
    const float nan=std::numeric_limits<float>::quiet_NaN();
    assert(h.run(d,FieldNpcDisplayFrontier::SeparatePose,nan));
    assert(h.shadows->count()==0 && h.queue->count()==0 && h.draws.size()==(alpha?3u:1u));
    assert(h.run(d,FieldNpcDisplayFrontier::SeparatePose,nan));
    assert(!h.run(d,FieldNpcDisplayFrontier::SeparatePose,0));
    auto alien=h.services();alien.memory.user=nullptr;
    assert(!advanceFieldNpcMainShadow(A,FieldNpcDisplayFrontier::SeparatePose,nan,alien,d));
  }
  Fixture bad;bad.words[A]=0x04000000;bad.hit=true;bad.incompleteHit=true;FieldNpcShadowCursor d;
  assert(!bad.run(d) && d.phase==60);
  const auto calls=bad.calls.size();assert(!bad.run(d) && bad.calls.size()==calls);
  Fixture angle;angle.words[A+0x28]=D;angle.normalized=std::numeric_limits<float>::quiet_NaN();
  FieldNpcShadowCursor e;assert(angle.run(e));
  assert(angle.calls[1].scalars[0]==270);
}
} // namespace
int main(){ordinaryTrace();allBranchesAndHolds();fusedOffset();frontiersAndExceptional();}
