#include "animation_pose_release.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Root=0x80415dc0,W=0x1000,P=0x10000,E=0x2000,T=0x3000;
using Op=std::tuple<char,uint32_t,uint32_t>;
struct Fixture {
 std::map<uint32_t,uint32_t> words;
 std::vector<Op> trace;
 int reject=-1;bool rejected=false;
 std::function<void(Fixture&,uint32_t,uint32_t)> called,stored;
 Fixture(){words={{Root,W},{W,E},{W+8,T},{W+0x10,P},{P,1},{P+4,0},{P+8,0},{P+0x10,0},{P+0x15c,44},{P+0x164,88},{E,1},{E+4,1},{E+8,55},{E+12,0},{T,1},{T+4,1},{T+8,66}};}
 bool gate(){if(!rejected && int(trace.size())==reject){rejected=true;return false;}return true;}
 BattleSequenceEntryServices services(){BattleSequenceEntryServices s;s.user=this;
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.gate())return {};auto i=f.words.find(a);if(i==f.words.end())return {};f.trace.emplace_back('r',a,i->second);return i->second;};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);if(!f.gate())return false;f.words[a]=v;f.trace.emplace_back('w',a,v);if(f.stored)f.stored(f,a,v);return true;};
  s.call=[](void* p,BattleSequenceCall& c){auto& f=*static_cast<Fixture*>(p);if(!f.gate())return false;f.trace.emplace_back('c',c.function,c.args[0]);if(f.called)f.called(f,c.function,c.args[0]);return true;};return s;
 }
 void run(){auto s=services();AnimationReleaseCursor c;unsigned visits=0;while(!releaseAnimationPose(0,s,c))assert(++visits<128);assert(!c.active && !c.pending);}
};
std::vector<Op> calls(const Fixture& f){std::vector<Op> out;for(auto op:f.trace)if(std::get<0>(op)=='c')out.push_back(op);return out;}
void everyHold(const Fixture& input){
 Fixture expected=input;expected.run();
 for(unsigned i=0;i<expected.trace.size();++i){Fixture f=input;f.reject=int(i);f.run();assert(f.rejected && f.words==expected.words && f.trace==expected.trace);}
}
void ordinary(){
 Fixture f;const auto original=f.words;f.run();
 const std::vector<Op> expected{{'r',Root,W},{'r',W+0x10,P},{'r',P+4,0},{'r',P,1},{'r',P,1},
  {'r',Root,W},{'r',P+0x10,0},{'r',W,E},{'r',E+4,1},{'w',E+4,0},{'r',E+4,0},{'w',E,0},
  {'r',E+12,0},{'r',Root,W},{'r',W+8,T},{'r',T+4,1},{'w',T+4,0},{'r',T+4,0},{'r',T+8,66},
  {'c',0x8018a260,66},{'w',T,0},{'r',E+8,55},{'c',0x8018a260,55},{'w',P,0}};
 assert(f.trace==expected);
 for(auto v:original)if(v.first!=P && v.first!=E && v.first!=E+4 && v.first!=T && v.first!=T+4)assert(f.words[v.first]==v.second);
 everyHold(Fixture{});
 Fixture shared;shared.words[E+4]=3;everyHold(shared);shared.run();assert(shared.words[E+4]==2 && shared.words[E]==1 && shared.words[T+4]==1 && !shared.words[P] && calls(shared).empty());
 Fixture texture;texture.words[T+4]=2;texture.run();assert(texture.words[T+4]==1 && texture.words[T]==1 && calls(texture)==(std::vector<Op>{{'c',0x8018a260,55}}));
 // Signed comparison follows wrapping decrement and a FRESH counter load.
 Fixture underflow;underflow.words[E+4]=underflow.words[T+4]=0;everyHold(underflow);underflow.run();assert(underflow.words[E+4]==0xffffffff && underflow.words[T+4]==0xffffffff && !underflow.words[P]);
 Fixture overflow;overflow.words[E+4]=0x80000000;overflow.run();assert(overflow.words[E+4]==0x7fffffff && calls(overflow).empty());
 Fixture fresh;fresh.stored=[](Fixture& f,uint32_t a,uint32_t){if(a==E+4)f.words[a]=1;};everyHold(fresh);fresh.run();assert(fresh.words[E]==1 && calls(fresh).empty());
}
void borrowed(){
 for(uint32_t n:{1u,2u,0x80000000u}){Fixture f;f.words[P+4]=9;f.words[P+8]=n;f.words[P]=0xc1;everyHold(f);f.run();assert(f.words[P+8]==n-1 && f.words[P]==0xc1 && f.words[E+4]==1 && calls(f).empty());}
 Fixture f;f.words[P+4]=1;f.words[P]=0xc1;everyHold(f);f.run();assert(f.words[P+8]==0xffffffff && !f.words[P] && calls(f)==(std::vector<Op>{{'c',0x8018a260,66},{'c',0x8018a260,55}}));
}
void mutation(){
 Fixture f;f.words[P]=0x41;
 f.called=[](Fixture& f,uint32_t fn,uint32_t arg){
  if(fn==0x8002f878 && arg==44){f.words[P]=0x81;f.words[Root]=0x1400;f.words[0x1400]=E;f.words[0x1408]=0x4000;f.words[E+12]=2;f.words[0x4018]=1;f.words[0x401c]=1;f.words[0x4020]=77;}
  if(fn==0x8018a260 && arg==77){assert(!f.words[E] && f.words[0x4018]==1 && f.words[P]==0x81);f.words[E+8]=99;}
 };
 everyHold(f);f.run();assert(calls(f)==(std::vector<Op>{{'c',0x8002f878,44},{'c',0x8002f878,88},{'c',0x8018a260,77},{'c',0x8018a260,99}}));assert(!f.words[0x4018] && !f.words[P] && f.words[T]==1);
 // Captured child arguments survive failed admission and changes by siblings.
 Fixture held;held.reject=19;auto s=held.services();AnimationReleaseCursor c;assert(!releaseAnimationPose(0,s,c) && c.pending==0x8018a260 && c.queued && c.call.args[0]==66);
 held.words[T+8]=100;auto other=s;other.user=&f;const auto trace=held.trace;assert(!releaseAnimationPose(1,s,c) && !releaseAnimationPose(0,other,c) && held.trace==trace);
 assert(releaseAnimationPose(0,s,c));assert(calls(held)==(std::vector<Op>{{'c',0x8018a260,66},{'c',0x8018a260,55}}));
}
}
void runAnimationReleaseFixture(){ordinary();borrowed();mutation();}
int main(){runAnimationReleaseFixture();}
