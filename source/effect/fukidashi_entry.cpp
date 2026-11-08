#include "fukidashi_entry.hpp"
#include <cstring>

namespace ttyd {
namespace {
constexpr uint32_t MarioAttachment=0xffffffff,PartyAttachment=0xfffffffe,ExtraAttachment=0xfffffffd;
constexpr uint32_t ClassName=0x802f36b0,Main=0x80132ea8,Zero=0x804228c0;
constexpr uint32_t InputYOffset=0x80422930,InitialSpin=0x8042294c;
constexpr uint32_t PartyExclamationVector=0x802f3654,PartyQuestionVector=0x802f3660;
constexpr uint32_t NpcExclamationVector=0x802f366c,NpcQuestionVector=0x802f3678;
constexpr uint32_t NpcExclamationKey=0x802f36bc,NpcQuestionKey=0x802f36d8;
constexpr uint32_t PoolEntry=0x8005ec68,HeapAllocate=0x800300f0,Position=0x80133480;
constexpr uint32_t MarioGet=0x8005e0c0,PartyGetId=0x80141050,ExtraGetId=0x80140fb8,PartyGetPointer=0x800bf1b4,Sound=0x800d9848;
constexpr uint32_t PayloadBytes=0x58,ActorPosition=0x8c,PartyPosition=0x58;
constexpr int32_t PayloadHeap=3,ImmortalLifetime=10000,SpinKind=6;
enum WorkOffset : uint32_t {Kind=0,InputX=0x10,InputY=0x14,InputZ=0x18,InputAngle=0x1c,Radius=0x20,
 Spin=0x24,Remaining=0x30,Counter=0x34,Variant=0x3c,Duration=0x40,Attachment=0x44,Alpha=0x48,Camera=0x4c,SpinPhase=0x54};
float real(uint32_t n){float f;std::memcpy(&f,&n,sizeof(f));return f;}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,sizeof(n));return n;}
bool same(const FukidashiEntryRequest& a,const FukidashiEntryRequest& b){
 return a.kind==b.kind && a.variant==b.variant && a.duration==b.duration && a.attachment==b.attachment &&
  bits(a.radius)==bits(b.radius) && bits(a.angle)==bits(b.angle) && std::memcmp(a.position.data(),b.position.data(),sizeof(float)*3)==0;
}
bool same(const FukidashiEntryServices& a,const FukidashiEntryServices& b){
 const auto& x=a.pool.memory;const auto& y=b.pool.memory;
 return x.user==y.user && x.word==y.word && x.storeWord==y.storeWord && x.storeByte==y.storeByte &&
  a.pool.user==b.pool.user && a.pool.heapAllocate==b.pool.heapAllocate && a.user==b.user &&
  a.binary==b.binary && a.position==b.position && a.mario==b.mario && a.partyId==b.partyId &&
  a.partyPointer==b.partyPointer && a.sound==b.sound;
}
struct Guard {bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
}
bool FukidashiEntryCursor::reset(){if(invoking_)return false;active_=false;phase_=Phase::Acquire;pending_=0;return true;}
std::optional<uint32_t> resumeFukidashiEntry(const FukidashiEntryRequest& r,const FukidashiEntryServices& s,FukidashiEntryCursor& c){
 using Phase=FukidashiEntryCursor::Phase;
 const auto& m=s.pool.memory;
 if(c.invoking_ || !m.word || !m.storeWord || !m.storeByte || !s.binary)return {};
 if(c.active_ && (!same(c.request_,r) || !same(c.owner_,s)))return {};
 Guard guard(c.invoking_);
 if(!c.active_){c.active_=true;c.request_=r;c.owner_=s;c.phase_=Phase::Acquire;c.pending_=0;c.soundPosition_=r.position;}
 const auto& request=c.request_;
 const auto word=[&](uint32_t a){return m.word(m.user,a);};
 const auto put=[&](uint32_t a,uint32_t n){m.storeWord(m.user,a,n);};
 const auto add=[&](float a,float b){return s.binary(s.user,FukidashiBinary::Add,a,b);};
 const auto finish=[&](){const auto effect=c.effect_;c.active_=false;c.phase_=Phase::Acquire;c.pending_=0;return std::optional<uint32_t>(effect);};
 const auto addActor=[&](uint32_t actor,uint32_t offset){
  std::array<float,3> xyz{};for(unsigned i=0;i<3;++i)xyz[i]=real(word(actor+offset+4*i));
  for(unsigned i=0;i<3;++i)c.soundPosition_[i]=add(c.soundPosition_[i],xyz[i]);
 };
 for(;;)switch(c.phase_){
 case Phase::Acquire:{
  c.pending_=PoolEntry;const auto effect=effectPoolEntry(s.pool);if(!effect)return {};
  c.effect_=*effect;put(c.effect_+0x14,ClassName);put(c.effect_+8,1);c.phase_=Phase::Allocate;break;
 }
 case Phase::Allocate:{
  c.pending_=HeapAllocate;if(!s.pool.heapAllocate)return {};
  const auto payload=s.pool.heapAllocate(s.pool.user,PayloadHeap,PayloadBytes);if(!payload)return {};
  c.payload_=*payload;const auto p=c.payload_;
  put(c.effect_+0xc,p);put(c.effect_+0x10,Main);put(p+Kind,uint32_t(request.kind));put(p+Variant,uint32_t(request.variant));
  const auto duration=request.duration<0?uint32_t(ImmortalLifetime):uint32_t(request.duration);
  put(p+Duration,duration);const auto yOffset=real(word(InputYOffset));put(p+Remaining,duration);
  const auto zero=word(Zero);const float y=add(yOffset,request.position[1]);
  put(p+Counter,0);put(p+Attachment,request.attachment);put(p+Spin,zero);put(p+Alpha,255);put(p+Camera,4);
  put(p+InputX,bits(request.position[0]));put(p+InputY,bits(y));put(p+InputZ,bits(request.position[2]));
  put(p+InputAngle,bits(request.angle));put(p+Radius,bits(request.radius));put(p+SpinPhase,0);
  c.phase_=Phase::Position;break;
 }
 case Phase::Position:
  c.pending_=Position;if(!s.position || !s.position(s.user,c.payload_,1))return {};
  if(request.kind==SpinKind)put(c.payload_+Spin,word(InitialSpin));
  c.phase_=Phase::ResolveAttachment;break;
 case Phase::ResolveAttachment:{
  // Source re-reads payload attachment AFTER calc_pos, independently of the
  // original attachment argument used later to choose the constructor cue.
  const auto attachment=word(c.payload_+Attachment);
  if(attachment==MarioAttachment)c.phase_=Phase::Mario;
  else if(attachment==PartyAttachment || attachment==ExtraAttachment){c.extra_=attachment==ExtraAttachment;c.phase_=Phase::PartyId;}
  else {
   if(attachment)for(unsigned i=0;i<3;++i)c.soundPosition_[i]=real(word(attachment+ActorPosition+4*i));
   c.phase_=Phase::SelectSound;
  }
  break;
 }
 case Phase::Mario:{
  c.pending_=MarioGet;if(!s.mario)return {};const auto actor=s.mario(s.user);if(!actor)return {};
  addActor(*actor,ActorPosition);c.phase_=Phase::SelectSound;break;
 }
 case Phase::PartyId:{
  c.pending_=c.extra_?ExtraGetId:PartyGetId;if(!s.partyId)return {};
  const auto id=s.partyId(s.user,c.extra_);if(!id)return {};c.partyId_=*id;c.phase_=Phase::PartyPointer;break;
 }
 case Phase::PartyPointer:{
  c.pending_=PartyGetPointer;if(!s.partyPointer)return {};
  const auto actor=s.partyPointer(s.user,c.partyId_);if(!actor)return {};
  if(*actor)addActor(*actor,PartyPosition);
  c.phase_=Phase::SelectSound;break;
 }
 case Phase::SelectSound:{
  uint32_t vector=0;
  if(request.attachment==PartyAttachment || request.attachment==ExtraAttachment){
   if(request.kind==0){c.sound_=93;vector=PartyExclamationVector;}
   else if(request.kind==1){c.sound_=94;vector=PartyQuestionVector;}
  }else if(request.attachment && request.attachment!=MarioAttachment){
   if(request.kind==0){c.sound_=NpcExclamationKey;vector=NpcExclamationVector;}
   else if(request.kind==1){c.sound_=NpcQuestionKey;vector=NpcQuestionVector;}
  }
  if(!vector)return finish();
  // All template words are read even though the source then overwrites XYZ.
  for(unsigned i=0;i<3;++i)(void)word(vector+4*i);
  c.phase_=Phase::Sound;break;
 }
 case Phase::Sound:
  c.pending_=Sound;if(!s.sound || !s.sound(s.user,c.sound_,c.soundPosition_))return {};
  return finish();
 }
}
}
