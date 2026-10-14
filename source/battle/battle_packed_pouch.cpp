#include "battle_packed_pouch.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
uint32_t initializePackedPouch(const PackedPouchInitializationServices& s){
 assert(s.allocate && s.message && s.memory.storeWord && s.memory.storeHalf && s.memory.storeByte);
 const auto pouch=s.allocate(s.user,0,0x5d4);assert(pouch);
 s.memory.storeWord(s.memory.user,0x8041eb00,pouch);
 // The full original memset precedes the explicit halfword loops.
 for(unsigned i=0;i<0x5d4;++i)s.memory.storeByte(s.memory.user,pouch+i,0);
 for(unsigned i=0;i<121;++i)s.memory.storeHalf(s.memory.user,pouch+0xa0+2*i,0);
 for(unsigned i=0;i<20;++i)s.memory.storeHalf(s.memory.user,pouch+0x192+2*i,0);
 for(unsigned i=0;i<32;++i)s.memory.storeHalf(s.memory.user,pouch+0x1ba+2*i,0);
 for(unsigned i=0;i<200;++i)s.memory.storeHalf(s.memory.user,pouch+0x1fa+2*i,0);
 for(unsigned i=0;i<200;++i)s.memory.storeHalf(s.memory.user,pouch+0x38a+2*i,0);
 auto half=[&](unsigned at,uint16_t v){s.memory.storeHalf(s.memory.user,pouch+at,v);};
 half(0x72,10);half(0x8e,10);half(0x76,5);half(0x90,5);half(0x92,3);half(0x94,3);
 half(0x70,10);half(0x74,5);half(0x7a,0);half(0x7e,0);half(0x7c,0);half(0x80,0);
 s.memory.storeWord(s.memory.user,pouch+0x84,0); // Source float0 at8042187c.
 half(0x88,0);half(0x8a,1);s.memory.storeByte(s.memory.user,pouch+0x98,0);s.memory.storeByte(s.memory.user,pouch+0x99,0);
 for(unsigned partner=1;partner<8;++partner){const unsigned at=partner*14;half(at,0);half(at+4,10);half(at+2,10);half(at+6,10);half(at+10,0);half(at+12,0);}
 const char* name=s.message(s.user,"name_party3");assert(name);
 // Native adapter rejects an oversized localized name rather than allowing a
 // write past source allocation; valid original US text follows strcpy order.
 const auto length=std::strlen(name);assert(length<0x14);
 for(unsigned i=0;i<=length;++i)s.memory.storeByte(s.memory.user,pouch+0x5c0+i,uint8_t(name[i]));
 return pouch;
}
int32_t packedPouchEquippedBadgeCount(uint32_t pouch,int32_t badge,const EventMemoryServices& s){
 assert(s.half);int32_t count=0;
 for(unsigned i=0;i<200;++i){const uint16_t raw=s.half(s.user,pouch+0x38a+2*i);const int32_t value=raw<0x8000?int32_t(raw):int32_t(raw)-65536;if(value==badge)++count;}
 return count;
}
}
