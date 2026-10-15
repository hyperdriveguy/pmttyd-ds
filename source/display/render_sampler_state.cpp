#include "platform.hpp"
namespace ttyd {
// Physical sampler repair only: this does not acknowledge or change any
// retained logical source texture owner. Multi-texture callers replace unit1/2.
void Renderer::parkUnusedSamplers() {
    // Citro3D1.7.1 reads texture->param for units1/2 before storing their
    // bindings. Null is unsafe. The original font is already required by init
    // and outlives all transient icon/menu/effect banks. No TEV stage samples
    // these fallback units; multi-texture passes replace the ones they use.
    C3D_TexBind(1,&fontTexture.texture);
    C3D_TexBind(2,&fontTexture.texture);
}
}
