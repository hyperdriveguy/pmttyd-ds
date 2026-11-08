#include "platform.hpp"
#ifdef __3DS__
namespace ttyd {
// Caller prepares outside FrameBegin. Read-only access never imports or
// replaces resources while an original queued callback is retained.
// This owner loads common-bank submenu2/3 only; circle/bubble0/1 and
// footer4..16 have distinct resources despite their related menu use.
bool Renderer::prepareWeaponMenuSurfaces(){
 if(!c3dReady||!programReady||!top)return false;
 if(!submenuSurfaceGpu)submenuSurfaceGpu=std::make_unique<NativeBattleSubmenuSurfaceGpu>();
 return submenuSurfaceGpu->resource().lease().current()||submenuSurfaceGpu->load("romfs:");
}
std::optional<BattleSubmenuSurfaceLease> Renderer::weaponMenuSurfaceLease()const{
 if(!c3dReady||!programReady||!top||!submenuSurfaceGpu)return {};
 auto lease=submenuSurfaceGpu->resource().lease();if(!lease.current())return {};return lease;
}
const BattleSubmenuSurfaceGpuResource* Renderer::weaponMenuSurfaceResources()const{
 return weaponMenuSurfaceLease()?&submenuSurfaceGpu->resource():nullptr;
}
// Invalidate the retained weapon frame/publication before releasing its atlas.
void Renderer::releaseWeaponMenuSurfaces(){
 releaseWeaponMenuFrame();
 if(submenuSurfaceGpu){submenuSurfaceGpu->release();submenuSurfaceGpu.reset();}
}
}
#endif
