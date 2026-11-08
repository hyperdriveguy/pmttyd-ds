#include "battle_class_footer_gpu.hpp"
#include <cmath>
#include <cstdio>
namespace ttyd {
bool BattleClassFooterGpuResource::load(
    const char *root, const BattleClassFooterTextureServices &s) {
  if (ready() || !root || !s.import || !s.release)
    return false;
  BattleClassFooterAssets candidate;
  const std::string base = root;
  if (!candidate.load((base + "/battle-class-footers.bin").c_str()))
    return false;
  const auto imported =
      s.import(s.user, (base + "/battle-class-footers.t3x").c_str());
  if (!imported)
    return false;
  // GPU_RGBA8 is native enum0. Descriptor IDs and original CMPR indices are
  // unrelated to the converted atlas storage format.
  if (!imported->identity || imported->width != 512 ||
      imported->height != 256 || imported->format != 0) {
    if (imported->identity)
      s.release(s.user, imported->identity);
    return false;
  }
  assets_ = candidate;
  admission_ = *imported;
  owner_ = s;
  // Value 0 is irrelevant: each shared allocation is a distinct import epoch.
  generation_ = std::make_shared<const uint8_t>(0);
  return true;
}
// Release precedes clearing this owner. Caller must retire borrowed packets
// first and serialize callbacks; this owner has no reentrant-cancellation guard.
void BattleClassFooterGpuResource::retire() {
  if (ready())
    owner_.release(owner_.user, admission_.identity);
  admission_ = {};
  generation_.reset();
  owner_ = {};
  assets_ = {};
}
bool BattleClassFooterGpuResource::accepts(
    const BattleClassFooterAtlasQuad &q) const {
  const auto *r = surface(q.index);
  if (!r || q.texture != identity() || q.generation != generation_ ||
      !q.generation ||
      (q.rgb != BattleClassFooterRGB::Modulate &&
       q.rgb != BattleClassFooterRGB::Raster))
    return false;
  for (const auto &v : q.vertices)
    if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z) ||
        !std::isfinite(v.u) || !std::isfinite(v.v) || v.u < r->u0 ||
        v.u > r->u1 || v.v > r->v0 || v.v < r->v1)
      return false;
  return true;
}
// XYZ uses row-major affine 3x4; normalized source ST maps into the exact
// catalog region with one V flip already encoded in its endpoints.
bool BattleClassFooterGpuResource::quad(
    uint32_t index, const BattleActMatrix &matrix,
    const std::array<std::array<float, 3>, 4> &positions,
    const std::array<std::array<float, 2>, 4> &uv,
    const std::array<uint32_t, 4> &colors, BattleClassFooterRGB rgb,
    const BattleClassFooterQuadSink &sink) const {
  const auto *region = surface(index);
  if (!region || !sink.append ||
      (rgb != BattleClassFooterRGB::Modulate &&
       rgb != BattleClassFooterRGB::Raster))
    return false;
  for (float v : matrix)
    if (!std::isfinite(v))
      return false;
  BattleClassFooterAtlasQuad packet;
  packet.texture = identity();
  packet.generation = generation_;
  packet.index = index;
  packet.rgb = rgb;
  for (unsigned i = 0; i < 4; ++i) {
    const auto &p = positions[i];
    const auto &st = uv[i];
    for (float v : p)
      if (!std::isfinite(v))
        return false;
    // This finite atlas provider admits only the complete original0..1 quad.
    // Repeating/mirrored/out-of-atlas requests need their own owner.
    for (float v : st)
      if (!std::isfinite(v) || v < 0 || v > 1)
        return false;
    auto &v = packet.vertices[i];
    v.x = matrix[0] * p[0] + matrix[1] * p[1] + matrix[2] * p[2] + matrix[3];
    v.y = matrix[4] * p[0] + matrix[5] * p[1] + matrix[6] * p[2] + matrix[7];
    v.z = matrix[8] * p[0] + matrix[9] * p[1] + matrix[10] * p[2] + matrix[11];
    if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z))
      return false;
    v.u = region->u0 + (region->u1 - region->u0) * st[0];
    v.v = region->v0 + (region->v1 - region->v0) * st[1];
    v.rgba = colors[i];
  }
  return sink.append(sink.user, packet);
}
bool validateBattleClassFooterBatch(
    const BattleClassBackend &batch,
    const BattleClassFooterGpuResource *resource) {
  for (const auto &q : batch.quads) {
    // Ordinary class primitives admit source flags 0/16(0x10)/48(0x30);
    // footer packets use the dedicated footer field and require flags 0.
    if (q.sourceFlags && q.sourceFlags != 16 && q.sourceFlags != 48)
      return false;
    if (q.texture == BattleClassTexture::Footer) {
      if (q.sourceFlags || !q.footer || !resource ||
          !resource->accepts(*q.footer))
        return false;
    } else if (q.footer || (q.texture != BattleClassTexture::Surfaces &&
                            q.texture != BattleClassTexture::ClassIcons &&
                            q.texture != BattleClassTexture::MessageIcons &&
                            q.texture != BattleClassTexture::Font))
      return false;
  }
  return true;
}
#ifdef __3DS__
bool NativeBattleClassFooterGpu::load(const char *root) {
  BattleClassFooterTextureServices s;
  s.user = this;
  s.import =
      [](void *p,
         const char *path) -> std::optional<BattleClassFooterTextureAdmission> {
    auto &o = *static_cast<NativeBattleClassFooterGpu *>(p);
    if (o.imported_)
      return {};
    FILE *file = std::fopen(path, "rb");
    if (!file)
      return {};
    C3D_FrameSync();
    o.imported_ = Tex3DS_TextureImportStdio(file, &o.texture_, nullptr, false);
    std::fclose(file);
    if (!o.imported_)
      return {};
    C3D_TexSetFilter(&o.texture_, GPU_LINEAR, GPU_LINEAR);
    C3D_TexSetWrap(&o.texture_, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
    return BattleClassFooterTextureAdmission{&o.texture_, o.texture_.width,
                                             o.texture_.height,
                                             uint32_t(o.texture_.fmt)};
  };
  s.release = [](void *p, const void *texture) {
    auto &o = *static_cast<NativeBattleClassFooterGpu *>(p);
    if (texture != &o.texture_ || !o.imported_)
      return;
    C3D_FrameSync();
    Tex3DS_TextureFree(o.imported_);
    C3D_TexDelete(&o.texture_);
    o.imported_ = nullptr;
    o.texture_ = {};
  };
  return resource_.load(root, s);
}
void NativeBattleClassFooterGpu::release() { resource_.retire(); }
bool NativeBattleClassFooterGpu::draw(const BattleClassFooterAtlasQuad &q,
                                      shaderProgram_s *shader, int uniform,
                                      const C3D_Mtx &projection,
                                      C3D_Tex *parked) {
  if (!resource_.accepts(q) || !shader || uniform < 0 || !parked)
    return false;
  // XYZ/ST/RGBA shader ABI. Samplers 1/2 stay bound to a valid parked image;
  // reset all six hardware TEV stages before configuring stage 0.
  C3D_BindProgram(shader);
  auto *attrs = C3D_GetAttrInfo();
  AttrInfo_Init(attrs);
  AttrInfo_AddLoader(attrs, 0, GPU_FLOAT, 3);
  AttrInfo_AddLoader(attrs, 1, GPU_FLOAT, 2);
  AttrInfo_AddLoader(attrs, 2, GPU_FLOAT, 4);
  C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uniform, &projection);
  C3D_TexBind(0, &texture_);
  C3D_TexBind(1, parked);
  C3D_TexBind(2, parked);
  for (unsigned i = 0; i < 6; ++i)
    C3D_TexEnvInit(C3D_GetTexEnv(i));
  auto *env = C3D_GetTexEnv(0);
  if (q.rgb == BattleClassFooterRGB::Raster) {
    C3D_TexEnvSrc(env, C3D_RGB, GPU_PRIMARY_COLOR);
    C3D_TexEnvFunc(env, C3D_RGB, GPU_REPLACE);
  } else {
    C3D_TexEnvSrc(env, C3D_RGB, GPU_TEXTURE0, GPU_PRIMARY_COLOR);
    C3D_TexEnvFunc(env, C3D_RGB, GPU_MODULATE);
  }
  C3D_TexEnvSrc(env, C3D_Alpha, GPU_TEXTURE0, GPU_PRIMARY_COLOR);
  C3D_TexEnvFunc(env, C3D_Alpha, GPU_MODULATE);
  // Class UI profile: color writes without depth testing; alpha must exceed
  // reference 8 (8-bit threshold). RGB blends SRC_ALPHA/ONE_MINUS_SRC_ALPHA;
  // alpha uses ONE/ONE_MINUS_SRC_ALPHA, unlike the separate HP adapter.
  C3D_CullFace(GPU_CULL_NONE);
  C3D_DepthTest(false, GPU_ALWAYS, GPU_WRITE_COLOR);
  C3D_AlphaTest(true, GPU_GREATER, 8);
  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA, GPU_ONE, GPU_ONE_MINUS_SRC_ALPHA);
  C3D_ImmDrawBegin(GPU_TRIANGLE_FAN);
  for (const auto &v : q.vertices) {
    C3D_ImmSendAttrib(v.x, v.y, v.z, 1);
    C3D_ImmSendAttrib(v.u, v.v, 0, 1);
    // Unpack source RGBA high byte first; convert 0..255 to shader 0..1.
    C3D_ImmSendAttrib(
        float(v.rgba >> 24) / 255, float((v.rgba >> 16) & 255) / 255,
        float((v.rgba >> 8) & 255) / 255, float(v.rgba & 255) / 255);
  }
  C3D_ImmDrawEnd();
  return true;
}
#endif
} // namespace ttyd
