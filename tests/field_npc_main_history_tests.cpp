#include "field_npc_main_history.hpp"
#include <cassert>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A=0x10000;
struct Fixture {
  using Effect=std::tuple<char,uint32_t,uint32_t>;
  std::map<uint32_t,uint32_t> words;
  std::vector<Effect> trace;
  unsigned attempts=0,fail=~0u;
  bool mutate=false;
  Fixture(){
    for(uint32_t o=0x8c;o<=0xdc;o+=4)words[A+o]=o;
    words[A]=0x100;words[0x8041e8ac]=2;
    words[A+0x8c]=0x7fc01234;words[A+0x90]=0x80000000;words[A+0x94]=0xff800001;
  }
  bool ready(){return attempts++!=fail;}
  FieldNpcClockServices services(){
    FieldNpcClockServices s;s.user=this;
    s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{
      auto &f=*static_cast<Fixture*>(p);if(!f.ready() || !f.words.count(a))return {};
      f.trace.emplace_back('r',a,f.words[a]);return f.words[a];
    };
    s.store=[](void*p,uint32_t a,uint32_t v){
      auto &f=*static_cast<Fixture*>(p);if(!f.ready())return false;
      f.words[a]=v;f.trace.emplace_back('w',a,v);
      if(f.mutate && a==A+0x9c){
        f.words[A+0x8c]=123;f.words[A+0x90]=456;f.words[A+0x94]=789;
        f.words[A]=0x100;f.words[0x8041e8ac]=2;
      }
      return true;
    };
    return s;
  }
  bool run(FieldNpcHistoryCursor&c){return advanceFieldNpcMainHistory(A,services(),c);}
};
void traceAndHolds(){
  Fixture f;const auto before=f.words;FieldNpcHistoryCursor c;assert(f.run(c));
  std::vector<Fixture::Effect> expected;
  auto copy=[&](uint32_t from,uint32_t to){
    expected.emplace_back('r',A+from,before.at(A+from));
    expected.emplace_back('r',A+from+4,before.at(A+from+4));
    expected.emplace_back('w',A+to,before.at(A+from));
    expected.emplace_back('w',A+to+4,before.at(A+from+4));
    expected.emplace_back('r',A+from+8,before.at(A+from+8));
    expected.emplace_back('w',A+to+8,before.at(A+from+8));
  };
  copy(0x8c,0x98);expected.emplace_back('r',A,0x100);
  expected.emplace_back('r',0x8041e8ac,2);
  copy(0xc8,0xd4);copy(0xbc,0xc8);copy(0xb0,0xbc);copy(0xa4,0xb0);copy(0x8c,0xa4);
  assert(f.trace==expected && f.words[A+0xa4]==0x7fc01234 &&
         f.words[A+0xa8]==0x80000000 && f.words[A+0xac]==0xff800001 &&
         f.words[A+0xd4]==0xc8 && f.words[A+0xb0]==0xa4);
  for(unsigned i=0;i<f.attempts;++i){
    Fixture h;h.fail=i;FieldNpcHistoryCursor d;assert(!h.run(d));h.fail=~0u;
    assert(h.run(d) && h.trace==expected && h.words==f.words);
    assert(h.run(d) && h.trace==expected);
  }
  auto alien=f.services();alien.user=nullptr;
  assert(!advanceFieldNpcMainHistory(A,alien,c));
  assert(!advanceFieldNpcMainHistory(A+0x340,f.services(),c));
}
void gatesAndFreshness(){
  for(uint32_t flags:{0u,0x100u})for(uint32_t count:{0u,1u,2u,0xffffffffu}){
    Fixture f;f.words[A]=flags;f.words[0x8041e8ac]=count;FieldNpcHistoryCursor c;
    assert(f.run(c));
    assert(f.trace.size()==((flags && !(count&1))?38u:(flags?8u:7u)));
    assert(f.words[A+0x98]==0x7fc01234);
    if(!flags || (count&1))assert(f.words[A+0xa4]==0xa4);
    if(!flags)for(auto e:f.trace)assert(std::get<1>(e)!=0x8041e8ac);
  }
  Fixture f;f.mutate=true;f.words[A]=0;f.words[0x8041e8ac]=1;
  FieldNpcHistoryCursor c;assert(f.run(c));
  assert(f.words[A+0x98]==0x7fc01234 && f.words[A+0x9c]==0x80000000 &&
         f.words[A+0xa0]==789 && f.words[A+0xa4]==123 && f.words[A+0xa8]==456 &&
         f.words[A+0xac]==789);
  // A hold after captured XY cannot turn them into fresh values on resume.
  Fixture h;h.fail=3;FieldNpcHistoryCursor d;assert(!h.run(d));
  h.words[A+0x8c]=11;h.words[A+0x90]=22;h.words[A+0x94]=33;h.fail=~0u;
  assert(h.run(d) && h.words[A+0x98]==0x7fc01234 && h.words[A+0x9c]==0x80000000 &&
         h.words[A+0xa0]==33 && h.words[A+0xa4]==11);
}
} // namespace
int main(){traceAndHolds();gatesAndFreshness();}
