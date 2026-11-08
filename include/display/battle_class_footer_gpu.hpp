#pragma once
#include "battle_class_footer_backend.hpp"
#include <string>
#ifdef __3DS__
#include <citro3d.h>
#include <tex3ds.h>
#endif
namespace ttyd {
struct BattleClassFooterTextureAdmission {
  const void *identity = nullptr;
  uint32_t width = 0, height = 0, format = 0;
};
struct BattleClassFooterTextureServices {
  void *user = nullptr;
  std::optional<BattleClassFooterTextureAdmission> (*import)(
      void *, const char *) = nullptr;
  void (*release)(void *, const void *) = nullptr;
};
enum class BattleClassFooterRGB { Modulate, Raster };
struct BattleClassFooterAtlasQuad {
  const void *texture = nullptr;
  // Reload-epoch identity only; retaining it does not own native texture pixels.
  std::shared_ptr<const uint8_t> generation;
  uint32_t index = 0;
  BattleClassFooterRGB rgb = BattleClassFooterRGB::Modulate;
  std::array<BattleClassVertex, 4> vertices{};
};
struct BattleClassFooterQuadSink {
  void *user = nullptr;
  bool (*append)(void *, const BattleClassFooterAtlasQuad &) = nullptr;
};
// Portable validation/publication policy with actual native import below.
// Explicit retire precedes source texture/catalog destruction. Sink must copy
// the packet; successful append is geometry admission, not GPU presentation.
// No implicit releasing destructor. Explicit retire while the callback owner
// and C3D remain alive; import/release callbacks must not reenter this resource.
class BattleClassFooterGpuResource {
public:
  BattleClassFooterGpuResource() = default;
  BattleClassFooterGpuResource(const BattleClassFooterGpuResource &) = delete;
  BattleClassFooterGpuResource &
  operator=(const BattleClassFooterGpuResource &) = delete;
  bool load(const char *root, const BattleClassFooterTextureServices &);
  void retire();
  bool ready() const { return admission_.identity != nullptr; }
  const BattleClassRegion *surface(uint32_t index) const {
    return ready() ? assets_.surface(index) : nullptr;
  }
  const void *identity() const { return admission_.identity; }
  const std::shared_ptr<const uint8_t> &generation() const {
    return generation_;
  }
  bool accepts(const BattleClassFooterAtlasQuad &) const;
  bool quad(uint32_t index, const BattleActMatrix &,
            const std::array<std::array<float, 3>, 4> &positions,
            const std::array<std::array<float, 2>, 4> &uv,
            const std::array<uint32_t, 4> &colors, BattleClassFooterRGB,
            const BattleClassFooterQuadSink &) const;

private:
  BattleClassFooterAssets assets_;
  BattleClassFooterTextureAdmission admission_;
  std::shared_ptr<const uint8_t> generation_;
  BattleClassFooterTextureServices owner_;
};
// Pure whole-batch preflight used before any native GPU mutation.
bool validateBattleClassFooterBatch(const BattleClassBackend &,
                                    const BattleClassFooterGpuResource *);
#ifdef __3DS__
// Native owner of the real imported atlas. Load before FrameBegin; release
// while C3D remains live, after every borrowed packet has been retired.
class NativeBattleClassFooterGpu {
public:
  NativeBattleClassFooterGpu() = default;
  NativeBattleClassFooterGpu(const NativeBattleClassFooterGpu &) = delete;
  NativeBattleClassFooterGpu &
  operator=(const NativeBattleClassFooterGpu &) = delete;
  bool load(const char *root);
  void release();
  const BattleClassFooterGpuResource &resource() const { return resource_; }
  // Caller owns actual current target/projection, FrameBegin and C2D flush /
  // restore. Shader is the existing XYZ/ST/RGBA class-menu shader. UI state
  // matches render_battle_class; packet acknowledgement itself never calls
  // this.
  bool draw(const BattleClassFooterAtlasQuad &, shaderProgram_s *,
            int transformUniform, const C3D_Mtx &projection,
            C3D_Tex *parkedSampler);

private:
  BattleClassFooterGpuResource resource_;
  C3D_Tex texture_{};
  Tex3DS_Texture imported_ = nullptr;
};
#endif
} // namespace ttyd
