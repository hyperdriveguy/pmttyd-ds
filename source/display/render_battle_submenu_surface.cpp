#include "platform.hpp"
#ifdef __3DS__
namespace ttyd {
// Caller prepares outside FrameBegin. Read-only access never imports or
// replaces resources while an original queued callback is retained.
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
void Renderer::releaseWeaponMenuSurfaces(){
 if(submenuSurfaceGpu){submenuSurfaceGpu->release();submenuSurfaceGpu.reset();}
}
}
#endif
