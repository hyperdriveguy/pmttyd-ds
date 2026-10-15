#include "source_gx_inherited_state.hpp"
#include <algorithm>
#include <utility>
#include <cstring>
namespace ttyd {namespace {
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
using Call=SourceGxInheritedCall;
constexpr uint32_t KColor=0x802b8be0,KAlpha=0x802b8c3c,Swap=0x802b8c98,Table=0x802b8ce0,
 Direct=0x802b87d0,NumInd=0x802b87ac,Scale=0x802b857c,Fog=0x802b8ff4,Range=0x802b9208,
 Blend=0x802b932c,Color=0x802b9380,Alpha=0x802b93ac,Z=0x802b93d8,ZLoc=0x802b940c,
 Dither=0x802b9514,Dst=0x802b9540,Pixel=0x802b9440,Field=0x802b957c,Cull=0x802b5c8c;
constexpr uint32_t scalarFunctions[]{NumInd,Fog,Range,Blend,Color,Alpha,Z,ZLoc,Dither,Dst,Pixel,Field,Cull};
Call call(uint32_t f,uint32_t pc,std::initializer_list<uint32_t> n){Call q;q.function=f;q.callsite=pc;q.count=unsigned(n.size());std::copy(n.begin(),n.end(),q.integers.begin());return q;}
bool tuple(const std::optional<Call>&q,std::initializer_list<uint32_t> v){return q&&q->count==v.size()&&std::equal(v.begin(),v.end(),q->integers.begin());}
// Ordinary numerical operands recovered from source LFS constants:0,1,.1.
// The verifier pins their original definitions. No extracted data blob or
// guessed guest mapping is embedded. Preserve the actual binary32 operands.
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
}
struct SourceGxInheritedSnapshot::Lifetime {
 SourceGxInheritedBinding binding;bool alive=true,stale=false,checking=false;
 bool valid(){
  if(!alive||stale||checking||!binding.process||!binding.current)return false;
  Guard guard(checking);const bool ok=binding.current(binding.user);
  if(!ok||!alive)stale=true;
  return !stale&&alive;
 }
};
struct SourceGxInheritedState::State {
 std::shared_ptr<SourceGxInheritedSnapshot::Lifetime> life;
 std::array<std::optional<Call>,16> kColor{},kAlpha{},swaps{},direct{};
 std::array<std::optional<Call>,4> tables{},scales{};
 std::array<std::optional<Call>,13> scalars{};
 bool cold=false,effects=false;
 bool apply(const Call&q){
  if(q.count>8)return false;
  for(unsigned i=q.count;i<8;++i)if(q.integers[i])return false;
  if(q.function!=Fog&&(q.fog||q.floatBits!=std::array<uint32_t,4>{}))return false;
  auto index=q.integers[0];auto n=[&](unsigned count){return q.count==count;};
  switch(q.function){
  case KColor:if(!n(2)||index>=16||q.integers[1]>31)return false;kColor[index]=q;return true;
  case KAlpha:if(!n(2)||index>=16||q.integers[1]>31)return false;kAlpha[index]=q;return true;
  case Swap:if(!n(3)||index>=16||q.integers[1]>3||q.integers[2]>3)return false;swaps[index]=q;return true;
  case Table:if(!n(5)||index>=4)return false;for(unsigned i=1;i<5;++i)if(q.integers[i]>3)return false;tables[index]=q;return true;
  case Direct:if(!n(1)||index>=16)return false;direct[index]=q;return true;
  case Scale:if(!n(3)||index>=4||q.integers[1]>8||q.integers[2]>8)return false;scales[index]=q;return true;
  default:break;
  }
  bool ok=false;
  switch(q.function){
  case NumInd:ok=n(1)&&index<=4;break;
  case Fog:ok=n(2)&&q.fog;break; // Preserve exact raw type/color/float bits.
  case Range:ok=n(3)&&index<=1;break;
  case Blend:ok=n(4)&&index<=3&&q.integers[1]<=7&&q.integers[2]<=7&&q.integers[3]<=15;break;
  case Color:case Alpha:case ZLoc:case Dither:ok=n(1)&&index<=1;break;
  case Z:ok=n(3)&&index<=1&&q.integers[1]<=7&&q.integers[2]<=1;break;
  case Dst:ok=n(2)&&index<=1&&q.integers[1]<=255;break;
  case Pixel:ok=n(2)&&index<=7&&q.integers[1]<=2;break;
  case Field:ok=n(2)&&index<=1&&q.integers[1]<=1;break;
  case Cull:ok=n(1)&&index<=3;break;
  default:return false;
  }
  if(!ok)return false;
  for(unsigned i=0;i<std::size(scalarFunctions);++i)if(scalarFunctions[i]==q.function){scalars[i]=q;return true;}
  return false;
 }
 const std::optional<Call>&get(uint32_t f)const{for(unsigned i=0;i<std::size(scalarFunctions);++i)if(scalarFunctions[i]==f)return scalars[i];return scalars[0];}
 bool ordinary(unsigned stages)const{
  if(!stages||stages>16||!tuple(get(NumInd),{0})||!tuple(get(Color),{1})||!tuple(get(Alpha),{1})||
   !tuple(get(Range),{0,0,0})||!tuple(get(Dither),{1})||!tuple(get(Dst),{0,0})||
   !tuple(get(Pixel),{0,0})||!tuple(get(Field),{1,1})||!get(Fog)||get(Fog)->integers[0]!=0||
   !tuple(tables[0],{0,0,1,2,3}))return false;
  // Fog type0 disables the effect independently of retained numeric/color
  // operands. Local primitives own cull/blend/depth/TEV/channel state.
  for(unsigned i=0;i<stages;++i)if(!tuple(swaps[i],{i,0,0})||!tuple(direct[i],{i}))return false;
  return true;
 }
};
SourceGxInheritedState::SourceGxInheritedState()=default;
SourceGxInheritedState::~SourceGxInheritedState(){retire();}
bool SourceGxInheritedState::bind(SourceGxInheritedBinding b){
 if(invoking_||state_||!b.process||!b.current)return false;
 Guard guard(invoking_);auto life=std::make_shared<SourceGxInheritedSnapshot::Lifetime>();life->binding=std::move(b);
 if(!life->valid())return false;
 state_=std::make_unique<State>();state_->life=std::move(life);return true;
}
bool SourceGxInheritedState::current()const{
 if(invoking_||!state_)return false;
 Guard guard(invoking_);return state_->life->valid();
}
bool SourceGxInheritedState::tracks(uint32_t f){
 if(f==KColor||f==KAlpha||f==Swap||f==Table||f==Direct||f==Scale)return true;
 for(auto x:scalarFunctions)if(f==x)return true;
 return false;
}
bool SourceGxInheritedState::apply(const Call&q){
 if(invoking_||!state_)return false;
 Guard guard(invoking_);if(!state_->life->valid())return false;
 const bool ok=state_->apply(q);if(ok)state_->effects=true;return ok; // No callbacks/allocation after successful mutation.
}
bool SourceGxInheritedState::initializeCold(){
 if(invoking_||!state_)return false;
 Guard guard(invoking_);auto&s=*state_;if(!s.life->valid())return false;if(s.cold)return true;
 if(s.effects||!s.life->binding.coldCurrent)return false;
 const bool admitted=s.life->binding.coldCurrent(s.life->binding.user);
 if(!s.life->valid()||!admitted)return false;
 // Execute actual defining LWZ804293e8 into a real private local before the
 // later fog use. No original guest stack pointer/register is manufactured.
 const uint32_t capturedFog=0; // Actual source packed RGBA0, not BSS adoption.
 for(unsigned stage=0;stage<16;++stage){
  s.apply(call(KColor,0x802b3090,{stage,6}));s.apply(call(KAlpha,0x802b309c,{stage,0}));s.apply(call(Swap,0x802b30ac,{stage,0,0}));
 }
 s.apply(call(Table,0x802b30d0,{0,0,1,2,3}));s.apply(call(Table,0x802b30e8,{1,0,0,0,3}));
 s.apply(call(Table,0x802b3100,{2,1,1,1,3}));s.apply(call(Table,0x802b3118,{3,2,2,2,3}));
 for(unsigned stage=0;stage<16;++stage)s.apply(call(Direct,0x802b3130,{stage}));
 s.apply(call(NumInd,0x802b3144,{0}));
 for(unsigned stage=0;stage<4;++stage)s.apply(call(Scale,0x802b3154+16*stage,{stage,0,0}));
 auto fog=call(Fog,0x802b31a8,{0,capturedFog});fog.fog=true;
 // Original late LFS order: f2=1, f4=f2, f1=0, f3=.1.
 const auto f2=bits(1.f);fog.floatBits={bits(0.f),f2,bits(.1f),f2};s.apply(fog);
 s.apply(call(Range,0x802b31b8,{0,0,0}));s.apply(call(Blend,0x802b31cc,{0,4,5,0}));
 s.apply(call(Color,0x802b31d4,{1}));s.apply(call(Alpha,0x802b31dc,{1}));s.apply(call(Z,0x802b31ec,{1,3,1}));
 s.apply(call(ZLoc,0x802b31f4,{1}));s.apply(call(Dither,0x802b31fc,{1}));s.apply(call(Dst,0x802b3208,{0,0}));
 s.apply(call(Pixel,0x802b3214,{0,0}));s.apply(call(Field,0x802b3220,{1,1}));s.cold=true;s.effects=true;return true;
}
bool SourceGxInheritedState::coldComplete()const{
 if(invoking_||!state_)return false;
 Guard guard(invoking_);return state_->life->valid()&&state_->cold;
}
bool SourceGxInheritedState::invalidate(){
 if(invoking_||!state_)return false;
 Guard guard(invoking_);if(!state_->life->valid())return false;
 auto life=state_->life;const bool cold=state_->cold;*state_=State{};state_->life=std::move(life);state_->cold=cold;state_->effects=true;return true;
}
std::shared_ptr<const SourceGxInheritedSnapshot>SourceGxInheritedState::capture(unsigned stages)const{
 if(invoking_||!state_)return {};
 Guard guard(invoking_);if(!state_->life->valid()||!state_->ordinary(stages))return {};
 auto out=std::make_shared<SourceGxInheritedSnapshot>();out->lifetime_=state_->life;out->stages_=stages;
 auto add=[&](const auto&list){for(const auto&q:list)if(q)out->evidence_[out->size_++]=*q;};
 add(state_->scalars);add(state_->tables);add(state_->scales);add(state_->swaps);add(state_->direct);add(state_->kColor);add(state_->kAlpha);
 return out;
}
void SourceGxInheritedState::retire(){if(state_)state_->life->alive=false;}
bool SourceGxInheritedSnapshot::current()const{return lifetime_&&lifetime_->valid();}
}
