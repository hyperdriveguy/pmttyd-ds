#include "animation_inner.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t W=0x90000000,P=0x91000000,J=0x92000000,E=0x93000000,H=0x94000000,D=0x95000000,T=0x96000000,G=0x97000000;
struct Op {char k;uint32_t a,b;bool operator==(const Op& o) const{return k==o.k && a==o.a && b==o.b;}};
struct Fixture {
 std::map<uint32_t,uint32_t> ram{{0x80415dc0,W},{P+0x10,2},{W,J},{P+0x14,3},{J+40,E},{P+0x28,7},{E+0xa0,H},{P+0x2c,9},{H,D},{P+0x30,0x3e800000},{D+0x1ac,T},{T+3*64+0x3c,0x98000000},{W+0x10c,0},{P+0xc,2},{D+0xc4,1},{P+0xf4,0xffffffff},{P+0xf0,0xffffffff},{P+0xe8,0x1800},{P+0xe0,0},{P,0},{0x8041fb28,0},{0x8041fb24,0x3f800000},{P+0x3c,0},{W+0xe8,0},{P+0x4c,0x99000000},{P+0x54,0x9a000000},{D+0xf0,2},{D+0xf8,3},{D+0x144,5},{W+0x10,P},{P+4,0},{W+0xf0,10},{W+0xf4,8},{W+0xec,0x9b000000},{W+0xf8,0x9c000000},{W+0xfc,0x9d000000},{0x8041e664,0},{0x8041e660,G},{G+4,6},{G+0x19,255},{G+0x1d,255}};
 std::vector<Op> trace;unsigned calls=0,hold=0;bool held=false;void (*after)(Fixture&,char,uint32_t)=nullptr;
 AnimationInnerTraversal traversal;BattleActMatrix copied{};AnimationInnerDrawRequest input;
 Fixture(){input.pose=P;input.pass=2;input.matrix={1,0,0,4,0,1,0,5,0,0,1,6};}
 bool admit(char k,uint32_t a,uint32_t b=0){if(++calls==hold && !held){held=true;return false;}trace.push_back({k,a,b});return true;}
 void changed(char k,uint32_t a){if(after)after(*this,k,a);}
 AnimationInnerServices services(){AnimationInnerServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);auto it=f.ram.find(a);if(it==f.ram.end() || !f.admit('r',a))return {};auto w=it->second;f.changed('r',a);return w;};
  s.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& f=*static_cast<Fixture*>(u);auto it=f.ram.find(a);if(it==f.ram.end() || !f.admit('h',a))return {};auto w=uint16_t(it->second);f.changed('h',a);return w;};
  s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(u);auto it=f.ram.find(a);if(it==f.ram.end() || !f.admit('b',a))return {};auto w=uint8_t(it->second);f.changed('b',a);return w;};
  s.store=[](void* u,uint32_t a,uint32_t w){auto& f=*static_cast<Fixture*>(u);if(!f.admit('w',a,w))return false;f.ram[a]=w;f.changed('w',a);return true;};
  s.gx=[](void* u,const BattleAudienceGXRequest& q){auto& f=*static_cast<Fixture*>(u);if(!f.admit('g',q.function,uint32_t(q.arguments[0])))return false;f.changed('g',q.function);return true;};
  s.callback=[](void* u,uint32_t fn,uint32_t pass){auto& f=*static_cast<Fixture*>(u);if(!f.admit('c',fn,pass))return false;f.changed('c',fn);return true;};
  s.facing.user=this;s.facing.word=s.word;s.facing.store=s.store;
  s.main=[](void* u,uint32_t h){auto& f=*static_cast<Fixture*>(u);if(!f.admit('m',h))return false;f.changed('m',h);return true;};
  s.copy=[](void* u,const BattleActMatrix& m,uint32_t a){auto& f=*static_cast<Fixture*>(u);if(!f.admit('p',a))return false;f.copied=m;f.changed('p',a);return true;};
  s.flush=[](void* u,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(u);if(!f.admit('f',a,n))return false;f.changed('f',a);return true;};
  s.invalidate=[](void* u){auto& f=*static_cast<Fixture*>(u);if(!f.admit('i',0x802b4df4))return false;f.changed('i',0);return true;};
  s.traverse=[](void* u,const AnimationInnerTraversal& q){auto& f=*static_cast<Fixture*>(u);if(!f.admit('t',q.group,q.current))return false;f.traversal=q;f.changed('t',q.group);return true;};return s;
 }
 void finish(AnimationInnerCursor& c){for(unsigned i=0;i<10000;++i)if(resumeAnimationInner(input,services(),c,1))return;assert(false);}
};
void exactPrefixAndAllHolds(){
 Fixture f;AnimationInnerCursor c;f.finish(c);assert(!c.active && f.traversal.parent==0xffffffff && f.traversal.group==4 && f.traversal.current==7 && f.traversal.next==9 && f.traversal.fraction==.25f && f.copied==f.input.matrix);
 const std::vector<Op> prefix{{'r',0x80415dc0,0},{'r',P+0x10,0},{'r',W,0},{'r',P+0x14,0},{'r',J+40,0},{'r',P+0x28,0},{'r',E+0xa0,0},{'r',P+0x2c,0},{'r',H,0},{'r',P+0x30,0},{'r',D+0x1ac,0},{'r',T+3*64+0x3c,0},{'r',W+0x10c,0},{'r',P+0xc,0},{'r',D+0xc4,0}};
 assert(std::vector<Op>(f.trace.begin(),f.trace.begin()+prefix.size())==prefix);
 assert(f.ram[W+0xe0]==2 && f.ram[W+0x18]==P && f.ram[W+0x1c]==0x98000000 && f.ram[W+0x20]==D && f.ram[0x8041e8a0]==0x803d8e70);
 for(unsigned h=1;h<=f.trace.size();++h){Fixture x;x.hold=h;AnimationInnerCursor cursor;x.finish(cursor);assert(x.held && x.trace==f.trace && x.ram==f.ram);}
}
void rejectsAndMaterial(){
 Fixture null;null.ram[T+3*64+0x3c]=0;null.ram.erase(W+0x10c);AnimationInnerCursor n;null.finish(n);assert(null.trace.size()==12);
 for(uint32_t mode:{0u,1u,0xffffffffu})for(uint32_t pose:{0u,1u,2u,0xffffffffu}){Fixture f;f.ram[W+0x10c]=mode;f.ram[P+0xc]=pose;AnimationInnerCursor c;f.finish(c);const bool reject=mode?pose==0:pose==1;assert(reject?f.trace.size()==14:f.trace.back().k=='t');}
 Fixture custom;custom.ram[P+0xe0]=0x12345678;custom.ram[P+0xf4]=0xffffff80;AnimationInnerCursor c;custom.finish(c);assert(!custom.ram.count(W+0xe0));unsigned callbacks=0;for(auto op:custom.trace){assert(op.k!='g');if(op.k=='c'){++callbacks;assert(op.a==0x12345678 && op.b==3);}}assert(callbacks==1);
 // Fresh map index then base on BOTH predicates; first callback can change
 // the second group's identity without replaying the first query.
 Fixture map;map.ram[P+0xe8]=0;map.after=[](Fixture& f,char k,uint32_t a){if(k=='b' && a==G+0x19){f.ram[0x8041e664]=1;f.ram[G+0x2f4+4]=4;f.ram[G+0x2f4+0x1d]=128;}};AnimationInnerCursor m;map.finish(m);assert(map.ram[W+0xe0]==3);std::vector<uint32_t> group;for(auto op:map.trace)if(op.a==0x8041e664 || op.a==0x8041e660)group.push_back(op.a);assert((group==std::vector<uint32_t>{0x8041e664,0x8041e660,0x8041e664,0x8041e660}));
 for(unsigned h=1;h<=map.trace.size();++h){Fixture x;x.ram[P+0xe8]=0;x.after=map.after;x.hold=h;AnimationInnerCursor a;x.finish(a);assert(x.held && x.trace==map.trace && x.ram==map.ram);}
 Fixture captured;captured.ram[P+0xc]=0;captured.after=[](Fixture& f,char k,uint32_t a){if(k=='r' && a==W+0x10c)f.ram[a]=1;};AnimationInnerCursor cap;captured.finish(cap);assert(captured.trace.back().k=='t');
 Fixture raw;raw.input.pass=64;raw.ram[D+0xc4]=1;AnimationInnerCursor r;raw.finish(r);assert(raw.ram[W+0xe0]==64);for(auto op:raw.trace)assert(op.k!='g');
}
void ringAndFreshFlush(){
 Fixture ring;ring.ram[P+0x3c]=0xffffffff;ring.ram[P+4]=2;AnimationInnerCursor c;ring.finish(c);assert(ring.ram[W+0xf4]==5 && ring.ram[W+0xf8]==0x9b000000 && ring.ram[W+0xfc]==0x9b000018 && ring.ram[W+0xe8]==0);unsigned mains=0;for(auto op:ring.trace)if(op.k=='m'){++mains;assert(op.a==0);}assert(mains==1);
 for(unsigned h=1;h<=ring.trace.size();++h){Fixture x;x.ram[P+0x3c]=0xffffffff;x.ram[P+4]=2;x.hold=h;AnimationInnerCursor a;x.finish(a);assert(x.held && x.trace==ring.trace && x.ram==ring.ram);}
 Fixture over;over.ram[P+0x3c]=0xffffffff;over.ram[P+4]=2;over.ram[W+0xf0]=4;over.ram[W+0xe8]=7;AnimationInnerCursor o;over.finish(o);assert(over.ram[W+0xf4]==8 && over.ram[W+0xf8]==0x9c000000 && over.ram[W+0xe8]==0);
 Fixture handle;handle.ram[P+0x3c]=0xffffffff;handle.ram[W+0x10]=P-7*0x170u;AnimationInnerCursor hc;handle.finish(hc);bool main7=false;for(auto op:handle.trace)if(op.k=='m'){assert(op.a==7);main7=true;}assert(main7);
 for(auto parameters:std::vector<std::array<uint32_t,4>>{{0xffffffffu,1,8,10},{0x7fffffffu,0,1,0x7fffffffu},{0x7fffffffu,1,1,10},{2,3,5,10}}){
  Fixture x;x.ram[P+0x3c]=0xffffffff;x.ram[P+4]=2;x.ram[D+0xf0]=parameters[0];x.ram[D+0xf8]=parameters[1];x.ram[W+0xf4]=parameters[2];x.ram[W+0xf0]=parameters[3];AnimationInnerCursor a;x.finish(a);
  assert(x.ram[W+0xf8]==0x9b000000u+12*parameters[2] && x.ram[W+0xfc]==0x9b000000u+12*(parameters[2]+parameters[0]) && x.ram[W+0xf4]==parameters[2]+parameters[0]+parameters[1] && x.ram[W+0xe8]==0);
 }
 Fixture fresh;fresh.ram[W+0xe8]=1;fresh.after=[](Fixture& f,char k,uint32_t a){if(k=='f' && a==0x9c000000){f.ram[W+0xe8]=0;f.ram[W+0xfc]=0x9e000000;f.ram[D+0xf8]=0;f.ram[P+0x28]=99;f.ram[P+0x30]=0;}if(k=='i')f.ram[D+0x144]=8;};AnimationInnerCursor f;fresh.finish(f);std::vector<Op> flush;for(auto op:fresh.trace)if(op.k=='f')flush.push_back(op);assert((flush==std::vector<Op>{{'f',0x9c000000,24},{'f',0x9e000000,0}}));assert(fresh.traversal.current==7 && fresh.traversal.fraction==.25f && fresh.traversal.group==7);
}
}
int main(){exactPrefixAndAllHolds();rejectsAndMaterial();ringAndFreshFlush();}
