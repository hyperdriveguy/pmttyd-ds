#include "battle_unit_rotation_events.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal=0x8041eb90,RotationOffset=0x6c;
constexpr int32_t UnchangedAxis=-250000000;
constexpr uint32_t ConversionExponent=0x43300000,ConversionSign=0x80000000;
constexpr uint32_t SignedConversionBias=0x802ee608,FullTurn=0x804226c8,Zero=0x804226a4;
uint32_t word(const EventMemoryServices&m,uint32_t a){assert(m.word);return m.word(m.user,a);}
float real(const EventMemoryServices&m,uint32_t a){const auto b=word(m,a);float v;std::memcpy(&v,&b,4);return v;}
void store(const EventMemoryServices&m,uint32_t a,float v){uint32_t b;std::memcpy(&b,&v,4);assert(m.storeWord);m.storeWord(m.user,a,b);}
int32_t signedWord(uint32_t v){return v<=0x7fffffff?int32_t(v):int32_t(int64_t(v)-0x100000000ll);}
float converted(int32_t v,const BattleUnitRotationEventServices&s){
 const auto& p=s.angle.motion.query.common.position;const auto&m=s.units.memory;
 // Original xoris/lis private double transport. Original fsubs consumes the
 // actual fresh bias and rounds the exact double subtraction to binary32.
 uint64_t bits=(uint64_t(ConversionExponent)<<32)|(uint32_t(v)^ConversionSign);double encoded;std::memcpy(&encoded,&bits,8);
 const auto high=word(m,SignedConversionBias),low=word(m,SignedConversionBias+4);
 bits=(uint64_t(high)<<32)|low;double bias;std::memcpy(&bias,&bits,8);
 assert(s.angle.subtract&&s.angle.motion.roundDouble);
 return s.angle.motion.roundDouble(p.numericUser,s.angle.subtract(p.numericUser,encoded,bias));
}
}
float convertPackedBattleRotationWord(int32_t value,const BattleUnitRotationEventServices&s){return converted(value,s);}
void addPackedBattleUnitRotate(uint32_t unit,const std::array<float,3>&delta,const BattleUnitRotationEventServices&s){
 const auto& p=s.angle.motion.query.common.position;const auto&m=s.units.memory;assert(p.add);
 for(unsigned axis=0;axis<3;++axis){const auto a=unit+RotationOffset+4*axis;
  const auto added=p.add(p.numericUser,real(m,a),delta[axis]);
  store(m,a,revisePackedJumpAngle(added,s.angle));
  // The successful store precedes both fresh threshold and result reads.
  const auto threshold=real(m,FullTurn);if(real(m,a)>=threshold)store(m,a,real(m,Zero));
 }
}
int32_t battleAddUnitRotateEvent(EventEntry&e,const BattleUnitRotationEventServices&s){
 const auto&u=s.units;const auto&m=u.memory;assert(u.values.resolve&&u.translate&&u.unit);
 const auto args=e.arguments;auto resolve=[&](unsigned i){return u.values.resolve(u.values.user,e,signedWord(word(m,args+4*i)));};
 const auto id=u.translate(u.user,e,resolve(0));std::array<float,3>delta;
 const auto x=resolve(1);const auto rawY=word(m,args+8);delta[0]=converted(x,s);
 const auto y=u.values.resolve(u.values.user,e,signedWord(rawY));const auto rawZ=word(m,args+12);delta[1]=converted(y,s);
 const auto z=u.values.resolve(u.values.user,e,signedWord(rawZ));const auto battle=word(m,BattleGlobal);delta[2]=converted(z,s);
 const auto unit=u.unit(u.user,battle,id);addPackedBattleUnitRotate(unit,delta,s);return 2;
}
void setPackedBattleUnitRotate(uint32_t unit,const std::array<float,3>&value,const BattleUnitRotationEventServices&s){
 const auto&m=s.units.memory;
 for(unsigned axis=0;axis<3;++axis){const auto a=unit+RotationOffset+4*axis;store(m,a,revisePackedJumpAngle(value[axis],s.angle));const auto threshold=real(m,FullTurn);if(real(m,a)>=threshold)store(m,a,real(m,Zero));}
}
int32_t battleSetUnitRotateEvent(EventEntry&e,const BattleUnitRotationEventServices&s){
 const auto&u=s.units;const auto&m=u.memory;assert(u.values.resolve&&u.translate&&u.unit);
 const auto args=e.arguments;auto resolve=[&](uint32_t raw){return u.values.resolve(u.values.user,e,signedWord(raw));};
 const auto id=u.translate(u.user,e,resolve(word(m,args)));std::array<uint32_t,3>raw;std::array<float,3>value;
 raw[0]=word(m,args+4);const auto x=resolve(raw[0]);raw[1]=word(m,args+8);value[0]=converted(x,s);
 const auto y=resolve(raw[1]);raw[2]=word(m,args+12);value[1]=converted(y,s);
 const auto z=resolve(raw[2]);const auto battle=word(m,BattleGlobal);value[2]=converted(z,s);
 const auto unit=u.unit(u.user,battle,id);std::array<float,3>previous;
 // Whole GetRotate80128898/1c: all three private outputs are source-produced,
 // including when none of the caller's raw operands is the sentinel.
 for(unsigned axis=0;axis<3;++axis)previous[axis]=real(m,unit+RotationOffset+4*axis);
 for(unsigned axis=0;axis<3;++axis)if(signedWord(raw[axis])==UnchangedAxis)value[axis]=previous[axis];
 setPackedBattleUnitRotate(unit,value,s);
 return 2;
}
int32_t battleCameraPriorityLimitEvent(EventEntry&e,const BattleUnitEventServices&s){
 const auto&m=s.memory;assert(s.values.resolve&&m.storeWord);const auto args=e.arguments;
 const auto priority=s.values.resolve(s.values.user,e,signedWord(word(m,args)));
 // Whole btl_camera_set_prilimit8012c0dc/c captures its own fresh global.
 const auto battle=word(m,BattleGlobal);m.storeWord(m.user,battle+0x275c,uint32_t(priority));return 2;
}
}
