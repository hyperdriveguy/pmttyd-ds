#include "field_bero_position.hpp"
#include "event_manager.hpp"
#include <cmath>
namespace ttyd {
namespace {
// Explicit single-precision boundaries preserve source operation grouping;
// supplied trig/FMA services still own numerical compatibility.
float single(float value){volatile float rounded=value;return rounded;}
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
std::optional<FieldBeroPosition> fieldBeroNormalPosition(std::array<int32_t,3> xyz,
 const std::string& name,int32_t direction,int32_t mode,float distance,const FieldBeroPositionServices& s) {
 FieldBeroPosition out;
 out.start=out.finish={float(xyz[0]),float(xyz[1]),float(xyz[2])};
 // X sentinel 100000 requests named center lookup, replacing all XYZ.
 if(xyz[0]==100000 && (!s.center || !s.center(s.user,name,out.start)))return std::nullopt;
 out.center=out.start;
 // Zero distance bypasses all trig/floor services. Nonzero uses direction/8
 // turns; mode1 reverses by 180deg. Start backs up 30, floor casts down
 // 1,000,000, finish advances 30+distance, recovery adds 37 to finish Y.
 if(distance!=0) {
  if(!s.floor || !s.sine || !s.cosine || !s.negativeMultiplySubtract || !s.multiplyAdd)return std::nullopt;
  auto heading=single(single(360.f*float(direction))*.125f);
  if(mode==1)heading=single(heading+180.f);
  const auto radians=single(single(0x1.921fb6p+2f*heading)/360.f);
  const auto sx=s.sine(s.user,radians);
  out.start.x=s.negativeMultiplySubtract(s.user,30.f,sx,out.start.x);
  const auto cz=s.cosine(s.user,radians);
  out.start.z=s.negativeMultiplySubtract(s.user,30.f,-cz,out.start.z);
  const auto floor=s.floor(s.user,out.start,{0,-1,0},1000000.f);
  if(!floor.resolved)return std::nullopt;
  if(floor.hit)out.start.y=floor.y;
  out.finish=out.start;
  const auto nextSine=s.sine(s.user,radians);
  const auto length=single(30.f+distance);
  out.finish.x=s.multiplyAdd(s.user,length,nextSine,out.finish.x);
  const auto nextCosine=s.cosine(s.user,radians);
  out.finish.z=s.multiplyAdd(s.user,length,-nextCosine,out.finish.z);
 }
 out.recovery={out.finish.x,single(37.f+out.finish.y),out.finish.z};
 return out;
}
// EVT float literal: truncate(single(value*1024)) then subtract 230000000
// with 32-bit wrap. Values <=-220000000 decode via +230000000 and /1024;
// others remain numeric integers. Reject exceptional fctiwz domain, do not clamp.
std::optional<FieldBeroStartPublication> fieldBeroStartPublication(FieldWallVector p) {
 FieldBeroStartPublication result;const float values[]={p.x,p.y,p.z};float decoded[3];
 for(unsigned n=0;n<3;++n) {
  const auto product=single(values[n]*1024.f);
  if(!std::isfinite(product) || double(product)<INT32_MIN || double(product)>=2147483648.)return std::nullopt;
  const auto encoded=signedWord(uint32_t(int32_t(product))-230000000u);result.words[n]=encoded;
  decoded[n]=encoded<=-220000000?single(float(signedWord(uint32_t(encoded)+230000000u))*.0009765625f):float(encoded);
 }
 result.position={decoded[0],decoded[1],decoded[2]};return result;
}
bool publishFieldBeroStart(EventEntry& e,const FieldBeroStartPublication& start) {
 for(unsigned n=0;n<3;++n)e.locals[10+n]=start.words[n];
 return true;
}
}
