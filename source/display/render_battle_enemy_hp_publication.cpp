#include "platform.hpp"
namespace ttyd {
void Renderer::bindEnemyHpPresentation(BattleEnemyHpOrderedPresentation* owner){orderedHpPresentation=owner;orderedHpCapturePass=0;}
bool Renderer::captureEnemyHpPresentation(uint64_t pass){
 if(!pass){orderedHpCapturePass=0;return true;}
 if(!orderedHpPresentation || orderedHpPresentation->buildingPass()!=pass)return false;
 orderedHpCapturePass=pass;return true;
}
std::optional<BattleEnemyHpIconResidency> Renderer::enemyHpIconResidency(uint32_t index)const{
 const auto image=sourceIcons.image(index);if(!image)return {};
 for(const auto& cell:sourceIconCells)if(cell.index==index && cell.gpu.imported && cell.generation){
   BattleEnemyHpIconResidency result{&cell.gpu.texture,cell.generation,index,image->width,image->height,cell.gpu.texture.width,cell.gpu.texture.height,uint32_t(cell.gpu.texture.fmt)};
   if(validBattleEnemyHpIconResidency(result))return result;
 }
 return {};
}
std::optional<BattleEnemyHpRasterView> Renderer::enemyHpRasterView(const Game& game)const{
 const auto* cameras=game.trioAttack.cameras();
 if(!game.trioBattleActive || !cameras || cameras->currentIndex()!=8)return {};
 const auto& p=finishingCameraDraw.projectionMatrix();if(!p || !iconViewport || !iconScissor)return {};
 // HP packet vertices already contain source model/view. Use actual source P,
 // without reapplying the last number icon's model/view or inventing a camera.
 const CameraViewMatrix identity{{{{1,0,0,0}},{{0,1,0,0}},{{0,0,1,0}}}};
 return BattleEnemyHpRasterView{nativeCameraTransform(*p,identity),iconViewport->rect,iconScissor->rect,iconViewport->depthScale,iconViewport->depthOffset};
}
bool Renderer::canDrawEnemyHpIcon(const BattleEnemyHpIconPacket& p)const{
 if(!validBattleEnemyHpIconPacket(p.packet))return false;
 const auto actual=enemyHpIconResidency(p.packet.image);const auto& wanted=p.residency;
 return actual && actual->texture==wanted.texture && actual->generation==wanted.generation && actual->image==wanted.image && actual->width==wanted.width && actual->height==wanted.height && actual->gpuWidth==wanted.gpuWidth && actual->gpuHeight==wanted.gpuHeight && actual->format==wanted.format;
}
bool Renderer::drawEnemyHpIcon(const BattleEnemyHpIconPacket& p){
 if(!canDrawEnemyHpIcon(p))return false;
 C3D_Tex* texture=nullptr;for(auto& cell:sourceIconCells)if(cell.index==p.packet.image && cell.gpu.imported && cell.generation==p.residency.generation){texture=&cell.gpu.texture;break;}
 if(!texture)return false;
 const auto& q=p.packet;const auto& v=q.viewport;const auto& s=q.scissor;
 C3D_SetViewport(v.x,v.y,v.width,v.height);C3D_SetScissor(GPU_SCISSOR_NORMAL,s.x,s.y,s.x+s.width,s.y+s.height);C3D_DepthMap(true,q.depthScale,q.depthOffset);
 publishSourceIconProgram(q.material,*texture);finishingDisplayPipeline(q.pipeline);
 C3D_Mtx matrix{};for(unsigned i=0;i<4;++i){auto& row=matrix.r[i];row.x=q.transform[i][0];row.y=q.transform[i][1];row.z=q.transform[i][2];row.w=q.transform[i][3];}
 C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&matrix);C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
 for(unsigned i=0;i<4;++i){const auto& pos=q.quad.positions[i];const auto& uv=q.quad.uv[i];C3D_ImmSendAttrib(pos[0],pos[1],pos[2],1);C3D_ImmSendAttrib((2.f+uv[0]*p.residency.width)/p.residency.gpuWidth,1.f-(2.f+uv[1]*p.residency.height)/p.residency.gpuHeight,0,1);C3D_ImmSendAttrib(1,1,1,1);}
 C3D_ImmDrawEnd();return true;
}
bool Renderer::drawEnemyHpOrdered(BattleEnemyHpOrderedPresentation& owner,NativeBattleEnemyHpGpu& hp){
 const auto publication=owner.snapshot();
 if(orderedHpPresentation!=&owner || !publication || !c3dReady || !c2dReady || !programReady || transformLoc<0 || !fontTexture.imported || !owner.validate(*publication))return false;
 // Entire combined publication, both real native owners, before the first
 // viewport/material/GPU write. No import, query callback or source execution
 // occurs in the replay loop. Caller keeps all GPU allocations live here.
 for(const auto& packet:publication->packets){
   if(const auto* icon=std::get_if<BattleEnemyHpIconPacket>(&packet)){if(!canDrawEnemyHpIcon(*icon))return false;}
   else if(!hp.resource().accepts(std::get<BattleEnemyHpRasterPacket>(packet).quad))return false;
 }
 C2D_Flush();
 for(const auto& packet:publication->packets){
   if(const auto* icon=std::get_if<BattleEnemyHpIconPacket>(&packet)){if(!drawEnemyHpIcon(*icon))return false;}
   else {const auto& p=std::get<BattleEnemyHpRasterPacket>(packet);const auto& v=p.view.viewport;const auto& s=p.view.scissor;
     C3D_SetViewport(v.x,v.y,v.width,v.height);C3D_SetScissor(GPU_SCISSOR_NORMAL,s.x,s.y,s.x+s.width,s.y+s.height);C3D_DepthMap(true,p.view.depthScale,p.view.depthOffset);
     C3D_Mtx matrix{};for(unsigned i=0;i<4;++i){auto& row=matrix.r[i];row.x=p.view.projection[i][0];row.y=p.view.projection[i][1];row.z=p.view.projection[i][2];row.w=p.view.projection[i][3];}
     BattleEnemyHpGpuBatch single;single.quads.push_back(p.quad);if(!hp.draw(single,&program,transformLoc,matrix,&fontTexture.texture))return false;
   }
 }
 C3D_SetViewport(0,0,240,400);C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_DepthMap(true,-1,0);return true;
}
}
