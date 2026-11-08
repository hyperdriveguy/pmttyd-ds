#include "fukidashi_effect.hpp"
#include <cstring>

namespace ttyd {
namespace {
// Original work layout; all offsets are byte offsets in guest memory.
constexpr uint32_t EffectPayload=0xc,InitialPosition=0x802f3684;
enum PayloadOffset : uint32_t {
 Kind=0,ComputedPosition=4,Angle=0x24,Remaining=0x30,Counter=0x34,
 Frame=0x38,Duration=0x40,Attachment=0x44,Camera=0x4c,SpinPhase=0x54
};
constexpr uint32_t OneShotTable=0x80417220,DurationTable=0x80417278;
constexpr uint32_t Zero=0x804228c0,SpinUp=0x80422940,SpinDown=0x80422944,SpinEnd=0x80422948;
constexpr uint32_t Delete=0x8005ea30,Position=0x80133480,Depth=0x8001086c,Display=0x80010bc4,Draw=0x8013174c;
constexpr int32_t SpinKind=6,AlternateModeKind=4,SpinReturnPoll=120,ImmortalLifetime=10000;
int32_t signedWord(uint32_t n){return n<=0x7fffffff?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t signedByte(uint8_t n){return n<128?int32_t(n):int32_t(n)-256;}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
bool same(const FukidashiServices& a,const FukidashiServices& b){
 const auto& x=a.memory;const auto& y=b.memory;
 return x.user==y.user && x.word==y.word && x.storeWord==y.storeWord && a.user==b.user && a.byte==b.byte && a.binary==b.binary && a.position==b.position && a.effectDelete==b.effectDelete && a.depth==b.depth && a.display==b.display;
}
struct Guard {bool& busy;Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;}};
}
bool FukidashiMainCursor::reset(){if(invoking_)return false;active_=false;phase_=pending_=0;return true;}
bool resumeFukidashiMain(uint32_t effect,const FukidashiServices& s,FukidashiMainCursor& c){
 if(c.invoking_ || !s.memory.word || !s.memory.storeWord || !s.byte || !s.binary)return false;
 if(c.active_ && (c.effect_!=effect || !same(c.owner_,s)))return false;
 Guard guard(c.invoking_);
 if(!c.active_){c.active_=true;c.phase_=c.pending_=0;c.effect_=effect;c.owner_=s;}
 const auto word=[&](uint32_t a){return s.memory.word(s.memory.user,a);};
 const auto put=[&](uint32_t a,uint32_t v){s.memory.storeWord(s.memory.user,a,v);};
 const auto finish=[&](){c.active_=false;c.phase_=c.pending_=0;return true;};
 for(;;)switch(c.phase_){
 case 0:{
  c.payload_=word(effect+EffectPayload);const auto p=c.payload_;
  // Original stack-vector initializer is read before capturing computed XYZ.
  for(unsigned i=0;i<3;++i)(void)word(InitialPosition+4*i);
  for(unsigned i=0;i<3;++i)c.oldPosition_[i]=real(word(p+ComputedPosition+4*i));
  const auto duration=word(p+Duration),kind=word(p+Kind);
  const auto frame=[&](uint32_t lifetime){
   const auto counter=signedWord(word(p+Counter));
   const auto table=word((lifetime?DurationTable:OneShotTable)+4*kind);
   return signedByte(s.byte(s.user,table+uint32_t(counter/2)));
  };
  auto value=frame(duration);
  put(p+Counter,word(p+Counter)+1);put(p+Frame,uint32_t(value));
  if(value==-1){c.phase_=1;break;}
  if(value<0){
   put(p+Counter,(uint32_t(-10)-uint32_t(value))*2);
   value=frame(word(p+Duration));
   put(p+Counter,word(p+Counter)+1);put(p+Frame,uint32_t(value));
  }
  if(signedWord(kind)==SpinKind){
   const auto spin=word(p+SpinPhase);
   if(spin==0){
    const float angle=real(word(p+Angle)),step=real(word(SpinUp)),limit=real(word(Zero));
    put(p+Angle,bits(s.binary(s.user,FukidashiBinary::Add,angle,step)));
    if(real(word(p+Angle))>=limit){put(p+Angle,bits(limit));put(p+SpinPhase,word(p+SpinPhase)+1);}
   }else if(spin==SpinReturnPoll){
    const float angle=real(word(p+Angle)),step=real(word(SpinDown)),limit=real(word(SpinEnd));
    put(p+Angle,bits(s.binary(s.user,FukidashiBinary::Subtract,angle,step)));
    if(real(word(p+Angle))<=limit){c.phase_=1;break;}
   }else put(p+SpinPhase,word(p+SpinPhase)+1);
  }else if(signedWord(word(p+Duration))>0){
   const auto remaining=word(p+Remaining);
   if(signedWord(remaining)<ImmortalLifetime)put(p+Remaining,remaining-1);
   if(signedWord(word(p+Remaining))<0){c.phase_=1;break;}
  }
  c.phase_=2;break;
 }
 case 1:
  c.pending_=Delete;if(!s.effectDelete || !s.effectDelete(s.user,effect))return false;return finish();
 case 2:
  c.phase_=word(c.payload_+Attachment)?5:6;break;
 case 5:
  c.pending_=Position;if(!s.position || !s.position(s.user,c.payload_,1))return false;
  c.phase_=6;break;
 case 6:
  c.mode_=signedWord(word(c.payload_+Kind))==AlternateModeKind?2:8;c.phase_=3;break;
 case 3:{
  c.pending_=Depth;if(!s.depth)return false;
  const auto depth=s.depth(s.user,c.oldPosition_);if(!depth)return false;
  c.depth_=*depth;c.camera_=signedWord(word(c.payload_+Camera));c.phase_=4;break;
 }
 case 4:
  c.pending_=Display;
  if(!s.display || !s.display(s.user,c.camera_,c.mode_,Draw,effect,c.depth_))return false;
  return finish();
 default:return false;
 }
}
}
