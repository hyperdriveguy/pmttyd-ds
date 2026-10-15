#include "mario_motion_sfx.hpp"
namespace ttyd {
// Old0 cancels idle then secondary (short-circuit), old10 cancels fall.
//1/2/3/11/15 have no sound cleanup here; other old motions are unavailable.
// A successful first cancel survives a second hold; clear each handle only
// after actual Off succeeds, so retry cannot stop that voice twice.
bool MarioMotionSfx::post(unsigned old,PositionalSfxRegistry& slots,const PositionalSfxStopServices& backend) {
 auto cancel=[&](int32_t& handle){if(handle!=-1){if(!slots.off(handle,backend))return false;handle=-1;}return true;};
 if(old==0)return cancel(idle) && cancel(secondary);
 if(old==10)return cancel(fall);
 return old==1 || old==2 || old==3 || old==11 || old==15;
}
}
