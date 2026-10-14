#include "platform.hpp"
#include <algorithm>
namespace ttyd {
bool Renderer::prepareClassMenuDrawing() {
  if (classVertices && classTextures[0].imported && classTextures[1].imported)
    return true;
  if (!classAssets.ready() &&
      !classAssets.load("romfs:/battle-class-surfaces.bin",
                        "romfs:/battle-class-icons.bin"))
    return false;
  C3D_FrameSync();
  if (!classVertices)
    classVertices =
        static_cast<Vertex *>(linearAlloc(sizeof(Vertex) * 6 * 1024));
  if (!classVertices)
    return false;
  const char *files[] = {"romfs:/battle-class-surfaces.t3x",
                         "romfs:/battle-class-icons.t3x"};
  for (unsigned i = 0; i < 2; ++i) {
    auto &slot = classTextures[i];
    if (slot.imported)
      continue;
    FILE *file = std::fopen(files[i], "rb");
    if (!file)
      return false;
    slot.imported =
        Tex3DS_TextureImportStdio(file, &slot.texture, nullptr, false);
    std::fclose(file);
    if (!slot.imported)
      return false;
    C3D_TexSetFilter(&slot.texture, GPU_LINEAR, GPU_LINEAR);
    C3D_TexSetWrap(&slot.texture, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
  }
  return true;
}
const BattleClassAssets *Renderer::classMenuAssets() {
  return prepareClassMenuDrawing() ? &classAssets : nullptr;
}
void Renderer::releaseClassMenuDrawing() {
  if (c3dReady)
    C3D_FrameSync();
  for (auto &slot : classTextures) {
    if (slot.imported) {
      Tex3DS_TextureFree(slot.imported);
      C3D_TexDelete(&slot.texture);
    }
    slot = Texture{};
  }
  if (classVertices)
    linearFree(classVertices);
  classVertices = nullptr;
  classVertexCount = 0;
}
bool Renderer::drawClassMenu(const BattleClassBackend &batch) {
  if (!batch.complete() || !prepareClassMenuDrawing())
    return false;
  if (batch.quads.size() > 1024 - classVertexCount / 6)
    return false;
  if (!fontTexture.imported || !messageIconTexture.imported)
    return false;
  for (const auto &q : batch.quads)
    if (q.sourceFlags && q.sourceFlags != 16)
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
  C3D_TexBind(1, nullptr);
  C3D_TexBind(2, nullptr);
  unsigned at = first;
  for (const auto &q : batch.quads) {
    C3D_Tex *texture = nullptr;
    switch (q.texture) {
    case BattleClassTexture::Surfaces:
      texture = &classTextures[0].texture;
      break;
    case BattleClassTexture::ClassIcons:
      texture = &classTextures[1].texture;
      break;
    case BattleClassTexture::MessageIcons:
      texture = &messageIconTexture.texture;
      break;
    case BattleClassTexture::Font:
      texture = &fontTexture.texture;
      break;
    }
    C3D_TexBind(0, texture);
    C3D_DrawArrays(GPU_TRIANGLES, at, 6);
    at += 6;
  }
  C2D_Prepare();
  C2D_SceneBegin(top);
  C3D_DepthTest(false, GPU_ALWAYS, GPU_WRITE_COLOR);
  C3D_AlphaTest(true, GPU_GREATER, 8);
  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA, GPU_ONE, GPU_ONE_MINUS_SRC_ALPHA);
  return true;
}
} // namespace ttyd
