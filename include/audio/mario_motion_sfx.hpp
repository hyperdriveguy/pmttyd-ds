#pragma once
#include "positional_sfx_registry.hpp"
namespace ttyd {
// MarioInit/ReInit explicit -1 stores at27c/280/284.
struct MarioMotionSfx {
 int32_t idle=-1,secondary=-1,fall=-1;
 bool post(unsigned oldMotion,PositionalSfxRegistry&,const PositionalSfxStopServices&);
};
}
