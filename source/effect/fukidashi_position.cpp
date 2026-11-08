#include "fukidashi_position.hpp"
#include <cstring>

namespace ttyd {
namespace {
constexpr uint32_t AxisInitializer=0x802f3648,Bias=0x802f36a0,Radians=0x804228c4;
constexpr uint32_t BaseRadius=0x804228e0,AngleOffset=0x80422950,Divisor=0x80422954,Half=0x8042291c;
constexpr uint32_t CameraGet=0x8000f2fc,Sine=0x80272410,Cosine=0x80271ea8,Rotate=0x8029e1c0;
constexpr uint32_t MarioGet=0x8005e0c0,PartyGetId=0x80141050,ExtraGetId=0x80140fb8,PartyGetPointer=0x800bf1b4;
constexpr uint32_t MarioAttachment=0xffffffff,PartyAttachment=0xfffffffe,ExtraAttachment=0xfffffffd;
constexpr uint32_t CameraYaw=0x114,ActorPosition=0x8c,PartyPosition=0x58,ArgumentScale=20;
enum WorkOffset : uint32_t {ComputedPosition=4,InputPosition=0x10,InputAngle=0x1c,Radius=0x20,Rotation=0x28,Auxiliary=0x2c,Attachment=0x44};
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float negate(float f){return real(bits(f)^0x80000000u);}
bool same(const FukidashiPositionServices& a,const FukidashiPositionServices& b){
 const auto& x=a.memory;const auto& y=b.memory;
 return x.user==y.user && x.word==y.word && x.storeWord==y.storeWord && a.user==b.user &&
  a.binary==b.binary && a.fused==b.fused && a.biasConvert==b.biasConvert && a.camera==b.camera &&
  a.sine==b.sine && a.cosine==b.cosine && a.rotate==b.rotate && a.mario==b.mario && a.partyId==b.partyId && a.partyPointer==b.partyPointer;
}
struct Guard {bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
}
bool FukidashiPositionCursor::reset(){if(invoking_)return false;active_=false;phase_=Phase::Camera;pending_=0;return true;}
bool resumeFukidashiPosition(uint32_t payload,int32_t secondArgument,const FukidashiPositionServices& s,FukidashiPositionCursor& c){
 using Phase=FukidashiPositionCursor::Phase;
 if(c.invoking_ || !s.memory.word || !s.memory.storeWord || !s.binary || !s.fused || !s.biasConvert)return false;
 if(c.active_ && (c.payload_!=payload || c.secondArgument_!=secondArgument || !same(c.owner_,s)))return false;
 Guard guard(c.invoking_);
 if(!c.active_){c.active_=true;c.payload_=payload;c.secondArgument_=secondArgument;c.owner_=s;c.phase_=Phase::Camera;c.pending_=0;}
 const auto word=[&](uint32_t a){return s.memory.word(s.memory.user,a);};
 const auto scalar=[&](uint32_t a){return real(word(a));};
 const auto wide=[&](uint32_t a){const auto high=word(a),low=word(a+4);return uint64_t(high)<<32|low;};
 const auto put=[&](uint32_t a,float f){s.memory.storeWord(s.memory.user,a,bits(f));};
 const auto add=[&](float a,float b){return s.binary(s.user,FukidashiPositionBinary::Add,a,b);};
 const auto sub=[&](float a,float b){return s.binary(s.user,FukidashiPositionBinary::Subtract,a,b);};
 const auto mul=[&](float a,float b){return s.binary(s.user,FukidashiPositionBinary::Multiply,a,b);};
 for(;;)switch(c.phase_){
 case Phase::Camera:{
  c.pending_=CameraGet;if(!s.camera)return false;const auto camera=s.camera(s.user,4);if(!camera)return false;
  c.camera_=*camera;c.phase_=Phase::PrepareSine;break;
 }
 case Phase::PrepareSine:{
  const auto factor=scalar(Radians),yaw=scalar(c.camera_+CameraYaw);
  (void)word(AxisInitializer);c.axis_[1]=scalar(AxisInitializer+4);
  c.trigInput_=mul(factor,yaw);(void)word(AxisInitializer+8);c.phase_=Phase::Sine;break;
 }
 case Phase::Sine:{
  c.pending_=Sine;if(!s.sine)return false;const auto value=s.sine(s.user,c.trigInput_);if(!value)return false;
  c.axis_[0]=float(*value);c.phase_=Phase::PrepareCosine;break;
 }
 case Phase::PrepareCosine:{
  const auto factor=scalar(Radians),yaw=scalar(c.camera_+CameraYaw);
  c.trigInput_=mul(factor,yaw);c.phase_=Phase::Cosine;break;
 }
 case Phase::Cosine:{
  c.pending_=Cosine;if(!s.cosine)return false;const auto value=s.cosine(s.user,c.trigInput_);if(!value)return false;
  c.axis_[2]=negate(float(*value));c.argumentProduct_=uint32_t(secondArgument)*ArgumentScale;
  c.phase_=Phase::PrepareRotation;break;
 }
 case Phase::PrepareRotation:{
  const auto offset=scalar(AngleOffset);const auto bias=wide(Bias);const auto factor=scalar(Radians),angle=scalar(payload+InputAngle);
  const auto delta=sub(angle,offset);const auto converted=s.biasConvert(s.user,c.argumentProduct_,bias);
  c.rotationInput_=mul(factor,negate(add(delta,converted)));c.phase_=Phase::Rotation;break;
 }
 case Phase::Rotation:{
  c.pending_=Rotate;if(!s.rotate)return false;const auto matrix=s.rotate(s.user,c.axis_,c.rotationInput_);if(!matrix)return false;
  c.matrix_=*matrix;c.phase_=Phase::Attachment;break;
 }
 case Phase::Attachment:{
  const auto attachment=word(payload+Attachment);c.attached_=true;c.actorPosition_=ActorPosition;
  if(attachment==MarioAttachment)c.phase_=Phase::Mario;
  else if(attachment==PartyAttachment || attachment==ExtraAttachment){c.extra_=attachment==ExtraAttachment;c.phase_=Phase::PartyId;}
  else {c.actor_=attachment;c.attached_=attachment!=0;c.phase_=Phase::Coordinates;}
  break;
 }
 case Phase::Mario:{
  c.pending_=MarioGet;if(!s.mario)return false;const auto actor=s.mario(s.user);if(!actor)return false;
  c.actor_=*actor;c.phase_=Phase::Coordinates;break;
 }
 case Phase::PartyId:{
  c.pending_=c.extra_?ExtraGetId:PartyGetId;if(!s.partyId)return false;
  const auto id=s.partyId(s.user,c.extra_);if(!id)return false;c.partyId_=*id;c.phase_=Phase::PartyPointer;break;
 }
 case Phase::PartyPointer:{
  c.pending_=PartyGetPointer;if(!s.partyPointer)return false;const auto actor=s.partyPointer(s.user,c.partyId_);if(!actor)return false;
  c.actor_=*actor;c.actorPosition_=PartyPosition;c.phase_=*actor?Phase::Coordinates:Phase::Tail;break;
 }
 case Phase::Coordinates:{
  const auto baseRadius=scalar(BaseRadius);
  for(unsigned i=0;i<3;++i){
   if(c.attached_){
    const auto radius=scalar(payload+Radius),actor=scalar(c.actor_+c.actorPosition_+4*i),input=scalar(payload+InputPosition+4*i);
    const auto distance=add(baseRadius,radius),coefficient=c.matrix_[4*i+1],base=add(actor,input);
    put(payload+ComputedPosition+4*i,s.fused(s.user,coefficient,distance,base));
   }else {
    const auto radius=scalar(payload+Radius),coefficient=c.matrix_[4*i+1],distance=add(baseRadius,radius);
    const auto input=scalar(payload+InputPosition+4*i);
    put(payload+ComputedPosition+4*i,s.fused(s.user,coefficient,distance,input));
   }
  }
  c.phase_=Phase::Tail;break;
 }
 case Phase::Tail:{
  const auto angle=scalar(payload+InputAngle),offset=scalar(AngleOffset);const auto bias=wide(Bias);
  const auto delta=sub(angle,offset),divisor=scalar(Divisor);const auto converted=s.biasConvert(s.user,c.argumentProduct_,bias),half=scalar(Half);
  put(payload+Rotation,add(delta,converted));const auto radius=scalar(payload+Radius);
  const auto ratio=s.binary(s.user,FukidashiPositionBinary::Divide,radius,divisor);
  put(payload+Auxiliary,s.fused(s.user,half,ratio,half));
  c.active_=false;c.phase_=Phase::Camera;c.pending_=0;return true;
 }
 }
}
}
