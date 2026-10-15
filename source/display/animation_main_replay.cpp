#include "animation_main_replay.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t CountOffsets[]={0xf0,0xf8,0x13c,0x140,0x12c},CacheOffsets[]={0x48,0x50,0x58,0x60,0x68},BaseOffsets[]={0x154,0x15c,0x1a0,0x1a4,0x190},KeyOffsets[]={4,12,28,36,20},StreamOffsets[]={0x2c,0x30,0x38,0x3c,0x34},Widths[]={4,4,2,4,12},DestinationWidths[]={12,12,1,4,24};
float real(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
float single(float f){volatile float x=f;return x;}
bool less(uint32_t a,uint32_t b){return (a^0x80000000u)<(b^0x80000000u);}
bool same(const AnimationMainEvaluation& a,const AnimationMainEvaluation& b){return a.work==b.work && a.pool==b.pool && a.pose==b.pose && a.model==b.model && a.timeline==b.timeline && a.positions==b.positions && a.normals==b.normals;}
bool same(const AnimationMainReplayServices& a,const AnimationMainReplayServices& b){return a.user==b.user && a.word==b.word && a.store==b.store && a.byte==b.byte && a.storeByte==b.storeByte && a.quantized==b.quantized;}
// One original copy load/store iteration per visit. remaining0 after setup is
// intentional for memcpy_as4: its first body executes before CTR decrements.
}
bool advanceAnimationReplayCopy(const AnimationMainReplayServices& s,AnimationReplayCopy& c){
 if(!c.active){c.active=true;c.remaining=c.function==0x80005634?c.bytes>>2:c.bytes;if(c.function==0x8000519c){if(!c.bytes){c={};return true;}c.backward=c.source<c.destination;if(c.backward){c.source+=c.bytes-1;c.destination+=c.bytes-1;}}}
 if(!c.loaded){
  if(c.function==0x80005634){if(!s.word)return false;auto v=s.word(s.user,c.source);if(!v)return false;c.value=*v;}
  else{if(!s.byte)return false;auto v=s.byte(s.user,c.source);if(!v)return false;c.value=*v;}
  c.loaded=true;
 }
 if(c.function==0x80005634){if(!s.store || !s.store(s.user,c.destination,c.value))return false;c.source+=4;c.destination+=4;}
 else{if(!s.storeByte || !s.storeByte(s.user,c.destination,uint8_t(c.value)))return false;const uint32_t step=c.backward?0xffffffffu:1u;c.source+=step;c.destination+=step;}
 c.loaded=false;if(--c.remaining){return false;}c={};return true;
}
bool advanceAnimationMainReplay(const AnimationMainEvaluation& input,const AnimationMainReplayServices& s,AnimationMainReplayCursor& c,AnimationMainReplayResult& out){
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;}else if(!same(c.input,input) || !same(c.owner,s))return false;
 const uint32_t r=input.pose,d=input.model,t=input.timeline;
 auto word=[&](uint32_t a,uint32_t& v){if(!s.word)return false;auto x=s.word(s.user,a);if(!x)return false;v=*x;return true;};
 auto load=[&](uint32_t a,float& v){uint32_t w;if(!word(a,w))return false;v=real(w);return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 auto byte=[&](uint32_t a,uint32_t& v){if(!s.byte)return false;auto x=s.byte(s.user,a);if(!x)return false;v=*x;return true;};
 auto quantized=[&](uint32_t a,float& v){if(!s.quantized)return false;auto x=s.quantized(s.user,a);if(!x)return false;v=*x;return true;};
 auto finish=[&](){out=c.result;c={};return true;};
 for(;;)switch(c.phase){
 case 0:if(!word(r+0x28,c.result.current))return false;++c.phase;break;
 case 1:if(!word(r+0x3c,c.old))return false;++c.phase;break;
 case 2:if(!word(r+0x2c,c.result.next))return false;++c.phase;break;
 case 3:if(!load(r+0x30,c.result.fraction))return false;c.channel=0;++c.phase;break;
 case 4:if(!word(d+CountOffsets[c.channel],c.result.counts[c.channel]))return false;if(++c.channel<5)break;
  c.channel=0;if(c.old==c.result.current)c.phase=90;else{c.rebuild=c.old==0xffffffff || less(c.result.current,c.old);c.phase=c.rebuild?5:20;}break;
 case 5:if(!word(t+0x28,c.key))return false;c.end=c.key+c.result.current*44u+44u;c.channel=0;++c.phase;break;
 case 6:if(c.channel==5){c.phase=10;break;}if(!c.result.counts[c.channel]){++c.channel;break;}
  c.copy={};c.copy.function=c.channel==2?0x8000519c:0x80005634;c.copy.bytes=c.result.counts[c.channel]*DestinationWidths[c.channel];++c.phase;break;
 case 7:if(!word(r+CacheOffsets[c.channel],c.copy.destination))return false;++c.phase;break;
 case 8:if(!word(d+BaseOffsets[c.channel],c.copy.source))return false;++c.phase;break;
 case 9:if(!advanceAnimationReplayCopy(s,c.copy))return false;++c.channel;c.phase=6;break;
 case 10:if(!word(r+0x3c,c.word))return false;c.phase=c.word==0xffffffff?11:12;break;
 case 11:if(!store(r+0x3c,0))return false;++c.phase;break;
 case 12:if(!load(0x8041fbac,c.scale))return false;c.phase=22;break;
 case 20:if(!word(t+0x28,c.key))return false;c.end=c.key+(c.result.current+1)*44u;c.key+=(c.old+1)*44u;++c.phase;break;
 case 21:if(!load(0x8041fbac,c.scale))return false;++c.phase;break;
 case 22:if(!(c.key<c.end))return finish();c.channel=0;c.phase=30;break;
 case 30:if(!word(c.key+KeyOffsets[c.channel],c.offset))return false;++c.phase;break;
 case 31:if(!word(c.key+KeyOffsets[c.channel]+4,c.count))return false;++c.phase;break;
 case 32:if(!word(t+StreamOffsets[c.channel],c.stream))return false;++c.phase;break;
 case 33:if(!word(r+CacheOffsets[c.channel],c.destination))return false;
  c.source=c.stream+c.offset*Widths[c.channel];c.word=c.source+c.count*Widths[c.channel];
  if(!(c.source<c.word)){c.phase=80;break;}
  c.entryIndex=0;c.count=uint32_t(c.word+Widths[c.channel]-1-c.source)/Widths[c.channel];
  c.bulkLimit=c.count&~uint32_t(c.channel==2 || c.channel==3?7:3);c.remaining=c.count?uint64_t(c.count):(uint64_t(1)<<32);c.phase=40;break;
 case 40:if(!byte(c.source,c.deltaIndex))return false;
  c.destination+=c.deltaIndex*DestinationWidths[c.channel];c.lane=0;c.phase=c.channel<2?41:c.channel==3?50:60;break;
 // Vector: all three original GQR4 loads precede the first destination read.
 case 41:if(!quantized(c.source+1+c.lane,c.deltas[c.lane]))return false;if(++c.lane<3)break;c.lane=0;++c.phase;break;
 case 42:if(!load(c.destination+4*c.lane,c.oldValue))return false;c.newValue=std::fma(c.scale,c.deltas[c.lane],c.oldValue);++c.phase;break;
 case 43:if(!store(c.destination+4*c.lane,bits(c.newValue)))return false;if(++c.lane<3)c.phase=42;else c.phase=79;break;
 // Node channel.
 case 50:if(!quantized(c.source+1,c.delta))return false;++c.phase;break;
 case 51:if(!load(c.destination,c.oldValue))return false;c.newValue=std::fma(c.scale,c.delta,c.oldValue);++c.phase;break;
 case 52:if(!store(c.destination,bits(c.newValue)))return false;c.phase=79;break;
 // Byte and texture: unsigned source increment, wrapping byte publication.
 case 60:if(!byte(c.source+1,c.increment))return false;++c.phase;break;
 case 61:if(!byte(c.destination,c.word))return false;c.word+=c.increment;++c.phase;break;
 case 62:if(!s.storeByte || !s.storeByte(s.user,c.destination,uint8_t(c.word)))return false;c.phase=c.channel==2?79:63;break;
 case 63:if(!load(c.destination+4,c.oldValue))return false;++c.phase;break;
 case 64:if(!load(c.source+4,c.delta))return false;c.newValue=single(c.oldValue+c.delta);++c.phase;break;
 case 65:if(!store(c.destination+4,bits(c.newValue)))return false;
  c.phase=c.entryIndex<c.bulkLimit && (c.entryIndex&3)!=3?66:68;break;
 case 66:if(!load(c.destination+8,c.oldValue))return false;++c.phase;break;
 case 67:if(!load(c.source+8,c.delta))return false;c.phase=70;break;
 case 68:if(!load(c.source+8,c.delta))return false;++c.phase;break;
 case 69:if(!load(c.destination+8,c.oldValue))return false;++c.phase;break;
 case 70:c.newValue=single(c.oldValue+c.delta);++c.phase;break;
 case 71:if(!store(c.destination+8,bits(c.newValue)))return false;c.phase=79;break;
 case 79:c.source+=Widths[c.channel];++c.entryIndex;if(--c.remaining){c.phase=40;return false;}c.phase=80;break;
 case 80:if(++c.channel<5){c.phase=30;break;}c.key+=44;c.phase=22;if(c.key<c.end)return false;break;
 case 90:if(!load(r+0x40,c.oldValue))return false;if(!(c.oldValue>c.result.fraction))return finish();++c.phase;break;
 case 91:if(!load(0x8041fb28,c.newValue))return false;++c.phase;break;
 case 92:if(!store(r+0x40,bits(c.newValue)))return false;return finish();
 default:return false;
 }
}
bool advanceAnimationMainCache(const AnimationMainEvaluation& input,const AnimationMainTimeServices& time,const AnimationMainReplayServices& replay,AnimationMainCacheCursor& c,AnimationMainReplayResult& out){
 if(!c.active){c={};c.active=true;c.input=input;c.timeOwner=time;c.replayOwner=replay;}
 else if(!same(c.input,input) || !same(c.replayOwner,replay) || c.timeOwner.user!=time.user || c.timeOwner.word!=time.word || c.timeOwner.store!=time.store || c.timeOwner.doubleWord!=time.doubleWord || c.timeOwner.divide!=time.divide || c.timeOwner.convertUnsigned!=time.convertUnsigned || c.timeOwner.truncate!=time.truncate)return false;
 if(c.phase==0){if(!advanceAnimationMainScalar(input,time,c.scalar))return false;c.phase=1;}
 if(!advanceAnimationMainReplay(input,replay,c.replay,out))return false;
 c={};return true;
}

}
