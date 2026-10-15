#include "platform.hpp"
namespace ttyd {
// Nine-slice frame emits9 quads*6=54 vertices. Count is frame allocations,
// not vertices; MaxWindowFrames bounds the shared arena across callbacks.
// Geometry/color remain source units until the viewport transform below.
bool Renderer::sourceWindowFrame(const WindowFrameMesh& mesh,bool onBottom) {
    if(!windowFrameVertices || !messagePanel.imported || windowFrameCount>=MaxWindowFrames)return false;
    C2D_Flush();
    const unsigned start=windowFrameCount++*54;
    const auto triangles=windowFrameTriangles(mesh);
    const auto* texture=Tex3DS_GetSubTexture(messagePanel.imported,0);
    for(unsigned n=0;n<54;++n) {
        const auto& p=triangles[n];windowFrameVertices[start+n]={p.x,p.y,p.z,
            texture->left+(texture->right-texture->left)*p.u,
            texture->top+(texture->bottom-texture->top)*p.v,
            mesh.color[0]/255.f,mesh.color[1]/255.f,mesh.color[2]/255.f,mesh.color[3]/255.f};
    }
    GSPGPU_FlushDataCache(windowFrameVertices+start,54*sizeof(Vertex));
    // Backend numeric adaptation: original SDK Rz*S then rectangle-center T.
    // The GPU performs projection; native half-size reflow follows source pose.
    C3D_Mtx local,viewport,model,projection,transform;
    Mtx_Identity(&local);Mtx_Translate(&local,mesh.centerX,mesh.centerY,0,true);
    // .01745329238474369 is the original degree->radian conversion. Top
    // center(200, 120), bottom(160, 120); each native target has240px height.
    // Y half-scale reverses source up-positive UI; horizontal scale is separate.
    Mtx_RotateZ(&local,mesh.rotationDegrees*.01745329238474369f,true);
    Mtx_Scale(&local,mesh.scale,mesh.scale,mesh.scale);
    Mtx_Identity(&viewport);Mtx_Translate(&viewport,onBottom?160:200,120,0,true);
    Mtx_Scale(&viewport,MessageWindowScale,-.5f,1);
    Mtx_Multiply(&model,&viewport,&local);
    Mtx_OrthoTilt(&projection,0,onBottom?320:400,240,0,-1,1,true);Mtx_Multiply(&transform,&projection,&model);
    C3D_BindProgram(&program);
    auto* attrs=C3D_GetAttrInfo();AttrInfo_Init(attrs);
    AttrInfo_AddLoader(attrs,0,GPU_FLOAT,3);AttrInfo_AddLoader(attrs,1,GPU_FLOAT,2);AttrInfo_AddLoader(attrs,2,GPU_FLOAT,4);
    auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,windowFrameVertices,sizeof(Vertex),3,0x210);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&transform);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_ALWAYS,GPU_WRITE_COLOR);
    C3D_AlphaTest(true,GPU_ALWAYS,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA);
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    C3D_TexBind(0,&messagePanel.texture);C3D_DrawArrays(GPU_TRIANGLES,start,54);
    C2D_Prepare();C2D_SceneBegin(onBottom?bottom:top);
    // Prepare restores C2D's GEQUAL/depth-write defaults. Source window
    // contents remain a UI layer; do not let stage depth occlude the next icons.
    C3D_DepthTest(false,GPU_ALWAYS,GPU_WRITE_COLOR);
    C3D_AlphaTest(true,GPU_GREATER,8);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    return true;
}
}
