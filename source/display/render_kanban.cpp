#include "platform.hpp"
namespace ttyd {
namespace {
template<class FlatTexture>
bool prepareFlatMessage(FlatTexture& target,const char* path) {
 FILE* file=std::fopen(path,"rb");if(!file)return false;
 target.imported=Tex3DS_TextureImportStdio(file,&target.texture,nullptr,false);std::fclose(file);
 if(!target.imported)return false;
 C3D_TexSetFilter(&target.texture,GPU_LINEAR,GPU_LINEAR);
 C3D_TexSetWrap(&target.texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);return true;
}
template<class FlatTexture>
// Flat original border uses560x176 content in 1024x256 texture backing.
// Diary has1px inset, kanban0. UI x uses MessageWindowScale; y uses-.5,
// centered at(200, 120). Tint strength0 preserves original RGB during alpha fade.
void flatMessageBorder(FlatTexture& target,const MessageWindowPlan& plan,uint8_t alpha,float inset=0) {
 if(!target.imported)return;
 Tex3DS_SubTexture cell{};cell.width=560;cell.height=176;
 cell.left=inset/1024;cell.right=(inset+560)/1024;cell.top=1-inset/256;cell.bottom=1-(inset+176)/256;
 C2D_ImageTint tint;C2D_PlainImageTint(&tint,C2D_Color32(255,255,255,alpha),0);
 C2D_Image image{&target.texture,&cell};const auto& box=plan.box;
 C2D_DrawImageAt(image,200+box.x*MessageWindowScale,120-box.y*.5f,.4f,&tint,
  box.width*MessageWindowScale/560,box.height*.5f/176);
}
}
bool Renderer::prepareKanban(){return prepareFlatMessage(messageKanban,"romfs:/message-kanban.t3x");}
bool Renderer::prepareDiary(){return prepareFlatMessage(messageDiary,"romfs:/message-diary.t3x");}
void Renderer::kanbanBorder(const MessageWindowPlan& p,uint8_t a){flatMessageBorder(messageKanban,p,a);}
void Renderer::diaryBorder(const MessageWindowPlan& p,uint8_t a){flatMessageBorder(messageDiary,p,a,1);}
}
