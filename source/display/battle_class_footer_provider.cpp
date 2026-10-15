#include "battle_class_footer_provider.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
std::optional<AnimationTPLImage>
readBattleClassFooterTexture(const BattleClassFooterMemoryReader &m,
                             uint32_t palette, uint32_t index) {
  if (!m.word || index < 4 || index > 16)
    return {};
  const auto read = [&](uint32_t a) { return m.word(m.user, a); };
  const auto battle = read(0x8041eb90);
  if (!battle)
    return {};
  const auto descriptor = read(*battle + 0x163f8);
  if (!descriptor || !*descriptor)
    return {};
  const auto holder = read(*descriptor + 0xa0);
  if (!holder || !*holder)
    return {};
  const auto actual = read(*holder);
  if (!actual || *actual != palette)
    return {};
  const uint64_t end = uint64_t(palette) + 217920;
  if (end > 0x100000000ULL)
    return {};
  const auto span = [&](uint32_t a, uint32_t n) {
    return a >= palette && uint64_t(a) + n <= end;
  };
  const auto magic = read(palette), count = read(palette + 4),
             table = read(palette + 8);
  if (!magic || *magic != 0x0020af30 || !count || *count != 97 || !table ||
      !span(*table, 97 * 8))
    return {};
  const auto image = read(*table + 8 * index),
             clut = read(*table + 8 * index + 4);
  if (!image || !span(*image, 36) || !clut || *clut)
    return {};
  const auto dimensions = read(*image), format = read(*image + 4),
             data = read(*image + 8), wrapS = read(*image + 12),
             wrapT = read(*image + 16), min = read(*image + 20),
             mag = read(*image + 24), bias = read(*image + 28),
             flags = read(*image + 32);
  if (!dimensions || !format || *format != 14 || !data || !wrapS || !wrapT ||
      !min || !mag || !bias || !flags)
    return {};
  AnimationTPLImage result;
  result.index = index;
  result.imageHeader = *image;
  result.height = uint16_t(*dimensions >> 16);
  result.width = uint16_t(*dimensions);
  const auto bytes = ((uint32_t(result.width) + 7) / 8) *
                     ((uint32_t(result.height) + 7) / 8) * 32;
  if (!result.width || !result.height || !span(*data, bytes) || *wrapS > 2 ||
      *wrapT > 2 || uint8_t(*flags) != 1)
    return {};
  result.format = *format;
  result.data = *data;
  result.wrapS = *wrapS;
  result.wrapT = *wrapT;
  result.minFilter = *min;
  result.magFilter = *mag;
  const auto bits = *bias;
  std::memcpy(&result.bias, &bits, 4);
  result.edge = uint8_t(*flags >> 24);
  result.minLOD = uint8_t(*flags >> 16);
  result.maxLOD = uint8_t(*flags >> 8);
  if (!std::isfinite(result.bias) || result.minLOD > result.maxLOD)
    return {};
  return result;
}

BattleClassFooterProvider::BattleClassFooterProvider(
    const BattleClassFooterGpuResource &r, BattleClassFooterQueryServices q,
    BattleClassFooterQuadSink sink)
    : resource_(r), generation_(r.generation()), query_(q), sink_(sink) {}
