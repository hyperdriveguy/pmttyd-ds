#include "animation_inner.hpp"
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
int32_t signed32(uint32_t w){return w<=0x7fffffff?int32_t(w):INT32_MIN+int32_t(w-0x80000000u);}
bool same(const AnimationInnerDrawRequest& a,const AnimationInnerDrawRequest& b){return a.pose==b.pose && a.pass==b.pass && !std::memcmp(a.matrix.data(),b.matrix.data(),48) && !std::memcmp(&a.angle,&b.angle,4) && !std::memcmp(&a.scale,&b.scale,4);}
bool same(const AnimationInnerServices& a,const AnimationInnerServices& b){return a.user==b.user && a.word==b.word && a.half==b.half && a.byte==b.byte && a.store==b.store && a.gx==b.gx && a.callback==b.callback && a.main==b.main && a.copy==b.copy && a.flush==b.flush && a.invalidate==b.invalidate && a.traverse==b.traverse && a.facing.user==b.facing.user && a.facing.word==b.facing.word && a.facing.store==b.facing.store && a.facing.call==b.facing.call && a.facing.matrix==b.facing.matrix;}
}
bool resumeAnimationInner(const AnimationInnerDrawRequest& input,const AnimationInnerServices& s,AnimationInnerCursor& c,unsigned visits){
 if(!visits || !s.word || !s.store)return false;
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;c.matrix=input.matrix;}
 if(!same(input,c.input) || !same(s,c.owner))return false;
 auto read=[&](uint32_t a,uint32_t& out){auto w=s.word(s.user,a);if(!w)return false;out=*w;return true;};
 auto store=[&](uint32_t a,uint32_t w){return s.store(s.user,a,w);};
 auto complete=[&]{c.active=false;return true;};
 const uint32_t p=c.input.pose;
 for(unsigned visit=0;visit<visits;++visit)switch(c.phase){
 // Original prefix interleaves sample loads with file/descriptor resolution.
 case 0:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 1:if(!read(p+0x10,c.index))return false;++c.phase;break;
 case 2:if(!read(c.work,c.models))return false;++c.phase;break;
 case 3:if(!read(p+0x14,c.animation))return false;++c.phase;break;
 case 4:if(!read(c.models+(c.index<<4)+8,c.entry))return false;++c.phase;break;
 case 5:if(!read(p+0x28,c.current))return false;++c.phase;break;
 case 6:if(!read(c.entry+0xa0,c.holder))return false;++c.phase;break;
 case 7:if(!read(p+0x2c,c.next))return false;++c.phase;break;
 case 8:if(!read(c.holder,c.model))return false;++c.phase;break;
 case 9:if(!read(p+0x30,c.fraction))return false;++c.phase;break;
 case 10:if(!read(c.model+0x1ac,c.directory))return false;++c.phase;break;
 case 11:if(!read(c.directory+(c.animation<<6)+0x3c,c.descriptor))return false;if(!c.descriptor)return complete();++c.phase;break;
 case 12:if(!read(c.work+0x10c,c.mode))return false;++c.phase;break;
 case 13:if(!read(p+0xc,c.value))return false;if(c.mode?c.value==0:c.value==1)return complete();c.phase=c.input.pass==2?14:15;break;
 case 14:if(!read(c.model+0xc4,c.flags))return false;c.phase=c.flags&1?17:15;break;
 case 15:if(!read(c.model+0xc4,c.flags))return false;++c.phase;break;
 case 16:{const auto shift=c.input.pass&63;if(!(c.flags&(shift<32?(1u<<shift):0)))return complete();++c.phase;break;}
 case 17:if(!read(p+0xf4,c.secondary))return false;++c.phase;break;
 case 18:if(!read(p+0xf0,c.primary))return false;++c.phase;break;
 case 19:if(!read(p+0xe8,c.flags))return false;c.phase=20;c.mapPhase=0;c.effective=c.input.pass;break;
 case 20:
  // Complete mapTestXLU; local alphas are captured BE low bytes. Each map
  // query independently loads INDEX then BASE, unsigned half flags then byte.
  switch(c.mapPhase){
  case 0:if(((c.flags&0x40) && (c.primary&255)!=255) || (c.secondary&255)!=255){c.effective=3;c.phase=21;break;}c.mapPhase=c.flags&0x800?5:1;break;
  case 1:if(!read(0x8041e664,c.mapIndex))return false;++c.mapPhase;break;
  case 2:if(!read(0x8041e660,c.mapBase))return false;c.mapGroup=c.mapBase+c.mapIndex*0x2f4u;++c.mapPhase;break;
  case 3:{auto h=s.half?s.half(s.user,c.mapGroup+4):std::nullopt;if(!h)return false;c.value=*h;c.mapPhase=c.value&2?4:5;break;}
  case 4:{auto b=s.byte?s.byte(s.user,c.mapGroup+0x19):std::nullopt;if(!b)return false;if(*b!=255){c.effective=3;c.phase=21;}else c.mapPhase=5;break;}
  case 5:if(c.flags&0x1000)c.phase=21;else c.mapPhase=6;break;
  case 6:if(!read(0x8041e664,c.mapIndex))return false;++c.mapPhase;break;
  case 7:if(!read(0x8041e660,c.mapBase))return false;c.mapGroup=c.mapBase+c.mapIndex*0x2f4u;++c.mapPhase;break;
  case 8:{auto h=s.half?s.half(s.user,c.mapGroup+4):std::nullopt;if(!h)return false;c.value=*h;if(c.value&4)c.mapPhase=9;else c.phase=21;break;}
  case 9:{auto b=s.byte?s.byte(s.user,c.mapGroup+0x1d):std::nullopt;if(!b)return false;if(*b!=255)c.effective=3;c.phase=21;break;}
  default:return false;
  }break;
 case 21:if(!read(p+0xe0,c.callback))return false;c.phase=c.callback?22:23;break;
 case 22:if(!s.callback || !s.callback(s.user,c.callback,c.effective))return false;c.phase=27;break;
 case 23:
  if(c.effective>3){c.phase=25;break;}
  if(!c.pending){c.gx={};const bool xlu=c.effective==3;switch(c.gxStep){
   case 0:c.gx.function=0x802b93d8;c.gx.arguments={1,3,xlu?0:1};break;
   case 1:c.gx.function=0x802b932c;c.gx.arguments={xlu?1:0,4,5,0};break;
   case 2:c.gx.function=0x802b940c;c.gx.arguments={c.effective==1 || xlu?1:0};break;
   case 3:c.gx.function=0x802b8d60;c.gx.arguments=c.effective==0 || c.effective==2?std::array<int32_t,7>{6,128,1,0,0}:std::array<int32_t,7>{7,0,0,7,0};break;
  }c.pending=true;}
  if(!s.gx || !s.gx(s.user,c.gx))return false;
  c.pending=false;if(++c.gxStep==4)c.phase=25;break;
 case 25:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 26:if(!store(c.work+0xe0,c.effective))return false;++c.phase;break;
 case 27:if(!advanceAnimationInnerFacing(p,c.input.angle,c.input.scale,c.input.matrix,s.facing,c.facing,c.matrix))return false;++c.phase;break;
 case 28:if(!read(p+0x3c,c.value))return false;c.phase=c.value==0xffffffff?29:70;break;
 case 29:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 30:if(!read(p+4,c.flags))return false;c.ring=c.flags&2;++c.phase;break;
 case 31:if(!read(c.work+0x10,c.pool))return false;c.handle=uint32_t((uint64_t(0xb21642c9u)*uint32_t(p-c.pool))>>32)>>8;c.phase=c.ring?32:60;break;
 // Inline ring. Capacity precedes the remaining MODEL resolution; signed
 // comparisons apply to wrapped32 sums. Oversize still calls Main +cleanup.
 case 32:if(!read(p+0x10,c.index))return false;++c.phase;break;
 case 33:if(!read(c.work,c.models))return false;++c.phase;break;
 case 34:if(!read(c.work+0xf0,c.capacity))return false;++c.phase;break;
 case 35:if(!read(c.models+(c.index<<4)+8,c.entry))return false;++c.phase;break;
 case 36:if(!read(c.entry+0xa0,c.holder))return false;++c.phase;break;
 case 37:if(!read(c.holder,c.value))return false;++c.phase;break;
 case 38:if(!read(c.value+0xf0,c.positions))return false;++c.phase;break;
 case 39:if(!read(c.value+0xf8,c.auxiliary))return false;c.total=c.positions+c.auxiliary;c.phase=signed32(c.total)>signed32(c.capacity)?60:40;break;
 case 40:if(!read(c.work+0xf4,c.cursor))return false;c.phase=signed32(c.cursor+c.total)>signed32(c.capacity)?41:42;break;
 case 41:if(!store(c.work+0xf4,0))return false;++c.phase;break;
 case 42:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 43:if(!store(c.work+0xe8,1))return false;++c.phase;break;
 case 44:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 45:if(!read(c.work+0xf4,c.cursor))return false;++c.phase;break;
 case 46:if(!read(c.work+0xec,c.base))return false;++c.phase;break;
 case 47:if(!store(c.work+0xf8,c.base+c.cursor*12))return false;++c.phase;break;
 case 48:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 49:if(!read(c.work+0xf4,c.cursor))return false;++c.phase;break;
 case 50:if(!store(c.work+0xf4,c.cursor+c.positions))return false;++c.phase;break;
 case 51:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 52:if(!read(c.work+0xf4,c.cursor))return false;++c.phase;break;
 case 53:if(!read(c.work+0xec,c.base))return false;++c.phase;break;
 case 54:if(!store(c.work+0xfc,c.base+c.cursor*12))return false;++c.phase;break;
 case 55:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 56:if(!read(c.work+0xf4,c.cursor))return false;++c.phase;break;
 case 57:if(!store(c.work+0xf4,c.cursor+c.auxiliary))return false;c.phase=60;break;
 case 60:if(!s.main || !s.main(s.user,c.handle))return false;c.phase=c.ring?61:70;break;
 case 61:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 62:if(!store(c.work+0xe8,0))return false;c.phase=70;break;
 case 70:if(!store(0x8041e89c,0))return false;++c.phase;break;
 case 71:if(!store(0x8041e8a0,0x803d8e70))return false;++c.phase;break;
 case 72:if(!s.copy || !s.copy(s.user,c.matrix,0x803d8e70))return false;++c.phase;break;
 case 73:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 74:if(!store(c.work+0x18,p))return false;++c.phase;break;
 case 75:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 76:if(!store(c.work+0x1c,c.descriptor))return false;++c.phase;break;
 case 77:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 78:if(!store(c.work+0x20,c.model))return false;++c.phase;break;
 case 79:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 80:if(!read(c.work+0xe8,c.selector))return false;++c.phase;break;
 // Each flush captures its own resource/pointer/count, with the selector
 // retained across both calls. The pointer precedes file/holder/MODEL loads.
 case 81:case 90:if(!read(p+0x10,c.index))return false;++c.phase;break;
 case 82:case 91:if(!read(c.work,c.models))return false;++c.phase;break;
 case 83:if(!read(c.selector?c.work+0xf8:p+0x4c,c.output))return false;++c.phase;break;
 case 84:case 93:if(!read(c.models+(c.index<<4)+8,c.entry))return false;++c.phase;break;
 case 85:case 94:if(!read(c.entry+0xa0,c.holder))return false;++c.phase;break;
 case 86:case 95:if(!read(c.holder,c.value))return false;++c.phase;break;
 case 87:if(!read(c.value+0xf0,c.bytes))return false;c.bytes*=12;++c.phase;break;
 case 88:if(!s.flush || !s.flush(s.user,c.output,c.bytes))return false;++c.phase;break;
 case 89:if(!read(0x80415dc0,c.work))return false;++c.phase;break;
 case 92:if(!read(c.selector?c.work+0xfc:p+0x54,c.output))return false;++c.phase;break;
 case 96:if(!read(c.value+0xf8,c.bytes))return false;c.bytes*=12;++c.phase;break;
 case 97:if(!s.flush || !s.flush(s.user,c.output,c.bytes))return false;++c.phase;break;
 case 98:if(!s.invalidate || !s.invalidate(s.user))return false;++c.phase;break;
 case 99:if(!read(c.model+0x144,c.value))return false;c.traversal={0xffffffff,c.value-1,c.current,c.next,real(c.fraction)};++c.phase;break;
 case 100:if(!s.traverse || !s.traverse(s.user,c.traversal))return false;return complete();
 default:return false;
 }
 return false;
}
}
