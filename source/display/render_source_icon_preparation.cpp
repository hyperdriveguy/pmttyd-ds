#include "platform.hpp"
namespace ttyd {
bool Renderer::prepareSourceIconImage(uint32_t image,const std::shared_ptr<BattleWeaponSourceIconPin>&pin){
 // Explicit caller precondition: outside FrameBegin/source callbacks. No
 // public Citro3D in-frame query exists; never inspect/reset private SDK state.
 if(sourceIconPreparing)return false;
 {struct Guard{bool&v;explicit Guard(bool&n):v(n){v=true;}~Guard(){v=false;}}guard(sourceIconPreparing);
  if(!pin||!pin->alive())return false;
  if(!sourceIcons.ready()&&!sourceIcons.load("romfs:/source-icons")){
   frontFailure="Original icon resources unavailable";return false;
  }
  if(!sourceIcons.image(image)||!pin->markPending(image)||!weaponSourceIconPins.attach(pin))return false;
 }
 return prepareSourceIconImageInternal(image);
}
bool Renderer::prepareSourceIconImageInternal(uint32_t index){
 if(sourceIconPreparing)return false;
 struct Guard{bool&v;explicit Guard(bool&n):v(n){v=true;}~Guard(){v=false;}}guard(sourceIconPreparing);
 if(!sourceIcons.ready()&&!sourceIcons.load("romfs:/source-icons")){
  frontFailure="Original icon resources unavailable";return false;
 }
  const auto original = sourceIcons.image(index);
  if (!original)
    return false;
  for (auto &cell : sourceIconCells)
    if (cell.index == index && cell.gpu.imported) {
      if(pendingSourceIcon==index)pendingSourceIcon.reset();
      return true;
    }
  SourceIconCell *next = nullptr;
  for (auto &cell : sourceIconCells) {
    if ((boundSourceIcon && cell.index == *boundSourceIcon) || retainedIcons.retains(cell.index) ||
        (orderedHpPresentation && orderedHpPresentation->retains(cell.index)) ||
        weaponSourceIconPins.retains(cell.index))
      continue;
    if (!next || !cell.gpu.imported || cell.stamp < next->stamp)
      next = &cell;
    if (!cell.gpu.imported)
      break;
  }
  if (!next)
    return false;
  C3D_FrameSync();
  next->generation.reset();
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
    return false;
  }
  next->gpu.imported =
      Tex3DS_TextureImportStdio(f, &next->gpu.texture, nullptr, false);
  std::fclose(f);
  // PICA minimum 8 and power-of-two backing must contain source dimensions
  // plus 4 total gutter pixels(2 each side). Admit exactly one unrotated RGBA8
  // image without mipmaps/LOD bias; UV replay removes that same gutter.
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
      Tex3DS_SubTextureRotated(sub) || next->gpu.texture.fmt != GPU_RGBA8 ||
      next->gpu.texture.minLevel || next->gpu.texture.maxLevel ||
      next->gpu.texture.lodBias ||
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
    return false;
  }
  C3D_TexSetFilter(&next->gpu.texture, GPU_LINEAR, GPU_LINEAR);
  C3D_TexSetWrap(&next->gpu.texture, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
  next->index = index;
  next->generation=std::make_shared<const uint8_t>(0);
  next->stamp = ++sourceIconStamp;
  if(pendingSourceIcon==index)pendingSourceIcon.reset();
  return true;
}
}
