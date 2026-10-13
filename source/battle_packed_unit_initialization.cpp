#include "battle_packed_unit_initialization.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
void changeInfo(uint32_t u,const EventMemoryServices& m){for(unsigned i=0;i<6;++i){const auto r=u+0xae8+12*i;m.storeByte(m.user,r,255);for(unsigned j=1;j<4;++j)m.storeByte(m.user,r+j,0);m.storeWord(m.user,r+4,0);}}
void real(uint32_t a,float v,const EventMemoryServices& m){uint32_t bits;std::memcpy(&bits,&v,4);m.storeWord(m.user,a,bits);}
}
bool initializePackedBattleUnitHelper(uint32_t function,uint32_t u,const BattleActorServices& s){
 const auto& m=s.memory;
 switch(function){
 case 0x8019673c:return true; // BattleStatusEffectInit is exactly blr.
 case 0x80253e84:assert(m.storeByte);for(unsigned i=0;i<0x7a0;++i)m.storeByte(m.user,u+0x348+i,0);return true;
 case 0x80195d34:assert(m.storeByte && m.storeWord);changeInfo(u,m);return true;
 case 0x80127b58:{assert(m.storeByte && m.storeWord && s.byte);for(unsigned i=0;i<30;++i)m.storeByte(m.user,u+0x118+i,0);
  // Exact BtlUnit_SetStatus(unit,16,1,-99) branch: accumulated signed byte,
  // negative→0, upper clamp99. No movement/reset branch exists for type16.
  auto value=uint8_t(uint32_t(s.byte(s.user,u+0x129))+uint32_t(-99));if(value>=128)value=0;else if(value>99)value=99;m.storeByte(m.user,u+0x129,value);changeInfo(u,m);return true;}
 case 0x80125fd8:{assert(m.half && m.storeHalf && m.storeWord);m.storeHalf(m.user,u+0x200,0);m.storeHalf(m.user,u+0x204,0);m.storeHalf(m.user,u+0x1fc,0);
  m.storeWord(m.user,u+0x208,uint32_t(int32_t(int16_t(m.half(m.user,u+0x10c)))));m.storeWord(m.user,u+0x20c,uint32_t(int32_t(int16_t(m.half(m.user,u+0x10c)))));
  const auto ratio=float(int16_t(m.half(m.user,u+0x10c)))/float(int16_t(m.half(m.user,u+0x108)));real(u+0x210,ratio,m);real(u+0x214,ratio,m);
  m.storeHalf(m.user,u+0x202,m.half(m.user,u+0x1fc)==1?40:20);m.storeHalf(m.user,u+0x206,uint16_t(int32_t(int16_t(m.half(m.user,u+0x202)))+30));return true;}
 default:return false;
 }
}
}
