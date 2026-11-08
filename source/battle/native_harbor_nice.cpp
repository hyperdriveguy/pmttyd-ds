#include "native_harbor_nice.hpp"
#include "event_manager.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t GpGlobal=0x80415fd0,Language=0x16c;
constexpr uint32_t LanguageBranches=0x8034cc44,GradeBranches=0x8034cc28,Widths=0x8034cb48;
constexpr uint32_t BaseScale=0x80422290,SignedBias=0x802ee608;
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
// Explicit stores preserve the original single-operation rounding boundaries.
float add(float a,float b){volatile float f=a+b;return f;}
float sub(float a,float b){volatile float f=a-b;return f;}
float mul(float a,float b){volatile float f=a*b;return f;}
bool same(const NativeHarborNiceServices&a,const NativeHarborNiceServices&b){return a.user==b.user&&a.lifetime==b.lifetime&&a.current==b.current&&a.word==b.word&&a.value==b.value&&a.spawn==b.spawn&&a.scale==b.scale&&a.sound==b.sound&&a.pitch==b.pitch;}
}
bool resumeNativeHarborNice(EventEntry&e,const NativeHarborNiceServices&s,NativeHarborNiceCursor&c){
 if(c.invoking_||!s.lifetime||!s.current||!s.current(s.user)||!(e.flags&1)||e.argumentCount<4)return false;
 if(!c.event_){c.event_=&e;c.id_=e.id;c.command_=e.currentCommand;c.args_=e.arguments;c.services_=s;}
 if(c.event_!=&e||c.id_!=e.id||c.command_!=e.currentCommand||c.args_!=e.arguments||!same(c.services_,s))return false;
 if(c.complete_)return true;
 c.invoking_=true;struct Guard{bool&v;~Guard(){v=false;}}guard{c.invoking_};
 if(c.stage_==0){
  size_t read=0;
  // The tape contains only completed source loads/evtGetValue calls. A hold
  // resumes its exact next provider; earlier private arithmetic may recompute.
  auto load=[&](uint32_t kind,uint32_t address,uint32_t&out)->bool{
   if(read<c.reads_.size()){const auto&r=c.reads_[read++];if(r.kind!=kind||r.address!=address)return false;out=r.value;return true;}
   c.pending_=kind==0?address:0x80031260;
   if(!s.current(s.user))return false;
   std::optional<uint32_t>v;
   if(kind==0){if(s.word)v=s.word(s.user,address);}else if(s.value){const auto q=s.value(s.user,e,int32_t(address));if(q)v=uint32_t(*q);}
   if(!v)return false;
   c.reads_.push_back({kind,address,*v});++read;out=*v;return true;
  };
  auto argument=[&](unsigned n,uint32_t&v){uint32_t raw;return load(0,c.args_+4*n,raw)&&load(1,raw,v);};
  uint32_t grade,gp,scaleBits,language,row=0;
  if(!argument(0,grade)||!load(0,GpGlobal,gp)||!load(0,BaseScale,scaleBits)||!load(0,gp+Language,language))return false;
  float scale=real(scaleBits);
  if(language<=6){uint32_t target;if(!load(0,LanguageBranches+4*language,target))return false;
   switch(target){case 0x80107000:row=1;break;case 0x80107008:row=2;break;case 0x80107010:row=3;break;case 0x80107018:row=4;break;case 0x80107020:row=5;break;case 0x80107028:row=6;break;case 0x80107030:row=0;break;default:return false;}}
  std::array<float,3>p;
  for(unsigned i=0;i<3;++i){uint32_t v,hi,lo;if(!argument(i+1,v)||!load(0,SignedBias,hi)||!load(0,SignedBias+4,lo))return false;
   uint64_t biasBits=(uint64_t(hi)<<32)|lo,convertedBits=(uint64_t(0x43300000)<<32)|(v^0x80000000u);double bias,converted;
   std::memcpy(&bias,&biasBits,8);std::memcpy(&converted,&convertedBits,8);p[i]=float(converted-bias);}
  if(grade>6){if(!s.current(s.user))return false;c.complete_=true;c.pending_=0;return true;}
  uint32_t target;if(!load(0,GradeBranches+4*grade,target))return false;
  constexpr uint32_t Targets[]={0x801070d8,0x80107108,0x80107138,0x80107168,0x80107198,0x801071c8,0x801071f0};
  if(target!=Targets[grade])return false;
  if(grade<=4){uint32_t factor;const uint32_t address=grade==0?0x80422294:grade==1?0x80422298:0x8042229c;if(!load(0,address,factor))return false;scale=mul(scale,real(factor));}
  uint32_t width,factor,half,right,left,threshold,shift;
  const auto rowAddress=Widths+row*32;
  if(!load(0,rowAddress,width)||!load(0,rowAddress+4*(grade+1),factor))return false;
  float radius=mul(mul(real(width),real(factor)),scale);
  if(!load(0,0x804222a0,half)||!load(0,0x804222a4,right))return false;
  radius=mul(radius,real(half));if(add(p[0],radius)>real(right))p[0]=sub(real(right),radius);
  if(!load(0,0x804222a8,left))return false;
  if(sub(p[0],radius)<real(left))p[0]=add(real(left),radius);
  if(!load(0,0x804222b0,threshold)||!load(0,0x804222ac,shift))return false;
  c.spawn_.type=grade<=2?0:grade-2;
  c.spawn_.position={sub(p[0],radius),p[1]<real(threshold)?add(real(shift),p[1]):sub(p[1],real(shift)),p[2]};
  constexpr uint32_t Sounds[]={0x802ee618,0x802ee634,0x802ee650,0x802ee664,0x802ee678,0x802ee68c,0x802ee6a4};
  c.scale_=scale;c.soundPosition_=p;c.sound_=Sounds[grade];c.stage_=1;
 }
 if(c.stage_==1){c.pending_=0x80144acc;if(!s.current(s.user)||!s.spawn)return false;auto v=s.spawn(s.user,c.spawn_);if(!v||!*v)return false;c.effect_=*v;c.stage_=2;}
 if(c.stage_==2){c.pending_=0x801072b4;if(!s.current(s.user)||!s.scale||!s.scale(s.user,c.effect_,c.scale_))return false;c.stage_=3;}
 if(c.stage_==3){c.pending_=0x800d9848;if(!s.current(s.user)||!s.sound)return false;auto v=s.sound(s.user,c.sound_,c.soundPosition_);if(!v)return false;c.handle_=*v;c.stage_=4;}
 if(c.stage_==4){c.pending_=0x800d9744;if(!s.current(s.user)||!s.pitch||!s.pitch(s.user,c.handle_,0))return false;c.stage_=5;}
 if(!s.current(s.user))return false;
 c.complete_=true;c.pending_=0;return true;
}
}
