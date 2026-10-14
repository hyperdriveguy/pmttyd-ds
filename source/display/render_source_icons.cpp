#include "platform.hpp"
#include <algorithm>
namespace ttyd {
C3D_Tex *Renderer::sourceIconTexture() {
  if (!boundSourceIcon)
    return nullptr;
  for (auto &cell : sourceIconCells)
    if (cell.index == *boundSourceIcon && cell.gpu.imported)
      return &cell.gpu.texture;
  return nullptr;
}
void Renderer::releaseSourceIcons() {
  bool any = false;
  for (const auto &cell : sourceIconCells)
    any |= cell.gpu.imported != nullptr;
  if (any)
    C3D_FrameSync();
  for (auto &cell : sourceIconCells) {
    if (cell.gpu.imported) {
      Tex3DS_TextureFree(cell.gpu.imported);
      C3D_TexDelete(&cell.gpu.texture);
    }
    cell = {};
  }
  if (finishingShadowProgram.texture == ShadowTexture::SourceIcon)
    finishingShadowProgram.texture.reset();
  pendingSourceIcon.reset();
  boundSourceIcon.reset();
}
void Renderer::prepareSourceIcons(const Game &game) {
  if (!game.trioBattleActive) {
    releaseSourceIcons();
    return;
  }
  if (!sourceIcons.ready() && !sourceIcons.load("romfs:/source-icons")) {
    frontFailure = "Original icon resources unavailable";
    return;
  }
  if (!pendingSourceIcon)
    return;
  const auto index = *pendingSourceIcon;
  const auto original = sourceIcons.image(index);
  if (!original)
    return;
  for (auto &cell : sourceIconCells)
    if (cell.index == index && cell.gpu.imported) {
      pendingSourceIcon.reset();
      return;
    }
  SourceIconCell *next = nullptr;
  for (auto &cell : sourceIconCells) {
    if (boundSourceIcon && cell.index == *boundSourceIcon)
      continue;
    if (!next || !cell.gpu.imported || cell.stamp < next->stamp)
      next = &cell;
    if (!cell.gpu.imported)
      break;
  }
  if (!next)
    return;
  C3D_FrameSync();
  if (next->gpu.imported) {
    Tex3DS_TextureFree(next->gpu.imported);
    C3D_TexDelete(&next->gpu.texture);
  }
  *next = {};
  const std::string path =
      "romfs:/source-icons/" + std::to_string(index) + ".t3x";
  FILE *f = std::fopen(path.c_str(), "rb");
  if (!f) {
    frontFailure = "Original icon cell missing: " + path;
    return;
  }
  next->gpu.imported =
      Tex3DS_TextureImportStdio(f, &next->gpu.texture, nullptr, false);
  std::fclose(f);
  const auto size = [](uint32_t n) {
    uint32_t p = 8;
    while (p < n + 4)
      p *= 2;
    return p;
  };
  const auto *sub = next->gpu.imported
                        ? Tex3DS_GetSubTexture(next->gpu.imported, 0)
                        : nullptr;
  if (!sub || Tex3DS_GetNumSubTextures(next->gpu.imported) != 1 ||
      Tex3DS_SubTextureRotated(sub) || next->gpu.texture.fmt != GPU_RGBA8 || next->gpu.texture.minLevel || next->gpu.texture.maxLevel || next->gpu.texture.lodBias ||
      next->gpu.texture.width != size(original->width) ||
      next->gpu.texture.height != size(original->height) ||
      sub->width != next->gpu.texture.width ||
      sub->height != next->gpu.texture.height) {
    if (next->gpu.imported) {
      Tex3DS_TextureFree(next->gpu.imported);
      C3D_TexDelete(&next->gpu.texture);
    }
    *next = {};
    frontFailure = "Original icon cell import mismatch: " + path;
    return;
  }
  C3D_TexSetFilter(&next->gpu.texture, GPU_LINEAR, GPU_LINEAR);
  C3D_TexSetWrap(&next->gpu.texture, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
  next->index = index;
  next->stamp = ++sourceIconStamp;
  pendingSourceIcon.reset();
}
bool Renderer::bindSourceIcon(uint32_t index) {
  for (auto &cell : sourceIconCells)
    if (cell.index == index && cell.gpu.imported) {
      const auto previous = boundSourceIcon;
      boundSourceIcon = index;
      ShadowGXRequest request;
      request.kind = ShadowGX::TextureBind;
      request.texture = ShadowTexture::SourceIcon;
      if (!finishingShadowRequest(request)) {
        boundSourceIcon = previous;
        return false;
      }
      cell.stamp = ++sourceIconStamp;
      return true;
    }
  pendingSourceIcon = index;
  return false;
}
std::optional<uint32_t> Renderer::sourceIconCall(const Game &game,
                                                 const SourceIconGXCall &r) {
  if (r.function == 0x8000f2fc) {
    if (r.integers != std::array<uint32_t, 8>{8, 0, 0, 0, 0, 0, 0, 0})
      return {};
    return game.trioAttack.sourceIconCamera(8);
  }
  if (r.function == 0x8000f2e4) {
    if (r.integers != std::array<uint32_t, 8>{})
      return {};
    return game.trioAttack.sourceIconCamera();
  }
  if (r.function == 0x802ba2fc) {
    if (r.integers != std::array<uint32_t, 8>{})
      return {};
    ShadowGXRequest q;
    q.kind = ShadowGX::PositionMatrix;
    for (unsigned row = 0; row < 3; ++row)
      for (unsigned col = 0; col < 4; ++col)
        q.matrix[row][col] = r.matrix[4 * row + col];
    return finishingShadowRequest(q) ? std::optional<uint32_t>(0)
                                     : std::nullopt;
  }
  if (r.function == 0x802ba39c) {
    if (r.integers != std::array<uint32_t, 8>{})
      return {};
    ShadowGXRequest q;
    q.kind = ShadowGX::CurrentMatrix;
    return finishingShadowRequest(q) ? std::optional<uint32_t>(0)
                                     : std::nullopt;
  }
  // These are actual directory/object/LOD and bound texture owners. Map
  // lighting, TEV/fog/depth and primitive providers still retain their waits.
  return sourceIcons.request(r, this, [](void *u, uint32_t index) {
    return static_cast<Renderer *>(u)->bindSourceIcon(index);
  });
}
} // namespace ttyd
