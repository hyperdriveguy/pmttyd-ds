#include "animation_draw_wrapper.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t W=0x90000000,P=0x91000000,J=0x92000000,E=0x93000000,H=0x94000000,D=0x95000000;
struct Fixture {
 std::map<uint32_t,uint32_t> memory{{0x80415dc0,W},{W+0x10,P},{P+0x90,0xffffffff},{P+0x10,2},{W,J},{J+40,E},{E+0xa0,H},{H,D},{D+0xc4,1}};
 std::vector<uint32_t> reads;unsigned calls=0,hold=0,innerCalls=0,innerCompleted=0;bool held=false,innerReady=true;void (*after)(Fixture&,uint32_t)=nullptr;
 AnimationInnerDrawRequest request;
 AnimationDrawWrapperInput input;
 Fixture(){input.pass=2;input.angle=-0.f;input.scale=2;for(unsigned i=0;i<12;++i)input.matrix[i]=float(i+1);}
 AnimationDrawWrapperServices services(){AnimationDrawWrapperServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(++f.calls==f.hold && !f.held){f.held=true;return {};}auto v=f.memory.find(a);if(v==f.memory.end())return {};auto w=v->second;f.reads.push_back(a);if(f.after)f.after(f,a);return w;};
  s.inner=[](void* u,const AnimationInnerDrawRequest& q){auto& f=*static_cast<Fixture*>(u);++f.innerCalls;f.request=q;if(!f.innerReady)return false;++f.innerCompleted;return true;};return s;
 }
 AnimationDrawWrapperFrontier finish(AnimationDrawWrapperCursor& c){for(unsigned i=0;i<100;++i){const auto f=resumeAnimationDrawWrapper(input,services(),c,1);if(f!=AnimationDrawWrapperFrontier::Pending)return f;}assert(false);return AnimationDrawWrapperFrontier::Pending;}
};
void traceAndEveryHold(){
 const std::vector<uint32_t> expected{0x80415dc0,W+0x10,P+0x90,P+0x10,W,J+40,E+0xa0,H,D+0xc4,P+0x90};
 for(unsigned hold=0;hold<=expected.size();++hold){Fixture f;f.hold=hold;AnimationDrawWrapperCursor c;assert(f.finish(c)==AnimationDrawWrapperFrontier::Complete && !c.active && f.reads==expected && f.innerCompleted==1 && (!hold || f.held));assert(f.request.pose==P && f.request.pass==2 && f.request.matrix==f.input.matrix && !std::memcmp(&f.request.angle,&f.input.angle,4) && f.request.scale==2);}
 Fixture f;f.innerReady=false;AnimationDrawWrapperCursor c;
 assert(resumeAnimationDrawWrapper(f.input,f.services(),c)==AnimationDrawWrapperFrontier::Pending && c.active && c.phase==12 && f.innerCalls==1);
 const auto reads=f.reads;f.memory[P+0x90]=7;f.memory[D+0xc4]=0;f.input.matrix[0]=-99;
 assert(resumeAnimationDrawWrapper(f.input,f.services(),c)==AnimationDrawWrapperFrontier::Pending && f.innerCalls==1 && f.reads==reads);
 f.input.matrix[0]=1;f.innerReady=true;assert(resumeAnimationDrawWrapper(f.input,f.services(),c)==AnimationDrawWrapperFrontier::Complete && f.reads==reads && f.innerCompleted==1 && f.request.pose==P);
}
void masksAndReloads(){
 for(uint32_t pass:{0u,1u,2u,31u,32u,63u,64u,0xffffffc0u,0xffffffffu})for(uint32_t mask:{0u,1u,2u,4u,0x80000000u}){
  Fixture f;f.input.pass=pass;f.memory[D+0xc4]=mask;AnimationDrawWrapperCursor c;assert(f.finish(c)==AnimationDrawWrapperFrontier::Complete);
  const auto shift=pass&63u;const bool admitted=(pass==2 && (mask&1)) || (shift<32 && (mask&(1u<<shift)));
  assert(f.innerCompleted==unsigned(admitted));assert(f.reads.back()==(admitted?P+0x90:D+0xc4));
 }
 Fixture f;f.memory[D+0xc4]=0;f.after=[](Fixture& f,uint32_t a){if(a==D+0xc4)f.memory[a]=4;if(a==P+0x10)f.memory[0x80415dc0]=0xdead0000;};AnimationDrawWrapperCursor c;assert(f.finish(c)==AnimationDrawWrapperFrontier::Complete && f.innerCompleted==1);unsigned count=0;for(auto a:f.reads)if(a==D+0xc4)++count;assert(count==2);
}
void explicitFrontiersAndIdentity(){
 Fixture linked;linked.memory[P+0x90]=3;AnimationDrawWrapperCursor c;assert(linked.finish(c)==AnimationDrawWrapperFrontier::LinkedPreparation && c.active && linked.reads.size()==3 && !linked.innerCalls);const auto reads=linked.reads;linked.memory[P+0x90]=0xffffffff;assert(linked.finish(c)==AnimationDrawWrapperFrontier::LinkedPreparation && linked.reads==reads);
 Fixture changed;changed.after=[](Fixture& f,uint32_t a){if(a==D+0xc4)f.memory[P+0x90]=5;};AnimationDrawWrapperCursor d;assert(changed.finish(d)==AnimationDrawWrapperFrontier::CallerRegister && !changed.innerCalls);const auto before=changed.reads;changed.memory[P+0x90]=0xffffffff;assert(changed.finish(d)==AnimationDrawWrapperFrontier::CallerRegister && changed.reads==before);
 Fixture f;AnimationDrawWrapperCursor i;assert(resumeAnimationDrawWrapper(f.input,f.services(),i,1)==AnimationDrawWrapperFrontier::Pending);const auto read=f.reads;auto other=f.services();other.inner=nullptr;assert(resumeAnimationDrawWrapper(f.input,other,i)==AnimationDrawWrapperFrontier::Pending && f.reads==read);auto input=f.input;input.pass=64;assert(resumeAnimationDrawWrapper(input,f.services(),i)==AnimationDrawWrapperFrontier::Pending && f.reads==read);input=f.input;input.angle=0;assert(resumeAnimationDrawWrapper(input,f.services(),i)==AnimationDrawWrapperFrontier::Pending && f.reads==read);assert(f.finish(i)==AnimationDrawWrapperFrontier::Complete);
 // Raw pose-address arithmetic wraps without native typed-handle admission.
 Fixture wrap;wrap.input.handle=0x10000000;wrap.memory[W+0x10]=P-wrap.input.handle*0x170u;AnimationDrawWrapperCursor w;assert(wrap.finish(w)==AnimationDrawWrapperFrontier::Complete && wrap.request.pose==P);
}
}
int main(){traceAndEveryHold();masksAndReloads();explicitFrontiersAndIdentity();}
