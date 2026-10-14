#include "platform.hpp"
#include "opening_book_view.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
bool Renderer::loadDemoPaper() {
    if(demoPaperAttempted)return demoPaperReady;
    demoPaperAttempted=true;
    FILE* f=std::fopen("romfs:/demo-page.bin","rb");if(!f)return false;
    char magic[8];uint32_t triangles=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMDPG001",8) && std::fread(&triangles,4,1,f)==1 && triangles==demoPaperSurfaces.size() && std::fread(demoPaperSurfaces.data(),sizeof(DemoPaperSurface),triangles,f)==triangles && std::fgetc(f)==EOF;
    std::fclose(f);
    for(const auto& s:demoPaperSurfaces)ok=ok && s.capture<2 && s.pass==2 && s.cull==1;
    if(!ok || !demoPaperAnimations.load("romfs:/demo-page.pmanim"))return false;
    const auto* pose=demoPaperAnimations.pose("A_1");
    demoPaperReady=pose && pose->bindings.size()==triangles*3 && pose->start==0 && pose->end==180 && !pose->loop;
    return demoPaperReady;
}
bool Renderer::frontBookPaper(OpeningDemo& demo,std::array<float,4>& presented,bool capture) {
    bool active=false;for(const auto& page:demo.book.pages)active|=page.active && !page.capturePending;
    if(!active)return true;
    C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    // Original capture rectangle and mesh, native orthographic outer projection.
    // Source camera/FOV/depth/image-base services remain a separate dependency.
    C3D_Mtx projection;if(capture)Mtx_Ortho(&projection,OpeningBookView::captureLeft,OpeningBookView::captureLeft+256,256,0,-100,100,true);
    else Mtx_OrthoTilt(&projection,0,400,240,0,-100,100,true);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&projection);
    if(capture)C3D_SetScissor(GPU_SCISSOR_NORMAL,0,16,193,256);
    else C3D_SetScissor(GPU_SCISSOR_NORMAL,0,0,240,400);
    C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GEQUAL,128);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ONE,GPU_ZERO,GPU_ONE,GPU_ZERO);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    bool ok=true;
    const auto* pose=demoPaperAnimations.pose("A_1");
    for(unsigned n=0;n<4;++n) {
        const auto& page=demo.book.pages[n];if(!page.active || page.capturePending)continue;
        auto texture=[&](const char* name)->FrontTexture*{auto i=std::find_if(frontTextures.begin(),frontTextures.end(),[&](const auto& t){return t.name==name;});return i==frontTextures.end()?nullptr:&*i;};
        auto* front=bookCaptureReady[n]?&bookCaptureTextures[n]:nullptr;auto* back=texture("demo-page-back");
        const float frame=demo.book.pageFrame(n);
        auto& sample=demoPaperSamples[n];
        if(!front || !back || !pose){ok=false;continue;}
        if(demoPaperFrames[n]!=frame){if(!pose->evaluate(frame,sample)){ok=false;continue;}demoPaperFrames[n]=frame;}
        if(sample.vertices.size()!=372){ok=false;continue;}
        const int first=count;
        for(unsigned v=0;v<372;++v) {
            const auto& point=sample.vertices[v];const auto& surface=demoPaperSurfaces[v/3];
            const float cell=surface.capture?1.f:0.f;
            const float u=(point.u*1024-cell*128-2)/124,vv=((1-point.v)*1024-2)/124;
            const float texU=surface.capture?u*193/256.f:u*float(back->size[0])/back->size[2];
            const float texV=surface.capture?1-vv*240/256.f:1-vv*float(back->size[1])/back->size[3];
            const bool final=n==3 && demo.book.adjusting;
            const float scale=final?demo.book.finalPaperScale:1;
            const auto position=openingBookPaperPoint(point.x,point.y,point.z,page.depth,scale,
                final?demo.book.finalPaperOffsetX:0,final?demo.book.finalPaperOffsetY:0,1-demo.book.fade);
            vertex(position[0],position[1],position[2],texU,texV,0xffffffff);
        }
        GSPGPU_FlushDataCache(vertices+first,372*sizeof(Vertex));
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
        // Original cull1, back face. Winding adaptation for the tilted image plane.
        C3D_CullFace(GPU_CULL_BACK_CCW);
        for(unsigned t=0;t<124;) {unsigned end=t+1;while(end<124 && demoPaperSurfaces[end].capture==demoPaperSurfaces[t].capture)++end;C3D_TexBind(0,demoPaperSurfaces[t].capture?front:&back->image.texture);C3D_DrawArrays(GPU_TRIANGLES,first+t*3,(end-t)*3);t=end;}
        presented[n]=frame;
    }
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_CullFace(GPU_CULL_NONE);C3D_AlphaTest(true,GPU_GREATER,0);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);C2D_Prepare();if(!capture)C2D_SceneBegin(top);return ok;
}
}
