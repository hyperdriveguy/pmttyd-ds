#include "field_npc_actor_event.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// Original US flag/position/get-position/direct-yaw/left-right callbacks.
// Addresses are guest dispatch identities, not host function pointers.
constexpr uint32_t Flag=0x800500b0,Position=0x80053158,Get=0x80052e98,Yaw=0x80050988,LeftRight=0x8005088c;
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
bool integer(float f,int32_t& v){if(!std::isfinite(f) || double(f)<-2147483648. || double(f)>=2147483648.)return false;v=int32_t(f);return true;}
// Native libm adaptation of reviseAngle: original double fmod, SP rounding,
// negative-zero preservation and post-add360 guard. No hardware parity claim.
float revise(float f){float r=float(std::fmod(double(f),360.));if(std::isnan(r))r=0.f;if(r<0.f){r=r+360.f;if(r>=360.f)r=0.f;}return r;}
}
bool fieldNpcActorEventFunction(uint32_t fn){return fn==Flag || fn==Position || fn==Get || fn==Yaw || fn==LeftRight;}
// Arguments capture once before resolution. Flag selector0 clears/1 sets,
// other selectors still resolve name but do not dereference actor. Position/Get
// use3 engine-space scalars; Get captures RAW destinations, not their values.
int32_t fieldNpcActorEvent(uint32_t fn,EventEntry& e,bool first,FieldNpcActorEventCursor& c,const FieldNpcActorEventServices& s){
 if(!fieldNpcActorEventFunction(fn) || !s.word || !s.value || !s.real || !s.resolve || !s.store || (fn==Get && !s.setReal))return 0;
 if(first)c={};
 if(!c.initialized){c.initialized=true;c.owner=s.user;c.entry=&e;c.id=e.id;c.function=fn;c.args=e.arguments;}
 if(c.owner!=s.user || c.entry!=&e || c.id!=e.id || c.function!=fn)return 0;
 const unsigned count=fn==Flag?3:fn==Position || fn==Get?4:2;
 if(c.stage==FieldNpcActorEventCursor::Stage::Arguments){
  for(;c.argument<count;++c.argument){const auto n=c.argument;uint32_t raw=0;if(uint64_t(c.args)+4*n>UINT32_MAX || !s.word(s.user,c.args+4*n,raw))return 0;
   if(fn==Flag){int32_t v=0;if(!s.value(s.user,e,signedWord(raw),v))return 0;if(n==0)c.selector=v;else if(n==1)c.name=uint32_t(v);else c.mask=uint32_t(v);}
   else if(!n){int32_t v=0;if(!s.value(s.user,e,signedWord(raw),v))return 0;c.name=uint32_t(v);}
   else if(fn==Get)c.destinations[n-1]=signedWord(raw);
   else {float v=0;if(!s.real(s.user,e,signedWord(raw),v))return 0;if(fn==LeftRight){if(!integer(v,c.selector))return 0;}else c.values[n-1]=v;}
  }
  c.stage=FieldNpcActorEventCursor::Stage::Resolve;
 }
 if(c.stage==FieldNpcActorEventCursor::Stage::Resolve){const auto a=s.resolve(s.user,e,c.name,fn!=Flag || c.selector==0 || c.selector==1);if(!a)return 0;c.actor=*a;c.stage=fn==LeftRight?FieldNpcActorEventCursor::Stage::Camera:FieldNpcActorEventCursor::Stage::Effects;}
 // LeftRight requires published camera4 yaw, original100(fde4)/5(fde0).
 // Revise(FMA(100, yaw, 5)/100), truncate int, then signed remainder(n+/-90)%360.
 // Result can be negative; direct Yaw below stores raw input without this math.
 if(c.stage==FieldNpcActorEventCursor::Stage::Camera){
  if(!s.cameraHeading)return 0;
  const auto h=s.cameraHeading(s.user,4);if(!h)return 0;
  uint32_t a=0,b=0;if(!s.word(s.user,0x8041fde4,a) || !s.word(s.user,0x8041fde0,b))return 0;
  const float t=std::fma(real(a),*h,real(b));const float u=t/real(a);int32_t n=0;if(!integer(revise(u),n))return 0;
  c.values[0]=float(c.selector==0?(n-90)%360:(n+90)%360);c.stage=FieldNpcActorEventCursor::Stage::Effects;
 }
 if(c.stage==FieldNpcActorEventCursor::Stage::Effects){
  if(fn==Flag){if(c.selector==0 || c.selector==1){uint32_t flags=0;if(!s.word(s.user,c.actor,flags))return 0;s.store(s.user,c.actor,c.selector?flags|c.mask:flags&~c.mask);}}
   // Read original unused zero-vector802c1470 first. Publish previous root
   //98/9c/a0, then current8c/90 from paired reloads and94 from LATE Z.
   // Do not collapse stores into one Vec3 or remove service-visible reads.
  else if(fn==Position){
   uint32_t zero=0;for(unsigned n=0;n<3;++n)if(!s.word(s.user,0x802c1470+4*n,zero))return 0;
   for(unsigned n=0;n<3;++n)s.store(s.user,c.actor+0x98+4*n,bits(c.values[n]));
   uint32_t x=0,y=0,z=0;const bool xy=s.word(s.user,c.actor+0x98,x) && s.word(s.user,c.actor+0x9c,y);assert(xy);(void)xy;
   s.store(s.user,c.actor+0x8c,x);s.store(s.user,c.actor+0x90,y);
   const bool ok=s.word(s.user,c.actor+0xa0,z);assert(ok);(void)ok;s.store(s.user,c.actor+0x94,z);
  }else if(fn==Get)c.stage=FieldNpcActorEventCursor::Stage::Output;
  else s.store(s.user,c.actor+0x144,bits(c.values[0]));
 }
 // Capture one source axis, then retry its checked setter without rereading
 // that axis. After success, the next axis is fresh; completed earlier outputs
 // never replay. This is per-output admission, not atomic XYZ publication.
 if(c.stage==FieldNpcActorEventCursor::Stage::Output){
  for(;c.output<3;++c.output){if(!c.outputCaptured){uint32_t v=0;if(!s.word(s.user,c.actor+0x8c+4*c.output,v))return 0;c.values[c.output]=real(v);c.outputCaptured=true;}
   if(!s.setReal(s.user,e,c.destinations[c.output],c.values[c.output]))return 0;
   c.outputCaptured=false;
  }
 }
 c={};return 2;
}
}
