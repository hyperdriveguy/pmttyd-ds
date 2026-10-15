#include "platform.hpp"
#include "opening_book_capture.hpp"
namespace ttyd {
// Four retained page textures have separate transient render targets. A target
// borrows its page texture; callers retire targets only after frame submission.
C3D_RenderTarget* Renderer::prepareBookCapture(unsigned page){
 if(page>=4)return nullptr;
 auto& texture=bookCaptureTextures[page];
 if(!bookCaptureReady[page]){
  if(!C3D_TexInitVRAM(&texture,OpeningBookCapture::size,OpeningBookCapture::size,GPU_RGBA8))return nullptr;
  bookCaptureReady[page]=true;
  C3D_TexSetFilter(&texture,GPU_LINEAR,GPU_LINEAR);
  C3D_TexSetWrap(&texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);
 }
 // The target owns its transient depth buffer; the retained texture belongs
 // to this page. Target deletion after FrameEnd waits for its GPU submissions.
 return C3D_RenderTargetCreateFromTex(&texture,GPU_TEXFACE_2D,0,GPU_RB_DEPTH24_STENCIL8);
}
// Caller synchronizes GPU readers before releasing these retained captures.
void Renderer::freeBookCaptures(){
 for(unsigned i=0;i<4;++i)if(bookCaptureReady[i]){C3D_TexDelete(&bookCaptureTextures[i]);bookCaptureReady[i]=false;}
}
}
