#include "animation_buffer_refresh.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Battle=0x80041c58,Refresh=0x80041c84,Allocate=0x80042ac8,Reset=0x80042e5c,Pose=0x80041ee0;
constexpr uint32_t Rebuild=1,Restore=2;
bool run(uint32_t,uint32_t,const BattleSequenceEntryServices&,AnimationBufferCursor&,unsigned);
int32_t signedWord(uint32_t value){int32_t result;std::memcpy(&result,&value,4);return result;}
struct Operations {
 const BattleSequenceEntryServices& s;AnimationBufferCursor& c;unsigned depth;
 bool load(uint32_t a,uint32_t& v){auto n=s.word?s.word(s.user,a):std::nullopt;if(!n)return false;v=*n;return true;}
 bool store(uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);}
 bool work(){return load(0x80415dc0,c.work);}
 bool call(uint32_t fn,std::array<uint32_t,5> args={}){
  if(!c.queued){c.call={};c.call.function=fn;c.call.args=args;c.queued=true;}
  c.pending=fn;if(!s.call || !s.call(s.user,c.call))return false;c.queued=false;return true;
 }
 bool child(uint32_t fn,uint32_t bytes=0){
  if(!c.child)c.child=std::make_unique<AnimationBufferCursor>();
  if(!run(fn,bytes,s,*c.child,depth+1))return false;
  c.value=c.child->result;if(fn==Rebuild || fn==Restore)c.work=c.child->work;c.child.reset();return true;
 }
 bool done(uint32_t result=0){c.result=result;c.active=false;c.pending=0;return true;}
 void next(){c.index+=1;c.offset+=0x170;c.phase=0;}
};
}
uint32_t pendingAnimationBufferFunction(const AnimationBufferCursor& c){return c.child?pendingAnimationBufferFunction(*c.child):c.pending;}
namespace {
bool run(uint32_t fn,uint32_t bytes,const BattleSequenceEntryServices& s,AnimationBufferCursor& c,unsigned depth){
 if(depth>64){c.pending=fn>2?fn:Refresh;return false;} // Native stack admission, never original success.
 if(!c.active){
  if(fn!=Battle && fn!=Refresh && fn!=Allocate && fn!=Reset && fn!=Pose && fn!=Rebuild && fn!=Restore)return false;
  c={};c.active=true;c.function=fn;c.bytes=bytes;c.pose=fn==Pose?bytes:0;c.pending=fn>2?fn:Refresh;
 }else if(c.function!=fn)return false;
 Operations m{s,c,depth};
 if(fn==Battle){for(;;)switch(c.phase){
  case 0:if(!m.work())return false;++c.phase;break;
  case 1:if(!m.store(c.work+0x10c,1))return false;++c.phase;break;
  case 2:if(!m.child(Refresh))return false;return m.done();default:return false;
 }}
 // Reset and Refresh share the actual scratch conditional/reset prefix.
 if(fn==Refresh || fn==Reset){for(;;)switch(c.phase){
  case 0:if(!m.work())return false;++c.phase;break;
  case 1:if(!m.load(c.work+0x104,c.base))return false;c.phase=c.base?5:2;break;
  case 2:if(!m.call(0x800300f0,{0,0x180000}))return false;c.base=c.call.result;++c.phase;break;
  case 3:if(!m.work())return false;++c.phase;break;
  case 4:if(!m.store(c.work+0x104,c.base))return false;++c.phase;break;
  case 5:if(!m.work())return false;++c.phase;break;
  case 6:if(!m.load(c.work+0x104,c.base))return false;++c.phase;break;
  case 7:if(!m.store(c.work+0x108,c.base))return false;++c.phase;break;
  case 8:if(fn==Reset)return m.done();if(!m.child(Rebuild))return false;return m.done();
  default:return false;
 }}
 if(fn==Rebuild || fn==Restore){for(;;)switch(c.phase){
  case 0:if(!m.work())return false;++c.phase;break;
  case 1:if(!m.load(c.work+0x14,c.value))return false;if(signedWord(c.index)>=signedWord(c.value))return m.done();++c.phase;break;
  case 2:if(!m.load(c.work+0x10,c.pose))return false;c.pose+=c.offset;++c.phase;break;
  case 3:if(!m.load(c.pose,c.value))return false;if(!c.value){m.next();break;}++c.phase;break;
  case 4:if(!m.load(c.work+0x10c,c.value))return false;c.phase=c.value?5:6;break;
  case 5:if(!m.load(c.pose+0xc,c.value))return false;if(!c.value){m.next();break;}c.phase=7;break;
  case 6:if(!m.load(c.pose+0xc,c.value))return false;if(c.value==1){m.next();break;}++c.phase;break;
  case 7:if(fn==Restore){c.phase=40;break;}c.buffer=0;c.phase=21;break;
  case 20:if(!m.work())return false;++c.phase;break;
  case 21:if(!m.load(c.pose+0x10,c.model))return false;++c.phase;break;
  case 22:if(!m.load(c.work,c.value))return false;++c.phase;break;
  case 23:if(!m.load(c.value+16*c.model+8,c.value))return false;++c.phase;break;
  case 24:if(!m.load(c.value+0xa0,c.value))return false;++c.phase;break;
  case 25:if(!m.load(c.value,c.model))return false;++c.phase;break;
  case 26:{constexpr uint32_t offsets[]={0xf0,0xf0,0xf8,0xf8,0x13c,0x140,0x140,0x12c};
   if(!m.load(c.model+offsets[c.buffer],c.bytes))return false;
   c.bytes*=c.buffer<4?12:c.buffer==4?1:c.buffer<7?4:24;++c.phase;break;}
  case 27:if(!m.child(Allocate,c.bytes))return false;++c.phase;break;
  case 28:{constexpr uint32_t offsets[]={0x48,0x4c,0x50,0x54,0x58,0x60,0x64,0x68};
   if(!m.store(c.pose+offsets[c.buffer],c.value))return false;
   if(c.buffer==4 || c.buffer==7){c.phase=29;break;}++c.buffer;c.phase=20;break;}
  case 29:if(!m.load(c.pose+(c.buffer==4?0x58:0x68),c.value))return false;++c.phase;break;
  case 30:if(!m.store(c.pose+(c.buffer==4?0x5c:0x6c),c.value))return false;if(c.buffer==7){c.phase=31;break;}++c.buffer;c.phase=20;break;
  case 31:case 32:if(!m.store(c.pose+0x3c,0xffffffff))return false;if(c.phase++==32)m.next();break;
  case 40:if(!m.child(Pose,c.pose))return false;++c.phase;break;
  case 41:if(!m.store(c.pose+0x3c,0xffffffff))return false;m.next();break;
  default:return false;
 }}
 // animPose_AllocBuffer has eight inlined reservations. Its FIRST overflow
 // resets and recursively restores selected poses; testAlloc first rebuilds
 // them inline instead. Preserve that distinction and the early cursor capture.
 if(fn==Pose){for(;;)switch(c.phase){
  case 0:if(!m.work())return false;++c.phase;break;
  case 1:if(!m.load(c.pose+0x10,c.model))return false;++c.phase;break;
  case 2:if(!m.load(c.work,c.value))return false;++c.phase;break;
  case 3:if(!m.load(c.work+0x108,c.reserved))return false;++c.phase;break;
  case 4:if(!m.load(c.value+16*c.model+8,c.value))return false;++c.phase;break;
  case 5:if(!m.load(c.value+0xa0,c.value))return false;++c.phase;break;
  case 6:if(!m.load(c.value,c.model))return false;++c.phase;break;
  case 7:{constexpr uint32_t offsets[]={0xf0,0xf0,0xf8,0xf8,0x13c,0x140,0x140,0x12c};
   if(!m.load(c.model+offsets[c.buffer],c.bytes))return false;
   c.bytes*=c.buffer<4?12:c.buffer==4?1:c.buffer<7?4:24;++c.phase;break;}
  case 8:c.aligned=(c.bytes+31)&~31u;++c.phase;break;
  case 9:if(!m.load(c.work+0x108,c.value))return false;++c.phase;break;
  case 10:if(!m.store(c.work+0x108,c.value+c.aligned))return false;++c.phase;break;
  case 11:if(!m.work())return false;++c.phase;break;
  case 12:if(!m.load(c.work+0x104,c.base))return false;++c.phase;break;
  case 13:if(!m.load(c.work+0x108,c.value))return false;++c.phase;break;
  case 14:if(c.value<c.base+0x180000){c.value=c.reserved;c.phase=30;break;}++c.phase;break;
  case 15:if(!m.child(Reset))return false;++c.phase;break;
  case 16:if(!m.child(Restore))return false;++c.phase;break;
  case 17:if(!m.load(c.work+0x108,c.reserved))return false;++c.phase;break;
  case 18:c.aligned=(c.aligned+31)&~31u;++c.phase;break;
  case 19:if(!m.load(c.work+0x108,c.value))return false;++c.phase;break;
  case 20:if(!m.store(c.work+0x108,c.value+c.aligned))return false;++c.phase;break;
  case 21:if(!m.work())return false;++c.phase;break;
  case 22:if(!m.load(c.work+0x104,c.base))return false;++c.phase;break;
  case 23:if(!m.load(c.work+0x108,c.value))return false;++c.phase;break;
  case 24:if(c.value<c.base+0x180000){c.value=c.reserved;c.phase=30;break;}++c.phase;break;
  case 25:if(!m.child(Refresh))return false;++c.phase;break;
  case 26:if(!m.child(Allocate,c.aligned))return false;c.phase=30;break;
  case 30:{constexpr uint32_t offsets[]={0x48,0x4c,0x50,0x54,0x58,0x60,0x64,0x68};
   if(!m.store(c.pose+offsets[c.buffer],c.value))return false;
   if(c.buffer==4 || c.buffer==7){++c.phase;break;}++c.buffer;c.phase=0;break;}
  case 31:if(!m.load(c.pose+(c.buffer==4?0x58:0x68),c.value))return false;++c.phase;break;
  case 32:if(!m.store(c.pose+(c.buffer==4?0x5c:0x6c),c.value))return false;
   if(c.buffer==7){++c.phase;break;}++c.buffer;c.phase=0;break;
  case 33:if(!m.store(c.pose+0x3c,0xffffffff))return false;return m.done();
  default:return false;
 }}
 // testAlloc: three literal reservations with source unsigned strict-end test.
 for(;;)switch(c.phase){
 case 0:c.aligned=(c.bytes+31)&~31u;if(!c.attempt && !m.work())return false;++c.phase;break;
 case 1:if(!m.load(c.work+0x108,c.reserved))return false;++c.phase;break;
 case 2:if(!m.load(c.work+0x108,c.value))return false;++c.phase;break;
 case 3:if(!m.store(c.work+0x108,c.value+c.aligned))return false;++c.phase;break;
 case 4:if(!m.work())return false;++c.phase;break;
 case 5:if(!m.load(c.work+0x104,c.base))return false;++c.phase;break;
 case 6:if(!m.load(c.work+0x108,c.value))return false;++c.phase;break;
 case 7:
  if(c.value<c.base+0x180000)return m.done(c.reserved);
  if(!c.attempt){c.phase=c.base?11:8;break;}
  c.phase=c.attempt==1?20:23;break;
 case 8:if(!m.call(0x800300f0,{0,0x180000}))return false;c.base=c.call.result;++c.phase;break;
 case 9:if(!m.work())return false;++c.phase;break;
 case 10:if(!m.store(c.work+0x104,c.base))return false;++c.phase;break;
 case 11:if(!m.work())return false;++c.phase;break;
 case 12:if(!m.load(c.work+0x104,c.base))return false;++c.phase;break;
 case 13:if(!m.store(c.work+0x108,c.base))return false;++c.phase;break;
 case 14:if(!m.child(Rebuild))return false;c.attempt=1;c.phase=0;break;
 case 20:if(!m.child(Reset))return false;++c.phase;break;
 case 21:if(!m.child(Restore))return false;c.attempt=2;c.phase=0;break;
 case 23:if(!m.child(Refresh))return false;++c.phase;break;
 case 24:if(!m.child(Allocate,c.aligned))return false;return m.done(c.value);
 default:return false;
 }
 }
}
bool runAnimationBufferFunction(uint32_t fn,uint32_t bytes,const BattleSequenceEntryServices& s,AnimationBufferCursor& c){
 if(fn!=Battle && fn!=Refresh && fn!=Allocate && fn!=Reset && fn!=Pose)return false;
 return run(fn,bytes,s,c,0);
}
}
