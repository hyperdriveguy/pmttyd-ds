#include "field_npc_main_display.hpp"
#include "display_queue.hpp"
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
constexpr uint32_t A = 0x10000;
uint32_t bits(float f) { uint32_t w; std::memcpy(&w, &f, 4); return w; }
struct Fixture {
  using Effect = std::tuple<char, uint32_t, uint32_t>;
  std::vector<Effect> trace;
  std::vector<FieldNpcDisplayRequest> calls;
  std::map<uint32_t, uint32_t> words;
  std::array<uint8_t,4> color{{255,255,255,255}};
  uint16_t timer = 0;
  std::array<float,3> projected{{0,0,-.5f}};
  unsigned attempts = 0, fail = ~0u, cameras = 0;
  bool mutate = false;
  std::unique_ptr<DisplayQueue> queue = std::make_unique<DisplayQueue>();
  Fixture() {
    words = {{A,0},{A+0x104,7},{A+0x114,0xffffffff},{A+0x1f4,4},
      {A+0x1d4,0x10000},{0x8041fcd8,bits(-1)},{0x8041fc7c,0},
      {0x8041fd5c,bits(-2)},{0x8041fd34,bits(2)},
      {0x8041fd60,bits(5000)},{0x8041fd64,bits(10000)}};
  }
  bool ready() { return attempts++ != fail; }
  FieldNpcDisplayServices services() {
    FieldNpcDisplayServices s; s.memory.user=this;
    s.memory.word=[](void*p,uint32_t a)->std::optional<uint32_t>{
      auto &f=*static_cast<Fixture*>(p);
      if(!f.ready() || !f.words.count(a))return {};
      f.trace.emplace_back('r',a,f.words[a]); return f.words[a];
    };
    s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{
      auto &f=*static_cast<Fixture*>(p);
      if(!f.ready() || a<A+0x114 || a>A+0x117)return {};
      auto v=f.color[a-A-0x114];f.trace.emplace_back('b',a,v);return v;
    };
    s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || a!=A+0x320)return {};
      f.trace.emplace_back('h',a,f.timer);return f.timer;
    };
    s.request=[](void*p,const FieldNpcDisplayRequest&q,FieldNpcDisplayResponse&r){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      if(q.function==0x8000f2fc) {
        assert(q.args[0]==4); r.word=++f.cameras==1?0x20000:0x30000;
      } else if(q.function==0x8029e6a4) {
        assert(q.args[0]==0x2011c && q.args[1]==A+0x8c);
        r.point={11,12,13};
      } else if(q.function==0x8029eb08) {
        const std::array<float,3> expected{{11,12,13}};
        assert(q.args[0]==0x3015c && q.point==expected);r.point=f.projected;
      } else if(q.function==0x80040730 || q.function==0x80040710) {
        assert(q.args[0]==7 && q.args[1]==0x40);
        if(f.mutate) { f.words[A+0x114]=0x12345678; f.words[A+0x104]=9; }
      } else if(q.function==0x800406b4) {
        assert(q.color==f.words[A+0x114]);
        assert(q.args[0]==(f.mutate?9u:7u));
      } else if(q.function==0x80010bc4) {
        assert(q.args[3]==A);
        auto admission=f.queue->enter(q.args[0],q.args[1],q.args[2],q.args[3],q.depth);
        if(admission==DisplayAdmission::Unavailable)return false;
        if(f.mutate) { ++f.words[A+0x1f4];f.color[3]=0;f.projected[2]=100; }
      } else assert(false);
      f.calls.push_back(q);f.trace.emplace_back('c',q.function,q.args[0]);return true;
    };
    return s;
  }
  FieldNpcDisplayFrontier run(FieldNpcDisplayCursor&c) {
    return advanceFieldNpcMainDisplay(A,services(),c);
  }
};
void ordinaryAndHolds() {
  Fixture f;FieldNpcDisplayCursor c;
  assert(f.run(c)==FieldNpcDisplayFrontier::Shadow);
  using E=Fixture::Effect;
  const std::vector<E> expected={
    {'r',A,0},{'c',0x8000f2fc,4},{'c',0x8029e6a4,0x2011c},
    {'c',0x8000f2fc,4},{'c',0x8029eb08,0x3015c},
    {'r',0x8041fcd8,bits(-1)},{'r',0x8041fc7c,0},
    {'r',0x8041fd5c,bits(-2)},{'r',0x8041fd34,bits(2)},
    {'b',A+0x114,255},{'b',A+0x115,255},{'b',A+0x116,255},{'b',A+0x117,255},
    {'r',A+0x104,7},{'c',0x80040710,7},
    {'r',0x8041fd60,bits(5000)},{'r',0x8041fc7c,0},
    {'r',0x8041fd64,bits(10000)},{'r',A,0},{'r',0x8041fcd8,bits(-1)},
    {'h',A+0x320,0},{'b',A+0x117,255},
    {'r',A+0x1f4,4},{'c',0x80010bc4,4},
    {'r',A+0x1f4,4},{'c',0x80010bc4,4},
    {'r',A+0x1f4,4},{'c',0x80010bc4,4}};
  assert(f.trace==expected && c.depth==-2500 && f.queue->count()==3);
  for(unsigned i=0;i<f.attempts;++i) {
    Fixture h;h.fail=i;FieldNpcDisplayCursor held;
    assert(h.run(held)==FieldNpcDisplayFrontier::Pending);
    h.fail=~0u;assert(h.run(held)==FieldNpcDisplayFrontier::Shadow);
    assert(h.trace==expected && h.queue->count()==3);
    assert(h.run(held)==FieldNpcDisplayFrontier::Shadow && h.trace==expected);
  }
  auto alien=f.services();alien.memory.user=nullptr;
  assert(advanceFieldNpcMainDisplay(A,alien,c)==FieldNpcDisplayFrontier::Pending);
  assert(advanceFieldNpcMainDisplay(A+0x340,f.services(),c)==FieldNpcDisplayFrontier::Pending);
}
void variants() {
  for(unsigned channel=0;channel<4;++channel) {
    Fixture f;f.color[channel]=0;f.mutate=true;FieldNpcDisplayCursor c;
    assert(f.run(c)==FieldNpcDisplayFrontier::Shadow);
    assert(f.calls[4].function==0x80040730 && f.calls[5].color==0x12345678);
    unsigned checks=0;for(auto t:f.trace)if(std::get<0>(t)=='b')++checks;
    assert(checks==channel+2);
    if(channel==3)assert(f.queue->count()==1 && f.calls.back().args[2]==0x8004c7e0);
    else {
      assert(f.queue->count()==3);
      for(unsigned i=0;i<3;++i) {
        const auto&q=f.calls[6+i];
        assert(q.args[0]==4+i && q.args[1]==i && q.args[2]==0x8004c50c && q.depth==-2500);
      }
    }
    // Every primitive in nonwhite branches can hold without replaying prior calls.
    const unsigned count=f.attempts;
    for(unsigned i=0;i<count;++i) {
      Fixture h;h.color[channel]=0;h.mutate=true;h.fail=i;FieldNpcDisplayCursor d;
      assert(h.run(d)==FieldNpcDisplayFrontier::Pending);h.fail=~0u;
      assert(h.run(d)==FieldNpcDisplayFrontier::Shadow && h.trace==f.trace);
    }
  }
}
void gates() {
  Fixture hidden;hidden.words[A]=0x800;FieldNpcDisplayCursor c;
  assert(hidden.run(c)==FieldNpcDisplayFrontier::History && hidden.trace.size()==1);
  for(auto pos:std::vector<std::array<float,3>>{{0,0,-1},{0,0,0},{-2.01f,0,-.5f},
      {2.01f,0,-.5f},{0,-2.01f,-.5f},{0,2.01f,-.5f}}) {
    for(bool force:{false,true}) {
      Fixture f;f.projected=pos;f.words[A]=force?0x40:0;FieldNpcDisplayCursor d;
      assert(f.run(d)==(force?FieldNpcDisplayFrontier::Shadow:FieldNpcDisplayFrontier::History));
    }
  }
  for(unsigned axis=0;axis<3;++axis)for(bool force:{false,true}) {
    Fixture f;f.projected[axis]=std::numeric_limits<float>::quiet_NaN();
    f.words[A]=force?0x40:0;FieldNpcDisplayCursor d;
    assert(f.run(d)==(force?FieldNpcDisplayFrontier::Shadow:FieldNpcDisplayFrontier::History));
    if(force && axis==2)assert(std::isnan(d.depth) && f.queue->count()==0);
  }
  for(float x:{-2.f,2.f})for(float y:{-2.f,2.f}) {
    Fixture f;f.projected={x,y,-.5f};FieldNpcDisplayCursor d;
    assert(f.run(d)==FieldNpcDisplayFrontier::Shadow);
  }
  for(unsigned mode=0;mode<4;++mode) {
    Fixture f;f.words[A]=mode==0?0x80:0;f.timer=mode?2:0;
    f.words[A+0x1d4]=mode==2?0x10000:0;FieldNpcDisplayCursor d;
    assert(f.run(d)==(mode==2?FieldNpcDisplayFrontier::Shadow:FieldNpcDisplayFrontier::SeparatePose));
    if(mode!=2)assert(f.queue->count()==0);
  }
  for(float z:{-3.f,-1.f,3.f}) {
    Fixture f;f.words[A]=0x40;f.projected[2]=z;FieldNpcDisplayCursor d;
    assert(f.run(d)==FieldNpcDisplayFrontier::Shadow);
    assert(d.depth==(z<=-1?0.f:-10000.f));
    if(z<=-1)assert(std::signbit(d.depth));
    if(z<-1)for(auto t:f.trace)assert(std::get<1>(t)!=0x8041fd64);
  }
}
} // namespace
int main() { ordinaryAndHolds();variants();gates(); }
