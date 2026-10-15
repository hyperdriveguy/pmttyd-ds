#include "source_gx_texture_object.hpp"
#include <cstring>
#include <algorithm>
namespace ttyd {namespace {
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
double real64(uint32_t hi,uint32_t lo){uint64_t n=(uint64_t(hi)<<32)|lo;double f;std::memcpy(&f,&n,8);return f;}
float mul(float a,float b){volatile float v=a*b;return v;}
// PPC rotate-and-mask insertion: first/last number bits from MSB0,not C++
// least-significant bit0. Preserve every source read/modify/store separately.
uint32_t insert(uint32_t old,uint32_t value,unsigned shift,unsigned first,unsigned last){const uint32_t mask=(0xffffffffu>>first)&(0xffffffffu<<(31-last));const auto rotated=shift?((value<<shift)|(value>>(32-shift))):value;return (old&~mask)|(rotated&mask);}
bool same(const SourceGxTextureObjectServices&a,const SourceGxTextureObjectServices&b){return a.user==b.user&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.current==b.current&&a.word==b.word&&a.byte==b.byte&&a.store==b.store&&a.clear==b.clear&&a.truncate==b.truncate;}
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
}
// pc values below are native instruction frontiers,not source bytes or time.
// Reads/operands survive holds; observed context loss stales until reset. A
// completed call consumes its cursor but never proves native texture residency.
struct SourceGxTextureObjectCursor::State {
 SourceGxTextureObjectServices s;SourceGxTextureObjectInput input;
 uint32_t fn=0,pc=0,pending=0,w=0,tmp=0,hi=0,lo=0,minimum=0,maximum=0,jump=0,kind=0,x=0,y=0;
 float f=0,g=0,h=0,scale=0,zero=0;bool invalid=false;
 bool valid(){if(invalid)return false;if(!s.current||!s.current(s.user)){invalid=true;return false;}return true;}
 bool read(uint32_t site,uint32_t at,uint32_t&out){pending=site;if(!valid()||!s.word)return false;auto v=s.word(s.user,at);if(!valid()||!v)return false;out=*v;return true;}
 bool readByte(uint32_t site,uint32_t at,uint32_t&out){pending=site;if(!valid()||!s.byte)return false;auto v=s.byte(s.user,at);if(!valid()||!v)return false;out=*v;return true;}
 bool write(uint32_t site,uint32_t at,uint32_t size,uint32_t value){pending=site;if(!valid()||!s.store)return false;const bool ready=s.store(s.user,at,size,value);return valid()&&ready;}
 bool trunc(uint32_t site,float value,uint32_t&out){pending=site;if(!valid()||!s.truncate)return false;auto v=s.truncate(s.user,value);if(!valid()||!v)return false;out=uint32_t(*v);return true;}
};
SourceGxTextureObjectCursor::SourceGxTextureObjectCursor()=default;
SourceGxTextureObjectCursor::~SourceGxTextureObjectCursor()=default;
bool SourceGxTextureObjectCursor::active()const{return bool(state_);}bool SourceGxTextureObjectCursor::stale()const{return state_&&state_->invalid;}
uint32_t SourceGxTextureObjectCursor::pending()const{return state_?state_->pending:0;}
bool SourceGxTextureObjectCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Each external load/store/conversion has its own retained instruction boundary.
// Reads captured before a held fctiwz are not repeated; later source loads stay late.
bool resumeSourceGxTextureObject(uint32_t function,const SourceGxTextureObjectInput&i,const SourceGxTextureObjectServices&s,SourceGxTextureObjectCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.current||(function!=0x802b72b0&&function!=0x802b7544))return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.fn!=function||c.input.integers!=i.integers||c.input.reals!=i.reals||!same(c.s,s))return false;}
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<SourceGxTextureObjectCursor::State>();auto&c=*cursor.state_;c.s=s;c.input=i;c.fn=function;c.pc=function==0x802b72b0?0:100;c.f=real(i.reals[2]);c.g=real(i.reals[0]);c.h=real(i.reals[1]);}
 auto&c=*cursor.state_;const auto&a=c.input.integers;const uint32_t obj=a[0];
