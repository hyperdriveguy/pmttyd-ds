#pragma once
#include "shadow_display.hpp"
namespace ttyd {
// Source shader/attribute register owner, independent of current PICA state.
// Logical indexed arrays are retained for a future geometry decoder; source
// identities are never cast to ARM pointers. Only the verified setup domain.
struct NativeShadowProgram {
    bool seeded=false,rasterRGB=false,fogOff=false,materialVertex=false,audienceColor=false;
    enum Alpha {Register,RegisterRaster,TextureRaster} alpha=Register;
    uint8_t registerAlpha=0;
    std::optional<uint32_t> material;
    std::optional<uint32_t> fogColor;
    std::optional<ShadowTexture> texture;
    std::optional<uint32_t> cull;
    uint32_t chans=0,texGens=0;
    std::array<uint32_t,3> descriptors{},formats{},arrays{},strides{};
    void afterDummy(){
        seeded=true;registerAlpha=129;rasterRGB=false;audienceColor=false;alpha=Register;
        chans=0;texGens=1;descriptors={1,0,1};formats[0]=4;formats[2]=4;
        // Dummy does not overwrite material, bound texture, fog, cull or arrays.
    }
    std::optional<NativeShadowProgram> request(const ShadowGXRequest&) const;
};
}
