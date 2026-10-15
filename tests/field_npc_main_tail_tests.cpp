#include "field_npc_main_tail.hpp"
#include "display_queue.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <memory>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t GP=0x10000,F=0x20000,F2=0x30000,M=0x40000,E=F+0x550;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
struct Fixture {
  using Effect=std::tuple<char,uint32_t,uint32_t>;
  std::map<uint32_t,uint32_t> words;
  std::map<uint32_t,uint8_t> bytes;
  std::map<uint32_t,uint16_t> halves;
  std::vector<Effect> trace;
  std::vector<FieldNpcTailRequest> calls;
  std::vector<FieldNpcDisplayRequest> draws;
  std::unique_ptr<DisplayQueue> queue=std::make_unique<DisplayQueue>();
  unsigned attempts=0,fail=~0u;
  uint32_t jump=1,fire=0;
  bool mutate=false, changeCount=false;
  Fixture(){
    words={{0x80415fd0,GP},{GP+0x168,F},{E,0},{E+4,99},{E+8,2},{E+0xc,0},
      {E+0x10,111},{E+0x14,222},{F2+0x57c,5},{F2+0x578,2},{F2+0x574,9},
      {M+0x8c,0x7fc01234},{M+0x90,bits(10)},{M+0x94,0x80000000},
      {0x8041fc7c,0},{0x8041fce4,bits(-600)},{0x8041fce8,bits(600)},
      {0x8041fc84,bits(60)}};
    bytes={{M+0x3c,2},{F+0x54d,0xaa},{F+0x54c,0xbb},{F+0x54e,0xcc},{F2+0x54d,0xdd}};
    halves[F]=0;
  }
  bool ready(){return attempts++!=fail;}
  FieldNpcTailServices services(){
    FieldNpcTailServices s;s.memory.user=this;
    s.memory.word=[](void*p,uint32_t a)->std::optional<uint32_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || !f.words.count(a))return {};
      f.trace.emplace_back('r',a,f.words[a]);return f.words[a];
    };
    s.memory.store=[](void*p,uint32_t a,uint32_t v){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      f.trace.emplace_back('w',a,v);f.words[a]=v;
      if(f.changeCount && a==E+0xc)f.words[a]=80;
      if(f.mutate && a==E+0x14)f.words[0x8041fc7c]=bits(7);
      return true;
    };
    s.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || !f.bytes.count(a))return {};
      f.trace.emplace_back('b',a,f.bytes[a]);return f.bytes[a];
    };
    s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || !f.halves.count(a))return {};
      f.trace.emplace_back('h',a,f.halves[a]);return f.halves[a];
    };
    s.storeByte=[](void*p,uint32_t a,uint8_t v){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      f.trace.emplace_back('B',a,v);f.bytes[a]=v;return true;
    };
    s.request=[](void*p,const FieldNpcTailRequest&q,FieldNpcTailResponse&r){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      if(q.function==0x80006c8c){
        assert(q.args[2]==20);
        assert((q.args[0]==5 && q.scalars[0]==-600 && q.scalars[1]==0) ||
               (q.args[0]==1 && q.scalars[0]==0 && q.scalars[1]==600));
        r.scalar=-123;
      } else if(q.function==0x8004727c)f.words[GP+0x168]=F2;
      else if(q.function==0x8005e0c0)r.word=M;
      else if(q.function==0x801fcc98){r.word=f.jump;if(f.mutate)f.bytes[M+0x3c]=0;}
      else if(q.function==0x801fe1b0)r.word=f.fire;
      else if(q.function==0x80216994){
        const std::array<uint32_t,3> point{{0x7fc01234,bits(70),0x80000000}};
        assert(q.args[0]==5 && q.point==point);
        if(f.mutate){f.words[GP+0x168]=0x90000;f.words[M+0x90]=bits(100);}
      } else assert(q.function==0x800470dc || q.function==0x80046a78);
      f.calls.push_back(q);f.trace.emplace_back('c',q.function,q.args[0]);return true;
    };
    s.queue=[](void*p,const FieldNpcDisplayRequest&q,FieldNpcDisplayResponse&){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      assert(q.function==0x80010bc4 && q.args[0]==9 && q.args[1]==2 &&
             q.args[2]==0x80047dfc && q.args[3]==E && bits(q.depth)==f.words[E+0x14]);
      if(f.queue->enter(q.args[0],q.args[1],q.args[2],q.args[3],q.depth)==DisplayAdmission::Unavailable)return false;
      f.draws.push_back(q);f.trace.emplace_back('q',q.args[3],bits(q.depth));return true;
    };
    return s;
  }
  bool run(FieldNpcTailCursor&c){return advanceFieldNpcMainTail(services(),c);}
};
void traceAndHolds(){
  Fixture f;FieldNpcTailCursor c;assert(f.run(c));
  const std::vector<Fixture::Effect> expected={{'r',0x80415fd0,GP},{'r',GP+0x168,F},
    {'r',E,0},{'h',F,0},{'B',F+0x54d,0}};
  assert(f.trace==expected && f.bytes[F+0x54c]==0xbb && f.bytes[F+0x54e]==0xcc);
  for(unsigned variant=0;variant<11;++variant){
    auto setup=[&](Fixture&h){
      if(variant<5){h.words[E]=1;h.words[E+0xc]=variant==0?0:variant==1?19:variant==2?79:variant==3?99:0x7fffffff;}
      if(variant==5){h.words[E]=1;h.words[E+8]=1;}
      if(variant>=6){h.halves[F]=variant==6?1:variant==7?3:5;}
      h.mutate=true;
      if(variant>=9){h.halves[F]=1;h.mutate=false;h.fire=1;h.jump=variant==9?1:0;}
    };
    Fixture base;setup(base);FieldNpcTailCursor done;assert(base.run(done));
    for(unsigned i=0;i<base.attempts;++i){
      Fixture h;setup(h);h.fail=i;FieldNpcTailCursor held;assert(!h.run(held));h.fail=~0u;
      assert(h.run(held) && h.trace==base.trace && h.words==base.words && h.bytes==base.bytes);
      assert(h.run(held) && h.trace==base.trace);
    }
  }
  auto alien=f.services();alien.memory.user=nullptr;
  assert(!advanceFieldNpcMainTail(alien,c));
}
void effects(){
  for(uint32_t state:{1u,2u,3u,0xffffffffu})for(uint32_t before:{0u,18u,19u,59u,60u,79u,80u,98u,99u,0x7fffffffu,0xffffffffu}){
    Fixture f;f.words[E]=1;f.words[E+8]=state;f.words[E+0xc]=before;FieldNpcTailCursor c;assert(f.run(c));
    const uint32_t next=before+1;
    const bool negative=next>0x7fffffff;
    const bool draw=state==1?(negative || next<=60):state==2?(negative || next<100):false;
    assert(f.queue->count()==unsigned(draw) && f.words[E]==1);
    assert(f.words[E+0xc]==((state==1 || state==2)?next:before));
    if((state==1 || state==2) && !draw)assert(f.words[E+4]==0 && f.words[E+0x14]==222);
    if(state==2 && draw){
      if(!negative && next>=20 && next<80)assert(f.calls.empty() && f.words[E+0x10]==0);
      else assert(f.calls[0].args[0]==(negative || next<20?5u:1u) && f.words[E+0x10]==bits(-123));
    }
  }
  Fixture f;f.words[E]=1;f.changeCount=true;FieldNpcTailCursor c;assert(f.run(c));
  assert(f.calls[0].args[0]==1 && f.calls[0].args[1]==0); // count is reloaded
  for(uint32_t zero:{0x80000000u,0x7fc01234u}){
    Fixture z;z.words[E]=1;z.words[E+8]=1;z.words[0x8041fc7c]=zero;z.mutate=true;
    FieldNpcTailCursor d;assert(z.run(d));assert(bits(z.draws[0].depth)==zero);
    assert(z.queue->count()==(zero==0x80000000u?1u:0u));
  }
}
void encounter(){
  for(uint32_t jump:{0u,1u})for(uint32_t fire:{0u,1u})for(uint32_t score:{0u,5u,0xffffffffu})for(uint32_t count:{1u,2u,0x80000000u}){
    Fixture f;f.halves[F]=1;f.jump=jump;f.fire=fire;f.words[F2+0x57c]=score;f.words[F2+0x578]=count;
    FieldNpcTailCursor c;assert(f.run(c));
    unsigned scores=0;for(auto q:f.calls)if(q.function==0x80216994)++scores;
    assert(scores==unsigned(jump && score==5 && count==2));
    assert(f.words[F2+0x57c]==(jump?0:score) && f.words[F2+0x578]==(jump?0:count));
    assert(f.words[F2+0x574]==(fire?9u:0u) && f.bytes[F+0x54d]==0 && f.bytes[F2+0x54d]==0xdd);
  }
  Fixture f;f.halves[F]=1;f.mutate=true;FieldNpcTailCursor c;assert(f.run(c));
  for(auto q:f.calls)assert(q.function!=0x801fe1b0); // fresh second mode gate
  assert(c.field==F && c.secondField==F2 && f.bytes[F+0x54d]==0);
  for(uint16_t state:{0,2,3,4,5,6,0xffff}){
    Fixture h;h.halves[F]=state;FieldNpcTailCursor d;assert(h.run(d));
    const bool call=state==2 || state==3 || state==5;
    assert(h.calls.size()==unsigned(call));
    if(call)assert(h.calls[0].function==(state==5?0x80046a78u:0x800470dcu));
  }
}
}
int main(){traceAndHolds();effects();encounter();}
