#include "animation_group.hpp"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t W=0x90000000,P=0x91000000,D=0x92000000,G=0x93000000,V=0x94000000,N=0x95000000,M=0x803d8e70,H=0x96000000,O=0x97000000,T=0x98000000;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
struct Op {char kind;uint32_t a=0,b=0,c=0;bool operator==(const Op& q)const{return kind==q.kind && a==q.a && b==q.b && c==q.c;}};
struct Fixture {
 std::map<uint32_t,uint32_t> ram{{0x80415dc0,W},{W+0x18,P},{W+0x20,D},{D+0x1a8,G},{P+0x5c,V},{P+0x64,N},{P,0},{P+0x154,0},{P+0x158,0x1234},{0x8041e89c,0},{0x8041e8a0,M},{0x8041fb24,bits(1)},{0x8041fb28,0},{0x8041fb44,bits(.5f)},{0x8041fb98,bits(-2)},{0x8041fb9c,bits(-3)},{0x8041fb6c,bits(-1)},{0x8041fba0,bits(.04f)},{0x802c1048,1},{0x802c104c,2},{0x802c1050,3},{P+0x15c,H},{H,O},{P+0x164,H+4},{H+4,O+0x1000},{P+0x120,H+8},{H+8,0},{P+0x11c,1},{P+0x14,2}};
 std::vector<Op> trace;std::vector<AnimationGroupNode> nodes;unsigned calls=0,hold=0;bool held=false,nodeWrites=true;
 void (*after)(Fixture&,char,uint32_t)=nullptr;AnimationInnerTraversal input{0xffffffff,0,7,9,.25f};
 Fixture(){for(unsigned i=0;i<4;++i){auto g=G+i*88;ram[g+0x40]=i==0?3:0xffffffff;ram[g+0x44]=i<2?i+1:0xffffffff;ram[g+0x48]=10+i;ram[g+0x4c]=i;ram[g+0x50]=i*24;ram[g+0x54]=1;ram[V+i]=i==2?0:i==1?255:1;ram[N+i*96+0xc]=bits(2+i);ram[N+i*96+0x10]=bits(3+i);ram[N+i*96+0x14]=bits(4+i);}
  for(unsigned i=0;i<12;++i){ram[M+4*i]=bits(float(i));ram[P+0x124+4*i]=bits(10.f+i);}for(uint32_t k:{1u,2u,3u,5u,0xffffffffu})ram[0x80415db0+k*4]=T;ram[T+16]=bits(3);ram[T+20]=bits(4);ram[W+0x10]=bits(3);ram[W+0x14]=bits(4);
 }
 bool admit(char kind,uint32_t a=0,uint32_t b=0,uint32_t c=0){if(++calls==hold && !held){held=true;return false;}trace.push_back({kind,a,b,c});return true;}
 void changed(char k,uint32_t a){if(after)after(*this,k,a);}
 void single(){ram[G+0x40]=ram[G+0x44]=0xffffffff;}
 AnimationGroupServices services(){AnimationGroupServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);auto i=f.ram.find(a);if(i==f.ram.end() || !f.admit('R',a,i->second))return {};auto w=i->second;f.changed('R',a);return w;};
  s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(u);auto i=f.ram.find(a);if(i==f.ram.end() || !f.admit('B',a,i->second))return {};auto w=i->second;f.changed('B',a);return uint8_t(w);};
  s.store=[](void* u,uint32_t a,uint32_t w){auto& f=*static_cast<Fixture*>(u);if(!f.admit('S',a,w))return false;f.ram[a]=w;f.changed('S',a);return true;};
  s.node=[](void* u,const AnimationGroupNode& q){auto& f=*static_cast<Fixture*>(u);if(!f.admit('N',q.node,q.joint,bits(q.parent[0])))return false;f.nodes.push_back(q);auto level=++f.ram[0x8041e89c];auto m=M+48*level;f.ram[0x8041e8a0]=m;if(f.nodeWrites)for(unsigned i=0;i<12;++i)f.ram[m+4*i]=bits(float(i)+(q.node-N)/96.f+10);f.changed('N',q.node);return true;};
  s.copy=[](void* u,uint32_t from,uint32_t to){auto& f=*static_cast<Fixture*>(u);if(!f.admit('C',from,to))return false;for(unsigned i=0;i<6;++i){auto a=f.ram.at(from+8*i),b=f.ram.at(from+8*i+4);f.ram[to+8*i]=a;f.ram[to+8*i+4]=b;}f.changed('C',to);return true;};
  s.material=[](void* u,uint32_t shape,uint32_t current){auto& f=*static_cast<Fixture*>(u);if(!f.admit('A',shape,current))return false;f.changed('A',shape);return true;};
  s.vivian=[](void* u,const AnimationGroupVivian& q){auto& f=*static_cast<Fixture*>(u);if(!f.admit('E',q.matrix,q.saved,bits(q.parameters[2])))return false;assert(q.parameters[0]==3 && q.parameters[1]==4);const auto kind=f.ram[P+0x11c];assert(q.parameters[2]==(kind==1?-2.f:kind==2?-3.f:kind==3?-1.f:0.f));assert(q.parameters[3]==(f.ram[P+0x11c]>=1 && f.ram[P+0x11c]<=3?.5f:0));f.changed('E',q.saved);return true;};
  s.callback=[](void* u,uint32_t fn,uint32_t data,uint32_t group,uint32_t matrix){auto& f=*static_cast<Fixture*>(u);if(!f.admit('K',fn,data,group))return false;assert(matrix==f.ram[0x8041e8a0]);f.changed('K',fn);return true;};
  s.render=[](void* u,uint32_t shape,uint32_t current){auto& f=*static_cast<Fixture*>(u);if(!f.admit('D',shape,current))return false;f.changed('D',shape);return true;};
  s.blend=[](void* u,uint32_t k,uint32_t m,uint32_t old)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(!f.admit('F',k,m,old))return {};volatile float delta=real(m)-real(old);auto out=bits(std::fma(real(k),float(delta),real(old)));f.changed('F',out);return out;};return s;
 }
 bool finish(AnimationGroupCursor& c,unsigned visits=7){for(unsigned i=0;i<10000;++i)if(resumeAnimationGroup(input,services(),c,visits))return true;return false;}
};
void traceAndEveryHold(){
 Fixture base;AnimationGroupCursor c;assert(base.finish(c));assert(base.nodes.size()==3 && base.ram[0x8041e89c]==0 && base.ram[0x8041e8a0]==M);
 assert((base.nodes[0].parent==std::array<float,3>{1,1,1}) && (base.nodes[1].parent==std::array<float,3>{2,3,4}) && (base.nodes[2].parent==std::array<float,3>{2,3,4}));
 const std::vector<Op> prefix{{'R',0x80415dc0,W},{'R',W+0x20,D},{'R',W+0x18,P},{'R',D+0x1a8,G},{'R',P+0x5c,V},{'R',G+0x4c,0},{'B',V,1},{'R',G+0x54,1},{'R',G+0x50,0},{'R',P+0x64,N},{'R',0x8041fb24,bits(1)}};
 assert(std::equal(prefix.begin(),prefix.end(),base.trace.begin()));
 for(unsigned at=1;at<=base.calls;++at){Fixture f;f.hold=at;AnimationGroupCursor held;assert(f.finish(held,1) && f.held && f.trace==base.trace && f.ram==base.ram);}
 std::vector<const AnimationGroupFrame*> frames;for(auto& f:c.frames)frames.push_back(f.get());base.trace.clear();base.nodes.clear();base.calls=0;assert(base.finish(c));assert(c.frames.size()==frames.size());for(unsigned i=0;i<frames.size();++i)assert(frames[i]==c.frames[i].get());
 Fixture empty;empty.input.group=0xffffffff;AnimationGroupCursor e;assert(empty.finish(e) && empty.trace.empty());
 // Exactly -1 is empty; raw wrapped indices otherwise reach memory providers.
 Fixture raw;raw.input.group=0xfffffffe;AnimationGroupCursor r;assert(!resumeAnimationGroup(raw.input,raw.services(),r,128) && !raw.trace.empty());
}
void publicationsAndCallbackFreshness(){
 Fixture f;f.single();f.ram[P]=0xc0;f.ram[P+0x154]=0x81234560;AnimationGroupCursor c;assert(f.finish(c));for(unsigned i=0;i<3;++i)assert(f.ram[O+i*4]==f.ram[M+48+0xc+i*0x10]);for(unsigned i=0;i<12;++i)assert(f.ram[O+0x1000+i*4]==f.ram[M+48+i*4]);
 auto index=[&](char k,uint32_t a){for(unsigned i=0;i<f.trace.size();++i)if(f.trace[i].kind==k && f.trace[i].a==a)return i;assert(false);return 0u;};
 assert(index('R',0x802c1048)<index('R',0x802c104c) && index('R',0x802c104c)<index('R',M+48+0xc) && index('R',M+48+0xc)<index('R',0x802c1050) && index('R',0x802c1050)<index('R',H) && index('R',H)<index('R',M+48+0x1c));
 for(unsigned at=1;at<=f.calls;++at){Fixture h;h.single();h.ram[P]=0xc0;h.ram[P+0x154]=0x81234560;h.hold=at;AnimationGroupCursor x;assert(h.finish(x,1) && h.trace==f.trace && h.ram==f.ram);}
 Fixture fresh;fresh.single();fresh.ram[P+0x154]=1;fresh.after=[](Fixture& x,char k,uint32_t){if(k=='K'){x.ram[0x80415dc0]=W+0x1000;x.ram[W+0x1000+0x18]=P+0x1000;x.ram[W+0x1000+0x20]=D+0x1000;x.ram[D+0x1000+0x1a8]=G+0x1000;x.ram[P+0x1000+0x5c]=V+0x1000;x.ram[G+0x44]=2;x.ram[G+0x40]=1;x.ram[G+0x48]=77;for(unsigned i:{1u,2u}){auto g=G+0x1000+i*88;x.ram[g+0x40]=0xffffffff;x.ram[g+0x4c]=i;x.ram[V+0x1000+i]=0;}}};AnimationGroupCursor a;assert(fresh.finish(a));bool shape=false,newGraph=false;for(auto& o:fresh.trace){if(o.kind=='D' && o.a==77 && o.b==7)shape=true;if(o.kind=='R' && o.a==G+0x1000+2*88+0x4c)newGraph=true;}assert(shape && newGraph && fresh.nodes.size()==1);
 Fixture underflow;underflow.single();underflow.after=[](Fixture& x,char k,uint32_t){if(k=='D')x.ram[0x8041e89c]=0;};AnimationGroupCursor z;assert(underflow.finish(z) && underflow.ram[0x8041e89c]==0xffffffff && underflow.ram[0x8041e8a0]==M-48);
 // Flag20 alone is not a rejection; unmatched group follows ordinary route.
 Fixture unmatched;unmatched.single();unmatched.ram[P]=0x20;unmatched.ram[H+8]=7;AnimationGroupCursor u;assert(unmatched.finish(u));assert(unmatched.ram[P]==0x20);bool drawn=false;for(auto& o:unmatched.trace)if(o.kind=='D')drawn=true;assert(drawn);
}
void specialAndAliasing(){
 for(uint32_t kind:{1u,2u,3u,4u,5u,0xffffffffu}){
  Fixture base;base.single();base.ram[P]=0x20;base.ram[P+0x11c]=kind;base.after=[](Fixture& f,char k,uint32_t){if(k=='E'){f.ram[0x80415dc0]=W+0x1000;f.ram[W+0x1000+0x18]=P+0x1000;f.ram[P+0x1000]=0xe3;f.ram[0x8041fba0]=bits(.25f);for(unsigned i=0;i<12;++i)f.ram[P+0x124+4*i]=bits(20.f+i);}if(k=='S')f.ram[0x8041fba0]=bits(.5f);};AnimationGroupCursor c;assert(base.finish(c));assert(base.ram[P]==0x20 && base.ram[P+0x1000]==0xc3);
  constexpr unsigned basis[]{0,1,2,4,5,6,8,9,10};for(unsigned i:basis)assert(base.ram[P+0x124+4*i]==bits(std::fma(.25f,-10.f,20.f+i)));for(unsigned i:{3u,7u,11u})assert(base.ram[P+0x124+4*i]==bits(20.f+i));for(auto& o:base.trace)assert(o.kind!='D' && o.kind!='K');
  for(unsigned at=1;at<=base.calls;++at){Fixture f;f.single();f.ram[P]=0x20;f.ram[P+0x11c]=kind;f.after=base.after;f.hold=at;AnimationGroupCursor h;assert(f.finish(h,1) && f.held && f.trace==base.trace && f.ram==base.ram);}
 }
 // Saved output aliases later source lanes. Each read is fresh, not a matrix snapshot.
 Fixture alias;alias.single();alias.ram[P]=0x20;alias.nodeWrites=false;alias.after=[](Fixture& f,char k,uint32_t){if(k=='N')f.ram[0x8041e8a0]=P+0x120;};auto expected=alias.ram;constexpr uint32_t basis[]{0,4,8,0x10,0x14,0x18,0x20,0x24,0x28};for(auto o:basis){volatile float delta=real(expected.at(P+0x120+o))-real(expected.at(P+0x124+o));expected[P+0x124+o]=bits(std::fma(.04f,float(delta),real(expected.at(P+0x124+o))));}AnimationGroupCursor a;assert(alias.finish(a));for(auto o:basis)assert(alias.ram[P+0x124+o]==expected[P+0x124+o]);
}
void retainedIdentity(){Fixture f;f.input.fraction=0;AnimationGroupCursor c;auto s=f.services();assert(!resumeAnimationGroup(f.input,s,c,1));const auto before=f.trace;auto q=f.input;q.next++;assert(!resumeAnimationGroup(q,s,c) && f.trace==before);q=f.input;q.fraction=-0.f;assert(!resumeAnimationGroup(q,s,c) && f.trace==before);auto changed=s;changed.render=nullptr;assert(!resumeAnimationGroup(f.input,changed,c) && f.trace==before);assert(f.finish(c));}
}
int main(){traceAndEveryHold();publicationsAndCallbackFreshness();specialAndAliasing();retainedIdentity();}
