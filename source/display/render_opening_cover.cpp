#include "platform.hpp"
#include "opening_book_cover.hpp"
#include <algorithm>
namespace ttyd {
bool Renderer::frontBookCover(const OpeningDemo& demo){
    if(demo.bookComplete && !demo.book.adjusting)return true;
    if(!frontImage("demo-1",0,0,0,0)||!frontImage("demo-2",0,0,0,0))return false;
    auto texture=[&](unsigned palette)->FrontTexture*{const auto name="demo-"+std::to_string(palette);auto i=std::find_if(frontTextures.begin(),frontTextures.end(),[&](const auto& t){return t.name==name;});return i==frontTextures.end()?nullptr:&*i;};
    const auto* front=texture(1);const auto* back=texture(2);
    if(!front||!back||front->size[0]!=288||front->size[1]!=352||back->size[0]!=288||back->size[1]!=352)return false;
    const auto quads=openingBookCover(288,352,demo.book.scale,demo.book.rotation,demo.book.offsetX,demo.book.offsetY,demo.book.fade);if(!quads)return false;
    if(demo.book.image>=8 && demo.book.image<=12){
        for(unsigned palette:{demo.book.image-5,demo.book.image}){
            const auto name="demo-"+std::to_string(palette);
            if(!frontImage(name.c_str(),0,0,0,0))return false;
        }
    }
    C2D_Flush();C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    C3D_Mtx projection,view,matrix;Mtx_PerspTilt(&projection,C3D_AngleFromDegrees(25),C3D_AspectRatioTop,100,10000,false);
    Mtx_LookAt(&view,FVec3_New(0,0,openingBookCameraDistance()),FVec3_New(0,0,0),FVec3_New(0,1,0),false);Mtx_Multiply(&matrix,&projection,&view);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&matrix);
    C3D_SetScissor(GPU_SCISSOR_NORMAL,0,0,240,400);
    C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);C3D_AlphaTest(false,GPU_ALWAYS,0);C3D_CullFace(GPU_CULL_NONE);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    bool ok=true;
    std::array<BookCoverQuad,5> composition{};unsigned quadCount=0;
    composition[quadCount++]=(*quads)[0];
    if(demo.book.image>=8 && demo.book.image<=12){
        const unsigned page=demo.book.image-8;
        composition[quadCount++]=*openingBookPage(288,352,demo.book.scale,page,demo.book.offsetX,demo.book.offsetY,demo.book.fade);
        composition[quadCount++]=*openingBookIllustration(demo.book.scale,page,demo.book.offsetX,demo.book.offsetY,demo.book.fade);
    }
    composition[quadCount++]=(*quads)[1];composition[quadCount++]=(*quads)[2];
    for(unsigned i=0;i<quadCount;++i){
        const auto& quad=composition[i];
        if(!openingBookFaceVisible(quad))continue;
        auto* art=texture(quad.palette);if(!art||count>12000-6){ok=false;continue;}
        const int first=count;constexpr unsigned indices[]={0,1,2,2,3,0};
        for(unsigned index:indices){const auto& p=quad.vertices[index];vertex(p.x,p.y,p.z,p.u*art->size[0]/art->size[2],1-p.v*art->size[1]/art->size[3],0x00ffffff|uint32_t(quad.alpha)<<24);}
        GSPGPU_FlushDataCache(vertices+first,6*sizeof(Vertex));auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
        C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,quad.blend?GPU_SRC_ALPHA:GPU_ONE,quad.blend?GPU_ONE_MINUS_SRC_ALPHA:GPU_ZERO,GPU_ONE,GPU_ZERO);
        C3D_TexBind(0,&art->image.texture);C3D_DrawArrays(GPU_TRIANGLES,first,6);
    }
    C3D_SetScissor(GPU_SCISSOR_DISABLE,0,0,240,400);C3D_AlphaTest(true,GPU_GREATER,0);C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);C2D_Prepare();C2D_SceneBegin(top);return ok;
}
}
