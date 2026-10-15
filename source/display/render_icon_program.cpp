#include "platform.hpp"
namespace ttyd {
// ShadowGX and SourceIconGXCall retain original function identity/arguments.
// Material, ambient, TEV-color and fog carry packed RGBA in a separate payload;
// do not reinterpret those bits as signed scalar arguments.
bool Renderer::sourceIconState(const ShadowGXRequest &r) {
  SourceIconGXCall call;
  call.function = uint32_t(r.kind);
  for (unsigned i = 0; i < 7; ++i)
    call.integers[i] = uint32_t(r.values[i]);
  call.reals = r.floats;
  if (r.kind == ShadowGX::ChanMaterial || r.kind == ShadowGX::ChanAmbient ||
      r.kind == ShadowGX::TevColor || r.kind == ShadowGX::Fog)
    call.integers[1] = r.color;
  return sourceIconProgram.request(call);
}
bool Renderer::applySourceIconProgram(const NativeIconPlan &p) {
  auto *texture = sourceIconTexture();
  if (!texture || finishingShadowProgram.texture != ShadowTexture::SourceIcon)
    return false;
  return publishSourceIconProgram(p,*texture);
}
// PICA publication changes physical state only; logical source owners remain
// retained separately. Native vertex ABI is XYZ/UV/RGBA; unused samplers park
// on a real texture even when TEV does not sample them.
bool Renderer::publishSourceIconProgram(const NativeIconPlan &p,C3D_Tex &image) {
  auto *texture=&image;
  C3D_BindProgram(&program);
  auto *attrs = C3D_GetAttrInfo();
  AttrInfo_Init(attrs);
  AttrInfo_AddLoader(attrs, 0, GPU_FLOAT, 3);
  AttrInfo_AddLoader(attrs, 1, GPU_FLOAT, 2);
  AttrInfo_AddLoader(attrs, 2, GPU_FLOAT, 4);
  C3D_TexBind(0, texture);
  parkUnusedSamplers();
  for (unsigned i = 0; i < 6; ++i)
    C3D_TexEnvInit(C3D_GetTexEnv(i));
  // Source RRGGBBAA -> native AABBGGRR. Constant alpha retains blend low 8;
  // grayscale accumulates weighted source R/G/B with three PICA stages.
  const auto color = [](uint32_t rgba) {
    return ((rgba & 255) << 24) | ((rgba & 0xff00) << 8) |
           ((rgba >> 8) & 0xff00) | (rgba >> 24);
  };
  if (p.mode == NativeIconPlan::Grayscale) {
    for (unsigned i = 0; i < 3; ++i) {
      auto *env = C3D_GetTexEnv(i);
      C3D_TexEnvColor(env, color((p.tint & 0xffffff00) | (p.blend & 255)));
      C3D_TexEnvSrc(env, C3D_RGB, GPU_TEXTURE0, GPU_CONSTANT, GPU_PREVIOUS);
      C3D_TexEnvOpRgb(env,
                      i == 0   ? GPU_TEVOP_RGB_SRC_R
                      : i == 1 ? GPU_TEVOP_RGB_SRC_G
                               : GPU_TEVOP_RGB_SRC_B,
                      GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_SRC_COLOR);
      C3D_TexEnvFunc(env, C3D_RGB, i ? GPU_MULTIPLY_ADD : GPU_MODULATE);
      if (i == 2) {
        C3D_TexEnvSrc(env, C3D_Alpha, GPU_TEXTURE0, GPU_CONSTANT);
        C3D_TexEnvFunc(env, C3D_Alpha, GPU_MODULATE);
      } else {
        C3D_TexEnvSrc(env, C3D_Alpha, GPU_TEXTURE0);
        C3D_TexEnvFunc(env, C3D_Alpha, GPU_REPLACE);
      }
    }
  } else {
    // Ordinary mode multiplies material*tint; Blended adds a third blend
    // constant multiplication. This is the admitted plan, not arbitrary GX TEV.
    const uint32_t constants[]{p.material, p.tint, p.blend};
    const unsigned count = p.mode == NativeIconPlan::Blended ? 3 : 2;
    for (unsigned i = 0; i < count; ++i) {
      auto *env = C3D_GetTexEnv(i);
      C3D_TexEnvColor(env, color(constants[i]));
      C3D_TexEnvSrc(env, C3D_Both, i ? GPU_PREVIOUS : GPU_TEXTURE0,
                    GPU_CONSTANT);
      C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
    }
  }
  // GX cull0 none/1 back/2 front; source fog color drops its RGBA alpha.
  // This admitted path disables fog rather than inventing a fog equation.
  C3D_CullFace(p.cull == 0   ? GPU_CULL_NONE
               : p.cull == 1 ? GPU_CULL_BACK_CCW
                             : GPU_CULL_FRONT_CCW);
  C3D_FogGasMode(GPU_NO_FOG, GPU_PLAIN_DENSITY, false);
  C3D_FogColor(p.fog >> 8);
  return true;
}
} // namespace ttyd
