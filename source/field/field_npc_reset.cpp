#include "field_npc_reset.hpp"
#include <cassert>
namespace ttyd {
// Source work8041e8b4, bank+14(hex), capacity+4, array+c, stride340.
// Selection is the raw argument, not live GP battleContext. After clear/store,
// ordinary path freshly resolves GP+168 to mode halfword; global e8ac=0 last.
void resetPackedNpcRegistry(int32_t selector,const PackedNpcResetServices& s){
 const auto& m=s.memory;assert(m.word && m.storeWord && m.storeHalf && s.clear);
 const uint32_t registry=0x8041e8b4+(selector?0x14u:0u);
 const auto capacity=m.word(m.user,registry+4),array=m.word(m.user,registry+12);
 s.clear(s.user,array,capacity*0x340u);
 m.storeWord(m.user,registry,0);
 if(!selector){const auto gp=m.word(m.user,0x80415fd0),field=m.word(m.user,gp+0x168);m.storeHalf(m.user,field,0);}
 m.storeWord(m.user,0x8041e8ac,0);
}
}
