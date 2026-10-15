#pragma once
#include "positional_sfx_registry.hpp"
// Native handles mirror source Mario fields27c/280/284;-1 means absent.
// post is cleanup for the OLD motion before transition, not a new-motion On.
namespace ttyd {
// MarioInit/ReInit explicit -1 stores at27c/280/284.
struct MarioMotionSfx {
 int32_t idle=-1,secondary=-1,fall=-1;
 bool post(unsigned oldMotion,PositionalSfxRegistry&,const PositionalSfxStopServices&);
};
}
