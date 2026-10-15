#pragma once
#include "field_jump.hpp"
#include "field_movement.hpp"
#include "mario_presentation.hpp"
namespace ttyd {
struct FieldItemMarioPost {
 void* user=nullptr;
 void (*call)(void*,unsigned previousMotion)=nullptr;
};
// Finite ordinary motion table rows. Missing nonnull exit providers refuse
// before mutation; special motion22/main00100000 need separate providers.
// Motion15 is item-get, 21/25/26 special admitted rows, 22 unsupported get.
// Main10000000 bypasses ordinary transition; main00100000 needs a provider
// for alternate heading aliases. These are source masks, not render state.
bool fieldItemMarioGetAvailable(const FieldJump&,uint32_t mainFlags,const FieldItemMarioPost&);
bool requestFieldItemMarioMotion(FieldJump&,FieldMovement&,uint32_t mainFlags,
                                MarioPresentation&,bool get,const FieldItemMarioPost&);
bool enterFieldItemMarioMotion(FieldJump&,FieldMovement&,uint32_t mainFlags,MarioPresentation&);
}
