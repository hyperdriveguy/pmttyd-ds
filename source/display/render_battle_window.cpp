#include "platform.hpp"
namespace ttyd {
// Explicit native preparation; ordinary draw/resource lookup never loads lazily.
bool Renderer::prepareWindowPrimitiveSurface(){
 if(!c3dReady||!programReady)return false;
 if(windowPrimitiveGpu&&windowPrimitiveGpu->resource().lease().current())return true;
 if(!windowPrimitiveGpu)windowPrimitiveGpu=std::make_unique<NativeBattleWindowSurfaceGpu>();
 return windowPrimitiveGpu->load("romfs:/");
}
void Renderer::releaseWindowPrimitiveSurface(){if(windowPrimitiveGpu)windowPrimitiveGpu->release();windowPrimitiveGpu.reset();}
const BattleWindowSurfaceResource* Renderer::windowPrimitiveSurfaceResources()const{
 return windowPrimitiveGpu&&windowPrimitiveGpu->resource().lease().current()?&windowPrimitiveGpu->resource():nullptr;
}
// Require live frame/shader/target and a real parked sampler before native
// preflight. This does not advance source work or establish its readiness.
bool Renderer::canDrawWindowPrimitivePacket(const BattleWindowPacket&p)const{
 return c3dReady&&c2dReady&&programReady&&top&&transformLoc>=0&&fontTexture.imported&&windowPrimitiveGpu&&windowPrimitiveGpu->canDraw(p,&program,transformLoc,&fontTexture.texture);
}
bool Renderer::drawWindowPrimitivePacket(const BattleWindowPacket&p){
 return canDrawWindowPrimitivePacket(p)&&windowPrimitiveGpu->draw(p,&program,transformLoc,&fontTexture.texture);
}
}
