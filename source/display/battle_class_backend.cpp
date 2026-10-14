#include "battle_class_backend.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
bool valid(const BattleClassRegion &r) {
  return r.width && r.height && std::isfinite(r.u0) && std::isfinite(r.v0) &&
         std::isfinite(r.u1) && std::isfinite(r.v1) && r.u0 >= 0 &&
         r.u1 > r.u0 && r.u1 <= 1 && r.v1 >= 0 && r.v0 > r.v1 && r.v0 <= 1;
}
bool readRegion(FILE *f, BattleClassRegion &r) {
  return std::fread(&r, sizeof(r), 1, f) == 1 && valid(r);
}
BattleClassVertex vertex(const BattleActMatrix &m, float x, float y, float u,
                         float v, uint32_t color) {
  return {m[0] * x + m[1] * y + m[3],
          m[4] * x + m[5] * y + m[7],
          m[8] * x + m[9] * y + m[11],
          u,
          v,
          color};
}
uint32_t multiplyColor(uint32_t a, uint32_t b) {
  uint32_t result = 0;
  for (unsigned shift = 0; shift < 32; shift += 8)
    result |= (((a >> shift) & 255) * ((b >> shift) & 255) / 255) << shift;
  return result;
}
} // namespace
bool BattleClassAssets::load(const char *surfaces, const char *icons) {
  std::array<BattleClassRegion, 2> nextSurfaces;
  std::array<BattleClassRegion, 10> nextIcons;
  FILE *f = std::fopen(surfaces, "rb");
  if (!f)
    return false;
  char magic[8];
  uint32_t count = 0;
  bool ok = std::fread(magic, 8, 1, f) == 1 &&
            !std::memcmp(magic, "PMBCLS01", 8) &&
            std::fread(&count, 4, 1, f) == 1 && count == 2;
  for (unsigned i = 0; ok && i < 2; ++i) {
    uint32_t id = 99;
    ok = std::fread(&id, 4, 1, f) == 1 && id == i &&
         readRegion(f, nextSurfaces[i]);
  }
  ok = ok && std::fgetc(f) == EOF;
  std::fclose(f);
  if (!ok)
    return false;
  if (nextSurfaces[0].width != 40 || nextSurfaces[0].height != 40 ||
      nextSurfaces[1].width != 128 || nextSurfaces[1].height != 80)
    return false;
  f = std::fopen(icons, "rb");
  if (!f)
    return false;
  ok = std::fread(magic, 8, 1, f) == 1 && !std::memcmp(magic, "PMICON1", 8) &&
       std::fread(&count, 4, 1, f) == 1 && count == 10;
  for (unsigned i = 0; ok && i < 10; ++i) {
    uint32_t id = 0, loop = 0, frames = 0, delay = 0;
    ok = std::fread(&id, 4, 1, f) == 1 && std::fread(&loop, 4, 1, f) == 1 &&
         std::fread(&frames, 4, 1, f) == 1 &&
         std::fread(&delay, 4, 1, f) == 1 && id == 334 + i && loop == 0 &&
         frames == 1 && delay == 0 && readRegion(f, nextIcons[i]);
    ok = ok && nextIcons[i].width == 48 && nextIcons[i].height == 48;
  }
  ok = ok && std::fgetc(f) == EOF;
  std::fclose(f);
  if (!ok)
    return false;
  surfaces_ = nextSurfaces;
  icons_ = nextIcons;
  ready_ = true;
  return true;
}
const BattleClassRegion *BattleClassAssets::surface(uint32_t id) const {
  return ready_ && id < 2 ? &surfaces_[id] : nullptr;
}
const BattleClassRegion *BattleClassAssets::icon(uint32_t id) const {
  return ready_ && id >= 334 && id < 344 ? &icons_[id - 334] : nullptr;
}
void BattleClassBackend::begin() {
  quads.clear();
  matrix_ = {};
  material_ = 0xffffffff;
  fontColor_ = 0x000000ff;
  pendingPoints_ = 0;
  loadedMatrix_ = loadedTexture_ = false;
}
bool BattleClassBackend::quad(const BattleClassRegion &r,
                              BattleClassTexture texture,
                              const BattleActMatrix &m,
                              const std::array<std::array<float, 2>, 4> &xy,
                              uint32_t color, uint32_t flags) {
  if (!valid(r))
    return false;
  BattleClassQuad q;
  q.texture = texture;
  q.sourceFlags = flags;
  const float us[] = {r.u0, r.u1, r.u1, r.u0}, vs[] = {r.v0, r.v0, r.v1, r.v1};
  for (unsigned i = 0; i < 4; ++i)
    q.vertices[i] = vertex(m, xy[i][0], xy[i][1], us[i], vs[i], color);
  quads.push_back(q);
  return true;
}
std::optional<uint32_t>
BattleClassBackend::request(const BattleClassRasterRequest &r) {
  using F = BattleClassRasterService;
  const auto &n = r.integers;
  const auto &f = r.reals;
  switch (r.function) {
  case F::Init2D:
    if (pendingPoints_)
      return {};
    material_ = 0xffffffff; // Source Init2D restores texture times raster,
                            // bypassing the prior material register.
    return 0;
  case F::MaterialColor:
    if (n.size() != 2 || n[0] != 1)
      return {};
    material_ = n[1];
    return 0;
  case F::TevStages:
    if (n != std::vector<uint32_t>{1})
      return {};
    return 0;
  case F::ColorOp:
  case F::AlphaOp:
    if (n != std::vector<uint32_t>({0, 0, 0, 0, 1, 0}))
      return {};
    return 0;
  case F::ColorIn:
    if (n != std::vector<uint32_t>({0, 15, 10, 2, 15}))
      return {};
    return 0;
  case F::AlphaIn:
    if (n != std::vector<uint32_t>({0, 7, 5, 4, 7}))
      return {};
    return 0;
  case F::Order:
    if (n != std::vector<uint32_t>({0, 0, 0, 4}))
      return {};
    return 0;
  case F::LoadMatrix:
    if (!r.matrix || n != std::vector<uint32_t>{0})
      return {};
    for (float value : *r.matrix)
      if (!std::isfinite(value))
        return {};
    matrix_ = *r.matrix;
    loadedMatrix_ = true;
    return 0;
  case F::TextureObject:
    if (!assets || !assets->ready() || n.size() != 2 || !assets->surface(n[1]))
      return {};
    return n[1];
  case F::LoadTexture:
    if (n.size() != 2 || n[1] != 0 || !assets || !assets->surface(n[0]))
      return {};
    texture_ = n[0];
    loadedTexture_ = true;
    return 0;
  case F::Begin:
    if (pendingPoints_ || n != std::vector<uint32_t>({128, 0, 4}) ||
        !loadedMatrix_ || !loadedTexture_)
      return {};
    pendingPoints_ = 4;
    return 0;
  case F::Point: {
    if (!pendingPoints_ || n.size() != 4 || f.size() != 4)
      return {};
    const auto *region = assets->surface(texture_);
    if (!region)
      return {};
    uint32_t color = 0;
    for (unsigned i = 0; i < 4; ++i) {
      if (n[i] > 255)
        return {};
      color |= n[i] << (24 - 8 * i);
    }
    const auto at = 4 - pendingPoints_;
    points_[at] = vertex(matrix_, f[0], f[1],
                         region->u0 + (region->u1 - region->u0) * f[2],
                         region->v0 + (region->v1 - region->v0) * f[3],
                         multiplyColor(color, material_));
    --pendingPoints_;
    if (!pendingPoints_)
      quads.push_back({BattleClassTexture::Surfaces, points_, 0});
    return 0;
  }
  case F::Quad: {
    if (pendingPoints_ || n.size() != 4 || f.size() != 4 || !loadedMatrix_ ||
        !loadedTexture_)
      return {};
    uint32_t color = 0;
    for (unsigned i = 0; i < 4; ++i) {
      if (n[i] > 255)
        return {};
      color |= n[i] << (24 - 8 * i);
    }
    const auto *region = assets->surface(texture_);
    if (!region)
      return {};
    return quad(*region, BattleClassTexture::Surfaces, matrix_,
                {{{f[0], f[1]}, {f[2], f[1]}, {f[2], f[3]}, {f[0], f[3]}}},
                multiplyColor(color, material_))
               ? std::optional<uint32_t>(0)
               : std::nullopt;
  }
  case F::Icon:
  case F::IconColor: {
    if (pendingPoints_ || !r.matrix ||
        n.size() != (r.function == F::Icon ? 2u : 3u))
      return {};
    // Other original icon flags require their own mirroring/clipping semantics.
    if (n[0] != 16 && n[0] != 48)
      return {};
    BattleClassRegion region;
    BattleClassTexture texture;
    if (assets && assets->icon(n[1])) {
      region = *assets->icon(n[1]);
      texture = BattleClassTexture::ClassIcons;
    } else {
      const auto *frame = messageIcon ? messageIcon(n[1]) : nullptr;
      if (!frame)
        return {};
      region = {frame->width, frame->height, frame->u0,
                frame->v0,    frame->u1,     frame->v1};
      texture = BattleClassTexture::MessageIcons;
    }
    const float left = -float(region.width) * .5f, right = -left,
                height = float(region.height);
    return quad(region, texture, *r.matrix,
                {{{left, height}, {right, height}, {right, 0}, {left, 0}}},
                n.size() == 3 ? n[2] : 0xffffffff, n[0])
               ? std::optional<uint32_t>(0)
               : std::nullopt;
  }
  case F::FontWidth: {
    if (n.size() != 1 || !font || !label)
      return {};
    const auto *text = label(n[0]);
    WindowFontMeasurement size;
    if (!text || !measurePlainWindowFont(*font, text, 2, size))
      return {};
    return size.width;
  }
  case F::FontStart:
    if (pendingPoints_)
      return {};
    fontColor_ = 0x000000ff;
    return 0;
  case F::FontColor:
    if (n.size() != 1)
      return {};
    fontColor_ = n[0];
    return 0;
  case F::FontString: {
    if (n.size() != 1 || !font || !label || !r.matrix || pendingPoints_ ||
        !random)
      return {};
    const auto *text = label(n[0]);
    WindowFontMeasurement size;
    if (!text || !measurePlainWindowFont(*font, text, 2, size))
      return {};
    struct Context {
      BattleClassBackend *b;
      BattleActMatrix matrix;
      float center;
      bool failed = false;
    };
    Context c{this, *r.matrix, -float(size.width) * .5f};
    WindowFontDrawServices s;
    s.user = &c;
    s.random = [](void *self, uint32_t bound) {
      return static_cast<Context *>(self)->b->random(bound);
    };
    s.emitQuad = [](void *self, const WindowFontQuad &g) {
      auto &c = *static_cast<Context *>(self);
      BattleClassRegion region{uint32_t(g.right - g.left),
                               uint32_t(g.top - g.bottom),
                               g.u0 / 512.f,
                               1 - g.v0 / 256.f,
                               g.u1 / 512.f,
                               1 - g.v1 / 256.f};
      if (!c.b->quad(
              region, BattleClassTexture::Font, c.matrix,
              {{{c.center + g.originX + g.left, float(g.originY + g.top)},
                {c.center + g.originX + g.right, float(g.originY + g.top)},
                {c.center + g.originX + g.right, float(g.originY + g.bottom)},
                {c.center + g.originX + g.left, float(g.originY + g.bottom)}}},
              g.rgba))
        c.failed = true;
    };
    WindowFontStyle style;
    style.color = style.palette0 = fontColor_;
    style.spacing = 2;
    return drawPlainWindowFont(*font, text, WindowFontConsumer::StringMatrix,
                               style, s) &&
                   !c.failed
               ? std::optional<uint32_t>(0)
               : std::nullopt;
  }
  default:
    return {};
  }
}
static_assert(sizeof(BattleClassRegion) == 24, "Private atlas region ABI");
} // namespace ttyd