#define READ(P,S,A,D,N) case P:if(!c.read(S,A,c.D))return false;c.pc=N;continue
#define BYTE(P,S,A,D,N) case P:if(!c.readByte(S,A,c.D))return false;c.pc=N;continue
#define WRITE(P,S,A,Z,V,N) case P:if(!c.write(S,A,Z,V))return false;c.pc=N;continue
#define TRUNC(P,S,F,D,N) case P:if(!c.trunc(S,F,c.D))return false;c.pc=N;continue
 for(;;){if(!c.valid())return false;switch(c.pc){
 // Init clears exactly32 actual object bytes through whole memset child.
 // Engaged clear0 completes. Then word0 packs wrapS[1:0],wrapT[3:2] and
 // linear-mag bit4; no inferred zeroed object replaces the clear.
 case 0:{c.pending=0x800050b4;if(!s.clear)return false;const auto v=s.clear(s.user,obj);if(!c.valid()||!v)return false;c.pc=1;continue;}
 READ(1,0x802b72ec,obj,w,2);
 WRITE(2,0x802b72f8,obj,4,insert(c.w,a[5],0,30,31),3);
 READ(3,0x802b7300,obj,w,4);
 WRITE(4,0x802b7308,obj,4,insert(c.w,a[6],2,28,29),5);
 READ(5,0x802b730c,obj,w,6);
 WRITE(6,0x802b7314,obj,4,insert(c.w,1,4,27,27),7);
 // Mipmap tests LOW8 only. Formats8..10 choose filter5,others6; no-mip
 // selects4. Byte31 mask1 marks mipmap,mask2 marks Init completion. maxLOD=floor(log2(max(U16 W,H)))
 // with startingffffffff for both0; no invented valid-dimension clamp.
 case 7:c.pc=uint8_t(a[7])?8:24;continue;
 BYTE(8,0x802b731c,obj+31,w,9);
 WRITE(9,0x802b732c,obj+31,1,c.w|1,10);
 READ(10,uint32_t(a[4]-8)<=2?0x802b7334:0x802b7348,obj,w,11);
 WRITE(11,uint32_t(a[4]-8)<=2?0x802b7340:0x802b7354,obj,4,insert(c.w,uint32_t(a[4]-8)<=2?5:6,5,24,26),12);
 case 12:{uint32_t n=std::max(uint32_t(uint16_t(a[2])),uint32_t(uint16_t(a[3])));c.maximum=0xffffffff;while(n){++c.maximum;n>>=1;}c.pc=13;continue;}
 READ(13,0x802b7384,0x80429448,hi,14);
 READ(14,0x802b7384,0x8042944c,lo,15);
 READ(15,0x802b738c,0x80429440,tmp,16);
 case 16:c.scale=real(c.tmp);c.pc=17;continue;
 // Capture object word4 BEFORE maxLOD conversion. Original unsigned double
 // bias29448 and scale29440 feed fctiwz; a hold must not reload that word.
 READ(17,0x802b7394,obj+4,w,18);
 case 18:{volatile float v=float(real64(0x43300000,c.maximum)-real64(c.hi,c.lo));c.f=mul(c.scale,v);c.pc=19;continue;}
 TRUNC(19,0x802b73a0,c.f,tmp,20);
 WRITE(20,0x802b73b0,obj+4,4,insert(c.w,c.tmp,8,16,23),26);
 READ(24,0x802b73b8,obj,w,25);
 WRITE(25,0x802b73c4,obj,4,insert(c.w,4,5,24,26),26);
 // Full format word goes at+20; word8 packs U16(width-1) low10,
 // U16(height-1) next10 and format low4. Image word12 encodes32-byte units
 // through source rotate27. High input format bits survive only the full word.
 WRITE(26,0x802b73c8,obj+20,4,a[4],27);
 READ(27,0x802b73d8,obj+8,w,28);
 WRITE(28,0x802b73e0,obj+8,4,insert(c.w,uint32_t(uint16_t(a[2]))-1,0,22,31),29);
 READ(29,0x802b73ec,obj+8,w,30);
 WRITE(30,0x802b73f8,obj+8,4,insert(c.w,uint32_t(uint16_t(a[3]))-1,10,12,21),31);
 READ(31,0x802b73fc,obj+8,w,32);
 WRITE(32,0x802b7404,obj+8,4,insert(c.w,a[4],20,8,11),33);
 READ(33,0x802b7408,obj+12,w,34);
 WRITE(34,0x802b7410,obj+12,4,insert(c.w,a[1],27,11,31),35);
 // Dispatch uses format low4 into original jump table803c4e84 (0..14),
 // default15. Targets choose byte30 kind and tile log2 width/height:3/3,
 //3/2 or2/2. Half28 stores wrapping tile product masked7fff. Unknown target
 // remains captured/pending; repairing table later cannot redispatch it.
 case 35:if((a[4]&15)>14){c.jump=0x802b7494;c.pc=37;}else c.pc=36;continue;
 READ(36,0x802b7424,0x803c4e84+4*(a[4]&15),jump,37);
 case 37:switch(c.jump){case 0x802b7430:c.kind=1;c.x=3;c.y=3;break;case 0x802b7444:c.kind=2;c.x=3;c.y=2;break;case 0x802b7458:case 0x802b7494:c.kind=2;c.x=2;c.y=2;break;case 0x802b746c:c.kind=3;c.x=2;c.y=2;break;case 0x802b7480:c.kind=0;c.x=3;c.y=3;break;default:c.pending=c.jump;return false;}c.pc=38;continue;
 WRITE(38,c.jump+4,obj+30,1,c.kind,39);
 WRITE(39,0x802b74d8,obj+28,2,((((uint32_t(uint16_t(a[2]))+(1u<<c.x)-1)>>c.x)*((uint32_t(uint16_t(a[3]))+(1u<<c.y)-1)>>c.y))&0x7fff),40);
 BYTE(40,0x802b74dc,obj+31,w,41);
 WRITE(41,0x802b74e4,obj+31,1,c.w|2,200);
 // LOD clamp/load order follows original fcmpo branches (NaNs remain unordered).
 // LOD bias clamps by original ordered branches:lower-4,upper>=4 uses
 //3.99,then scale32. NaN stays unordered until actual fctiwz provider.
 // Capture word0 BEFORE conversion; do not replace this with host clamp().
 READ(100,0x802b7548,0x80429450,tmp,101);
 case 101:if(c.f<real(c.tmp)){c.f=real(c.tmp);c.pc=105;}else c.pc=102;continue;
 READ(102,0x802b755c,0x80429454,tmp,103);
 case 103:c.pc=c.f>=real(c.tmp)?104:105;continue;
 READ(104,0x802b756c,0x80429458,tmp,106);
 case 106:c.f=real(c.tmp);c.pc=105;continue;
 READ(105,0x802b7570,0x8042945c,tmp,107);
 case 107:c.scale=real(c.tmp);c.pc=108;continue;
 READ(108,0x802b7578,obj,w,109);
 case 109:c.f=mul(c.scale,c.f);c.pc=110;continue;
 TRUNC(110,0x802b7580,c.f,tmp,111);
 WRITE(111,0x802b7590,obj,4,insert(c.w,c.tmp,9,15,22),112);
 READ(112,0x802b75a4,obj,w,113);
 WRITE(113,0x802b75b0,obj,4,insert(c.w,a[2]==1?1:0,4,27,27),114);
 // minFilter indexes original byte table with FULL unsigned argument and
 // wrapped address; mag is exactly==1. Low8 edgeLOD is inverted into bit8.
 // Clear bits17/18; pack anisotropy19..20 and biasClamp21 from raw arguments.
 BYTE(114,0x802b75b8,0x8041e558+a[1],tmp,115);
 READ(115,0x802b75bc,obj,w,116);
 WRITE(116,0x802b75c4,obj,4,insert(c.w,c.tmp,5,24,26),117);
 READ(117,0x802b75d8,obj,w,118);
 WRITE(118,0x802b75e4,obj,4,insert(c.w,uint8_t(a[4])?0:1,8,23,23),119);
 READ(119,0x802b75e8,obj,w,120);
 WRITE(120,0x802b75f0,obj,4,insert(c.w,0,17,14,14),121);
 READ(121,0x802b75f4,obj,w,122);
 WRITE(122,0x802b75fc,obj,4,insert(c.w,0,18,13,13),123);
 READ(123,0x802b7600,obj,w,124);
 WRITE(124,0x802b7608,obj,4,insert(c.w,a[5],19,11,12),125);
 READ(125,0x802b760c,obj,w,126);
 WRITE(126,0x802b7614,obj,4,insert(c.w,a[3],21,10,10),127);
 // Min/max LOD separately clamp0..10,scale16 and call fctiwz. Write min
 // to object word4 BEFORE max conversion,then reload that word afterwards.
 // Aliases across a max-conversion hold therefore differ from earlier captures.
 READ(127,0x802b7618,0x80429460,tmp,128);
 case 128:if(c.g<real(c.tmp)){c.g=real(c.tmp);c.pc=132;}else c.pc=129;continue;
 READ(129,0x802b762c,0x80429464,tmp,130);
 case 130:if(c.g>real(c.tmp))c.g=real(c.tmp);c.pc=132;continue;
 READ(132,0x802b763c,0x80429440,tmp,133);
 case 133:c.scale=real(c.tmp);c.pc=134;continue;
 READ(134,0x802b7640,0x80429460,tmp,135);
 case 135:c.zero=real(c.tmp);c.g=mul(c.scale,c.g);c.pc=136;continue;
 TRUNC(136,0x802b764c,c.g,minimum,137);
 case 137:if(c.h<c.zero){c.h=c.zero;c.pc=141;}else c.pc=138;continue;
 READ(138,0x802b7664,0x80429464,tmp,139);
 case 139:if(c.h>real(c.tmp))c.h=real(c.tmp);c.pc=141;continue;
 READ(141,0x802b7674,0x80429440,tmp,142);
 case 142:c.scale=real(c.tmp);c.pc=143;continue;
 READ(143,0x802b7678,obj+4,w,144);
 case 144:c.w=insert(c.w,c.minimum,0,24,31);c.h=mul(c.scale,c.h);c.pc=145;continue;
 WRITE(145,0x802b7684,obj+4,4,c.w,146);
 TRUNC(146,0x802b7688,c.h,maximum,147);
 READ(147,0x802b768c,obj+4,w,148);
 WRITE(148,0x802b769c,obj+4,4,insert(c.w,c.maximum,8,16,23),200);
 case 200:cursor.state_.reset();return true;
 default:return false;
 }}
#undef READ
#undef BYTE
#undef WRITE
#undef TRUNC
}
}
