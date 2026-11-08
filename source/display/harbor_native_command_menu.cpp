#include "harbor_native_command_menu.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
namespace {
constexpr unsigned MaxClassRows = 7, MaxRows = 64, VisibleListRows = 4,
                   QuadBudget = 1024;
constexpr uint32_t Black = 0x17120eff, White = 0xffffffff,
                   Disabled = 0x777777ff;
constexpr uint32_t CursorIcon = 504;
constexpr float NativeCenterX = 200, NativeCenterY = 120, SourcePerPixel = 2;
bool sameOwner(const std::weak_ptr<const void> &a,
               const std::weak_ptr<const void> &b) {
  return !a.owner_before(b) && !b.owner_before(a);
}
bool region(const BattleClassRegion &r) {
  return r.width && r.height && std::isfinite(r.u0) && std::isfinite(r.v0) &&
         std::isfinite(r.u1) && std::isfinite(r.v1) && r.u0 >= 0 && r.u1 <= 1 &&
         r.u1 > r.u0 && r.v1 >= 0 && r.v0 <= 1 && r.v0 > r.v1;
}
void quad(BattleClassBackend &b, BattleClassTexture texture,
          const BattleClassRegion &r, float x, float y, float w, float h,
          uint32_t rgba, uint32_t flags = 0) {
  BattleClassQuad q{};
  q.texture = texture;
  q.sourceFlags = flags;
  const float left = (x - NativeCenterX) * SourcePerPixel,
              right = (x + w - NativeCenterX) * SourcePerPixel;
  const float top = (NativeCenterY - y) * SourcePerPixel,
              bottom = (NativeCenterY - y - h) * SourcePerPixel;
  q.vertices = {{{left, top, 0, r.u0, r.v0, rgba},
                 {right, top, 0, r.u1, r.v0, rgba},
                 {right, bottom, 0, r.u1, r.v1, rgba},
                 {left, bottom, 0, r.u0, r.v1, rgba}}};
  b.quads.push_back(q);
}
// Original texture0 is one quarter-disc. DrawMainIconCircle801191a8 uses
// these four XY/UV quads (802ef650/802ef6d0), not one stretched full texture.
// This manual native reflow is flat at source Z=0 and angle=0; it does not
// advance the original ring animation or acknowledge a source GX callback.
constexpr float CircleSourceDiameter = 40, CircleSourceCenterY = 20;
constexpr uint32_t CircleGray = 0xc0c0c0ff, CircleShadow = 0x00000080;
constexpr std::array<std::array<float, 8>, 4> CircleXY{{
    {{-20,40, 0,40, 0,20, -20,20}},
    {{20,40, 20,20, 0,20, 0,40}},
    {{-20,20, 0,20, 0,0, -20,0}},
    {{0,20, 20,20, 20,0, 0,0}}}};
constexpr std::array<std::array<float, 8>, 4> CircleUV{{
    {{0,0, 1,0, 1,1, 0,1}},
    {{0,0, 0,1, 1,1, 1,0}},
    {{0,1, 1,1, 1,0, 0,0}},
    {{1,1, 0,1, 0,0, 1,0}}}};
void appendCircle(BattleClassBackend &b, const BattleClassRegion &r,
            float centerX, float centerY, float diameter) {
  const float scale = diameter / CircleSourceDiameter;
  // Original angle0 shadow offsets: X=4/1.3, Y=-3.0769231319.
  // The source1.3 transform is absorbed by this explicit native diameter.
  constexpr float SourceDepthScale = 1.3f;
  constexpr float SourceShadowX = 4.f / SourceDepthScale;
  constexpr float SourceShadowY = -3.076923131942749f;
  for (unsigned pass = 0; pass < 2; ++pass) {
    const float dx = pass ? 0 : SourceShadowX;
    const float dy = pass ? 0 : SourceShadowY;
    for (unsigned quadrant = 0; quadrant < CircleXY.size(); ++quadrant) {
      BattleClassQuad q{};
      q.texture = BattleClassTexture::Surfaces;
      for (unsigned v = 0; v < q.vertices.size(); ++v) {
        const auto lane = v * 2;
        const float x = centerX + (CircleXY[quadrant][lane] + dx) * scale;
        const float y = centerY +
            (CircleSourceCenterY - CircleXY[quadrant][lane+1] - dy) * scale;
        q.vertices[v] = {(x-NativeCenterX)*SourcePerPixel,
                         (NativeCenterY-y)*SourcePerPixel, 0,
                         r.u0+(r.u1-r.u0)*CircleUV[quadrant][lane],
                         r.v0+(r.v1-r.v0)*CircleUV[quadrant][lane+1],
                         pass ? CircleGray : CircleShadow};
      }
      b.quads.push_back(q);
    }
  }
}
bool text(BattleClassBackend &b, const std::string &label, float x, float y,
          float available, uint32_t color) {
  if (label.empty())
    return false;
  float width = 0;
  for (unsigned char c : label) {
    if (c < 32 || c >= 127 || c == '<' || c == '>')
      return false;
    const auto index = b.font->mapping[c];
    if (index >= b.font->glyphs.size())
      return false;
    const auto &g = b.font->glyphs[index];
    if (!g.width || !g.height || g.x + g.width > 512 || g.y + g.height > 256)
      return false;
    width += g.width * .55f;
  }
  const float scale = .55f * std::min(1.f, available / width);
  if (scale < .32f)
    return false;
  for (unsigned char c : label) {
    const auto &g = b.font->glyphs[b.font->mapping[c]];
    BattleClassRegion r{g.width,
                        g.height,
                        g.x / 512.f,
                        1 - g.y / 256.f,
                        (g.x + g.width) / 512.f,
                        1 - (g.y + g.height) / 256.f};
    quad(b, BattleClassTexture::Font, r, x, y, g.width * scale,
         g.height * scale, color);
    x += g.width * scale;
  }
  return true;
}
bool icon(BattleClassBackend &b,
          const std::function<const MessageIconFrame *(uint32_t)> &lookup,
          uint32_t id, float x, float y, float size, bool enabled,
          uint32_t color = White) {
  if (const auto *r = b.assets->icon(id)) {
    if (!region(*r))
      return false;
    quad(b, BattleClassTexture::ClassIcons, *r, x, y, size, size, color,
         enabled ? 16 : 48);
    return true;
  }
  const auto *f = lookup ? lookup(id) : nullptr;
  if (!f)
    return false;
  BattleClassRegion r{f->width, f->height, f->u0, f->v0, f->u1, f->v1};
  if (!region(r))
    return false;
  quad(b, BattleClassTexture::MessageIcons, r, x, y, size, size, color,
       enabled ? 16 : 48);
  return true;
}
} // namespace
bool HarborNativeMenuRow::operator==(const HarborNativeMenuRow &b) const {
  return identity == b.identity && detail == b.detail && icon == b.icon &&
         enabled == b.enabled && label == b.label &&
         sourceName == b.sourceName && sourceReason == b.sourceReason &&
         sourceOrdinal == b.sourceOrdinal && sourceItem == b.sourceItem;
}
bool sameHarborNativeMenu(const HarborNativeMenuSnapshot &a,
                          const HarborNativeMenuSnapshot &b) {
  return a.runtime && a.menuOwner && !a.registration.expired() &&
         !b.registration.expired() && a.facadeRegistration.current() &&
         b.facadeRegistration.current() && a.facade && a.facade == b.facade &&
         a.runtime == b.runtime && sameOwner(a.registration, b.registration) &&
         a.menuOwner == b.menuOwner && a.battle == b.battle &&
         a.work == b.work && a.kind == b.kind && a.type == b.type &&
         a.phase == b.phase && a.turn == b.turn && a.modelTick == b.modelTick &&
         a.selected == b.selected && a.rows == b.rows;
}
std::optional<HarborNativeMenuPlan> prepareHarborNativeMenu(
    const HarborNativeMenuSnapshot &snapshot,
    const BattleClassSurfaceLease &surface, const BattleFontLease &font,
    const std::function<const MessageIconFrame *(uint32_t)> &lookup) {
  if (!sameHarborNativeMenu(snapshot, snapshot) || !snapshot.battle ||
      snapshot.rows.empty() || snapshot.rows.size() > MaxRows ||
      snapshot.selected < 0 ||
      size_t(snapshot.selected) >= snapshot.rows.size() || !surface.current() ||
      !surface.assets || !font.current() || !font.font)
    return {};
  HarborNativeMenuPlan p;
  p.snapshot = snapshot;
  p.batch.assets = surface.assets.get();
  p.batch.surfaceLease = surface;
  p.batch.fontLease = font;
  p.batch.font = font.font.get();
  auto &b = p.batch;
  if (snapshot.kind == HarborNativeMenuKind::Class) {
    if (!snapshot.work || snapshot.rows.size() > MaxClassRows)
      return {};
    const auto *circle = b.assets->surface(0);
    const auto *bubble = b.assets->surface(1);
    if (!circle || !bubble || !region(*circle) || !region(*bubble))
      return {};
    // Explicit native radial reflow, not original measured screen positions.
    constexpr std::array<std::array<float, 2>, 7> centers{{{{103, 131}},
                                                           {{55, 145}},
                                                           {{48, 102}},
                                                           {{69, 65}},
                                                           {{113, 59}},
                                                           {{156, 70}},
                                                           {{191, 92}}}};
    for (size_t slot = 1; slot < snapshot.rows.size(); ++slot) {
      const auto index =
          (size_t(snapshot.selected) + slot) % snapshot.rows.size();
      const auto &row = snapshot.rows[index];
      const auto &at = centers[slot];
      constexpr float NativeCircleDiameter = 36;
      appendCircle(b, *circle, at[0], at[1], NativeCircleDiameter);
      if (!icon(b, lookup, row.icon, at[0] - 13, at[1] - 13, 26, row.enabled,
                CircleGray))
        return {};
    }
    const auto &selected = snapshot.rows[size_t(snapshot.selected)];
    quad(b, BattleClassTexture::Surfaces, *bubble, 77, 108, 174, 70, White);
    if (!icon(b, lookup, selected.icon, 87, 121, 32, selected.enabled) ||
        !text(b, selected.label, 125, 125, 113,
              selected.enabled ? Black : Disabled))
      return {};
  } else {
    // Original nine-slice art; source-centered half-size native window adapter.
    constexpr float Left = 48, Top = 78, Width = 304, Height = 137;
    p.window = type0WindowFrame((Left - NativeCenterX) * SourcePerPixel,
                                (NativeCenterY - Top) * SourcePerPixel,
                                Width * SourcePerPixel, Height * SourcePerPixel,
                                16, {255, 255, 255, 255});
    const auto first =
        (size_t(snapshot.selected) / VisibleListRows) * VisibleListRows;
    for (size_t index = first;
         index < std::min(snapshot.rows.size(), first + VisibleListRows);
         ++index) {
      const auto &row = snapshot.rows[index];
      const float y = Top + 12 + float(index - first) * 29;
      if (index == size_t(snapshot.selected) &&
          !icon(b, lookup, CursorIcon, Left + 8, y, 20, true))
        return {};
      float labelX = Left + 38;
      if (snapshot.kind == HarborNativeMenuKind::Weapon) {
        if (!icon(b, lookup, row.icon, Left + 37, y, 23, row.enabled))
          return {};
        labelX = Left + 67;
      }
      if (!text(b, row.label, labelX, y + 3, Left + Width - 12 - labelX,
                row.enabled ? Black : Disabled))
        return {};
    }
  }
  const char *controls = snapshot.kind == HarborNativeMenuKind::Class
                             ? "D-pad: Select  A: Confirm"
                             : "D-pad: Select  A: Confirm  B: Back";
  if (!text(b, controls, 57, 223, 300, Black) ||
      !text(b, controls, 56, 222, 300, White))
    return {};
  if (!validHarborNativeMenuPlan(p))
    return {};
  return p;
}
bool validHarborNativeMenuPlan(const HarborNativeMenuPlan &p) {
  if (!sameHarborNativeMenu(p.snapshot, p.snapshot) || !p.batch.complete() ||
      p.batch.quads.empty() || p.batch.quads.size() > QuadBudget ||
      !p.batch.surfaceLease.current() || !p.batch.fontLease.current() ||
      p.batch.assets != p.batch.surfaceLease.assets.get() ||
      p.batch.font != p.batch.fontLease.font.get())
    return false;
  for (const auto &q : p.batch.quads) {
    if (q.texture != BattleClassTexture::Surfaces &&
        q.texture != BattleClassTexture::ClassIcons &&
        q.texture != BattleClassTexture::MessageIcons &&
        q.texture != BattleClassTexture::Font)
      return false;
    if (q.sourceFlags != 0 && q.sourceFlags != 16 && q.sourceFlags != 48)
      return false;
    for (const auto &v : q.vertices)
      if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z) ||
          !std::isfinite(v.u) || !std::isfinite(v.v) || v.u < 0 || v.u > 1 ||
          v.v < 0 || v.v > 1)
        return false;
  }
  if (p.window) {
    for (const auto &v : p.window->quads)
      if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z) ||
          !std::isfinite(v.u) || !std::isfinite(v.v))
        return false;
  }
  return true;
}
} // namespace ttyd
