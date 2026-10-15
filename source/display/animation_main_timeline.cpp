#include "animation_main_timeline.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
float single(float f){volatile float x=f;return x;}
bool less(uint32_t a,uint32_t b){return (a^0x80000000u)<(b^0x80000000u);}
bool positive(uint32_t a){return a && !(a&0x80000000u);}
bool same(const AnimationMainEvaluation& a,const AnimationMainEvaluation& b){return a.work==b.work && a.pool==b.pool && a.pose==b.pose && a.model==b.model && a.timeline==b.timeline && a.positions==b.positions && a.normals==b.normals;}
bool same(const AnimationMainTimelineServices& a,const AnimationMainTimelineServices& b){return a.user==b.user && a.word==b.word && a.store==b.store;}
}
bool advanceAnimationMainTimeline(const AnimationMainEvaluation& input,uint32_t loop,const AnimationMainTimelineServices& s,AnimationMainTimelineCursor& c){
 if(!c.active){c={};c.active=true;c.input=input;c.loop=loop;c.owner=s;}else if(!same(c.input,input) || c.loop!=loop || !same(c.owner,s))return false;
 const uint32_t r=input.pose,t=input.timeline;
 auto word=[&](uint32_t a,uint32_t& v){if(!s.word)return false;auto w=s.word(s.user,a);if(!w)return false;v=*w;return true;};
 auto load=[&](uint32_t a,float& f){uint32_t w;if(!word(a,w))return false;f=real(w);return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 for(;;)switch(c.phase){
 case 0:if(!word(t+8,c.count))return false;++c.phase;break;
 case 1:if(!load(r+0x24,c.selected))return false;++c.phase;break;
 case 2:if(!word(t+0x28,c.pointer))return false;c.index=0;c.phase=positive(c.count)?3:4;break;
 case 3:case 10:{float time;if(!load(c.pointer,time))return false;if(time>c.selected){++c.phase;break;}c.pointer+=c.phase==3?44u:8u;++c.index;if(c.index==c.count){++c.phase;break;}return false;}
 case 4:if(!store(r+0x28,c.index-1))return false;++c.phase;break;
 case 5:if(!word(r+0x28,c.previous))return false;++c.phase;break;
 case 6:if(!store(r+0x2c,c.previous+1))return false;++c.phase;break;
 case 7:if(!word(t+0x20,c.count))return false;++c.phase;break;
 case 8:if(!load(r+0x24,c.selected))return false;++c.phase;break;
 case 9:if(!word(t+0x40,c.pointer))return false;c.index=0;c.phase=positive(c.count)?10:11;break;
 case 11:if(!store(r+0x34,c.index-1))return false;++c.phase;break;
 case 12:if(!word(r+0x34,c.previous))return false;++c.phase;break;
 case 13:if(!store(r+0x38,c.previous+1))return false;c.phase=loop?22:14;break;
 case 14:if(!word(r+0x2c,c.next))return false;++c.phase;break;
 case 15:if(!word(t+8,c.count))return false;c.phase=less(c.next,c.count)?18:16;break;
 case 16:if(!word(r+0x28,c.previous))return false;++c.phase;break;
 case 17:if(!store(r+0x2c,c.previous))return false;++c.phase;break;
 case 18:if(!word(r+0x38,c.next))return false;++c.phase;break;
 case 19:if(!word(t+0x20,c.count))return false;c.phase=less(c.next,c.count)?22:20;break;
 case 20:if(!word(r+0x34,c.previous))return false;++c.phase;break;
 case 21:if(!store(r+0x38,c.previous))return false;++c.phase;break;
 case 22:if(!word(r+0x34,c.index))return false;++c.phase;break;
 case 23:if(!word(t+0x40,c.keys))return false;++c.phase;break;
 case 24:if(!word(c.keys+(c.index<<3)+4,c.state))return false;++c.phase;break;
 case 25:if(!store(r+0x80,c.state))return false;++c.phase;break;
 case 26:if(!word(r+0x28,c.current))return false;++c.phase;break;
 case 27:if(!word(t+0x28,c.keys))return false;++c.phase;break;
 case 28:if(!load(r+0x24,c.selected))return false;++c.phase;break;
 case 29:if(!load(c.keys+c.current*44u,c.keyTime))return false;c.numerator=single(c.selected-c.keyTime);c.phase=loop?30:39;break;
 case 30:if(!word(r+0x2c,c.next))return false;++c.phase;break;
 case 31:if(!word(t+8,c.count))return false;c.phase=less(c.next,c.count)?40:32;break;
 case 32:if(!store(r+0x2c,0))return false;++c.phase;break;
 case 33:if(!word(r+0x28,c.current))return false;++c.phase;break;
 case 34:if(!word(t+0x24,c.control))return false;++c.phase;break;
 case 35:if(!word(t+0x28,c.keys))return false;++c.phase;break;
 case 36:if(!load(c.control+8,c.end))return false;++c.phase;break;
 case 37:if(!load(c.keys+c.current*44u,c.keyTime))return false;c.denominator=single(c.end-c.keyTime);c.phase=41;break;
 case 39:if(!word(r+0x2c,c.next))return false;++c.phase;break;
 case 40:{float next;if(!load(c.keys+c.next*44u,next))return false;c.denominator=single(next-c.keyTime);++c.phase;break;}
 case 41:if(!load(0x8041fb28,c.zero))return false;c.fraction=c.denominator==c.zero?c.zero:single(c.numerator/c.denominator);++c.phase;break;
 case 42:if(!store(r+0x30,bits(c.fraction)))return false;c={};return true;
 default:return false;
 }
}
bool advanceAnimationMainScalar(const AnimationMainEvaluation& input,const AnimationMainTimeServices& s,AnimationMainScalarCursor& c){
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;}
 else if(!same(c.input,input) || c.owner.user!=s.user || c.owner.word!=s.word || c.owner.store!=s.store || c.owner.doubleWord!=s.doubleWord || c.owner.divide!=s.divide || c.owner.convertUnsigned!=s.convertUnsigned || c.owner.truncate!=s.truncate)return false;
 if(c.phase==0){if(!advanceAnimationMainTime(input,s,c.time,c.timeResult))return false;c.phase=1;}
 const AnimationMainTimelineServices memory{s.user,s.word,s.store};
 if(!advanceAnimationMainTimeline(input,c.timeResult.loop,memory,c.timeline))return false;
 c={};return true;
}

}