bool BattleClassFooterProvider::ready() const {
  return generation_ && resource_.ready() &&
         generation_ == resource_.generation();
}
std::optional<uint32_t> BattleClassFooterProvider::object(uint32_t palette,
                                                          uint32_t index) {
  const auto *region = resource_.surface(index);
  if (!ready() || !region || !query_.texture)
    return {};
  const auto image = query_.texture(query_.user, palette, index);
  if (!image || image->index != index || image->width != region->width ||
      image->height != region->height || image->format != 14)
    return {};
  objects_[index - 4] = *image;
  return index; // Constructed native object handle in this exact owner.
}
bool BattleClassFooterProvider::quad(
    const std::array<std::array<float, 3>, 4> &p,
    const std::array<std::array<float, 2>, 4> &uv,
    const std::array<uint32_t, 4> &rgba) {
  return ready() && matrix_ && texture_ && rgb_ &&
         resource_.quad(*texture_, *matrix_, p, uv, rgba, *rgb_, sink_);
}
std::optional<uint32_t>
BattleClassFooterProvider::request(const BattleTexturePlaneRequest &q) {
  if (!ready())
    return {};
  using K = BattleTexturePlaneKind;
  using F = BattleTexturePlaneCall;
  if (q.kind != K::Call) {
    if (!lanes_)
      return {};
    const unsigned ordinal = lanes_ - 1, vertex = ordinal / 9,
                   lane = ordinal % 9;
    auto p = positions_;
    auto uv = uv_;
    auto colors = colors_;
    if (lane == 3 || lane == 4 || lane == 5 || lane == 6) {
      if (q.kind != K::FifoByte || q.integers.size() != 1 ||
          q.integers[0] > 255 || !q.reals.empty() || q.matrix)
        return {};
      const unsigned shift = 24 - 8 * (lane - 3);
      colors[vertex] =
          (colors[vertex] & ~(255u << shift)) | (q.integers[0] << shift);
    } else {
      if (q.kind != K::FifoFloat || q.reals.size() != 1 ||
          !std::isfinite(q.reals[0]) || !q.integers.empty() || q.matrix)
        return {};
      if (lane < 3)
        p[vertex][lane] = q.reals[0];
      else
        uv[vertex][lane - 7] = q.reals[0];
    }
    if (ordinal == 35) {
      if (!quad(p, uv, colors))
        return {};
      lanes_ = 0;
    } else
      ++lanes_;
    positions_ = p;
    uv_ = uv;
    colors_ = colors;
    return 0;
  }
  if (lanes_)
    return {};
  const auto &n = q.integers;
  if (!q.reals.empty())
    return {};
  switch (q.function) {
  case F::CurrentCamera:
    if (!n.empty() || q.matrix || !query_.camera)
      return {};
    return query_.camera(query_.user, {});
  case F::TextureObject:
    if (n.size() != 2 || q.matrix)
      return {};
    return object(n[0], n[1]);
  case F::LoadTexture:
    if (n.size() != 2 || n[1] || n[0] < 4 || n[0] > 16 || !objects_[n[0] - 4] ||
        q.matrix)
      return {};
    texture_ = n[0];
    footerSelected_ = true;
    return 0;
  case F::TextureFormat:
  case F::TextureWidth:
  case F::TextureHeight: {
    if (n.size() != 1 || n[0] < 4 || n[0] > 16 || !objects_[n[0] - 4] ||
        q.matrix)
      return {};
    const auto &image = *objects_[n[0] - 4];
    return q.function == F::TextureFormat  ? image.format
           : q.function == F::TextureWidth ? uint32_t(image.width)
                                           : uint32_t(image.height);
  }
  case F::LoadMatrix:
    if (n != std::vector<uint32_t>{0} || !q.matrix)
      return {};
    for (float v : *q.matrix)
      if (!std::isfinite(v))
        return {};
    matrix_ = *q.matrix;
    return 0;
  case F::Begin:
    if (n != std::vector<uint32_t>({128, 0, 4}) || q.matrix || !matrix_ ||
        !texture_ || !rgb_ || !sink_.append)
      return {};
    lanes_ = 1;
    positions_ = {};
    uv_ = {};
    colors_ = {};
    return 0;
  case F::ColorIn:
    if (n != std::vector<uint32_t>({0, 15, 15, 15, 10}) || q.matrix)
      return {};
    rgb_ = BattleClassFooterRGB::Raster;
    return 0;
  case F::TevOp:
    if (n != std::vector<uint32_t>({0, 0}) || q.matrix)
      return {};
    rgb_ = BattleClassFooterRGB::Modulate;
    return 0;
  default:
    break;
  }
  if (q.matrix)
    return {};
  bool valid = false;
  switch (q.function) {
  case F::Cull:
    valid = n == std::vector<uint32_t>{0};
    break;
  case F::ClearDescriptors:
    valid = n.empty();
    break;
  case F::Descriptor:
    valid =
        n.size() == 2 && (n[0] == 9 || n[0] == 11 || n[0] == 13) && n[1] == 1;
    break;
  case F::AttributeFormat:
    valid = n == std::vector<uint32_t>({0, 9, 1, 4, 0}) ||
            n == std::vector<uint32_t>({0, 11, 1, 5, 0}) ||
            n == std::vector<uint32_t>({0, 13, 1, 4, 0});
    break;
  case F::Channels:
  case F::TextureGenerators:
  case F::TevStages:
    valid = n == std::vector<uint32_t>{1};
    break;
  case F::ChannelControl:
    valid = n == std::vector<uint32_t>({4, 0, 0, 1, 0, 2, 2});
    break;
  case F::TextureGenerator:
    valid = n == std::vector<uint32_t>({0, 1, 4, 60, 0, 125});
    break;
  case F::ColorOp:
  case F::AlphaOp:
    valid = n == std::vector<uint32_t>({0, 0, 0, 0, 1, 0});
    break;
  case F::AlphaIn:
    valid = n == std::vector<uint32_t>({0, 7, 5, 4, 7});
    break;
  case F::Order:
    valid = n == std::vector<uint32_t>({0, 0, 0, 4});
    break;
  default:
    break;
  }
  return valid ? std::optional<uint32_t>(0) : std::nullopt;
}
std::optional<uint32_t>
BattleClassFooterProvider::rasterRequest(const BattleClassRasterRequest &q,
                                         BattleClassBackend &backend) {
  if (!ready() || lanes_)
    return {};
  using F = BattleClassRasterService;
  const auto &n = q.integers;
  if (q.function == F::Camera) {
    if (n != std::vector<uint32_t>{8} || !query_.camera)
      return {};
    return query_.camera(query_.user, 8);
  }
  if (q.function == F::Init2D) {
    const auto v = backend.request(q);
    if (!v)
      return {};
    rgb_ = BattleClassFooterRGB::Modulate;
    return v;
  }
  if (q.function == F::LoadMatrix) {
    if (n != std::vector<uint32_t>{0} || !q.matrix)
      return {};
    const auto v = backend.request(q);
    if (!v)
      return {};
    matrix_ = *q.matrix;
    return v;
  }
  if (q.function == F::TextureObject) {
    if (n.size() != 2)
      return {};
    if (n[1] >= 4 && n[1] <= 16) {
      const auto v = object(n[0], n[1]);
      if (v)
        footerSelected_ = true;
      return v;
    }
    const auto v = backend.request(q);
    if (v)
      footerSelected_ = false;
    return v;
  }
  if (q.function == F::LoadTexture) {
    if (!footerSelected_)
      return backend.request(q);
    BattleTexturePlaneRequest r;
    r.function = BattleTexturePlaneCall::LoadTexture;
    r.integers = n;
    return request(r);
  }
  if (q.function == F::Quad && footerSelected_) {
    if (n.size() != 4 || q.reals.size() != 4 || q.matrix)
      return {};
    uint32_t rgba = 0;
    for (auto color : n) {
      if (color > 255)
        return {};
      rgba = (rgba << 8) | color;
    }
    const auto &f = q.reals;
    const std::array<std::array<float, 3>, 4> p{
        {{f[0], f[1], 0}, {f[2], f[1], 0}, {f[2], f[3], 0}, {f[0], f[3], 0}}};
    const std::array<std::array<float, 2>, 4> uv{
        {{0, 0}, {1, 0}, {1, 1}, {0, 1}}};
    return quad(p, uv, {rgba, rgba, rgba, rgba}) ? std::optional<uint32_t>(0)
                                                 : std::nullopt;
  }
  return backend.request(q);
}
} // namespace ttyd
