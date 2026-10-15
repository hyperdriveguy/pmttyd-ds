#include "field_door_parameter.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
// Resolve record/selector EVT operands first; copy direct fields as signed
// bit-preserving words. Selectors map(offset, count): 0(4, 1), 1(c, 3), 2(18, 2),
// 3(20, 2), 6(28, 3), 7(40, 2), 8(8, 1), 9(0, 1), offsets hex.
// Geometry selectors 4/5/10/11 return0 hold; unsigned selector>11 returns2
// without record access. Each output reads destination BEFORE record word.
int32_t fieldDoorParameterEvent(EventEntry& e,const FieldDoorParameterServices& s) {
 assert(s.word && s.value && s.setValue);
 const auto args=e.arguments;
 const auto record=uint32_t(s.value(s.user,e,signedWord(s.word(s.user,args))));
 const auto selector=uint32_t(s.value(s.user,e,signedWord(s.word(s.user,args+4))));
 if(selector>11)return 2; // Original unsigned range check; no record access.
 uint32_t offset=0,count=1;
 switch(selector) {
 case 0:offset=4;break;
 case 1:offset=0x0c;count=3;break;
 case 2:offset=0x18;count=2;break;
 case 3:offset=0x20;count=2;break;
 case 6:offset=0x28;count=3;break;
 case 7:offset=0x40;count=2;break;
 case 8:offset=8;break;
 case 9:offset=0;break;
 default:return 0; // Required door_position_sub geometry/numeric provider absent.
 }
 if(s.admit && !s.admit(s.user,record,selector))return 0;
 for(uint32_t n=0;n<count;++n) {
  const auto destination=signedWord(s.word(s.user,args+8+4*n));
  const auto value=signedWord(s.word(s.user,record+offset+4*n));
  s.setValue(s.user,e,destination,value);
 }
 return 2;
}
}
