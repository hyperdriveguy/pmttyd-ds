#pragma once
#include "shadow_display.hpp"
namespace ttyd {
// Source shader/attribute register owner, independent of current PICA state.
// Logical indexed arrays are retained for a future geometry decoder; source
// identities are never cast to ARM pointers. Only the verified setup domain.
struct NativeShadowProgram {
    bool seeded=false,rasterRGB=false,fogOff=false,materialVertex=false,alphaMaterialVertex=false,audienceColor=false,textureRGB=false;
    std::optional<uint32_t> ambient,tevKColor0;
    std::array<std::optional<std::array<int32_t,7>>,2> channelControl;
    enum Alpha {Register,RegisterRaster,TextureRaster,Raster,Zero} alpha=Register;
    uint8_t registerAlpha=0;
    std::optional<uint32_t> material;
    std::optional<uint32_t> fogColor;
    std::optional<ShadowTexture> texture;
    std::optional<uint32_t> cull;
    uint32_t chans=0,texGens=0,tevStages=1,constantStageMask=0;
    std::optional<uint32_t> indirectStages;
    std::array<uint32_t,3> descriptors{},formats{},arrays{},strides{};
    void afterDummy(){
        seeded=true;registerAlpha=129;rasterRGB=false;audienceColor=false;textureRGB=false;alpha=Register;
        chans=0;texGens=1;tevStages=1;descriptors={1,0,1};formats[0]=4;formats[2]=4;
        // Dummy does not overwrite material, bound texture, fog, cull or arrays.
    }
    std::optional<NativeShadowProgram> request(const ShadowGXRequest&) const;
};
}
