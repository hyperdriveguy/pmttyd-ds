#include "animation_main_time.hpp"
#include "animation_sample_time.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
double real64(uint64_t b){double f;std::memcpy(&f,&b,8);return f;}
float single(float f){volatile float x=f;return x;}
bool same(const AnimationMainEvaluation& a,const AnimationMainEvaluation& b){return a.work==b.work && a.pool==b.pool && a.pose==b.pose && a.model==b.model && a.timeline==b.timeline && a.positions==b.positions && a.normals==b.normals;}
bool same(const AnimationMainTimeServices& a,const AnimationMainTimeServices& b){return a.user==b.user && a.word==b.word && a.store==b.store && a.doubleWord==b.doubleWord && a.divide==b.divide && a.convertUnsigned==b.convertUnsigned && a.truncate==b.truncate;}
}
bool advanceAnimationMainTime(const AnimationMainEvaluation& input,const AnimationMainTimeServices& s,AnimationMainTimeCursor& c,AnimationMainTimeResult& out){
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;}else if(!same(c.input,input) || !same(c.owner,s))return false;
 const auto r=input.pose,t=input.timeline;
 auto word=[&](uint32_t a,uint32_t& v){if(!s.word)return false;auto w=s.word(s.user,a);if(!w)return false;v=*w;return true;};
 auto load=[&](uint32_t a,float& f){uint32_t b;if(!word(a,b))return false;f=real(b);return true;};
 auto store=[&](uint32_t a,float f){return s.store && s.store(s.user,a,bits(f));};
 for(;;)switch(c.phase){
 case 0:if(!word(r+0xc,c.mode))return false;++c.phase;break;
 case 1:if(!word(t+0x24,c.control))return false;++c.phase;break;
 case 2:if(!word(c.control,c.loop))return false;++c.phase;break;
 case 3:if(!word(0x80415fd0,c.gp))return false;++c.phase;break;
 case 4:if(!word(0x800000f8,c.bus))return false;++c.phase;break;
 case 5:if(!word(c.gp+(c.mode?0x38:0x40),c.hi))return false;++c.phase;break;
 case 6:if(!word(c.gp+(c.mode?0x3c:0x44),c.lo))return false;c.divisor=uint32_t((uint64_t(0x10624dd3)*(c.bus>>2))>>32)>>6;++c.phase;break;
 case 7:if(!s.divide || !s.divide(s.user,uint64_t(c.hi)<<32|c.lo,c.divisor,c.quotient))return false;++c.phase;break;
 case 8:if(!word(r+0x1c,c.epochLo))return false;++c.phase;break;
 case 9:if(!word(r+0x18,c.epochHi))return false;++c.phase;break;
 case 10:if(!s.convertUnsigned || !s.convertUnsigned(s.user,c.quotient-(uint64_t(c.epochHi)<<32|c.epochLo),c.converted))return false;++c.phase;break;
 case 11:if(!load(0x8041fba4,c.sixty))return false;++c.phase;break;
 case 12:if(!load(0x8041fba8,c.thousand))return false;c.frame=single(c.sixty*c.converted);++c.phase;break;
 case 13:if(!load(0x8041fb28,c.zero))return false;c.frame=single(c.frame/c.thousand);++c.phase;break;
 case 14:if(!store(r+0x20,c.frame))return false;++c.phase;break;
 case 15:if(!load(r+0x20,c.frame))return false;++c.phase;break;
 case 16:if(!load(r+0x44,c.rate))return false;c.frame=single(c.frame*c.rate);++c.phase;break;
 case 17:if(!store(r+0x20,c.frame))return false;++c.phase;break;
 case 18:if(!word(t+0x24,c.control))return false;++c.phase;break;
 case 19:if(!load(r+0x20,c.frame))return false;++c.phase;break;
 case 20:if(!load(c.control+8,c.end))return false;++c.phase;break;
 case 21:if(!load(c.control+4,c.start))return false;c.span=single(c.end-c.start);c.selected=c.start;
  if(c.span==c.zero){c.ratio=single(c.frame-c.start);c.phase=40;}
  else if(c.loop){c.ratio=single(c.frame-c.start);if(c.ratio<c.zero){c.ratio=c.zero;c.phase=40;}else{c.ratio=single(c.ratio/c.span);c.phase=30;}}
  else{if(c.frame<c.start)c.ratio=c.zero;else{c.ratio=single(single(c.frame-c.start)/c.span);c.selected=c.frame>c.end?c.end:c.frame;}c.phase=40;}break;
 case 30:{if(!s.doubleWord)return false;auto v=s.doubleWord(s.user,0x802c1070);if(!v)return false;c.bias=*v;++c.phase;break;}
 case 31:if(!store(r+0x84,c.ratio))return false;++c.phase;break;
 case 32:if(!load(r+0x84,c.ratio))return false;++c.phase;break;
 case 33:{if(!s.truncate)return false;auto v=s.truncate(s.user,c.ratio);if(!v)return false;c.integer=*v;
  const uint64_t encoded=0x4330000000000000ull|uint32_t(uint32_t(c.integer)^0x80000000u);
  const float index=single(float(real64(encoded)-real64(c.bias)));c.selected=animationFnmsubs(index,c.span,c.frame);c.phase=41;break;}
 case 40:if(!store(r+0x84,c.ratio))return false;++c.phase;break;
 case 41:if(!store(r+0x24,c.selected))return false;out.loop=c.loop;c={};return true;
 default:return false;
 }
}
}
