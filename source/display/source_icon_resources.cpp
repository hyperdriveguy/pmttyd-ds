#include "source_icon_resources.hpp"
#include "event_gp_addresses.hpp"
#include <cstdio>
#include <cstring>
#include <string>
namespace ttyd {
std::optional<uint32_t>
sourceIconCameraIdentity(const CameraManager *cameras,
                         std::optional<uint32_t> index) {
  if (!cameras)
    return {};
  const auto selected = index ? index : cameras->currentIndex();
  if (!selected || !cameras->camera(*selected))
    return {};
  return 0x84000000u + 0x400u * (*selected);
}
std::optional<uint32_t> sourceIconRuntimeWord(const CameraManager *cameras,
                                              const EventGpAddresses &gp,
                                              uint32_t address) {
  uint32_t value;
  if (gp.readWord(address, value))
    return value;
  if (cameras && address >= 0x84000000u &&
      address < 0x84000000u + 13 * 0x400u) {
    const auto offset = (address - 0x84000000u) % 0x400u;
    const auto *camera = cameras->camera((address - 0x84000000u) / 0x400u);
    if(camera && offset==0x114){std::memcpy(&value,&camera->yaw,4);return value;}
    if (!camera || !camera->viewPublished || offset < 0x11c ||
        offset >= 0x14c || offset % 4)
      return {};
    const auto i = (offset - 0x11c) / 4;
    std::memcpy(&value, &camera->matrices.view[i / 4][i % 4], 4);
    return value;
  }
  return {};
}
namespace {
std::optional<std::vector<uint8_t>> read(const std::string &path, size_t max) {
  FILE *f = std::fopen(path.c_str(), "rb");
  if (!f)
    return {};
  if (std::fseek(f, 0, SEEK_END)) {
    std::fclose(f);
    return {};
  }
  auto size = std::ftell(f);
  if (size <= 0 || size > long(max) || std::fseek(f, 0, SEEK_SET)) {
    std::fclose(f);
    return {};
  }
  std::vector<uint8_t> data(size);
  bool ok = std::fread(data.data(), data.size(), 1, f) == 1;
  std::fclose(f);
  return ok ? std::optional<std::vector<uint8_t>>(std::move(data))
            : std::nullopt;
}
uint16_t be16(const std::vector<uint8_t> &b, size_t p) {
  return uint16_t(uint16_t(b[p]) << 8 | b[p + 1]);
}
} // namespace
bool SourceIconResources::load(const char *root) {
  if (ready() || !root)
    return false;
  const std::string base = root;
  const auto dir = read(base + "/icon.bin", 65536),
             constants = read(base + "/constants.bin", 188);
  const auto textures =
      AnimationTextureResource::load((base + "/icon.tpl").c_str());
  if (!dir || !constants || constants->size() != 188 || !textures ||
      dir->size() < 4)
    return false;
  const auto count = be16(*dir, 0);
  if (!count || 2u * (uint32_t(count) + 1) > dir->size())
    return false;
  for (uint32_t id = 0; id < count; ++id) {
    const auto offset = be16(*dir, 2 * (id + 1));
    if (size_t(offset) + 8 > dir->size())
      return false;
    const auto frames = be16(*dir, offset + 6);
    if (!frames || size_t(offset) + 8 + 4u * frames > dir->size())
      return false;
    uint32_t duration = 0;
    for (uint32_t n = 0; n < frames; ++n) {
      if (!textures->image(be16(*dir, offset + 8 + 4 * n)))
        return false;
      duration += be16(*dir, offset + 10 + 4 * n);
    }
    if (frames > 1 && !duration)
      return false;
  }
  directory_ = *dir;
  constants_ = *constants;
  textures_ = textures;
  objects_.assign(textures->count(), 0);
  return true;
}
std::optional<uint16_t> SourceIconResources::half(uint32_t a) const {
  if (!ready())
    return {};
  if (a >= Directory && uint64_t(a) - Directory + 2 <= directory_.size())
    return be16(directory_, a - Directory);
  struct Span {
    uint32_t base, size, offset;
  };
  for (const auto &s : std::array<Span, 7>{
           {{0x80415040,24,0},{0x80415080,32,24},{0x8041f864,60,56},{0x8041f860,4,116},{0x8041f8a0,4,120},{0x80426884,40,124},{0x80419f30,24,164}}})
    if (a >= s.base && uint64_t(a) - s.base + 2 <= s.size)
      return be16(constants_, s.offset + a - s.base);
  return {};
}
std::optional<uint32_t> SourceIconResources::word(uint32_t a) const {
  if (!ready())
    return {};
  switch (a) {
  case 0x8041e630:
    return Palette;
  case 0x8041e634:
    return Directory;
  case 0x8041e638:
    return Directory;
  case 0x8041e63c:
    return Palette;
  }
  const auto h = half(a), l = half(a + 2);
  return h && l ? std::optional<uint32_t>(uint32_t(*h) << 16 | *l)
                : std::nullopt;
}
std::optional<uint32_t>
SourceIconResources::request(const SourceIconGXCall &r, void *user,
                             bool (*bind)(void *, uint32_t)) {
  if (!ready())
    return {};
  const auto &a = r.integers;
  if (r.function == 0x80037a80) {
    if (a[0] != Palette || a[1] >= count())
      return {};
    for (unsigned i = 2; i < 8; ++i)
      if (a[i])
        return {};
    objects_[a[1]] = 1;
    return a[1] + 1;
  }
  if (!a[0] || a[0] > count() || !objects_[a[0] - 1])
    return {};
  const uint32_t index = a[0] - 1;
  if (r.function == 0x802b7544) {
    if (a != std::array<uint32_t, 8>{a[0], 1, 1, 0, 0, 0, 0, 0} ||
        r.reals != std::array<float, 4>{})
      return {};
    objects_[index] = 2;
    return 0;
  }
  if (r.function == 0x802b7880) {
    if (a != std::array<uint32_t, 8>{a[0], 0, 0, 0, 0, 0, 0, 0} ||
        objects_[index] != 2 || !bind || !bind(user, index))
      return {};
    return 0;
  }
  if (r.function == 0x802b76c4 || r.function == 0x802b76d4) {
    if (a != std::array<uint32_t, 8>{a[0], 0, 0, 0, 0, 0, 0, 0})
      return {};
    const auto info = image(index);
    return r.function == 0x802b76c4 ? info->width : info->height;
  }
  return {};
}
} // namespace ttyd
