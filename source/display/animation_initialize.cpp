#include "animation_initialize.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Parent=0x80042854;
float asFloat(uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
double asDouble(uint64_t bits){double value;std::memcpy(&value,&bits,8);return value;}
uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
float rounded(float value){volatile float result=value;return result;}
struct Operations {
 const AnimationInitializeServices& s;AnimationInitializeCursor& c;
 bool load(uint32_t a,uint32_t& value){auto v=s.memory.word?s.memory.word(s.memory.user,a):std::nullopt;if(!v)return false;value=*v;return true;}
 bool work(){return load(0x80415dc0,c.work);}
 bool store(uint32_t a,uint32_t value){return s.memory.store && s.memory.store(s.memory.user,a,value);}
 bool call(uint32_t function,std::array<uint32_t,5> args={}){
  if(!c.queued){c.call={};c.call.function=function;c.call.args=args;c.queued=true;}
  c.pending=function;if(!s.memory.call || !s.memory.call(s.memory.user,c.call))return false;
  c.queued=false;c.pending=Parent;return true;
 }
};
}
bool nativeAnimationTangent(void*,double angle,double& result){result=std::tan(angle);return true;}
bool initializeAnimation(const AnimationInitializeServices& s,AnimationInitializeCursor& c){
 if(!c.active){c={};c.active=true;c.pending=Parent;}
 Operations m{s,c};
 for(;;)switch(c.phase){
 case 0:if(!m.work())return false;++c.phase;break;
 case 1:{constexpr uint32_t offsets[]={4,0xc,0x14,0xf0,0xf4},values[]={64,64,256,1024,0};
  if(!m.store(c.work+offsets[c.index],values[c.index]))return false;
  if(++c.index<5){c.phase=0;break;}c.index=0;++c.phase;break;}
 case 2:if(!m.work())return false;++c.phase;break;
 case 3:{constexpr uint32_t offsets[]={4,0xc,0x14,0xf0},strides[]={16,12,0x170,12};
  if(!m.load(c.work+offsets[c.allocation],c.value))return false;
  c.value*=strides[c.allocation];++c.phase;break;}
 case 4:if(!m.call(0x800300f0,{0,c.value}))return false;c.pointer=c.call.result;++c.phase;break;
 case 5:if(!m.work())return false;++c.phase;break;
 case 6:{constexpr uint32_t offsets[]={0,8,0x10,0xec};
  if(!m.store(c.work+offsets[c.allocation],c.pointer))return false;
  if(++c.allocation==3){++c.phase;break;}c.phase=c.allocation<4?2:10;break;}
 case 7:if(!m.work())return false;++c.phase;break;
 case 8:if(!m.store(c.work+0xe8,0))return false;c.phase=2;break;
 case 10:if(!m.work())return false;++c.phase;break;
 case 11:{constexpr uint32_t offsets[]={0x30,0x34,0x38,0x24,0x28,0x2c,0xcc,0xd0,0xd4,0xd8,0xdc,0x104,0x10c};
  if(!m.store(c.work+offsets[c.index],c.index==9 || c.index==10?1:0))return false;
  if(++c.index<13){c.phase=10;break;}c.index=0;++c.phase;break;}
 case 12:if(!m.work())return false;++c.phase;break;
 case 13:if(!m.load(c.work+0x104,c.pointer))return false;c.phase=c.pointer?17:14;break;
 case 14:if(!m.call(0x800300f0,{0,0x180000}))return false;c.pointer=c.call.result;++c.phase;break;
 case 15:if(!m.work())return false;++c.phase;break;
 case 16:if(!m.store(c.work+0x104,c.pointer))return false;++c.phase;break;
 // Source captures W, then factor, then that W104, then double bias, then
 // stores that W108. Do not hoist count/constants or reload W mid-publication.
 case 17:if(!m.work())return false;++c.phase;break;
 case 18:if(!m.load(0x8041fbb4,c.value))return false;c.factor=asFloat(c.value);++c.phase;break;
 case 19:if(!m.load(c.work+0x104,c.pointer))return false;++c.phase;break;
 case 20:{const auto value=s.doubleWord?s.doubleWord(s.memory.user,0x802c1070):std::nullopt;
  if(!value)return false;
  c.bias=asDouble(*value);c.phase=22;break;}
 case 22:if(!m.store(c.work+0x108,c.pointer))return false;++c.phase;break;
 case 23:{const uint32_t low=uint32_t(int32_t(c.index)-89)^0x80000000u;
  const float converted=rounded(float(asDouble(0x4330000000000000ULL|low)-c.bias));
  c.angle=double(rounded(c.factor*converted));++c.phase;break;}
 case 24:c.pending=0x802724e8;if(!s.tangent || !s.tangent(s.tangentUser,c.angle,c.tangentResult))return false;
  c.value=bits(rounded(float(c.tangentResult)));c.pending=Parent;++c.phase;break;
 case 25:if(!m.store(0x803d9580+4*c.index,c.value))return false;
  c.phase=++c.index<178?23:26;break;
 case 26:if(!m.call(0x8018a61c,{0,0x802c11d8}))return false;c.handle=c.call.result;c.phase=c.handle?28:36;break;
 case 28:if(!m.load(c.handle+0xa0,c.holder))return false;++c.phase;break;
 case 29:if(!m.load(0x80415db4,c.destination))return false;++c.phase;break;
 case 30:if(!m.load(c.holder,c.payload))return false;++c.phase;break;
 case 31:if(!m.call(0x80005634,{c.destination,c.payload,0xc00}))return false;++c.phase;break;
 case 32:if(!m.load(c.handle+0xa0,c.holder))return false;++c.phase;break;
 case 33:if(!m.load(c.holder,c.payload))return false;++c.phase;break;
 case 34:if(!m.call(0x80005634,{0x80415da0,c.payload+0xc00,16}))return false;++c.phase;break;
 case 35:if(!m.call(0x8018a260,{c.handle}))return false;++c.phase;break;
 case 36:if(!m.call(0x80189d38,{0}))return false;++c.phase;break;
 case 37:if(!m.call(0x8018a61c,{0,0x802c11e8}))return false;c.handle=c.call.result;++c.phase;break;
 case 38:if(!m.work())return false;++c.phase;break;
 case 39:if(!m.store(c.work+0x100,c.handle))return false;c.active=false;c.pending=0;return true;
 default:return false;
 }
}
}
