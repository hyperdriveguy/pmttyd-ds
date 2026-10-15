#include "battle_field_initialize.hpp"
namespace ttyd {
bool initializeFieldBattle(const BattleSequenceEntryServices& s,BattleFieldInitializeCursor& c){
 if(!c.active){c={};c.active=true;c.pending=0x800738ac;}
 auto load=[&](uint32_t a,uint32_t& out){auto n=s.word?s.word(s.user,a):std::nullopt;if(!n)return false;out=*n;return true;};
 auto byte=[&](uint32_t a,uint32_t& out){auto n=s.byte?s.byte(s.user,a):std::nullopt;if(!n)return false;out=*n;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 auto call=[&](uint32_t fn,std::array<uint32_t,5> args=std::array<uint32_t,5>{}){if(!c.queued){c.call={};c.call.function=fn;c.call.args=args;c.queued=true;}c.pending=fn;if(!s.call || !s.call(s.user,c.call))return false;c.queued=false;return true;};
 for(;;)switch(c.phase){
 case 0:if(!load(0x80415fd0,c.gp))return false;++c.phase;break;
 case 1:c.pending=0x8004829c;if(!load(c.gp+0x168,c.field))return false;c.information=c.field+0x20;++c.phase;break;
 case 2:if(!load(c.field+8,c.npc))return false;++c.phase;break;
 case 3:if(!call(0x800d5eb0))return false;c.pouch=c.call.result;++c.phase;break;
 case 4:if(!call(0x8000e95c,{4,2}))return false;++c.phase;break;
 case 5:if(!load(0x80415fd0,c.gp))return false;++c.phase;break;
 case 6:if(!load(c.gp+0xc,c.value))return false;c.phase=c.value?7:9;break;
 case 7:if(!call(0x80009d90))return false;c.value=c.call.result?c.call.result:0x802c2abc;++c.phase;break;
 case 8:if(!call(0x8008465c,{c.value,0}))return false;++c.phase;break;
 case 9:if(!call(0x80030078))return false;++c.phase;break;
 case 10:if(!store(0x8041eb90,0))return false;++c.phase;break;
 case 11:if(!call(0x80041c58))return false;++c.phase;break;
 case 12:if(!call(0x8004ce58))return false;++c.phase;break;
 case 13:if(!load(0x80415fd0,c.gp))return false;++c.phase;break;
 case 14:if(!store(c.gp+0x14,1))return false;++c.phase;break;
 case 15:if(!call(0x800377b0))return false;++c.phase;break;
 case 16:if(!call(0x800dc110,{0x10}))return false;++c.phase;break;
 case 17:if(!call(0x80114518,{c.information}))return false;++c.phase;break;
 case 18:if(!call(0x80114598,{c.information,0}))return false;++c.phase;break;
 case 19:if(!call(0x80141050))return false;c.value=c.call.result;++c.phase;break;
 case 20:if(!call(0x800bf1b4,{c.value}))return false;c.party=c.call.result;c.phase=c.party?21:25;break;
 case 21:if(!byte(c.party+0x31,c.value))return false;c.kind=c.value<128?c.value:c.value-256;++c.phase;break;
 case 22:if(!call(0x800f83e4,{c.kind}))return false;c.value=c.call.result;++c.phase;break;
 case 23:{auto n=s.half?s.half(s.user,c.pouch+14*c.kind):std::nullopt;if(!n)return false;c.phase=(*n&2)?25:24;break;}
 case 24:if(!call(0x80114590,{c.information,c.value}))return false;++c.phase;break;
 case 25:if(!load(c.field+0x1c,c.value))return false;c.phase=(c.value&1)?28:26;if(c.value&1)c.value=0;break;
 case 26:if(!load(c.field+0x10,c.value))return false;
  switch(c.value){case 0x20000:c.value=1;break;case 0x40000:c.value=2;break;case 0x80000:c.value=3;break;case 0x100000:c.value=4;break;case 0x200000:c.value=5;break;case 0x400000:c.value=6;break;case 0x800000:c.value=7;break;case 0x1000000:c.value=8;break;case 0x10000000:c.phase=27;continue;default:c.value=0;break;}c.phase=28;break;
 case 27:if(!load(c.field+0x14,c.value))return false;++c.phase;break;
 case 28:if(!call(0x80114588,{c.information,c.value}))return false;++c.phase;break;
 case 29:if(!call(0x80114510,{c.information,c.npc+0x230}))return false;++c.phase;break;
 case 30:case 32:case 34:if(!byte(c.npc+0x2f4+(c.phase-30)/2,c.value))return false;++c.phase;break;
 case 31:case 33:case 35:if(!s.storeByte || !s.storeByte(s.user,c.information+(c.phase==31?0x18:c.phase==33?0x1a:0x1b),uint8_t(c.value)))return false;++c.phase;break;
 case 36:if(!call(0x800f8f04,{c.information}))return false;c.active=false;c.pending=0;return true;
 default:return false;
 }
}
}
