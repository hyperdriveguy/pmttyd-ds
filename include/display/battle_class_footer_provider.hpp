#pragma once
#include "animation_texture.hpp"
#include "battle_class_footer_gpu.hpp"
#include "battle_texture_plane.hpp"
namespace ttyd {
struct BattleClassFooterMemoryReader {
  void *user = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
};
// Safe native binding to the actual executing VM's already-relocated common
// bank. Validates the live B/FileEntry/pointer-cell/palette chain and reads
// original dimensions/format/sampler metadata; no synthetic query defaults.
std::optional<AnimationTPLImage>
readBattleClassFooterTexture(const BattleClassFooterMemoryReader &,
                             uint32_t palette, uint32_t index);

struct BattleClassFooterQueryServices {
  void *user = nullptr;
  // optional index8 means source camGetPtr(8); absent means camGetCurPtr().
  std::optional<uint32_t> (*camera)(void *, std::optional<uint32_t>) = nullptr;
  // Resolve actual palette identity/descriptor in the executing VM. No atlas
  // index alone, guessed pointer or synthesized format/dimensions is accepted.
  std::optional<AnimationTPLImage> (*texture)(void *, uint32_t palette,
                                              uint32_t index) = nullptr;
};
// Logical GX/FIFO owner for the concrete original footer atlas. It appends one
// copied packet at final ST acknowledgement, never a separate later overlay.
class BattleClassFooterProvider {
public:
  BattleClassFooterProvider(const BattleClassFooterGpuResource &,
                            BattleClassFooterQueryServices,
                            BattleClassFooterQuadSink);
  std::optional<uint32_t> request(const BattleTexturePlaneRequest &);
  // Combined original class/footer router; matrix/Init2D setters reach both
  // owners, texture selection chooses subsequent Quad's actual surface bank.
  // TexturePlane2 remains the source linked helper, not a shortcut success.
  std::optional<uint32_t> rasterRequest(const BattleClassRasterRequest &,
                                        BattleClassBackend &);

private:
  const BattleClassFooterGpuResource &resource_;
  const std::shared_ptr<const uint8_t> generation_;
  BattleClassFooterQueryServices query_;
  BattleClassFooterQuadSink sink_;
  std::array<std::optional<AnimationTPLImage>, 13> objects_{};
  std::optional<uint32_t> texture_;
  std::optional<BattleActMatrix> matrix_;
  std::optional<BattleClassFooterRGB> rgb_;
  bool footerSelected_ = false;
  uint32_t lanes_ = 0;
  std::array<std::array<float, 3>, 4> positions_{};
  std::array<std::array<float, 2>, 4> uv_{};
  std::array<uint32_t, 4> colors_{};
  std::optional<uint32_t> object(uint32_t palette, uint32_t index);
  bool ready() const;
  bool quad(const std::array<std::array<float, 3>, 4> &,
            const std::array<std::array<float, 2>, 4> &,
            const std::array<uint32_t, 4> &);
};
} // namespace ttyd
