#pragma once
#include "battle_class_raster.hpp"
#include "message_icons.hpp"
#include "window_font.hpp"
#include <functional>
namespace ttyd {
// Native 24-byte metadata ABI: U32 width/height plus four float UV endpoints.
struct BattleClassRegion {
  uint32_t width = 0, height = 0;
  float u0 = 0, v0 = 0, u1 = 0, v1 = 0;
};
class BattleClassAssets {
public:
  bool load(const char *surface, const char *icons);
  bool ready() const { return ready_; }
  const BattleClassRegion *surface(uint32_t id) const;
  const BattleClassRegion *icon(uint32_t id) const;

private:
  bool ready_ = false;
  std::array<BattleClassRegion, 2> surfaces_{};
  std::array<BattleClassRegion, 10> icons_{};
};
enum class BattleClassTexture { Surfaces, ClassIcons, MessageIcons, Font };
struct BattleClassVertex {
  float x = 0, y = 0, z = 0, u = 0, v = 0;
  uint32_t rgba = 0;
};
struct BattleClassQuad {
  BattleClassTexture texture;
  std::array<BattleClassVertex, 4> vertices;
  uint32_t sourceFlags = 0;
};
// Finite native UI backend. Queries are synchronous; geometry is retained in
// source request order for the native GPU. No action-command/render-frame
// clock.
class BattleClassBackend {
public:
  const BattleClassAssets *assets = nullptr;
  const MessageFont *font = nullptr;
  std::function<const char *(uint32_t)> label;
  std::function<const MessageIconFrame *(uint32_t)> messageIcon;
  std::vector<BattleClassQuad> quads;
  std::function<uint32_t(uint32_t)> random;
  void begin();
  std::optional<uint32_t> request(const BattleClassRasterRequest &);
  // No pending four-point primitive; does not imply original event completion.
  bool complete() const { return pendingPoints_ == 0; }

private:
  BattleActMatrix matrix_{};
  uint32_t material_ = 0xffffffff, fontColor_ = 0x000000ff, texture_ = 0,
           pendingPoints_ = 0;
  std::array<BattleClassVertex, 4> points_{};
  bool loadedMatrix_ = false, loadedTexture_ = false;
  bool quad(const BattleClassRegion &, BattleClassTexture,
            const BattleActMatrix &,
            const std::array<std::array<float, 2>, 4> &, uint32_t,
            uint32_t = 0);
};
} // namespace ttyd
