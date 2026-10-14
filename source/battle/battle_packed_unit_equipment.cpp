#include "battle_packed_unit_equipment.hpp"
#include <cassert>
namespace ttyd {
void equipPackedUnitItems(uint32_t u,int32_t mode,int32_t item,const PackedUnitEquipmentServices& s){const auto& m=s.resources.memory.memory;assert(s.resources.pouch && m.word && m.half && m.storeByte);const auto p=s.resources.pouch(s.resources.user);if(!u)return;
 if(uint32_t(mode)&1)for(unsigned i=0;i<40;++i)m.storeByte(m.user,u+0x2e0+i,0);
 if(m.word(m.user,u+8)==220)return;
 assert(s.equipOne);if(uint32_t(mode)&6){const auto retained=int32_t(uint32_t(mode)&~1u);for(unsigned i=0;i<200;++i)s.equipOne(s.user,u,retained,int32_t(int16_t(m.half(m.user,p+0x38a+2*i))));}else s.equipOne(s.user,u,mode,item);
 revisePackedUnitResources(u,s.resources);
}
bool equipPackedNoItem(int32_t item){return item==0;}
}
