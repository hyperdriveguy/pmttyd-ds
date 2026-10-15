#include "battle_class_icon_material.hpp"
#include "platform.hpp"
#include <algorithm>
namespace ttyd {
// Native class-menu budget is 1024 quads*6 vertices shared across callbacks
// in this frame. Surface/icon atlases are separate from font/message icons.
bool Renderer::prepareClassMenuDrawing() {
  if (!c3dReady || !programReady || !top)
    return false;
  if (!classSurfaceGpu)
    classSurfaceGpu = std::make_unique<NativeBattleClassSurfaceGpu>();
  if (!classSurfaceGpu->resource().ready() && !classSurfaceGpu->load("romfs:"))
    return false;
  if (!classVertices) {
    C3D_FrameSync();
    classVertices = static_cast<Vertex *>(linearAlloc(sizeof(Vertex) * 6 * 1024));
  }
  return classVertices != nullptr;
}
const BattleClassAssets *Renderer::classMenuAssets() {
  if (!prepareClassMenuDrawing()) return nullptr;
  return classSurfaceGpu->resource().lease().assets.get();
}
std::optional<BattleClassSurfaceLease> Renderer::classMenuSurfaceLease()const {
  if (!c3dReady || !programReady || !top || !classVertices ||
      !classSurfaceGpu || !classSurfaceGpu->resource().ready()) return {};
  return classSurfaceGpu->resource().lease();
}
const BattleClassFooterGpuResource *Renderer::classFooterResources() {
  if (!c3dReady || !programReady || !top)
    return nullptr;
  if (!classFooterGpu)
    classFooterGpu = std::make_unique<NativeBattleClassFooterGpu>();
  if (!classFooterGpu->resource().ready() && !classFooterGpu->load("romfs:"))
    return nullptr;
  return &classFooterGpu->resource();
}
void Renderer::releaseClassMenuDrawing() {
  releaseClassMenuFrame();
  if (c3dReady)
    C3D_FrameSync();
  if (classSurfaceGpu) {
    classSurfaceGpu->release();
    classSurfaceGpu.reset();
  }
  if (classFooterGpu) {
    classFooterGpu->release();
    classFooterGpu.reset();
  }
  if (classVertices)
    linearFree(classVertices);
  classVertices = nullptr;
  classVertexCount = 0;
}
// A complete batch must belong to this exact asset owner. Preflight the whole
// batch before appending vertices; partial menus must not consume arena space.
// sourceFlags 0=ordinary geometry, 16=normal icon, 48=disabled icon color
// program.
bool Renderer::drawClassMenu(const BattleClassBackend &batch) {
  // Resource preparation belongs before FrameBegin. Preflight a complete
  // ordered batch, including every footer lifetime, before any GPU mutation.
  if (!c3dReady || !c2dReady || !programReady || !top || !batch.complete() ||
      !classSurfaceGpu || !classSurfaceGpu->resource().accepts(batch.surfaceLease) ||
      batch.assets != batch.surfaceLease.assets.get() || !classVertices ||
      transformLoc < 0)
    return false;
  if (batch.quads.size() > 1024 - classVertexCount / 6)
    return false;
  if (!fontTexture.imported || !messageIconTexture.imported)
    return false;
  if (!validateBattleClassFooterBatch(
          batch, classFooterGpu ? &classFooterGpu->resource() : nullptr))
    return false;
  if (batch.quads.empty())
    return true;
  C2D_Flush();
  const unsigned first = classVertexCount;
  constexpr unsigned indices[] = {0, 1, 2, 0, 2, 3};
  for (const auto &q : batch.quads)
    for (unsigned index : indices) {
      const auto &v = q.vertices[index];
      classVertices[classVertexCount++] = {v.x,
                                           v.y,
                                           v.z,
                                           v.u,
                                           v.v,
                                           float((v.rgba >> 24) & 255) / 255,
                                           float((v.rgba >> 16) & 255) / 255,
                                           float((v.rgba >> 8) & 255) / 255,
                                           float(v.rgba & 255) / 255};
    }
  GSPGPU_FlushDataCache(classVertices + first,
                        (classVertexCount - first) * sizeof(Vertex));
  C3D_Mtx viewport, projection, transform;
  Mtx_Identity(&viewport);
  // Source centered UI -> top pixels: half size, Y reversed, center(200, 120).
  // Z scale 0 flattens UI; the native rotated target represents400x240 pixels.
  Mtx_Translate(&viewport, 200, 120, 0, true);
  Mtx_Scale(&viewport, .5f, -.5f, 0);
  Mtx_OrthoTilt(&projection, 0, 400, 240, 0, -1, 1, true);
  Mtx_Multiply(&transform, &projection, &viewport);
  C3D_BindProgram(&program);
  auto *attrs = C3D_GetAttrInfo();
  AttrInfo_Init(attrs);
  AttrInfo_AddLoader(attrs, 0, GPU_FLOAT, 3);
  AttrInfo_AddLoader(attrs, 1, GPU_FLOAT, 2);
  AttrInfo_AddLoader(attrs, 2, GPU_FLOAT, 4);
  auto *buffers = C3D_GetBufInfo();
  BufInfo_Init(buffers);
  BufInfo_Add(buffers, classVertices, sizeof(Vertex), 3, 0x210);
  C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, transformLoc, &transform);
  C3D_CullFace(GPU_CULL_NONE);
  C3D_DepthTest(false, GPU_ALWAYS, GPU_WRITE_COLOR);
  C3D_AlphaTest(true, GPU_GREATER, 8);
  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA, GPU_ONE, GPU_ONE_MINUS_SRC_ALPHA);
  for (unsigned i = 0; i < 6; ++i)
    C3D_TexEnvInit(C3D_GetTexEnv(i));
  auto *env = C3D_GetTexEnv(0);
  C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR,
                GPU_PRIMARY_COLOR);
  C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
  parkUnusedSamplers();
  unsigned at = first;
  for (const auto &q : batch.quads) {
    if (q.texture == BattleClassTexture::Footer) {
      // Same source-ordered queue: no separate later overlay or source retry.
      classFooterGpu->draw(*q.footer, &program, transformLoc, transform,
                           &fontTexture.texture);
      at += 6;
      continue;
    }
    // Restore the borrowed arena after an immediate footer primitive.
    auto *quadBuffers = C3D_GetBufInfo();
    BufInfo_Init(quadBuffers);
    BufInfo_Add(quadBuffers, classVertices, sizeof(Vertex), 3, 0x210);
    C3D_Tex *texture = nullptr;
    switch (q.texture) {
    case BattleClassTexture::Surfaces:
      texture = classSurfaceGpu->texture(0, batch.surfaceLease);
      break;
    case BattleClassTexture::ClassIcons:
      texture = classSurfaceGpu->texture(1, batch.surfaceLease);
      break;
    case BattleClassTexture::MessageIcons:
      texture = &messageIconTexture.texture;
      break;
    case BattleClassTexture::Font:
      texture = &fontTexture.texture;
      break;
    case BattleClassTexture::Footer:
      break;
    }
    // Reinitialize before every quad so a disabled icon cannot leak its
    // three-stage color program into a later bubble, normal icon or glyph.
    for (unsigned stage = 0; stage < 6; ++stage)
      C3D_TexEnvInit(C3D_GetTexEnv(stage));
    // Disabled icon uses separate weighted R/G/B accumulation stages.
    // Preserve record alpha at the requested stage; do not tint every later
    // glyph with this temporary program.
    if (q.sourceFlags == 48) {
      const auto stages = disabledClassIconProgram();
      for (unsigned i = 0; i < stages.size(); ++i) {
        const auto &source = stages[i];
        auto *stage = C3D_GetTexEnv(i);
        const GPU_TEVOP_RGB channel =
            source.channel == ClassIconChannel::Red     ? GPU_TEVOP_RGB_SRC_R
            : source.channel == ClassIconChannel::Green ? GPU_TEVOP_RGB_SRC_G
                                                        : GPU_TEVOP_RGB_SRC_B;
        C3D_TexEnvColor(stage,
                        C2D_Color32(source.coefficient, source.coefficient,
                                    source.coefficient, 255));
        C3D_TexEnvSrc(stage, C3D_RGB, GPU_TEXTURE0, GPU_CONSTANT, GPU_PREVIOUS);
        C3D_TexEnvOpRgb(stage, channel, GPU_TEVOP_RGB_SRC_COLOR,
                        GPU_TEVOP_RGB_SRC_COLOR);
        C3D_TexEnvFunc(stage, C3D_RGB,
                       source.addPrevious ? GPU_MULTIPLY_ADD : GPU_MODULATE);
        C3D_TexEnvSrc(stage, C3D_Alpha, GPU_TEXTURE0, GPU_PRIMARY_COLOR,
                      GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(stage, C3D_Alpha,
                       source.recordAlpha ? GPU_MODULATE : GPU_REPLACE);
      }
    } else {
      auto *stage = C3D_GetTexEnv(0);
      C3D_TexEnvSrc(stage, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR,
                    GPU_PRIMARY_COLOR);
      C3D_TexEnvFunc(stage, C3D_Both, GPU_MODULATE);
    }
    C3D_TexBind(0, texture);
    C3D_DrawArrays(GPU_TRIANGLES, at, 6);
    at += 6;
  }
  // Clear the extra disabled-icon stages before restoring Citro2D's layer.
  // This also leaves ordinary texture/record modulation for any cached state.
  for (unsigned stage = 0; stage < 6; ++stage)
    C3D_TexEnvInit(C3D_GetTexEnv(stage));
  auto *restored = C3D_GetTexEnv(0);
  C3D_TexEnvSrc(restored, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR,
                GPU_PRIMARY_COLOR);
  C3D_TexEnvFunc(restored, C3D_Both, GPU_MODULATE);
  C2D_Prepare();
  C2D_SceneBegin(top);
  C3D_DepthTest(false, GPU_ALWAYS, GPU_WRITE_COLOR);
  C3D_AlphaTest(true, GPU_GREATER, 8);
  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA, GPU_ONE, GPU_ONE_MINUS_SRC_ALPHA);
  return true;
}
} // namespace ttyd
