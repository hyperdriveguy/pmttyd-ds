#include "platform.hpp"
#include <cmath>
namespace ttyd {
void Renderer::freeDemoEffects() {
    if(!demoEffectsReady && !demoEffectTextures[0].imported && !demoEffectTextures[1].imported)return;
    C3D_FrameSync();for(auto& texture:demoEffectTextures)if(texture.imported){Tex3DS_TextureFree(texture.imported);C3D_TexDelete(&texture.texture);texture.imported=nullptr;}
    demoEffectsReady=false;
}
bool Renderer::frontDemoEffects(const DemoChestEffects& effects,CameraPoint eye,CameraPoint at) {
    if(effects.failed)return false;
    if(!effects.started)return true;
    if(!demoEffectsReady){for(unsigned i=0;i<2;++i){char path[96];std::snprintf(path,sizeof(path),"romfs:/demo-crystal-%u.t3x",109+i);
            FILE* f=std::fopen(path,"rb");if(!f){freeDemoEffects();return false;}auto& texture=demoEffectTextures[i];
            texture.imported=Tex3DS_TextureImportStdio(f,&texture.texture,nullptr,false);std::fclose(f);if(!texture.imported){freeDemoEffects();return false;}
            C3D_TexSetFilter(&texture.texture,GPU_LINEAR,GPU_LINEAR);
            C3D_TexSetWrap(&texture.texture,i?GPU_MIRRORED_REPEAT:GPU_CLAMP_TO_EDGE,GPU_MIRRORED_REPEAT);
        }demoEffectsReady=true;
    }
    // Source Camera4 yaw billboard and Camera5 display requests share this
    // scoped native scene view. Original separate camera/manager/GX numeric
    // ownership remains an explicitly documented adapter.
    const float yaw=std::atan2(eye.x-at.x,eye.z-at.z)*57.29577951308232f;
    const auto draws=effects.draw(yaw);const auto rgb=GPU_WRITEMASK(GPU_WRITE_RED|GPU_WRITE_GREEN|GPU_WRITE_BLUE);
    C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GEQUAL,rgb);C3D_AlphaTest(false,GPU_ALWAYS,0);
    C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
    for(const auto& draw:draws){if(draw.camera!=5)return false;const int first=count;
        for(unsigned n=0;n<draw.count;++n){const auto& v=draw.vertices[n];vertex(v.xyz[0]*.01f,v.xyz[1]*.01f,v.xyz[2]*.01f,v.uv[0],1-v.uv[1],0xffffffff,&v.color);}
        if(count!=first+int(draw.count))return false;
        auto* buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,vertices,sizeof(Vertex),3,0x210);
        GSPGPU_FlushDataCache(vertices+first,draw.count*sizeof(Vertex));
        for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
        auto* env=C3D_GetTexEnv(0);
        if(draw.texture>=0){if(draw.texture!=109 && draw.texture!=110)return false;C3D_TexBind(0,&demoEffectTextures[draw.texture-109].texture);
            C3D_TexEnvSrc(env,C3D_RGB,draw.textureColor?GPU_TEXTURE0:GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);
            C3D_TexEnvFunc(env,C3D_RGB,draw.textureColor?GPU_MODULATE:GPU_REPLACE);
            C3D_TexEnvSrc(env,C3D_Alpha,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Alpha,GPU_MODULATE);
        }else {C3D_TexEnvSrc(env,C3D_Both,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_REPLACE);}
        C3D_DrawArrays(GPU_TRIANGLES,first,draw.count);
    }
    for(int n=0;n<6;++n)C3D_TexEnvInit(C3D_GetTexEnv(n));
    auto* env=C3D_GetTexEnv(0);C3D_TexEnvSrc(env,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(env,C3D_Both,GPU_MODULATE);
    C3D_DepthTest(true,GPU_GREATER,GPU_WRITEMASK(rgb|GPU_WRITE_DEPTH));C3D_AlphaTest(true,GPU_GREATER,8);return true;
}
}
