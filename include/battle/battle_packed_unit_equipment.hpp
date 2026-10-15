#pragma once
#include "battle_packed_unit_resources.hpp"
// Mode bits are source helper flags: 1 clears40 badge-counter bytes; 2/4
// select the full equipped pouch list. equipOne owns nonzero item semantics.
namespace ttyd {
struct PackedUnitEquipmentServices {
 PackedUnitResourceServices resources;void* user=nullptr;
 void (*equipOne)(void*,uint32_t unit,int32_t mode,int32_t sourceItem)=nullptr;
};
// Complete BtlUnit_EquipItem800f831c outer/reset/list/ReviseHpFp ownership.
// EquipOne provider retains the original helper800f7ca4; no badge is inferred.
void equipPackedUnitItems(uint32_t unit,int32_t mode,int32_t item,const PackedUnitEquipmentServices&);
// Original helper's first cmpwi(item,0)/beqlr route, before any unit access.
// false means another original badge branch still needs its service.
bool equipPackedNoItem(int32_t sourceItem);
}
