#include "battle_packed_unit_vectors.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
bool setPackedUnitVector(uint32_t function,uint32_t entity,const std::array<float,3>& values,const PackedUnitVectorServices& s){
 uint32_t offset=0;bool sentinel=false,revise=false,clamp=false;
 switch(function){
 case 0x80128c6c:offset=0x3c;break;
 case 0x801289a0:offset=0x30;break;
 case 0x801283d8:offset=0x48;sentinel=true;break;
 case 0x80128680:offset=0x60;revise=true;break;
 case 0x801288b4:offset=0x6c;revise=true;clamp=true;break;
 case 0x80128604:offset=0x78;sentinel=true;break;
 case 0x80128524:offset=0x90;sentinel=true;break;
 case 0x80128574:offset=0x9c;sentinel=true;break;
 case 0x801284a4:offset=0x6c;sentinel=true;break;
 case 0x80128470:offset=0x60;sentinel=true;break;
 case 0x80128654:offset=0x3c;break;
 case 0x801287b4:offset=0x48;break;
 case 0x801285d0:offset=0x54;sentinel=true;break;
 default:return false;
 }
 const auto& m=s.memory;assert(m.word && m.storeWord);auto real=[&](uint32_t a){const auto bits=m.word(m.user,a);float value;std::memcpy(&value,&bits,4);return value;};
 for(unsigned axis=0;axis<3;++axis){auto value=values[axis];if(sentinel && value==real(0x804226c4))continue;if(revise){assert(s.reviseAngle);value=s.reviseAngle(s.user,value);if(clamp && value>=real(0x804226c8))value=real(0x804226a4);}uint32_t bits;std::memcpy(&bits,&value,4);m.storeWord(m.user,entity+offset+4*axis,bits);}
 return true;
}
}
