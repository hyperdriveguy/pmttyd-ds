#include "source_font_state.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
bool supported(uint32_t f){switch(f){
case 0x8007773c:case 0x80077758:case 0x8007776c:case 0x80077774:
case 0x800777e0:case 0x80077844:case 0x800778b4:case 0x800778e0:
case 0x800779e4:case 0x80077a08:case 0x80077aa8:case 0x80077ab4:
case 0x80077ac0:case 0x80077b4c:return true;default:return false;}}
bool same(const SourceFontStateServices&a,const SourceFontStateServices&b){
const auto&m=a.actors.memory;const auto&n=b.actors.memory;
return a.user==b.user&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.current==b.current&&a.request==b.request&&a.matrix==b.matrix&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event&&m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall;
}
// Shared color is raw RGBA; only the local effective alpha multiplies by
// global alpha/255. Integer product/division is exact for byte operands.
uint32_t effective(uint32_t raw,uint8_t alpha){return (raw&0xffffff00)|((raw&255)*alpha/255);}
}
// Fourteen source helpers share real globals. Native phases/queued requests
// retain complete child operands; they are not per-label style snapshots.
// Observed owner loss is sticky; a held void child is not completion0.
struct SourceFontStateCursor::State {
 SourceFontStateInput input;SourceFontStateServices owner;
 uint32_t phase=0,pending=0,result=0;bool invalid=false,queued=false,matrixQueued=false;
 SourceFontStateCall call;BattleAudienceMatrixRequest matrix;
 bool valid(){if(invalid)return false;if(!owner.current||!owner.current(owner.user))invalid=true;return !invalid;}
 uint32_t word(uint32_t at){if(!valid())return 0;const auto v=owner.actors.memory.word(owner.actors.memory.user,at);valid();return v;}
 uint8_t byte(uint32_t at){if(!valid())return 0;const auto v=owner.actors.byte(owner.actors.user,at);valid();return v;}
 void store(uint32_t at,uint32_t n){if(valid())owner.actors.memory.storeWord(owner.actors.memory.user,at,n);valid();}
 void storeByte(uint32_t at,uint8_t n){if(valid())owner.actors.memory.storeByte(owner.actors.memory.user,at,n);valid();}
 template<class F>bool request(uint32_t f,uint32_t site,F fill){
  pending=f;if(!valid())return false;
  if(!queued){call={};call.function=f;call.callsite=site;fill(call);queued=true;}
  if(!valid()||!owner.request)return false;
  const bool ok=owner.request(owner.user,call);if(!valid()||!ok)return false;
  queued=false;return true;
 }
 bool request(uint32_t f,uint32_t site,std::initializer_list<uint32_t> args){return request(f,site,[&](auto&q){q.integers=args;});}
};
SourceFontStateCursor::SourceFontStateCursor()=default;SourceFontStateCursor::~SourceFontStateCursor()=default;
bool SourceFontStateCursor::active()const{return bool(state_);}bool SourceFontStateCursor::stale()const{return state_&&state_->invalid;}
uint32_t SourceFontStateCursor::pending()const{return state_?state_->pending:0;}
bool SourceFontStateCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<uint32_t> resumeSourceFontState(const SourceFontStateInput&i,const SourceFontStateServices&s,SourceFontStateCursor&cursor){
 const auto&m=s.actors.memory;
 if(cursor.invoking_||!supported(i.function)||!s.lifetime||!s.current||!m.word||!s.actors.byte||!m.storeWord||!m.storeByte)return {};
 if(cursor.state_){const auto&c=*cursor.state_;if(c.input.function!=i.function||c.input.argument!=i.argument||c.input.scaleBits!=i.scaleBits||c.input.privateColor!=i.privateColor||!same(c.owner,s))return {};}
 cursor.invoking_=true;struct Guard{bool&b;~Guard(){b=false;}}guard{cursor.invoking_};
 if(!cursor.state_){if(!s.current(s.user))return {};cursor.state_=std::make_unique<SourceFontStateCursor::State>();cursor.state_->input=i;cursor.state_->owner=s;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return {};if(c.phase==0){
  switch(i.function){
  // ScaleVec reads ALL XYZ words before any global store; Scale uses raw
  // binary32 bits. Globals:e9b0/4/8 XYZ,e9ac raw RGBA,e9a8 alpha,e9a4 edge.
  case 0x8007773c:{const auto x=c.word(i.argument),y=c.word(i.argument+4),z=c.word(i.argument+8);c.store(0x8041e9b0,x);c.store(0x8041e9b4,y);c.store(0x8041e9b8,z);c.phase=100;break;}
  case 0x80077758:c.store(0x8041e9b0,i.scaleBits);c.store(0x8041e9b4,i.scaleBits);c.store(0x8041e9b8,i.scaleBits);c.phase=100;break;
  case 0x8007776c:c.result=c.word(0x8041e9ac);c.phase=100;break;
  case 0x80077774:{const auto raw=c.word(0x8041e9ac);const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,raw);c.result=effective(raw,alpha);c.phase=1;break;}
  // FontDrawColor stores raw shared color,then MUTATES caller alpha and
  // rereads caller word for effective TEV. Guest aliases can change shared
  // color too; repeated use scales again. PrivateColor is a borrowed local.
  case 0x800777e0:{
   const auto a=i.privateColor?uint8_t(*i.privateColor):c.byte(i.argument+3);
   const auto raw=i.privateColor?*i.privateColor:c.word(i.argument);const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,raw);
   const auto scaled=uint8_t(uint32_t(a)*alpha/255);
   if(i.privateColor){if(c.valid())*i.privateColor=(*i.privateColor&0xffffff00)|scaled;}
   else c.storeByte(i.argument+3,scaled);
   c.result=i.privateColor?*i.privateColor:c.word(i.argument);c.phase=1;break;
  }
  // Palette index<<2 wraps32 into416010; no invented three-color bound.
  // Edge setters below change only the CPU word,not GX readiness.
  case 0x80077844:{const auto raw=c.word(0x80416010+(i.argument<<2));const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,raw);c.result=effective(raw,alpha);c.phase=1;break;}
  case 0x80077aa8:case 0x80077ab4:c.store(0x8041e9a4,i.function==0x80077ab4);c.phase=100;break;
  // Alpha-start and default-start preserve DISTINCT XYZ read/store order.
  // Scale literals2b6c/70/74,palette0 and edge0 precede whole JUT DrawStart.
  // Alpha-start uses argument low8; default uses255,not prior alpha.
  case 0x80077ac0:{
   const auto raw=c.word(0x80416010),x=c.word(0x802c2b6c),y=c.word(0x802c2b70),z=c.word(0x802c2b74);
   c.storeByte(0x8041e9a8,uint8_t(i.argument));c.store(0x8041e9b4,y);c.store(0x8041e9b8,z);c.store(0x8041e9a4,0);c.store(0x8041e9ac,raw);c.store(0x8041e9b0,x);c.result=effective(raw,uint8_t(i.argument));c.phase=1;break;
  }
  case 0x80077b4c:{
   const auto raw=c.word(0x80416010),y=c.word(0x802c2b70),x=c.word(0x802c2b6c),z=c.word(0x802c2b74);
   c.store(0x8041e9b4,y);c.store(0x8041e9b8,z);c.store(0x8041e9a4,0);c.store(0x8041e9ac,raw);c.storeByte(0x8041e9a8,255);c.store(0x8041e9b0,x);c.result=effective(raw,255);c.phase=1;break;
  }
  default:c.phase=1;break;
  }continue;
 }
 if(c.phase==100){const auto result=i.function==0x8007776c?c.result:0;if(!c.valid())return {};cursor.state_.reset();return result;}
 if(i.function==0x80077774||i.function==0x800777e0||i.function==0x80077844||i.function==0x80077ac0||i.function==0x80077b4c){
  const bool start=i.function==0x80077ac0||i.function==0x80077b4c;
  const uint32_t site=i.function==0x80077774?0x800777cc:i.function==0x800777e0?0x80077830:i.function==0x80077844?0x800778a0:i.function==0x80077ac0?0x80077b38:0x80077bc8;
  if(!c.request(start?0x80074bfc:0x802b8ab8,site,[&](auto&q){if(!start)q.integers={1};q.color=c.result;}))return {};
  c.phase=100;continue;
 }
 // NoiseOff sets texgens1 THEN TEV stages1. RainbowOff changes stages only.
 // Neither restores saved bindings or defines a native noise/rainbow boolean.
 if(i.function==0x800778b4){
  if(c.phase==1){if(!c.request(0x802b5084,0x800778c4,{1}))return {};c.phase=2;continue;}
  if(!c.request(0x802b8fcc,0x800778cc,{1}))return {};
  c.phase=100;continue;
 }
 if(i.function==0x800779e4){if(!c.request(0x802b8fcc,0x800779f4,{1}))return {};c.phase=100;continue;}
 // Rainbow stage1:order(null texcoord255,null map255,color channel4),
 // ADD/zero-bias/1x-scale/clamp1/outputPREV0 ops,color inputs15/0/10/15 and
 // alpha7/0/5/7 are original GX enums,not host colors or byte alpha values.
 if(i.function==0x80077a08){
  bool ok=false;switch(c.phase){
  case 1:ok=c.request(0x802b8fcc,0x80077a18,{2});break;
  case 2:ok=c.request(0x802b8e30,0x80077a2c,{1,255,255,4});break;
  case 3:ok=c.request(0x802b89e8,0x80077a48,{1,0,0,0,1,0});break;
  case 4:ok=c.request(0x802b8a50,0x80077a64,{1,0,0,0,1,0});break;
  case 5:ok=c.request(0x802b8960,0x80077a7c,{1,15,0,10,15});break;
  case 6:ok=c.request(0x802b89a4,0x80077a94,{1,7,0,5,7});break;
  default:return {};
  }if(!ok)return {};c.phase=c.phase==6?100:c.phase+1;continue;
 }
 // Noise stage1 uses texcoord1,map2,null color255,KAlpha selector0;
 // color inputs15/15/15/0,alpha7/0/4/7. Two texgens/stages are explicit
 // ordered GX requests,not acknowledgment of loaded Window texture.
 if(i.function==0x800778e0){
  bool ok=false;switch(c.phase){
  case 1:ok=c.request(0x802b5084,0x800778f0,{2});break;
  case 2:ok=c.request(0x802b8fcc,0x800778f8,{2});break;
  case 3:ok=c.request(0x802b8c3c,0x80077904,{1,0});break;
  case 4:ok=c.request(0x802b8e30,0x80077918,{1,1,2,255});break;
  case 5:ok=c.request(0x802b89e8,0x80077934,{1,0,0,0,1,0});break;
  case 6:ok=c.request(0x802b8a50,0x80077950,{1,0,0,0,1,0});break;
  case 7:ok=c.request(0x802b8960,0x80077968,{1,15,15,15,0});break;
  case 8:ok=c.request(0x802b89a4,0x80077980,{1,7,0,4,7});break;
  // Fresh global Window object+640hex is original index50 at32-byte stride.
  // Scale is actual(1/64,1/64,1); capture once per matrix child. Then load
  // texture matrix33/type1 and texgen(1,1,5,33,0,125),with no readiness shortcut.
  case 9:ok=c.request(0x802b7880,0x80077990,[&](auto&q){q.integers={c.word(0x8041ea24)+0x640,2};});break;
  case 10:{
   c.pending=0x8029e2b0;if(!c.matrixQueued){c.matrix={};c.matrix.function=0x8029e2b0;const auto xy=c.word(0x80420438),z=c.word(0x804203e4);c.matrix.xyz={real(xy),real(xy),real(z)};c.matrixQueued=true;}
   if(!c.valid()||!c.owner.matrix)return {};
   ok=c.owner.matrix(c.owner.user,c.matrix)&&c.valid();break;
  }
  case 11:ok=c.request(0x802ba3d0,0x800779b4,[&](auto&q){q.integers={33,1};q.matrix=c.matrix.output;});break;
  case 12:ok=c.request(0x802b4e04,0x800779d0,{1,1,5,33,0,125});break;
  default:return {};
  }if(!ok||!c.valid())return {};c.phase=c.phase==12?100:c.phase+1;continue;
 }
 return {};
 }
}
}
