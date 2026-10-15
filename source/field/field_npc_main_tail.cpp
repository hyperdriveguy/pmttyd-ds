#include "field_npc_main_tail.hpp"
#include <cstring>
namespace ttyd {
namespace {
// Word bits preserve copied positions/literals; signedWord avoids an
// out-of-range unsigned cast. single() retains source float rounding boundaries.
float real(uint32_t w) { float f;std::memcpy(&f,&w,4);return f; }
uint32_t bits(float f) { uint32_t w;std::memcpy(&w,&f,4);return w; }
int32_t signedWord(uint32_t w) { return w<=0x7fffffff?int32_t(w):int32_t(int64_t(w)-0x100000000LL); }
float single(float f) { volatile float v=f;return v; }
}
// Runs only AFTER actor traversal. GP80415fd0->F168->E=F550:effect word0
// alone gates state8/counterc; E4 is NOT the gate. Retain ORIGINAL F throughout
// callbacks, plus later distinct F2 for bookkeeping. Providers stable across holds.
bool advanceFieldNpcMainTail(const FieldNpcTailServices &s, FieldNpcTailCursor &c) {
  const auto &m=s.memory;
  if(!m.word || !m.store)return false;
  if(!c.active){c={};c.active=true;c.owner=s;}
  if(c.owner.memory.user!=m.user || c.owner.memory.word!=m.word ||
     c.owner.memory.store!=m.store || c.owner.byte!=s.byte || c.owner.half!=s.half ||
     c.owner.storeByte!=s.storeByte || c.owner.request!=s.request || c.owner.queue!=s.queue)return false;
  auto read=[&](uint32_t a,uint32_t &v){auto w=m.word(m.user,a);if(!w)return false;v=*w;return true;};
  auto load=[&](uint32_t a,float &v){uint32_t w;if(!read(a,w))return false;v=real(w);return true;};
  auto store=[&](uint32_t a,uint32_t v){return m.store(m.user,a,v);};
  auto request=[&]{return s.request && s.request(m.user,c.call,c.response);};
  for(;;)switch(c.phase){
  case 0:
    if(!read(0x80415fd0,c.gp))return false;
    ++c.phase;break;
  case 1:
    if(!read(c.gp+0x168,c.field))return false;
    c.effect=c.field+0x550;++c.phase;break;
  case 2:
    if(!read(c.effect,c.value))return false;
    c.phase=c.value?3:40;break;
  case 3:
    if(!read(c.effect+8,c.value))return false;
    c.phase=c.value==2?4:(c.value==1?20:40);break;
  // Increment wrapping32 counter,store,then RELOAD before signed branches.
  // State2:<20 curve5(-600->0),20..79 zeroX,80..99 curve1(0->+600),>=100
  // clearE4. State1:>60 clearE4,otherwise one zero feeds X/Y/queue depth.
  // Negative wrapped counters follow signed tests; no clamp or timer rewrite.
  case 4:case 20:
    if(!read(c.effect+0xc,c.count))return false;
    ++c.count;++c.phase;break;
  case 5:case 21:
    if(!store(c.effect+0xc,c.count))return false;
    ++c.phase;break;
  case 6:case 22:
    if(!read(c.effect+0xc,c.count))return false;
    if(c.phase==22)c.phase=signedWord(c.count)>60?30:23;
    else c.phase=signedWord(c.count)<20?7:(signedWord(c.count)<80?14:(signedWord(c.count)<100?10:30));
    break;
  case 7:
    if(!load(0x8041fce4,c.from))return false;
    ++c.phase;break;
  case 8:
    if(!load(0x8041fc7c,c.to))return false;
    c.call={0x80006c8c,{5,c.count,20},{c.from,c.to}};
    c.phase=12;break;
  case 10:
    if(!load(0x8041fc7c,c.from))return false;
    ++c.phase;break;
  case 11:
    if(!load(0x8041fce8,c.to))return false;
    c.call={0x80006c8c,{1,c.count-80,20},{c.from,c.to}};
    ++c.phase;break;
  case 12:
    if(!request())return false;
    c.interpolated=c.response.scalar;++c.phase;break;
  case 13:
    if(!store(c.effect+0x10,bits(c.interpolated)))return false;
    c.phase=16;break;
  case 14:
    if(!read(0x8041fc7c,c.value))return false;
    ++c.phase;break;
  case 15:
    if(!store(c.effect+0x10,c.value))return false;
    ++c.phase;break;
  // State2 loads a DISTINCT fresh ZERO for Y/queue after X publication;
  // plateauX has its own earlier zero. State1 shares one zero across both.
  // dispEntry camera9,class2,callback80047dfc,actualE; no callback dispatch here.
  case 16:
    if(!load(0x8041fc7c,c.zero))return false;
    ++c.phase;break;
  case 17:
    if(!store(c.effect+0x14,bits(c.zero)))return false;
    c.phase=26;break;
  case 23:
    if(!load(0x8041fc7c,c.zero))return false;
    ++c.phase;break;
  case 24:
    if(!store(c.effect+0x10,bits(c.zero)))return false;
    ++c.phase;break;
  case 25:
    if(!store(c.effect+0x14,bits(c.zero)))return false;
    ++c.phase;break;
  case 26:
    c.draw={0x80010bc4,{9,2,0x80047dfc,c.effect}};c.draw.depth=c.zero;
    ++c.phase;break;
  case 27:
    if(!s.queue || !s.queue(m.user,c.draw,c.drawResponse))return false;
    c.phase=40;break;
  case 30:
    if(!store(c.effect+4,0))return false;
    c.phase=40;break;
  // Fresh field mode half:1 encounter4727c,2/3 talk470dc,5 battle46a78;
  // all others skip. Only encounter continues into Kpa queries/bookkeeping.
  case 40:{
    auto v=s.half?s.half(m.user,c.field):std::nullopt;if(!v)return false;
    c.call={};
    if(*v==1){c.call.function=0x8004727c;c.phase=41;}
    else if(*v==2 || *v==3){c.call.function=0x800470dc;c.phase=80;}
    else if(*v==5){c.call.function=0x80046a78;c.phase=80;}
    else c.phase=90;
    break;
  }
  case 41:
    if(!request())return false;
    ++c.phase;break;
  // Capture F2 BEFORE fresh parent Mario lookup. Two independent Mario3c==2
  // gates surround jump801fcc98/fire801fe1b0; jump may change the second gate.
  // Each child itself fetches Mario again; preserve parent pointer for score copy.
  case 42:
    if(!read(0x80415fd0,c.gp))return false;
    ++c.phase;break;
  case 43:
    if(!read(c.gp+0x168,c.secondField))return false;
    c.call={0x8005e0c0};++c.phase;break;
  case 44:
    if(!request())return false;
    c.mario=c.response.word;++c.phase;break;
  case 45:{
    auto v=s.byte?s.byte(m.user,c.mario+0x3c):std::nullopt;if(!v)return false;
    c.call={0x801fcc98};c.phase=*v==2?46:47;break;
  }
  case 46:
    if(!request())return false;
    c.jump=c.response.word;++c.phase;break;
  case 47:{
    auto v=s.byte?s.byte(m.user,c.mario+0x3c):std::nullopt;if(!v)return false;
    c.call={0x801fe1b0};c.phase=*v==2?48:49;break;
  }
  case 48:
    if(!request())return false;
    c.fire=c.response.word;++c.phase;break;
  case 49:
    c.phase=c.jump?50:70;break;
  // Jump true: signed scoreF2+57c>0 then signed578>=2 admits score80216994.
  // Copy parent X/Y words,load original60(fc84),SP Y+60,then fresh Z word.
  // Child receives retained score and copied XYZ, never fabricated stack pointers.
  case 50:
    if(!read(c.secondField+0x57c,c.score))return false;
    c.phase=signedWord(c.score)>0?51:60;break;
  case 51:
    if(!read(c.secondField+0x578,c.value))return false;
    c.call={0x80216994,{c.score}};
    c.phase=signedWord(c.value)>=2?52:60;break;
  case 52:
    if(!read(c.mario+0x8c,c.call.point[0]))return false;
    ++c.phase;break;
  case 53:
    if(!read(c.mario+0x90,c.call.point[1]))return false;
    ++c.phase;break;
  case 54:
    if(!load(0x8041fc84,c.height))return false;
    c.call.point[1]=bits(single(real(c.call.point[1])+c.height));
    ++c.phase;break;
  case 55:
    if(!read(c.mario+0x94,c.call.point[2]))return false;
    ++c.phase;break;
  case 56:
    if(!request())return false;
    c.phase=60;break;
  // Jump true clears578 THEN57c regardless of score eligibility; jump false
  // preserves both. Fire false clears574; true preserves. Final byte54d clears
  // through ORIGINAL F even after callbacks change GP/F2/Mario; neighbors intact.
  case 60:
    if(!store(c.secondField+0x578,0))return false;
    ++c.phase;break;
  case 61:
    if(!store(c.secondField+0x57c,0))return false;
    c.phase=70;break;
  case 70:
    c.phase=c.fire?90:71;break;
  case 71:
    if(!store(c.secondField+0x574,0))return false;
    c.phase=90;break;
  case 80:
    if(!request())return false;
    c.phase=90;break;
  case 90:
    if(!s.storeByte || !s.storeByte(m.user,c.field+0x54d,0))return false;
    ++c.phase;break;
  case 91:return true;
  default:return false;
  }
}
} // namespace ttyd
