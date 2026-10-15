#include "animation_main_interpolation.hpp"
#include "animation_sample_time.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
float real(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}
float single(float f){volatile float v=f;return v;}
bool less(uint32_t a,uint32_t b){return (a^0x80000000u)<(b^0x80000000u);}
int signedByte(uint32_t v){return v<128?int(v):int(v)-256;}
bool same(const AnimationMainEvaluation& a,const AnimationMainEvaluation& b){return a.work==b.work && a.pool==b.pool && a.pose==b.pose && a.model==b.model && a.timeline==b.timeline && a.positions==b.positions && a.normals==b.normals;}
bool same(const AnimationMainReplayResult& a,const AnimationMainReplayResult& b){return a.current==b.current && a.next==b.next && bits(a.fraction)==bits(b.fraction) && a.counts==b.counts;}
bool same(const AnimationMainReplayServices& a,const AnimationMainReplayServices& b){return a.user==b.user && a.word==b.word && a.store==b.store && a.byte==b.byte && a.storeByte==b.storeByte && a.quantized==b.quantized && a.copy==b.copy;}
bool same(const AnimationMainTimeServices& a,const AnimationMainTimeServices& b){return a.user==b.user && a.word==b.word && a.store==b.store && a.doubleWord==b.doubleWord && a.divide==b.divide && a.convertUnsigned==b.convertUnsigned && a.truncate==b.truncate;}
}
bool advanceAnimationMainInterpolation(const AnimationMainEvaluation& input,const AnimationMainReplayResult& captured,const AnimationMainReplayServices& s,AnimationMainInterpolationCursor& c){
 if(!c.active){c={};c.active=true;c.input=input;c.captured=captured;c.owner=s;}else if(!same(c.input,input) || !same(c.captured,captured) || !same(c.owner,s))return false;
 const uint32_t r=input.pose,t=input.timeline;
 auto word=[&](uint32_t a,uint32_t& v){if(!s.word)return false;auto x=s.word(s.user,a);if(!x)return false;v=*x;return true;};
 auto load=[&](uint32_t a,float& v){uint32_t w;if(!word(a,w))return false;v=real(w);return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 auto byte=[&](uint32_t a,uint32_t& v){if(!s.byte)return false;auto x=s.byte(s.user,a);if(!x)return false;v=*x;return true;};
 auto quantized=[&](uint32_t a,float& v){if(!s.quantized)return false;auto x=s.quantized(s.user,a);if(!x)return false;v=*x;return true;};
 auto beginCopy=[&](uint32_t destination,uint32_t source,uint32_t n){c.copy={};c.copy.function=0x80005634;c.copy.destination=destination;c.copy.source=source;c.copy.bytes=n;};
 for(;;)switch(c.phase){
 case 0:c.channel=0;c.phase=captured.current==captured.next?1:less(captured.current,captured.next)?10:90;break;
 // Equal-key outputs use the original source cache and captured model counts.
 case 1:if(c.channel==3){c.phase=90;break;}c.count=captured.counts[c.channel==2?3:c.channel];if(!c.count){++c.channel;break;}c.phase=c.channel==2?2:3;break;
 case 2:if(!word(r+0x64,c.destination))return false;++c.phase;break;
 case 3:if(!word(r+(c.channel==0?0x48:c.channel==1?0x50:0x60),c.cache))return false;
  beginCopy(c.channel==2?c.destination:c.channel==0?input.positions:input.normals,c.cache,c.count*(c.channel==2?4u:12u));++c.phase;break;
 case 4:if(!advanceAnimationReplayCopy(s,c.copy))return false;++c.channel;c.phase=1;break;
 // Increasing-key setup. NEXT key pointer is retained across all copies.
 case 10:if(!word(t+0x28,c.table))return false;c.key=c.table+captured.next*44u;++c.phase;break;
 case 11:if(!load(0x8041fbac,c.scale))return false;++c.phase;break;
 case 12:if(!word(t+0x2c,c.stream))return false;++c.phase;break;
 case 13:if(!word(c.key+4,c.offset))return false;c.scaledFraction=single(c.scale*captured.fraction);++c.phase;break;
 case 14:if(!word(c.key+8,c.count))return false;++c.phase;break;
 case 15:if(!word(r+0x48,c.cache))return false;c.source=c.stream+c.offset*4u;c.end=c.source+c.count*4u;c.destination=input.positions;
  if(captured.counts[0]){beginCopy(input.positions,c.cache,captured.counts[0]*12u);c.phase=16;}else c.phase=17;break;
 case 16:if(!advanceAnimationReplayCopy(s,c.copy))return false;++c.phase;break;
 case 17:if(!(c.source<c.end)){c.phase=30;break;}c.remaining=uint32_t(c.end+3u-c.source)>>2;c.phase=20;break;
 // Both vector channels capture three source deltas BEFORE destination reads.
 case 20:if(!byte(c.source,c.index))return false;c.destination+=12u*c.index;c.lane=0;++c.phase;break;
 case 21:if(!quantized(c.source+1+c.lane,c.deltas[c.lane]))return false;if(++c.lane<3)break;c.lane=0;++c.phase;break;
 case 22:if(!load(c.destination+4*c.lane,c.oldValue))return false;c.newValue=std::fma(c.deltas[c.lane],c.scaledFraction,c.oldValue);++c.phase;break;
 case 23:if(!store(c.destination+4*c.lane,bits(c.newValue)))return false;if(++c.lane<3){c.phase=22;break;}c.source+=4;if(--c.remaining){c.phase=20;return false;}c.phase=c.channel==0?30:40;break;
 // Auxiliary setup uses the SAME previously captured next-key pointer.
 case 30:c.channel=1;if(!word(c.key+12,c.offset))return false;++c.phase;break;
 case 31:if(!word(c.key+16,c.count))return false;c.destination=input.normals;++c.phase;break;
 case 32:if(!word(t+0x30,c.stream))return false;++c.phase;break;
 case 33:if(!word(r+0x50,c.cache))return false;c.source=c.stream+c.offset*4u;c.end=c.source+c.count*4u;
  if(captured.counts[1]){beginCopy(input.normals,c.cache,captured.counts[1]*12u);c.phase=34;}else c.phase=35;break;
 case 34:if(!advanceAnimationReplayCopy(s,c.copy))return false;++c.phase;break;
 case 35:if(!(c.source<c.end)){c.phase=40;break;}c.remaining=uint32_t(c.end+3u-c.source)>>2;c.phase=20;break;
 // Node duration reloads the time table; headers still use the captured key.
 case 40:if(!word(t+0x28,c.table))return false;++c.phase;break;
 case 41:if(!word(c.key+36,c.offset))return false;++c.phase;break;
 case 42:if(!word(c.key+40,c.count))return false;++c.phase;break;
 case 43:if(!load(c.table+captured.next*44u,c.nextTime))return false;++c.phase;break;
 case 44:if(!load(c.table+captured.current*44u,c.oldValue))return false;++c.phase;break;
 case 45:if(!word(t+0x3c,c.stream))return false;c.duration=single(c.nextTime-c.oldValue);++c.phase;break;
 case 46:if(!word(r+0x60,c.cache))return false;c.source=c.stream+c.offset*4u;c.end=c.source+c.count*4u;++c.phase;break;
 case 47:if(!word(r+0x64,c.destination))return false;
  if(captured.counts[3]){beginCopy(c.destination,c.cache,captured.counts[3]*4u);c.phase=48;}else c.phase=49;break;
 case 48:if(!advanceAnimationReplayCopy(s,c.copy))return false;++c.phase;break;
 case 49:c.square=single(captured.fraction*captured.fraction);if(!(c.source<c.end)){c.phase=90;break;}c.remaining=uint32_t(c.end+3u-c.source)>>2;c.phase=50;break;
 // Tangent gate reads byte3 BEFORE byte0 and still advances sparse destination.
 case 50:if(!byte(c.source+3,c.angle))return false;++c.phase;break;
 case 51:if(!byte(c.source,c.index))return false;c.destination+=4u*c.index;c.phase=signedByte(c.angle)>=89 || signedByte(c.angle)<=-89?79:52;break;
 case 52:if(!quantized(c.source+1,c.delta))return false;c.cube=single(captured.fraction*c.square);++c.phase;break;
 case 53:if(!byte(c.source+2,c.index))return false;++c.phase;break;
 case 54:if(!load(0x8041fb64,c.two))return false;c.h10=animationFnmsubs(c.two,c.square,c.cube);++c.phase;break;
 case 55:if(!load(0x8041fbb0,c.three))return false;++c.phase;break;
 case 56:if(!load(0x803d9580u+0x164u+uint32_t(signedByte(c.index))*4u,c.tangent))return false;c.h01=single(c.three*c.square);++c.phase;break;
 case 57:if(!load(0x8041fb98,c.negativeTwo))return false;c.h10=single(captured.fraction+c.h10);c.tangent=single(c.duration*c.tangent);++c.phase;break;
 case 58:if(!load(0x8041fbac,c.scale))return false;++c.phase;break;
 case 59:if(!load(0x803d9580u+0x164u+uint32_t(signedByte(c.angle))*4u,c.otherTangent))return false;
  c.h01=std::fma(c.negativeTwo,c.cube,c.h01);c.delta=single(c.scale*c.delta);c.term=single(c.h10*c.tangent);++c.phase;break;
 case 60:if(!load(c.destination,c.oldValue))return false;
  c.h10=single(c.cube-c.square);c.otherTangent=single(c.duration*c.otherTangent);c.term=std::fma(c.h01,c.delta,c.term);c.term=std::fma(c.h10,c.otherTangent,c.term);c.newValue=single(c.oldValue+c.term);++c.phase;break;
 case 61:if(!store(c.destination,bits(c.newValue)))return false;c.phase=79;break;
 case 79:c.source+=4;if(--c.remaining){c.phase=50;return false;}c.phase=90;break;
 // All three control paths publish the retained current then retained fraction.
 case 90:if(!store(r+0x3c,captured.current))return false;++c.phase;break;
 case 91:if(!store(r+0x40,bits(captured.fraction)))return false;c={};return true;
 default:return false;
 }
}
bool advanceAnimationMainGeometry(const AnimationMainEvaluation& input,const AnimationMainTimeServices& time,const AnimationMainReplayServices& memory,AnimationMainGeometryCursor& c){
 if(!c.active){c={};c.active=true;c.input=input;c.timeOwner=time;c.memoryOwner=memory;}
 else if(!same(c.input,input) || !same(c.timeOwner,time) || !same(c.memoryOwner,memory))return false;
 if(c.phase==0){if(!advanceAnimationMainCache(input,time,memory,c.cache,c.captured))return false;c.phase=1;}
 if(!advanceAnimationMainInterpolation(input,c.captured,memory,c.interpolation))return false;
 c={};return true;
}
}
