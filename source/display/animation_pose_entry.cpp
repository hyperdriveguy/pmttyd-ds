#include "animation_pose_entry.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}
}
// Fixed US function: case labels are original PCs, decimal immediates below
// are PPC operands. r holds 32 source GPRs; f retains two float words.
// SDA2 r2=80427440; SDA r13=8041cf20. r13-29024=>W root80415dc0,
// r13-28496=>GP root80415fd0. r27=802c0d18 is the local literal base.
// Caller must retain service/mapping ownership: Entry pins name/mode but does
// not fingerprint callbacks or supply its own reentry/lifetime lock.
bool enterAnimationPose(uint32_t name,uint32_t mode,const AnimationEntryServices& s,AnimationEntryCursor& c){
 if(!c.active){c={};c.active=true;c.pc=0x8004177c;c.name=name;c.mode=mode;c.r[2]=0x80427440;c.r[13]=0x8041cf20;c.r[3]=name;c.r[4]=mode;}
 else if(c.name!=name || c.mode!=mode)return false;
 auto& r=c.r;
 // Register writes clear semantic-stack tags; register copies propagate them.
 // Five child arguments are r3..r7 (mask 31); never fabricate a stack address.
 auto reg=[&](unsigned i,uint32_t v){r[i]=v;c.stackRegisters&=~(1u<<i);};
 auto load=[&](uint32_t a,uint32_t& v){auto n=s.memory.word?s.memory.word(s.memory.user,a):std::nullopt;if(!n)return false;v=*n;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.memory.store && s.memory.store(s.memory.user,a,v);};
 // One explicit continuation per original instruction: completed reads/stores
 // and child arguments never replay across an unavailable primitive. This is
 // the recovered Entry body, not an interpreter for arbitrary executable data.
 for(;;){c.pending=c.pc;switch(c.pc){
 // Modes 0/1/2 calls fileSetCurrentArchiveType80189d38 with 1/2/0.
 // Other modes skip it. W10/W14 hold pose base/count; scan first whole
 // flags==0 at stride 368 decimal (0x170). Exhaustion uses pointer 0/index -1,
 // then reaches the original null access, not graceful native admission.
 case 0x8004177c: {const auto stack=(c.stackRegisters>>4)&1;reg(28,r[4]);c.stackRegisters|=stack<<28;}c.pc=0x80041780;break;
 case 0x80041780: reg(4,uint32_t(-32724)<<16);c.pc=0x80041784;break;
 case 0x80041784: {const auto stack=(c.stackRegisters>>3)&1;reg(29,r[3]);c.stackRegisters|=stack<<29;}c.pc=0x80041788;break;
 case 0x80041788: c.condition=(signedWord(r[28])>1)-(signedWord(r[28])<1);c.pc=0x8004178c;break;
 case 0x8004178c: reg(27,r[4]+uint32_t(3352));c.pc=0x80041790;break;
 case 0x80041790: if(c.condition==0){c.pc=0x800417bc;break;}c.pc=0x80041794;break;
 case 0x80041794: if(!(c.condition<0)){c.pc=0x800417a4;break;}c.pc=0x80041798;break;
 case 0x80041798: c.condition=(signedWord(r[28])>0)-(signedWord(r[28])<0);c.pc=0x8004179c;break;
 case 0x8004179c: if(!(c.condition<0)){c.pc=0x800417b0;break;}c.pc=0x800417a0;break;
 case 0x800417a0: if(true){c.pc=0x800417d0;break;}c.pc=0x800417a4;break;
 case 0x800417a4: c.condition=(signedWord(r[28])>3)-(signedWord(r[28])<3);c.pc=0x800417a8;break;
 case 0x800417a8: if(!(c.condition<0)){c.pc=0x800417d0;break;}c.pc=0x800417ac;break;
 case 0x800417ac: if(true){c.pc=0x800417c8;break;}c.pc=0x800417b0;break;
 case 0x800417b0: reg(3,uint32_t(1));c.pc=0x800417b4;break;
 case 0x800417b4: if(!c.queued){c.call={};c.call.function=0x80189d38;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x80189d38;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x800417b8;break;
 case 0x800417b8: if(true){c.pc=0x800417d0;break;}c.pc=0x800417bc;break;
 case 0x800417bc: reg(3,uint32_t(2));c.pc=0x800417c0;break;
 case 0x800417c0: if(!c.queued){c.call={};c.call.function=0x80189d38;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x80189d38;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x800417c4;break;
 case 0x800417c4: if(true){c.pc=0x800417d0;break;}c.pc=0x800417c8;break;
 case 0x800417c8: reg(3,uint32_t(0));c.pc=0x800417cc;break;
 case 0x800417cc: if(!c.queued){c.call={};c.call.function=0x80189d38;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x80189d38;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x800417d0;break;
 case 0x800417d0: if(!load(r[13]+uint32_t(-29024),r[5]))return false;c.stackRegisters&=~(1u<<5);c.pc=0x800417d4;break;
 case 0x800417d4: reg(30,uint32_t(0));c.pc=0x800417d8;break;
 case 0x800417d8: {const auto stack=(c.stackRegisters>>30)&1;reg(4,r[30]);c.stackRegisters|=stack<<4;}c.pc=0x800417dc;break;
 case 0x800417dc: if(!load(r[5]+uint32_t(20),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800417e0;break;
 case 0x800417e0: c.ctr=r[0];c.pc=0x800417e4;break;
 case 0x800417e4: c.condition=(signedWord(r[0])>0)-(signedWord(r[0])<0);c.pc=0x800417e8;break;
 case 0x800417e8: if(!(c.condition>0)){c.pc=0x80041814;break;}c.pc=0x800417ec;break;
 case 0x800417ec: if(!load(r[5]+uint32_t(16),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800417f0;break;
 case 0x800417f0: reg(3,r[0]+r[4]);c.pc=0x800417f4;break;
 case 0x800417f4: if(!load(r[3]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800417f8;break;
 case 0x800417f8: c.condition=r[0]?1:0;c.pc=0x800417fc;break;
 case 0x800417fc: if(!(c.condition==0)){c.pc=0x80041808;break;}c.pc=0x80041800;break;
 case 0x80041800: {const auto stack=(c.stackRegisters>>3)&1;reg(31,r[3]);c.stackRegisters|=stack<<31;}c.pc=0x80041804;break;
 case 0x80041804: if(true){c.pc=0x8004181c;break;}c.pc=0x80041808;break;
 case 0x80041808: reg(4,r[4]+uint32_t(368));c.pc=0x8004180c;break;
 case 0x8004180c: reg(30,r[30]+uint32_t(1));c.pc=0x80041810;break;
 case 0x80041810: if(--c.ctr!=0){c.pc=0x800417ec;break;}c.pc=0x80041814;break;
 case 0x80041814: reg(31,uint32_t(0));c.pc=0x80041818;break;
 case 0x80041818: reg(30,uint32_t(-1));c.pc=0x8004181c;break;
 // P0 OR active1, P4/P8=0. W0/W4 hold MODEL base/count; 16-byte rows
 // contain flags0, refs4, file8, texture index c. Active scan reloads W/base;
 // FileEntry+a0 -> holder[0] MODEL; compare MODEL+4 via strcmp8026d464.
 case 0x8004181c: if(!load(r[31]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041820;break;
 case 0x80041820: reg(25,uint32_t(0));c.pc=0x80041824;break;
 case 0x80041824: {const auto stack=(c.stackRegisters>>25)&1;reg(22,r[25]);c.stackRegisters|=stack<<22;}c.pc=0x80041828;break;
 case 0x80041828: reg(0,r[0]|1u);c.pc=0x8004182c;break;
 case 0x8004182c: if(!store(r[31]+uint32_t(0),r[0]))return false;c.pc=0x80041830;break;
 case 0x80041830: if(!store(r[31]+uint32_t(4),r[25]))return false;c.pc=0x80041834;break;
 case 0x80041834: if(!store(r[31]+uint32_t(8),r[25]))return false;c.pc=0x80041838;break;
 case 0x80041838: if(!load(r[13]+uint32_t(-29024),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x8004183c;break;
 case 0x8004183c: if(!load(r[3]+uint32_t(4),r[23]))return false;c.stackRegisters&=~(1u<<23);c.pc=0x80041840;break;
 case 0x80041840: if(true){c.pc=0x80041894;break;}c.pc=0x80041844;break;
 case 0x80041844: if(!load(r[13]+uint32_t(-29024),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041848;break;
 case 0x80041848: if(!load(r[3]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x8004184c;break;
 case 0x8004184c: reg(21,r[0]+r[22]);c.pc=0x80041850;break;
 case 0x80041850: if(!load(r[21]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041854;break;
 case 0x80041854: c.condition=r[0]?1:0;c.pc=0x80041858;break;
 case 0x80041858: if(c.condition==0){c.pc=0x8004188c;break;}c.pc=0x8004185c;break;
 case 0x8004185c: if(!load(r[21]+uint32_t(8),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041860;break;
 case 0x80041860: {const auto stack=(c.stackRegisters>>29)&1;reg(4,r[29]);c.stackRegisters|=stack<<4;}c.pc=0x80041864;break;
 case 0x80041864: if(!load(r[3]+uint32_t(160),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041868;break;
 case 0x80041868: if(!load(r[3]+uint32_t(0),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x8004186c;break;
 case 0x8004186c: reg(3,r[3]+uint32_t(4));c.pc=0x80041870;break;
 case 0x80041870: if(!c.queued){c.call={};c.call.function=0x8026d464;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026d464;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041874;break;
 case 0x80041874: c.condition=(signedWord(r[3])>0)-(signedWord(r[3])<0);c.pc=0x80041878;break;
 case 0x80041878: if(!(c.condition==0)){c.pc=0x8004188c;break;}c.pc=0x8004187c;break;
 case 0x8004187c: if(!load(r[21]+uint32_t(4),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041880;break;
 case 0x80041880: reg(0,r[3]+uint32_t(1));c.pc=0x80041884;break;
 case 0x80041884: if(!store(r[21]+uint32_t(4),r[0]))return false;c.pc=0x80041888;break;
 case 0x80041888: if(true){c.pc=0x80041a60;break;}c.pc=0x8004188c;break;
 case 0x8004188c: reg(22,r[22]+uint32_t(16));c.pc=0x80041890;break;
 case 0x80041890: reg(25,r[25]+uint32_t(1));c.pc=0x80041894;break;
 case 0x80041894: c.condition=(signedWord(r[25])>signedWord(r[23]))-(signedWord(r[25])<signedWord(r[23]));c.pc=0x80041898;break;
 case 0x80041898: if(c.condition<0){c.pc=0x80041844;break;}c.pc=0x8004189c;break;
 // Model miss selects first free row, publishes active1 / increments refs,
 // then fileAllocf8018a61c(type 5, "%s/%s", "a", name). SDA2-30988/-30980
 // are8041fb34/fb3c. Completed null handle is source failure, not a hold.
 case 0x8004189c: if(!load(r[13]+uint32_t(-29024),r[4]))return false;c.stackRegisters&=~(1u<<4);c.pc=0x800418a0;break;
 case 0x800418a0: reg(25,uint32_t(0));c.pc=0x800418a4;break;
 case 0x800418a4: {const auto stack=(c.stackRegisters>>25)&1;reg(3,r[25]);c.stackRegisters|=stack<<3;}c.pc=0x800418a8;break;
 case 0x800418a8: if(!load(r[4]+uint32_t(4),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800418ac;break;
 case 0x800418ac: c.ctr=r[0];c.pc=0x800418b0;break;
 case 0x800418b0: c.condition=(signedWord(r[0])>0)-(signedWord(r[0])<0);c.pc=0x800418b4;break;
 case 0x800418b4: if(!(c.condition>0)){c.pc=0x800418dc;break;}c.pc=0x800418b8;break;
 case 0x800418b8: if(!load(r[4]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800418bc;break;
 case 0x800418bc: reg(26,r[0]+r[3]);c.pc=0x800418c0;break;
 case 0x800418c0: if(!load(r[26]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800418c4;break;
 case 0x800418c4: c.condition=r[0]?1:0;c.pc=0x800418c8;break;
 case 0x800418c8: if(!(c.condition==0)){c.pc=0x800418d0;break;}c.pc=0x800418cc;break;
 case 0x800418cc: if(true){c.pc=0x800418e4;break;}c.pc=0x800418d0;break;
 case 0x800418d0: reg(3,r[3]+uint32_t(16));c.pc=0x800418d4;break;
 case 0x800418d4: reg(25,r[25]+uint32_t(1));c.pc=0x800418d8;break;
 case 0x800418d8: if(--c.ctr!=0){c.pc=0x800418b8;break;}c.pc=0x800418dc;break;
 case 0x800418dc: reg(26,uint32_t(0));c.pc=0x800418e0;break;
 case 0x800418e0: reg(25,uint32_t(-1));c.pc=0x800418e4;break;
 case 0x800418e4: reg(0,uint32_t(1));c.pc=0x800418e8;break;
 case 0x800418e8: {const auto stack=(c.stackRegisters>>29)&1;reg(6,r[29]);c.stackRegisters|=stack<<6;}c.pc=0x800418ec;break;
 case 0x800418ec: if(!store(r[26]+uint32_t(0),r[0]))return false;c.pc=0x800418f0;break;
 case 0x800418f0: reg(3,uint32_t(5));c.pc=0x800418f4;break;
 case 0x800418f4: reg(4,r[2]+uint32_t(-30988));c.pc=0x800418f8;break;
 case 0x800418f8: reg(5,r[2]+uint32_t(-30980));c.pc=0x800418fc;break;
 case 0x800418fc: if(!load(r[26]+uint32_t(4),r[7]))return false;c.stackRegisters&=~(1u<<7);c.pc=0x80041900;break;
 case 0x80041900: reg(0,r[7]+uint32_t(1));c.pc=0x80041904;break;
 case 0x80041904: if(!store(r[26]+uint32_t(4),r[0]))return false;c.pc=0x80041908;break;
 case 0x80041908: /* no floating varargs in the admitted source formatting requests */c.pc=0x8004190c;break;
 case 0x8004190c: if(!c.queued){c.call={};c.call.function=0x8018a61c;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8018a61c;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041910;break;
 case 0x80041910: if(!store(r[26]+uint32_t(8),r[3]))return false;c.pc=0x80041914;break;
 case 0x80041914: if(!load(r[26]+uint32_t(8),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041918;break;
 case 0x80041918: c.condition=r[3]?1:0;c.pc=0x8004191c;break;
 case 0x8004191c: if(!(c.condition==0)){c.pc=0x8004193c;break;}c.pc=0x80041920;break;
 // Null model handle OR null payload returns index-2 after clearing active
 // and decrementing refs; payload-null first calls fileFree8018a260.
 // The stale file word is preserved. No transactional whole-pose rollback.
 case 0x80041920: reg(0,uint32_t(0));c.pc=0x80041924;break;
 case 0x80041924: reg(25,uint32_t(-2));c.pc=0x80041928;break;
 case 0x80041928: if(!store(r[26]+uint32_t(0),r[0]))return false;c.pc=0x8004192c;break;
 case 0x8004192c: if(!load(r[26]+uint32_t(4),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041930;break;
 case 0x80041930: reg(0,r[3]+uint32_t(-1));c.pc=0x80041934;break;
 case 0x80041934: if(!store(r[26]+uint32_t(4),r[0]))return false;c.pc=0x80041938;break;
 case 0x80041938: if(true){c.pc=0x80041a60;break;}c.pc=0x8004193c;break;
 case 0x8004193c: if(!load(r[3]+uint32_t(160),r[4]))return false;c.stackRegisters&=~(1u<<4);c.pc=0x80041940;break;
 case 0x80041940: if(!load(r[4]+uint32_t(0),r[5]))return false;c.stackRegisters&=~(1u<<5);c.pc=0x80041944;break;
 case 0x80041944: c.condition=r[5]?1:0;c.pc=0x80041948;break;
 case 0x80041948: if(!(c.condition==0)){c.pc=0x8004196c;break;}c.pc=0x8004194c;break;
 case 0x8004194c: if(!c.queued){c.call={};c.call.function=0x8018a260;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8018a260;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041950;break;
 case 0x80041950: reg(0,uint32_t(0));c.pc=0x80041954;break;
 case 0x80041954: reg(25,uint32_t(-2));c.pc=0x80041958;break;
 case 0x80041958: if(!store(r[26]+uint32_t(0),r[0]))return false;c.pc=0x8004195c;break;
 case 0x8004195c: if(!load(r[26]+uint32_t(4),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041960;break;
 case 0x80041960: reg(0,r[3]+uint32_t(-1));c.pc=0x80041964;break;
 case 0x80041964: if(!store(r[26]+uint32_t(4),r[0]))return false;c.pc=0x80041968;break;
 case 0x80041968: if(true){c.pc=0x80041a60;break;}c.pc=0x8004196c;break;
 // sprintf8026a0b8 writes semantic stack+8: "%s/%s-" at r27+848,
 // prefix "a", loaded MODEL+68decimal(44hex). Retain text across file holds.
 // W8/Wc texture base/count:12-byte rows flags0/refs4/file8; compare actual
 // FileEntry+32 filename. A match increments texture refs and shares its row.
 case 0x8004196c: reg(3,0);c.stackRegisters|=1u<<3;c.pc=0x80041970;break;
 case 0x80041970: reg(4,r[27]+uint32_t(848));c.pc=0x80041974;break;
 case 0x80041974: reg(6,r[5]+uint32_t(68));c.pc=0x80041978;break;
 case 0x80041978: reg(5,r[2]+uint32_t(-30980));c.pc=0x8004197c;break;
 case 0x8004197c: /* no floating varargs in the admitted source formatting requests */c.pc=0x80041980;break;
 case 0x80041980: if(!c.queued){c.call={};c.call.function=0x8026a0b8;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026a0b8;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.stack=c.call.text;c.queued=false;c.pc=0x80041984;break;
 case 0x80041984: if(!load(r[13]+uint32_t(-29024),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041988;break;
 case 0x80041988: reg(22,uint32_t(0));c.pc=0x8004198c;break;
 case 0x8004198c: {const auto stack=(c.stackRegisters>>22)&1;reg(21,r[22]);c.stackRegisters|=stack<<21;}c.pc=0x80041990;break;
 case 0x80041990: if(!load(r[3]+uint32_t(12),r[23]))return false;c.stackRegisters&=~(1u<<23);c.pc=0x80041994;break;
 case 0x80041994: if(true){c.pc=0x800419e8;break;}c.pc=0x80041998;break;
 case 0x80041998: if(!load(r[13]+uint32_t(-29024),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x8004199c;break;
 case 0x8004199c: if(!load(r[3]+uint32_t(8),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800419a0;break;
 case 0x800419a0: reg(24,r[0]+r[21]);c.pc=0x800419a4;break;
 case 0x800419a4: if(!load(r[24]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x800419a8;break;
 case 0x800419a8: c.condition=r[0]?1:0;c.pc=0x800419ac;break;
 case 0x800419ac: if(c.condition==0){c.pc=0x800419e0;break;}c.pc=0x800419b0;break;
 case 0x800419b0: if(!load(r[24]+uint32_t(8),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x800419b4;break;
 case 0x800419b4: c.condition=r[3]?1:0;c.pc=0x800419b8;break;
 case 0x800419b8: if(c.condition==0){c.pc=0x800419e0;break;}c.pc=0x800419bc;break;
 case 0x800419bc: reg(3,r[3]+uint32_t(32));c.pc=0x800419c0;break;
 case 0x800419c0: reg(4,0);c.stackRegisters|=1u<<4;c.pc=0x800419c4;break;
 case 0x800419c4: if(!c.queued){c.call={};c.call.function=0x8026d464;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026d464;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x800419c8;break;
 case 0x800419c8: c.condition=(signedWord(r[3])>0)-(signedWord(r[3])<0);c.pc=0x800419cc;break;
 case 0x800419cc: if(!(c.condition==0)){c.pc=0x800419e0;break;}c.pc=0x800419d0;break;
 case 0x800419d0: if(!load(r[24]+uint32_t(4),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x800419d4;break;
 case 0x800419d4: reg(0,r[3]+uint32_t(1));c.pc=0x800419d8;break;
 case 0x800419d8: if(!store(r[24]+uint32_t(4),r[0]))return false;c.pc=0x800419dc;break;
 case 0x800419dc: if(true){c.pc=0x80041a5c;break;}c.pc=0x800419e0;break;
 case 0x800419e0: reg(21,r[21]+uint32_t(12));c.pc=0x800419e4;break;
 case 0x800419e4: reg(22,r[22]+uint32_t(1));c.pc=0x800419e8;break;
 case 0x800419e8: c.condition=(signedWord(r[22])>signedWord(r[23]))-(signedWord(r[22])<signedWord(r[23]));c.pc=0x800419ec;break;
 case 0x800419ec: if(c.condition<0){c.pc=0x80041998;break;}c.pc=0x800419f0;break;
 case 0x800419f0: if(!load(r[13]+uint32_t(-29024),r[4]))return false;c.stackRegisters&=~(1u<<4);c.pc=0x800419f4;break;
 case 0x800419f4: reg(22,uint32_t(0));c.pc=0x800419f8;break;
 case 0x800419f8: {const auto stack=(c.stackRegisters>>22)&1;reg(3,r[22]);c.stackRegisters|=stack<<3;}c.pc=0x800419fc;break;
 case 0x800419fc: if(!load(r[4]+uint32_t(12),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041a00;break;
 case 0x80041a00: c.ctr=r[0];c.pc=0x80041a04;break;
 case 0x80041a04: c.condition=(signedWord(r[0])>0)-(signedWord(r[0])<0);c.pc=0x80041a08;break;
 case 0x80041a08: if(!(c.condition>0)){c.pc=0x80041a30;break;}c.pc=0x80041a0c;break;
 case 0x80041a0c: if(!load(r[4]+uint32_t(8),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041a10;break;
 case 0x80041a10: reg(21,r[0]+r[3]);c.pc=0x80041a14;break;
 case 0x80041a14: if(!load(r[21]+uint32_t(0),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041a18;break;
 case 0x80041a18: c.condition=r[0]?1:0;c.pc=0x80041a1c;break;
 case 0x80041a1c: if(!(c.condition==0)){c.pc=0x80041a24;break;}c.pc=0x80041a20;break;
 case 0x80041a20: if(true){c.pc=0x80041a38;break;}c.pc=0x80041a24;break;
 case 0x80041a24: reg(3,r[3]+uint32_t(12));c.pc=0x80041a28;break;
 case 0x80041a28: reg(22,r[22]+uint32_t(1));c.pc=0x80041a2c;break;
 case 0x80041a2c: if(--c.ctr!=0){c.pc=0x80041a0c;break;}c.pc=0x80041a30;break;
 case 0x80041a30: reg(21,uint32_t(0));c.pc=0x80041a34;break;
 case 0x80041a34: reg(22,uint32_t(-1));c.pc=0x80041a38;break;
 // Texture miss calls fileAlloc8018a5f8(formatted stack name,type 4).
 // Unlike model-null, null TEXTURE has no matching rollback branch: continue
 // to timestamp/buffer child. Native resource adoption must reject absence.
 case 0x80041a38: reg(0,uint32_t(1));c.pc=0x80041a3c;break;
 case 0x80041a3c: reg(3,0);c.stackRegisters|=1u<<3;c.pc=0x80041a40;break;
 case 0x80041a40: if(!store(r[21]+uint32_t(0),r[0]))return false;c.pc=0x80041a44;break;
 case 0x80041a44: reg(4,uint32_t(4));c.pc=0x80041a48;break;
 case 0x80041a48: if(!load(r[21]+uint32_t(4),r[5]))return false;c.stackRegisters&=~(1u<<5);c.pc=0x80041a4c;break;
 case 0x80041a4c: reg(0,r[5]+uint32_t(1));c.pc=0x80041a50;break;
 case 0x80041a50: if(!store(r[21]+uint32_t(4),r[0]))return false;c.pc=0x80041a54;break;
 case 0x80041a54: if(!c.queued){c.call={};c.call.function=0x8018a5f8;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8018a5f8;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041a58;break;
 case 0x80041a58: if(!store(r[21]+uint32_t(8),r[3]))return false;c.pc=0x80041a5c;break;
 case 0x80041a5c: if(!store(r[26]+uint32_t(12),r[22]))return false;c.pc=0x80041a60;break;
 case 0x80041a60: if(!store(r[31]+uint32_t(16),r[25]))return false;c.pc=0x80041a64;break;
 case 0x80041a64: if(!load(r[31]+uint32_t(16),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041a68;break;
 case 0x80041a68: c.condition=(signedWord(r[0])>-2)-(signedWord(r[0])<-2);c.pc=0x80041a6c;break;
 case 0x80041a6c: if(!(c.condition==0)){c.pc=0x80041a80;break;}c.pc=0x80041a70;break;
 case 0x80041a70: reg(0,uint32_t(0));c.pc=0x80041a74;break;
 case 0x80041a74: reg(3,uint32_t(-2));c.pc=0x80041a78;break;
 case 0x80041a78: if(!store(r[31]+uint32_t(0),r[0]))return false;c.pc=0x80041a7c;break;
 case 0x80041a7c: if(true){c.pc=0x80041c44;break;}c.pc=0x80041a80;break;
 // P14 animation=0. Bus800000f8 >>2, multiply-high by10624dd3
 // (4194<<16 +19923), then >>6 gives quarter-bus ticks/millisecond.
 // GP40/44 for mode0, GP38/3c otherwise; __div2i80264ad8 is signed 64.
 // P1c LOW is stored before P18 HIGH, preserving hold/read ordering.
 case 0x80041a80: reg(0,uint32_t(0));c.pc=0x80041a84;break;
 case 0x80041a84: c.condition=(signedWord(r[28])>0)-(signedWord(r[28])<0);c.pc=0x80041a88;break;
 case 0x80041a88: if(!store(r[31]+uint32_t(20),r[0]))return false;c.pc=0x80041a8c;break;
 case 0x80041a8c: if(!(c.condition==0)){c.pc=0x80041ac4;break;}c.pc=0x80041a90;break;
 case 0x80041a90: reg(3,uint32_t(-32768)<<16);c.pc=0x80041a94;break;
 case 0x80041a94: reg(4,uint32_t(4194)<<16);c.pc=0x80041a98;break;
 case 0x80041a98: if(!load(r[3]+uint32_t(248),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041a9c;break;
 case 0x80041a9c: reg(5,r[4]+uint32_t(19923));c.pc=0x80041aa0;break;
 case 0x80041aa0: if(!load(r[13]+uint32_t(-28496),r[6]))return false;c.stackRegisters&=~(1u<<6);c.pc=0x80041aa4;break;
 case 0x80041aa4: reg(0,r[0]>>2);c.pc=0x80041aa8;break;
 case 0x80041aa8: reg(0,uint32_t((uint64_t(r[5])*r[0])>>32));c.pc=0x80041aac;break;
 case 0x80041aac: if(!load(r[6]+uint32_t(64),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041ab0;break;
 case 0x80041ab0: if(!load(r[6]+uint32_t(68),r[4]))return false;c.stackRegisters&=~(1u<<4);c.pc=0x80041ab4;break;
 case 0x80041ab4: reg(5,uint32_t(0));c.pc=0x80041ab8;break;
 case 0x80041ab8: reg(6,r[0]>>6);c.pc=0x80041abc;break;
 case 0x80041abc: if(!c.queued){c.call={};c.call.function=0x80264ad8;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x80264ad8;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);reg(4,c.call.low);c.queued=false;c.pc=0x80041ac0;break;
 case 0x80041ac0: if(true){c.pc=0x80041af4;break;}c.pc=0x80041ac4;break;
 case 0x80041ac4: reg(3,uint32_t(-32768)<<16);c.pc=0x80041ac8;break;
 case 0x80041ac8: reg(4,uint32_t(4194)<<16);c.pc=0x80041acc;break;
 case 0x80041acc: if(!load(r[3]+uint32_t(248),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041ad0;break;
 case 0x80041ad0: reg(5,r[4]+uint32_t(19923));c.pc=0x80041ad4;break;
 case 0x80041ad4: if(!load(r[13]+uint32_t(-28496),r[6]))return false;c.stackRegisters&=~(1u<<6);c.pc=0x80041ad8;break;
 case 0x80041ad8: reg(0,r[0]>>2);c.pc=0x80041adc;break;
 case 0x80041adc: reg(0,uint32_t((uint64_t(r[5])*r[0])>>32));c.pc=0x80041ae0;break;
 case 0x80041ae0: if(!load(r[6]+uint32_t(56),r[3]))return false;c.stackRegisters&=~(1u<<3);c.pc=0x80041ae4;break;
 case 0x80041ae4: if(!load(r[6]+uint32_t(60),r[4]))return false;c.stackRegisters&=~(1u<<4);c.pc=0x80041ae8;break;
 case 0x80041ae8: reg(5,uint32_t(0));c.pc=0x80041aec;break;
 case 0x80041aec: reg(6,r[0]>>6);c.pc=0x80041af0;break;
 case 0x80041af0: if(!c.queued){c.call={};c.call.function=0x80264ad8;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x80264ad8;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);reg(4,c.call.low);c.queued=false;c.pc=0x80041af4;break;
 // ZERO at SDA2-31000=8041fb28. Initialize cache3c=-1,fraction40=0,
 // discrete80=0,ratio84=0 and float70/74/7c=0; modec=input,link90=-1.
 // AllocBuffer80041ee0 takes POSE POINTER, not numeric handle.
 case 0x80041af4: if(!store(r[31]+uint32_t(28),r[4]))return false;c.pc=0x80041af8;break;
 case 0x80041af8: reg(4,uint32_t(-1));c.pc=0x80041afc;break;
 case 0x80041afc: if(!load(r[2]+uint32_t(-31000),c.f[0]))return false;c.pc=0x80041b00;break;
 case 0x80041b00: reg(0,uint32_t(0));c.pc=0x80041b04;break;
 case 0x80041b04: if(!store(r[31]+uint32_t(24),r[3]))return false;c.pc=0x80041b08;break;
 case 0x80041b08: {const auto stack=(c.stackRegisters>>31)&1;reg(3,r[31]);c.stackRegisters|=stack<<3;}c.pc=0x80041b0c;break;
 case 0x80041b0c: if(!store(r[31]+uint32_t(60),r[4]))return false;c.pc=0x80041b10;break;
 case 0x80041b10: if(!store(r[31]+uint32_t(64),c.f[0]))return false;c.pc=0x80041b14;break;
 case 0x80041b14: if(!store(r[31]+uint32_t(128),r[0]))return false;c.pc=0x80041b18;break;
 case 0x80041b18: if(!store(r[31]+uint32_t(132),c.f[0]))return false;c.pc=0x80041b1c;break;
 case 0x80041b1c: if(!store(r[31]+uint32_t(112),c.f[0]))return false;c.pc=0x80041b20;break;
 case 0x80041b20: if(!store(r[31]+uint32_t(116),c.f[0]))return false;c.pc=0x80041b24;break;
 case 0x80041b24: if(!store(r[31]+uint32_t(124),c.f[0]))return false;c.pc=0x80041b28;break;
 case 0x80041b28: if(!store(r[31]+uint32_t(12),r[28]))return false;c.pc=0x80041b2c;break;
 case 0x80041b2c: if(!store(r[31]+uint32_t(144),r[4]))return false;c.pc=0x80041b30;break;
 case 0x80041b30: if(!c.queued){c.call={};c.call.function=0x80041ee0;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x80041ee0;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041b34;break;
 // After buffer child: E0/E4/E8/EC=0; colors F0/F4 use source words
 // fb0c/fb10; scalesFC/F8=ONE at fb24;100=ZERO,rate44=ONE.
 // PSMTXIdentity8029dd04 writes inline P124 (292decimal).
 // Unmentioned stale fields survive; there is deliberately no pose memset.
 case 0x80041b34: reg(0,uint32_t(0));c.pc=0x80041b38;break;
 case 0x80041b38: if(!load(r[2]+uint32_t(-31004),c.f[1]))return false;c.pc=0x80041b3c;break;
 case 0x80041b3c: if(!store(r[31]+uint32_t(224),r[0]))return false;c.pc=0x80041b40;break;
 case 0x80041b40: reg(3,r[31]+uint32_t(292));c.pc=0x80041b44;break;
 case 0x80041b44: if(!load(r[2]+uint32_t(-31000),c.f[0]))return false;c.pc=0x80041b48;break;
 case 0x80041b48: if(!store(r[31]+uint32_t(228),r[0]))return false;c.pc=0x80041b4c;break;
 case 0x80041b4c: if(!store(r[31]+uint32_t(232),r[0]))return false;c.pc=0x80041b50;break;
 case 0x80041b50: if(!store(r[31]+uint32_t(236),r[0]))return false;c.pc=0x80041b54;break;
 case 0x80041b54: if(!load(r[2]+uint32_t(-31028),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041b58;break;
 case 0x80041b58: if(!store(r[31]+uint32_t(240),r[0]))return false;c.pc=0x80041b5c;break;
 case 0x80041b5c: if(!load(r[2]+uint32_t(-31024),r[0]))return false;c.stackRegisters&=~(1u<<0);c.pc=0x80041b60;break;
 case 0x80041b60: if(!store(r[31]+uint32_t(244),r[0]))return false;c.pc=0x80041b64;break;
 case 0x80041b64: if(!store(r[31]+uint32_t(252),c.f[1]))return false;c.pc=0x80041b68;break;
 case 0x80041b68: if(!store(r[31]+uint32_t(248),c.f[1]))return false;c.pc=0x80041b6c;break;
 case 0x80041b6c: if(!store(r[31]+uint32_t(256),c.f[0]))return false;c.pc=0x80041b70;break;
 case 0x80041b70: if(!store(r[31]+uint32_t(68),c.f[1]))return false;c.pc=0x80041b74;break;
 case 0x80041b74: if(!c.queued){c.call={};c.call.function=0x8029dd04;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8029dd04;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041b78;break;
 // Four 12-byte sort names at802c11a8,b4,c0,cc select kinds 1/1/2/3
 // and live tail pointers80415da0/a4/a8/ac. Default kind 0 owner80415dc4.
 // Publish P11c/P120, then clear callback154/data158 and return pose index.
 case 0x80041b78: {const auto stack=(c.stackRegisters>>29)&1;reg(3,r[29]);c.stackRegisters|=stack<<3;}c.pc=0x80041b7c;break;
 case 0x80041b7c: reg(4,r[27]+uint32_t(1168));c.pc=0x80041b80;break;
 case 0x80041b80: if(!c.queued){c.call={};c.call.function=0x8026d464;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026d464;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041b84;break;
 case 0x80041b84: c.condition=(signedWord(r[3])>0)-(signedWord(r[3])<0);c.pc=0x80041b88;break;
 case 0x80041b88: if(!(c.condition==0)){c.pc=0x80041ba0;break;}c.pc=0x80041b8c;break;
 case 0x80041b8c: reg(3,uint32_t(1));c.pc=0x80041b90;break;
 case 0x80041b90: reg(0,r[13]+uint32_t(-29056));c.pc=0x80041b94;break;
 case 0x80041b94: if(!store(r[31]+uint32_t(284),r[3]))return false;c.pc=0x80041b98;break;
 case 0x80041b98: if(!store(r[31]+uint32_t(288),r[0]))return false;c.pc=0x80041b9c;break;
 case 0x80041b9c: if(true){c.pc=0x80041c34;break;}c.pc=0x80041ba0;break;
 case 0x80041ba0: {const auto stack=(c.stackRegisters>>29)&1;reg(3,r[29]);c.stackRegisters|=stack<<3;}c.pc=0x80041ba4;break;
 case 0x80041ba4: reg(4,r[27]+uint32_t(1180));c.pc=0x80041ba8;break;
 case 0x80041ba8: if(!c.queued){c.call={};c.call.function=0x8026d464;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026d464;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041bac;break;
 case 0x80041bac: c.condition=(signedWord(r[3])>0)-(signedWord(r[3])<0);c.pc=0x80041bb0;break;
 case 0x80041bb0: if(!(c.condition==0)){c.pc=0x80041bcc;break;}c.pc=0x80041bb4;break;
 case 0x80041bb4: reg(0,uint32_t(1));c.pc=0x80041bb8;break;
 case 0x80041bb8: reg(3,r[13]+uint32_t(-29056));c.pc=0x80041bbc;break;
 case 0x80041bbc: if(!store(r[31]+uint32_t(284),r[0]))return false;c.pc=0x80041bc0;break;
 case 0x80041bc0: reg(0,r[3]+uint32_t(4));c.pc=0x80041bc4;break;
 case 0x80041bc4: if(!store(r[31]+uint32_t(288),r[0]))return false;c.pc=0x80041bc8;break;
 case 0x80041bc8: if(true){c.pc=0x80041c34;break;}c.pc=0x80041bcc;break;
 case 0x80041bcc: {const auto stack=(c.stackRegisters>>29)&1;reg(3,r[29]);c.stackRegisters|=stack<<3;}c.pc=0x80041bd0;break;
 case 0x80041bd0: reg(4,r[27]+uint32_t(1192));c.pc=0x80041bd4;break;
 case 0x80041bd4: if(!c.queued){c.call={};c.call.function=0x8026d464;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026d464;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041bd8;break;
 case 0x80041bd8: c.condition=(signedWord(r[3])>0)-(signedWord(r[3])<0);c.pc=0x80041bdc;break;
 case 0x80041bdc: if(!(c.condition==0)){c.pc=0x80041bf8;break;}c.pc=0x80041be0;break;
 case 0x80041be0: reg(0,uint32_t(2));c.pc=0x80041be4;break;
 case 0x80041be4: reg(3,r[13]+uint32_t(-29056));c.pc=0x80041be8;break;
 case 0x80041be8: if(!store(r[31]+uint32_t(284),r[0]))return false;c.pc=0x80041bec;break;
 case 0x80041bec: reg(0,r[3]+uint32_t(8));c.pc=0x80041bf0;break;
 case 0x80041bf0: if(!store(r[31]+uint32_t(288),r[0]))return false;c.pc=0x80041bf4;break;
 case 0x80041bf4: if(true){c.pc=0x80041c34;break;}c.pc=0x80041bf8;break;
 case 0x80041bf8: {const auto stack=(c.stackRegisters>>29)&1;reg(3,r[29]);c.stackRegisters|=stack<<3;}c.pc=0x80041bfc;break;
 case 0x80041bfc: reg(4,r[27]+uint32_t(1204));c.pc=0x80041c00;break;
 case 0x80041c00: if(!c.queued){c.call={};c.call.function=0x8026d464;for(unsigned i=0;i<5;++i)c.call.args[i]=r[3+i];c.call.stackArguments=(c.stackRegisters>>3)&31;if(c.call.stackArguments)c.call.text=c.stack;c.queued=true;}
   c.pending=0x8026d464;if(!s.call || !s.call(s.memory.user,c.call))return false;
   reg(3,c.call.result);c.queued=false;c.pc=0x80041c04;break;
 case 0x80041c04: c.condition=(signedWord(r[3])>0)-(signedWord(r[3])<0);c.pc=0x80041c08;break;
 case 0x80041c08: if(!(c.condition==0)){c.pc=0x80041c24;break;}c.pc=0x80041c0c;break;
 case 0x80041c0c: reg(0,uint32_t(3));c.pc=0x80041c10;break;
 case 0x80041c10: reg(3,r[13]+uint32_t(-29056));c.pc=0x80041c14;break;
 case 0x80041c14: if(!store(r[31]+uint32_t(284),r[0]))return false;c.pc=0x80041c18;break;
 case 0x80041c18: reg(0,r[3]+uint32_t(12));c.pc=0x80041c1c;break;
 case 0x80041c1c: if(!store(r[31]+uint32_t(288),r[0]))return false;c.pc=0x80041c20;break;
 case 0x80041c20: if(true){c.pc=0x80041c34;break;}c.pc=0x80041c24;break;
 case 0x80041c24: reg(3,uint32_t(0));c.pc=0x80041c28;break;
 case 0x80041c28: reg(0,r[13]+uint32_t(-29020));c.pc=0x80041c2c;break;
 case 0x80041c2c: if(!store(r[31]+uint32_t(284),r[3]))return false;c.pc=0x80041c30;break;
 case 0x80041c30: if(!store(r[31]+uint32_t(288),r[0]))return false;c.pc=0x80041c34;break;
 case 0x80041c34: reg(0,uint32_t(0));c.pc=0x80041c38;break;
 case 0x80041c38: {const auto stack=(c.stackRegisters>>30)&1;reg(3,r[30]);c.stackRegisters|=stack<<3;}c.pc=0x80041c3c;break;
 case 0x80041c3c: if(!store(r[31]+uint32_t(340),r[0]))return false;c.pc=0x80041c40;break;
 case 0x80041c40: if(!store(r[31]+uint32_t(344),r[0]))return false;c.pc=0x80041c44;break;
 case 0x80041c44:c.result=r[3];c.active=false;c.pending=0;return true;
 default:return false;
 }}}
}
