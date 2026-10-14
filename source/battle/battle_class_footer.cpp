#include "battle_class_footer.hpp"
#include <cstring>
namespace ttyd {
namespace {
using F = BattleClassRasterService;
struct Footer {
  const BattleClassFooterServices &s;
  uint32_t word(uint32_t a) const {
    const auto &m = s.raster.memory.memory;
    return m.word(m.user, a);
  }
  void put(uint32_t a, uint32_t n) const {
    const auto &m = s.raster.memory.memory;
    m.storeWord(m.user, a, n);
  }
  float real(uint32_t a) const {
    const auto n = word(a);
    float f;
    std::memcpy(&f, &n, 4);
    return f;
  }
  void floating(uint32_t a, float f) const {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    put(a, n);
  }
  float c(int32_t offset) const { return real(0x80427440u + uint32_t(offset)); }
  int32_t half(uint32_t a) const {
    const auto &m = s.raster.memory.memory;
    const auto n = m.half(m.user, a);
    return n < 32768 ? int32_t(n) : int32_t(n) - 65536;
  }
  std::optional<uint32_t>
  call(F f, std::vector<uint32_t> n = {}, std::vector<float> v = {},
       std::optional<BattleActMatrix> matrix = {}) const {
    return s.raster.request ? s.raster.request(s.raster.user, {f, n, v, matrix})
                            : std::nullopt;
  }
  std::optional<std::array<float, 3>> project(uint32_t u) const {
    return s.project ? s.project(s.user, u + 0x3c) : std::nullopt;
  }
  bool button(int32_t type, float x, float y, float angle) const {
    return s.button && s.button(s.user, type, x, y, angle);
  }
  void decrement(uint32_t w) const {
    const float value = real(w + 0x120), zero = c(-20500);
    floating(w + 0x120, value > zero ? value - c(-20368) : zero);
  }
  bool pinch(bool party) const {
    const auto b = currentBattleAddress(s.raster.memory);
    const auto camera = call(F::Camera, {8});
    if (!camera)
      return false;
    const auto unit = party ? battlePartyUnit(b, s.raster.memory)
                            : battleMarioUnit(b, s.raster.memory);
    if (party && !unit)
      return true;
    const auto hp = half(unit + 0x10c);
    if (hp <= 0 || hp >= half(unit + 0x108) || hp > 5)
      return true;
    uint32_t index = 0, color = 0xffe940ff;
    if (hp == 1) {
      const auto gp = word(0x80415fd0), clock = word(gp + 0x1c),
                 language = word(gp + 0x16c);
      const auto &m = s.raster.memory.memory;
      index = m.half(m.user, 0x80416d30 + language * 2);
      if (clock % 50 > 24)
        color = 0xe9522fff;
    } else {
      const auto gp = word(0x80415fd0), language = word(gp + 0x16c);
      const auto &m = s.raster.memory.memory;
      index = m.half(m.user, 0x80416d24 + language * 2);
    }
    const auto palette = word(word(word(b + 0x163f8) + 0xa0));
    const auto object = call(F::TextureObject, {palette, index});
    if (!object || !call(F::LoadTexture, {*object, 0}))
      return false;
    const auto position = project(unit);
    if (!position)
      return false;
    const auto unitX = real(unit + 0x3c);
    const bool right = unitX > c(-20388);
    const auto zero = c(-20500), twenty = c(-20468);
    const float x =
        party ? (*position)[0] - twenty
              : (right ? (*position)[0] - twenty : twenty + (*position)[0]);
    const float y = c(-20384) + (*position)[1],
                z = party && right ? c(-20436) : (party ? zero : twenty);
    const auto &math = s.raster.math;
    if (!math.translation || !math.concat)
      return false;
    const auto local = math.translation(math.user, {x, y, z});
    if (!local)
      return false;
    BattleActMatrix view;
    for (unsigned i = 0; i < 12; ++i)
      view[i] = real(*camera + 0x11c + 4 * i);
    auto matrix = math.concat(math.user, view, *local);
    if (!matrix)
      return false;
    const auto language = word(word(0x80415fd0) + 0x16c);
    if (language) {
      if (!math.scale)
        return false;
      const auto value = c(-20380);
      const auto scale = math.scale(math.user, value, value, value);
      if (!scale)
        return false;
      matrix = math.concat(math.user, *matrix, *scale);
      if (!matrix)
        return false;
    }
    if (!call(F::LoadMatrix, {0}, {}, *matrix))
      return false;
    return call(F::Quad,
                {color >> 24, (color >> 16) & 255, (color >> 8) & 255,
                 color & 255},
                {c(-20376), c(-20496), c(-20396), c(-20372)})
        .has_value();
  }
};
} // namespace
bool packedBattleClassPartyButton(const BattleClassFooterServices &s) {
  Footer f{s};
  const auto b = currentBattleAddress(s.raster.memory), w = f.word(b + 0x1c78);
  if (!f.call(F::Camera, {8}))
    return false;
  const auto mario = battleMarioUnit(b, s.raster.memory),
             party = battlePartyUnit(b, s.raster.memory);
  if (!party) {
    if (!s.audienceItem)
      return false;
    const auto item = s.audienceItem(s.user, 0);
    if (!item)
      return false;
    if (*item == -1) {
      f.floating(w + 0x120, f.c(-20480));
      return true;
    }
    f.decrement(w);
    const auto point = f.project(mario);
    if (!point)
      return false;
    return f.button(5, (*point)[0], (*point)[1] - f.c(-20468),
                    f.real(w + 0x120));
  }
  const auto marioPoint = f.project(mario);
  if (!marioPoint)
    return false;
  const auto partyPoint = f.project(party);
  if (!partyPoint)
    return false;
  const float x = ((*marioPoint)[0] + (*partyPoint)[0]) * f.c(-20364),
              y = (*marioPoint)[1];
  if (!s.audienceItem)
    return false;
  const auto item = s.audienceItem(s.user, 0);
  if (!item)
    return false;
  if (*item != -1) {
    if (f.real(w + 0x120) == f.c(-20480)) {
      if (!s.sound || !s.sound(s.user, 0x802ef7a4))
        return false;
    }
    f.decrement(w);
    return f.button(5, x, y - f.c(-20468), f.real(w + 0x120));
  }
  f.floating(w + 0x120, f.c(-20480));
  if (!s.changePosition)
    return false;
  const auto allowed = s.changePosition(s.user, b);
  if (!allowed)
    return false;
  return !*allowed || f.button(4, x, y - f.c(-20468), f.c(-20500));
}
bool packedBattleClassMarioPinch(const BattleClassFooterServices &s) {
  return Footer{s}.pinch(false);
}
bool packedBattleClassPartyPinch(const BattleClassFooterServices &s) {
  return Footer{s}.pinch(true);
}
bool packedBattleClassPinchMarks(const BattleClassFooterServices &s) {
  Footer f{s};
  const auto mario =
      battleMarioUnit(currentBattleAddress(s.raster.memory), s.raster.memory);
  if (!f.call(F::Init2D))
    return false;
  if (f.real(mario + 0x3c) > f.c(-20388))
    return packedBattleClassPartyPinch(s) && packedBattleClassMarioPinch(s);
  return packedBattleClassMarioPinch(s) && packedBattleClassPartyPinch(s);
}
bool packedBattleClassButton(int32_t type, float x, float y, float angle,
                             const BattleClassFooterServices &s) {
  Footer f{s};
  const auto &math = s.raster.math;
  if (!math.rotation)
    return false;
  const auto rotated = math.rotation(math.user, f.c(-20472) * angle, 121);
  if (!rotated)
    return false;
  // PSMTXTransApply adds XYZ to the captured matrix's translation; it is not
  // an arbitrary replacement translation or reversed concatenation.
  auto matrix = *rotated;
  matrix[3] += x;
  matrix[7] += y;
  matrix[11] += f.c(-20500);
  // btlDispTexPlane2 remains a linked helper with actual palette/size/GX state.
  if (!s.raster.request)
    return false;
  return s.raster
      .request(s.raster.user,
               {F::TexturePlane2,
                {uint32_t(type), f.word(0x80427440u + uint32_t(-20508)), 0},
                {},
                matrix})
      .has_value();
}
} // namespace ttyd
