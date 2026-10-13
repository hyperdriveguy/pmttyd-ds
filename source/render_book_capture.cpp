#include "platform.hpp"
#include "opening_book_capture.hpp"
namespace ttyd {
bool Renderer::captureBookPage(OpeningDemo& demo,unsigned page){
 if(page>=4 || !demo.book.pages[page].active || !demo.book.pages[page].capturePending || !top || top->frameBuf.width!=240 || top->frameBuf.height!=400 || top->frameBuf.colorFmt!=GPU_RB_RGBA8)return false;
 if(!bookCapturePixels)bookCapturePixels=static_cast<uint8_t*>(linearAlloc(240*400*4));
 if(!bookCapturePixels)return false;
 if(!bookCaptureReady[page] && !C3D_TexInit(&bookCaptureTextures[page],256,256,GPU_RGBA8))return false;
 bookCaptureReady[page]=true;
 // Caller has ended/synchronized the frame containing the old composition.
 constexpr uint32_t flags=GX_TRANSFER_FLIP_VERT(0)|GX_TRANSFER_OUT_TILED(0)|GX_TRANSFER_RAW_COPY(0)|GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)|GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8)|GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);
 C3D_SyncDisplayTransfer(static_cast<uint32_t*>(top->frameBuf.colorBuf),GX_BUFFER_DIM(240,400),reinterpret_cast<uint32_t*>(bookCapturePixels),GX_BUFFER_DIM(240,400),flags);
 GSPGPU_InvalidateDataCache(bookCapturePixels,240*400*4);
 auto& texture=bookCaptureTextures[page];
 if(!packOpeningBookCapture(bookCapturePixels,240*400*4,static_cast<uint8_t*>(texture.data),texture.size))return false;
 C3D_TexSetFilter(&texture,GPU_LINEAR,GPU_LINEAR);C3D_TexSetWrap(&texture,GPU_CLAMP_TO_EDGE,GPU_CLAMP_TO_EDGE);C3D_TexFlush(&texture);
 demo.book.captureComplete(page);return true;
}
void Renderer::freeBookCaptures(){
 for(unsigned i=0;i<4;++i)if(bookCaptureReady[i]){C3D_TexDelete(&bookCaptureTextures[i]);bookCaptureReady[i]=false;}
 if(bookCapturePixels){linearFree(bookCapturePixels);bookCapturePixels=nullptr;}
}
}
