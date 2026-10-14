#include "battle_class_raster.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
using F = BattleClassRasterService;
struct Draw {
  const BattleClassRasterServices &s;
  uint32_t word(uint32_t a) const {
    return s.memory.memory.word(s.memory.memory.user, a);
  }
  uint16_t half(uint32_t a) const {
    return s.memory.memory.half(s.memory.memory.user, a);
  }
  float constant(int32_t offset) const {
    return real(0x80427440u + uint32_t(offset));
  }
  float real(uint32_t a) const {
    const auto n = word(a);
    float f;
    std::memcpy(&f, &n, 4);
    return f;
  }
  std::optional<uint32_t>
  call(F function, std::vector<uint32_t> integers = {},
       std::vector<float> reals = {},
       std::optional<BattleActMatrix> matrix = {}) const {
    return s.request ? s.request(s.user, {function, std::move(integers),
                                          std::move(reals), std::move(matrix)})
                     : std::nullopt;
  }
  bool setup(uint32_t color) const {
    return call(F::Init2D).has_value() &&
           call(F::MaterialColor, {1, color}).has_value() &&
           call(F::TevStages, {1}).has_value() &&
           call(F::ColorOp, {0, 0, 0, 0, 1, 0}).has_value() &&
           call(F::ColorIn, {0, 15, 10, 2, 15}).has_value() &&
           call(F::AlphaOp, {0, 0, 0, 0, 1, 0}).has_value() &&
           call(F::AlphaIn, {0, 7, 5, 4, 7}).has_value() &&
           call(F::Order, {0, 0, 0, 4}).has_value();
  }
  std::optional<BattleActMatrix> scale(float x, float y, float z) const {
    return s.math.scale ? s.math.scale(s.math.user, x, y, z) : std::nullopt;
  }
  std::optional<BattleActMatrix> translate(float x, float y, float z) const {
    return s.math.translation ? s.math.translation(s.math.user, {x, y, z})
                              : std::nullopt;
  }
  std::optional<BattleActMatrix> rotate(float a) const {
    return s.math.rotation ? s.math.rotation(s.math.user, a, 121)
                           : std::nullopt;
  }
  std::optional<BattleActMatrix> concat(const BattleActMatrix &a,
                                        const BattleActMatrix &b) const {
    return s.math.concat ? s.math.concat(s.math.user, a, b) : std::nullopt;
  }
  BattleActMatrix cameraMatrix(uint32_t camera) const {
    BattleActMatrix m;
    for (unsigned i = 0; i < 12; ++i)
      m[i] = real(camera + 0x11c + 4 * i);
    return m;
  }
  bool texture(uint32_t b, uint32_t index) const {
    const auto owner = word(b + 0x163f8), palette = word(word(owner + 0xa0));
    const auto object = call(F::TextureObject, {palette, index});
    return object && call(F::LoadTexture, {*object, 0}).has_value();
  }
  std::optional<float> offset(float angle, bool circle) const {
    std::optional<float> result;
    const auto zero = constant(-20500), halfTurn = constant(-20488);
    if (angle >= zero && angle <= halfTurn) {
      const float t = (constant(-20272) * angle) / halfTurn;
      const float v = constant(-20400) - t;
      result = circle ? v / constant(-20280) : v;
    }
    if (angle >= halfTurn && angle <= constant(-20476)) {
      const float t = (constant(-20272) * (angle - halfTurn)) / halfTurn;
      const float v = constant(-20268) + t;
      result = circle ? v / constant(-20280) : v;
    }
    return result;
  }
  std::optional<BattleActMatrix> compose(const BattleActMatrix &base,
                                         const BattleActMatrix &rotation,
                                         const BattleActMatrix &scale) const {
    auto m = concat(base, rotation);
    if (!m)
      return {};
    return concat(*m, scale);
  }
  uint32_t icon(uint32_t row) const {
    const auto id = half(row + 0xc);
    return id ? id : 403;
  }
};
} // namespace
bool drawPackedBattleClassCircle(uint32_t row, const BattleActMatrix &base,
                                 const std::array<float, 3> &position,
                                 float parameter,
                                 const BattleClassRasterServices &s) {
  Draw d{s};
  const auto b = currentBattleAddress(s.memory);
  const auto camera = d.call(F::Camera, {8});
  if (!camera)
    return false;
  if (!s.truncate)
    return false;
  const auto shade = s.truncate(s.user, d.constant(-20380) * position[2]);
  if (!shade)
    return false;
  const auto gray = (192u - uint32_t(*shade)) & 255;
  if (!d.setup(d.word(0x80427440u + uint32_t(-20532))))
    return false;
  const float depthScale =
      std::fma(-d.constant(-20276), position[2], d.constant(-20280));
  const auto scale = d.scale(depthScale, depthScale, d.constant(-20452));
  if (!scale)
    return false;
  const float radians = d.constant(-20472) * parameter;
  const auto rotation = d.rotate(radians);
  if (!rotation)
    return false;
  auto matrix = d.compose(base, *rotation, *scale);
  if (!matrix)
    return false;
  matrix = d.concat(d.cameraMatrix(*camera), *matrix);
  if (!matrix || !d.call(F::LoadMatrix, {0}, {}, *matrix))
    return false;
  if (!d.texture(b, 0))
    return false;
  const auto offset = d.offset(parameter, true);
  if (!offset)
    return false;
  // Copies of the two original32-float templates; primitive callbacks can
  // mutate the source, but cannot retarget these retained local copies.
  std::array<float, 32> xy, uv;
  for (unsigned i = 0; i < 32; ++i)
    xy[i] = d.real(0x802ef650 + 4 * i);
  for (unsigned i = 0; i < 32; ++i)
    uv[i] = d.real(0x802ef6d0 + 4 * i);
  const auto shadowY = d.constant(-20264);
  for (unsigned q = 0; q < 4; ++q) {
    if (!d.call(F::Begin, {128, 0, 4}))
      return false;
    for (unsigned v = 0; v < 4; ++v) {
      const auto i = q * 8 + v * 2;
      if (!d.call(F::Point, {0, 0, 0, 128},
                  {*offset + xy[i], shadowY + xy[i + 1], uv[i], uv[i + 1]}))
        return false;
    }
  }
  for (unsigned q = 0; q < 4; ++q) {
    if (!d.call(F::Begin, {128, 0, 4}))
      return false;
    for (unsigned v = 0; v < 4; ++v) {
      const auto i = q * 8 + v * 2;
      if (!d.call(F::Point, {gray, gray, gray, 255},
                  {xy[i], xy[i + 1], uv[i], uv[i + 1]}))
        return false;
    }
  }
  const auto translation =
      d.translate(d.constant(-20500), d.constant(-20260), d.constant(-20500));
  if (!translation)
    return false;
  const float iconScale =
      d.constant(-20256) *
      std::fma(-d.constant(-20276), position[2], d.constant(-20280));
  const auto scaled = d.scale(iconScale, iconScale, d.constant(-20452));
  if (!scaled)
    return false;
  auto iconRotation = d.rotate(radians);
  if (!iconRotation)
    return false;
  if (!s.identity)
    return false;
  iconRotation = s.identity(s.user);
  if (!iconRotation)
    return false;
  matrix = d.compose(base, *iconRotation, *scaled);
  if (!matrix)
    return false;
  matrix = d.concat(*matrix, *translation);
  if (!matrix)
    return false;
  const auto icon = d.icon(row);
  const bool enabled = d.word(row + 4) == 1;
  const auto sourceColor =
      d.word(0x80427440u + uint32_t(enabled ? -20528 : -20524));
  const auto color =
      (gray << 24) | (gray << 16) | (gray << 8) | (sourceColor & 255);
  return d.call(F::IconColor, {enabled ? 16u : 48u, icon, color}, {}, *matrix)
      .has_value();
}
bool drawPackedBattleClassBubble(uint32_t w, uint32_t row,
                                 const BattleActMatrix &base, float parameter,
                                 const BattleClassRasterServices &s) {
  Draw d{s};
  const auto b = currentBattleAddress(s.memory);
  const auto camera = d.call(F::Camera, {8});
  if (!camera)
    return false;
  if (!d.setup(d.word(0x80427440u + uint32_t(-20544))))
    return false;
  const float one = d.constant(-20452), zero = d.constant(-20500);
  const auto scale = d.scale(one, one, one);
  if (!scale)
    return false;
  auto translation = d.translate(zero, d.constant(-20252), zero);
  if (!translation)
    return false;
  const float radians = d.constant(-20472) * parameter;
  auto rotation = d.rotate(radians);
  if (!rotation)
    return false;
  auto matrix = d.compose(base, *rotation, *scale);
  if (!matrix)
    return false;
  matrix = d.concat(*matrix, *translation);
  if (!matrix)
    return false;
  matrix = d.concat(d.cameraMatrix(*camera), *matrix);
  if (!matrix || !d.call(F::LoadMatrix, {0}, {}, *matrix))
    return false;
  if (!d.texture(b, 1))
    return false;
  const auto offset = d.offset(parameter, false);
  if (!offset)
    return false;
  const bool mirrored = d.word(w + 0x1c) != 0;
  const float left = d.constant(mirrored ? -20244 : -20248),
              right = d.constant(mirrored ? -20248 : -20244);
  if (!d.call(F::Quad, {0, 0, 0, 128},
              {left + *offset, d.constant(-20292), right + *offset,
               d.constant(-20268)}))
    return false;
  if (!d.call(F::Quad, {255, 255, 255, 255},
              {left, d.constant(-20240), right, zero}))
    return false;
  translation = d.translate(d.constant(-20236), d.constant(-20468), zero);
  if (!translation)
    return false;
  const auto iconScale = d.scale(one, one, one);
  if (!iconScale)
    return false;
  rotation = d.rotate(radians);
  if (!rotation)
    return false;
  matrix = d.compose(base, *rotation, *translation);
  if (!matrix)
    return false;
  matrix = d.concat(*matrix, *iconScale);
  if (!matrix)
    return false;
  const auto icon = d.icon(row);
  const bool enabled = d.word(row + 4) == 1;
  if (!d.call(F::Icon, {enabled ? 16u : 48u, icon}, {}, *matrix))
    return false;
  const auto label = d.word(row + 8);
  translation = d.translate(zero, d.constant(-20232), zero);
  if (!translation)
    return false;
  const auto measured = d.call(F::FontWidth, {label});
  if (!measured)
    return false;
  const float width = float(*measured & 65535), maxWidth = d.constant(-20480),
              fontScale = d.constant(-20228);
  const float xScale =
      width > maxWidth ? fontScale * (maxWidth / width) : fontScale;
  const auto textScale = d.scale(xScale, fontScale, fontScale);
  if (!textScale)
    return false;
  rotation = d.rotate(radians);
  if (!rotation)
    return false;
  matrix = d.compose(base, *rotation, *translation);
  if (!matrix)
    return false;
  matrix = d.concat(*matrix, *textScale);
  if (!matrix)
    return false;
  if (!d.call(F::FontStart))
    return false;
  const bool textEnabled = d.word(row + 4) == 1;
  const auto color =
      d.word(0x80427440u + uint32_t(textEnabled ? -20540 : -20536));
  if (!d.call(F::FontColor, {color}))
    return false;
  return d.call(F::FontString, {label}, {}, *matrix).has_value();
}
} // namespace ttyd
