#include "source_mario_move_position.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <sstream>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t E=0x4000,A=0x5000,M=0x6000,N=0x7000,MP=0x8041e900,GP=0x81000000;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
float scalar(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
struct World {
 EventEntry entry;std::map<uint32_t,uint32_t>words;std::map<uint32_t,uint16_t>halves;
 std::map<uint32_t,float>values;std::vector<std::string>trace;unsigned accepted=0,held=0,mpReads=0;
 int holdAt=-1;bool mutation=false,other=false,live=true,reenter=false,loseAtAngle=false,partialMotion=false;
 unsigned motionPhase=0,prefix=0;SourceMarioMovePosition*cursor=nullptr;
 std::shared_ptr<const void>lifetime=std::make_shared<int>(1);
 World(){entry.id=17;entry.currentCommand=0x905065cc;words[E+0x18]=A;words[MP]=M;words[0x80415fd0]=GP;words[GP+4]=60;words[0x80421b0c]=0x3ecccccd;words[0x80421ad8]=0x3f800000;
  words[A]=0xfe363c80;words[A+4]=0xfe363c82;words[A+8]=0xf24c5a80;values[words[A]]=3;values[words[A+4]]=4;values[words[A+8]]=150;
  for(auto m:{M,N}){words[m]=0;words[m+0x8c]=bits(0);words[m+0x94]=bits(0);words[m+0x1a4]=bits(90);words[m+0x188]=bits(2.25f);halves[m+0x2e]=0;}
 }
 bool gate(){if(int(accepted)==holdAt&&held<2){++held;return false;}++accepted;if(reenter&&cursor){assert(!cursor->resume(entry,E,false,services()));assert(!cursor->reset());}return true;}
 void record(const char*kind,std::initializer_list<uint64_t>v){std::ostringstream s;s<<kind;for(auto n:v)s<<':'<<std::hex<<n;trace.push_back(s.str());}
 std::optional<uint32_t>word(uint32_t a){if(!gate())return {};if(a==MP&&++mpReads==2&&other)words[MP]=N;assert(words.count(a));const auto v=words.at(a);record("W",{a,v});if(mutation&&a==0x80421ad8)words[M+0x1a4]=bits(123);return v;}
 std::optional<uint16_t>half(uint32_t a){if(!gate())return {};assert(halves.count(a));const auto v=halves.at(a);record("H",{a,v});return v;}
 std::optional<uint64_t>doubleWord(uint32_t a){if(!gate())return {};assert(a==0x802e3eb8);record("D",{a,0x4330000080000000ULL});return 0x4330000080000000ULL;}
 bool store(uint32_t a,uint32_t v){if(!gate())return false;record("S",{a,v});words[a]=v;if(loseAtAngle&&a==M+0x1a4)live=false;return true;}
 bool storeHalf(uint32_t a,uint16_t v){if(!gate())return false;record("T",{a,v});halves[a]=v;return true;}
 std::optional<float>eventFloat(uint32_t raw){if(!gate())return {};assert(values.count(raw));const auto v=values.at(raw);record("F",{raw,bits(v)});return v;}
 std::optional<float>number(const SourceMarioMoveNumber&q){if(!gate())return {};float v=0;switch(q.kind){
 case SourceMarioMoveNumberKind::Distance:v=std::hypot(q.values[2]-q.values[0],q.values[3]-q.values[1]);break;
 case SourceMarioMoveNumberKind::Angle:v=std::atan2(q.values[2]-q.values[0],q.values[1]-q.values[3])*57.29577951308232f;break;
 case SourceMarioMoveNumberKind::Multiply:v=q.values[0]*q.values[1];break;
 case SourceMarioMoveNumberKind::Divide:v=q.values[0]/q.values[1];break;
 case SourceMarioMoveNumberKind::Add:v=q.values[0]+q.values[1];break;
 case SourceMarioMoveNumberKind::SignedWordMinusDouble:{const uint64_t encoded=0x4330000000000000ULL|uint32_t(q.word^0x80000000u);double a,b;std::memcpy(&a,&encoded,8);std::memcpy(&b,&q.doubleBits,8);v=float(a-b);break;}
 }record("N",{uint64_t(q.kind),q.function,q.callsite,bits(q.values[0]),bits(q.values[1]),bits(q.values[2]),bits(q.values[3]),q.word,q.doubleBits,bits(v)});if(mutation&&q.kind==SourceMarioMoveNumberKind::Distance)words[M+0x8c]=bits(1.5f);return v;}
 std::optional<int32_t>truncate(float v){if(!gate())return {};assert(std::isfinite(v)&&double(v)>=-2147483648.&&double(v)<2147483648.);record("I",{bits(v),uint32_t(int32_t(v))});return int32_t(v);}
 bool motion(int32_t action){if(partialMotion&&motionPhase<2){if(!motionPhase)++prefix;++motionPhase;return false;}if(!gate())return false;record("C",{uint32_t(action)});if(mutation){const auto m=words[MP];words[m+0x1a0]=bits(999);words[m+0x1a4]=bits(888);halves[m+0x4e]=777;words[m+0x180]=bits(666);}return true;}
 SourceMarioMovePositionServices services(){SourceMarioMovePositionServices s;s.lifetime=lifetime;s.user=this;s.current=[](void*p){return static_cast<World*>(p)->live;};s.word=[](void*p,uint32_t a){return static_cast<World*>(p)->word(a);};s.half=[](void*p,uint32_t a){return static_cast<World*>(p)->half(a);};s.doubleWord=[](void*p,uint32_t a){return static_cast<World*>(p)->doubleWord(a);};s.storeWord=[](void*p,uint32_t a,uint32_t v){return static_cast<World*>(p)->store(a,v);};s.storeHalf=[](void*p,uint32_t a,uint16_t v){return static_cast<World*>(p)->storeHalf(a,v);};s.eventFloat=[](void*p,EventEntry&,uint32_t raw){return static_cast<World*>(p)->eventFloat(raw);};s.number=[](void*p,const SourceMarioMoveNumber&q){return static_cast<World*>(p)->number(q);};s.truncate=[](void*p,float v){return static_cast<World*>(p)->truncate(v);};s.motion=[](void*p,int32_t a){return static_cast<World*>(p)->motion(a);};return s;}
 float fp(uint32_t a){return scalar(*word(a));}
 void sf(uint32_t a,float v){assert(store(a,bits(v)));}
 float num(SourceMarioMoveNumberKind kind,uint32_t pc,std::array<float,4>v,uint32_t hz=0,uint64_t bias=0,uint32_t fn=0){SourceMarioMoveNumber q;q.kind=kind;q.callsite=pc;q.values=v;q.word=hz;q.doubleBits=bias;q.function=fn;return *number(q);}
};
// Immediate successful original bodies, independent control flow. Numerical
// providers are explicit native TEST arithmetic, not Gekko numerical parity.
int32_t reference(World&w,bool first){const auto args=*w.word(E+0x18),m=*w.word(MP);const float x=*w.eventFloat(*w.word(args)),z=*w.eventFloat(*w.word(args+4));float rate=*w.eventFloat(*w.word(args+8));
 if(first){const float sx=w.fp(m+0x8c),sz=w.fp(m+0x94);const float distance=w.num(SourceMarioMoveNumberKind::Distance,0x800e5fe8,{sx,sz,x,z},0,0,0x8000722c);
  const float ax=w.fp(m+0x8c),az=w.fp(m+0x94);const float angle=w.num(SourceMarioMoveNumberKind::Angle,0x800e6000,{ax,az,x,z},0,0,0x8000703c);w.sf(m+0x1a4,angle);
  if(*w.word(m)&0x02000000){const auto scale=w.fp(0x80421b0c);rate=w.num(SourceMarioMoveNumberKind::Multiply,0x800e6018,{rate,scale,0,0});}
  const auto gp=*w.word(0x80415fd0),hz=*w.word(gp+4);const auto bias=*w.doubleWord(0x802e3eb8);const auto one=w.fp(0x80421ad8),direction=w.fp(m+0x1a4);
  const auto frequency=w.num(SourceMarioMoveNumberKind::SignedWordMinusDouble,0x800e6048,{},hz,bias);rate=w.num(SourceMarioMoveNumberKind::Divide,0x800e604c,{rate,frequency,0,0},hz,bias);
  const auto a=w.num(SourceMarioMoveNumberKind::Divide,0x800e6050,{one,rate,0,0},hz,bias),b=w.num(SourceMarioMoveNumberKind::Divide,0x800e6054,{distance,rate,0,0},hz,bias);
  const auto ticks=*w.truncate(w.num(SourceMarioMoveNumberKind::Add,0x800e6058,{a,b,0,0},hz,bias));const auto force=*w.word(MP),flags=*w.word(force);assert(w.store(force,flags|0x20));w.sf(force+0x1a0,direction);w.sf(force+0x1a4,direction);assert(w.storeHalf(force+0x4e,uint16_t(ticks)));w.sf(force+0x180,rate);
  const auto motion=*w.half(force+0x2e);if(motion!=25&&motion!=21&&motion!=22&&motion!=26){const auto dash=w.fp(force+0x188);assert(w.motion(rate>=dash?2:1));w.sf(force+0x1a0,direction);w.sf(force+0x1a4,direction);assert(w.storeHalf(force+0x4e,uint16_t(ticks)));w.sf(force+0x180,rate);}
 }
 if(*w.word(m)&0x20)return 0;w.sf(m+0x8c,x);w.sf(m+0x94,z);return 2;
}
std::optional<int32_t>finish(World&w,SourceMarioMovePosition&c,bool first){w.cursor=&c;for(unsigned i=0;i<500;++i){const auto v=c.resume(w.entry,E,i?false:first,w.services());if(v)return v;}return {};}
void differential(){for(bool first:{false,true})for(bool scale:{false,true})for(uint16_t motion:{uint16_t(0),uint16_t(21),uint16_t(22),uint16_t(25),uint16_t(26)}){
 World expected;expected.words[M]=scale?0x02000000:0;expected.halves[M+0x2e]=motion;expected.mutation=true;const auto result=reference(expected,first);
 for(int wait=-1;wait<int(expected.accepted);++wait){World actual;actual.words[M]=scale?0x02000000:0;actual.halves[M+0x2e]=motion;actual.mutation=true;actual.holdAt=wait;actual.reenter=true;SourceMarioMovePosition c;const auto v=finish(actual,c,first);assert(v&&*v==result&&c.complete());assert(actual.trace==expected.trace&&actual.words==expected.words&&actual.halves==expected.halves);if(wait>=0)assert(actual.held==2);}
 }}
void independentActorsAndPoll(){World expected;expected.other=true;const auto result=reference(expected,true);World actual;actual.other=true;SourceMarioMovePosition c;assert(finish(actual,c,true)==result&&result==2);assert(actual.trace==expected.trace&&actual.words[N]&0x20);World poll;SourceMarioMovePosition p;assert(finish(poll,p,true)==0);assert(p.reset());poll.words[M]&=~0x20u;poll.values[0xfe363c80]=9;poll.values[0xfe363c82]=12;auto s=poll.services();s.number=nullptr;s.motion=nullptr;s.half=nullptr;s.doubleWord=nullptr;s.truncate=nullptr;assert(p.resume(poll.entry,E,false,s)==2);assert(scalar(poll.words[M+0x8c])==9&&scalar(poll.words[M+0x94])==12);}
void holdsAndIdentity(){World w;SourceMarioMovePosition c;auto s=w.services();s.number=nullptr;assert(!c.resume(w.entry,E,true,s)&&c.pending()==0x8000722c);assert(w.trace.size()==10&&!c.reset());auto replaced=s;replaced.user=nullptr;const auto count=w.accepted;assert(!c.resume(w.entry,E,true,replaced)&&w.accepted==count);auto aliased=s;aliased.lifetime=std::shared_ptr<const void>(s.lifetime.get(),[](const void*){});assert(!c.resume(w.entry,E,true,aliased)&&w.accepted==count);World lost;lost.loseAtAngle=true;SourceMarioMovePosition p;assert(!finish(lost,p,true));const auto prefix=lost.trace.size();lost.live=true;assert(!p.resume(lost.entry,E,false,lost.services())&&lost.trace.size()==prefix&&!p.reset());World child;child.partialMotion=true;child.mutation=true;SourceMarioMovePosition partial;assert(finish(child,partial,true)==0&&child.prefix==1&&child.motionPhase==2&&scalar(child.words[M+0x1a4])==123);}
}
int main(){differential();independentActorsAndPoll();holdsAndIdentity();std::puts("Whole original mov_pos2/force-start trace, every external lane twice, fresh actor/field reads, held partial motion and genuine poll semantics pass");return 0;}
