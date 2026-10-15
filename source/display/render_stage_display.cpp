#include "platform.hpp"
#include <cmath>
namespace ttyd {
// Original GX mode 0x80 is a four-vertex quad, 0x90 one triangle. Native
// fan preserves quad order. This provider only admits unlit raster color:
// descriptors{DIRECT, DIRECT, NONE}, position format 4(F32), color 5(RGBA8),
// one channel/stage and no texture generators. Unknown material stays pending.
bool Renderer::finishingStagePrimitive(const BattleStagePrimitive& r){
    if(!((r.mode==0x80 && r.count==4) || (r.mode==0x90 && r.count==3)))return false;
    const auto& p=finishingShadowProgram;const auto* pipeline=finishingDisplayState.state();
    if(!pipeline || !pipeline->rasterAdmitted() || !p.seeded || p.chans!=1 || p.texGens || p.tevStages!=1 ||
       p.descriptors!=std::array<uint32_t,3>{1,1,0} || p.formats[0]!=4 || p.formats[1]!=5 ||
       !p.channelControl[0] || (*p.channelControl[0])[1] || !p.materialVertex || !p.rasterRGB || p.textureRGB || p.audienceColor ||
       (p.alpha!=NativeShadowProgram::Raster && p.alpha!=NativeShadowProgram::Zero))return false;
    for(unsigned i=0;i<r.count;++i)for(float v:r.vertices[i].position)if(!std::isfinite(v))return false;
    if(!finishingShadowGpu(p))return false;
    C3D_ImmDrawBegin(r.mode==0x80?GPU_TRIANGLE_FAN:GPU_TRIANGLES);
    for(unsigned i=0;i<r.count;++i){const auto& v=r.vertices[i];
        C3D_ImmSendAttrib(v.position[0],v.position[1],v.position[2],1);
        C3D_ImmSendAttrib(0,0,0,1); // Native shader layout; source TEV consumes no ST.
        C3D_ImmSendAttrib(v.color[0]/255.f,v.color[1]/255.f,v.color[2]/255.f,v.color[3]/255.f);
    }
    C3D_ImmDrawEnd();return true;
}
}
