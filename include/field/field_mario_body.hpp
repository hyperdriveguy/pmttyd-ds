#pragma once
#include "field_movement.hpp"
namespace ttyd {
// Bounded ordinary MarioInit adaptation: form memset0, collision20/37,
// walk/dash1/2.25 and marioResetGravity multiplier1. Not full MarioInit.
struct FieldMarioBody {
 uint8_t form=0;
 float width=20,height=37,walkBase=1,dashBase=2.25f,multiplier=1;
 FieldMovementRates groundRates(uint32_t main) const {
  const float factor=(main&0x2000000)?0.5f:1.f;
  return {(walkBase*factor)*multiplier*.01f,(dashBase*factor)*multiplier*.01f};
 }
 float walkThreshold() const{return walkBase*.01f;}
 float airCap() const{return (dashBase*multiplier)*.01f;}
 bool ordinary(uint32_t main) const{return form==0 && !(main&0x4100000) && multiplier==1;}
};
}
