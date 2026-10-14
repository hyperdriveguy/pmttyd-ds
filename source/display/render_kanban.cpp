#include "platform.hpp"
namespace ttyd {
bool Renderer::prepareKanban() {
 FILE* file=std::fopen("romfs:/message-kanban.t3x","rb");if(!file)return false;
 messageKanban.imported=Tex3DS_TextureImportStdio(file,&messageKanban.texture,nullptr,false);std::fclose(file);
 if(!messageKanban.imported)return false;
 C3D_TexSetFilter(&messageKanban.texture,GPU_LINEAR,GPU_LINEAR);
 C3D_TexSetWrap(&messageKanban.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);return true;
}
void Renderer::kanbanBorder(const MessageWindowPlan& plan,uint8_t alpha) {
 if(!messageKanban.imported)return;
 Tex3DS_SubTexture cell{};cell.width=560;cell.height=176;
 cell.left=0;cell.right=560.f/1024;cell.top=1;cell.bottom=1-176.f/256;
 C2D_ImageTint tint;C2D_PlainImageTint(&tint,C2D_Color32(255,255,255,alpha),0);
 C2D_Image image{&messageKanban.texture,&cell};const auto& box=plan.box;
 C2D_DrawImageAt(image,200+box.x*MessageWindowScale,120-box.y*.5f,.4f,&tint,
  box.width*MessageWindowScale/560,box.height*.5f/176);
}
}
