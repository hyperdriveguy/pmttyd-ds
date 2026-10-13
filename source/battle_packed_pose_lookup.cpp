#include "battle_packed_pose_lookup.hpp"
#include <cassert>
namespace ttyd {
namespace {int32_t signWord(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}}
uint32_t packedBattlePoseName(uint32_t part,int32_t type,const EventMemoryServices& m){assert(m.word);auto table=m.word(m.user,part+0x1bc);const int32_t fallback=type<=28?28:type<40?39:69;if(type>=70)return 0;
 while(table){const auto tag=signWord(m.word(m.user,table));if(tag>=70)return 0;if(tag==type || tag==fallback)return m.word(m.user,table+4);table+=8;}
 return 0;
}
}
