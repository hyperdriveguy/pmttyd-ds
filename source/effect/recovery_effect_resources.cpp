#include "recovery_effect_resources.hpp"
#include <cstring>
#include <utility>
namespace ttyd {
namespace {
bool read32(FILE *f, uint32_t &value) {
  uint8_t b[4];
  if (std::fread(b, 1, 4, f) != 4)
    return false;
  value = uint32_t(b[0]) | (uint32_t(b[1]) << 8) | (uint32_t(b[2]) << 16) |
          (uint32_t(b[3]) << 24);
  return true;
}
} // namespace
bool RecoveryEffectResources::load(const char *path) {
  FILE *f = std::fopen(path, "rb");
  if (!f)
    return false;
  const bool okay = read(f);
  std::fclose(f);
  return okay;
}
bool RecoveryEffectResources::read(FILE *f) {
  if (!f)
    return false;
  char magic[8];
  uint32_t count = 0;
  if (std::fread(magic, 1, 8, f) != 8 || std::memcmp(magic, "PMREC001", 8) ||
      !read32(f, count) || !count || count > 64)
    return false;
  std::vector<Span> next;
  next.reserve(count);
  uint64_t end = 0, total = 0;
  for (uint32_t i = 0; i < count; ++i) {
    uint32_t address, size;
    if (!read32(f, address) || !read32(f, size) || !size ||
        uint64_t(address) + size > 0x100000000ull || (i && address < end) ||
        (total += size) > 65536)
      return false;
    end = uint64_t(address) + size;
    next.push_back({address, std::vector<uint8_t>(size)});
  }
  for (auto &span : next)
    if (std::fread(span.bytes.data(), 1, span.bytes.size(), f) !=
        span.bytes.size())
      return false;
  if (std::fgetc(f) != EOF || std::ferror(f))
    return false;
  spans_ = std::move(next);
  return true;
}
const uint8_t *RecoveryEffectResources::find(uint32_t address,
                                             uint32_t size) const {
  for (const auto &span : spans_) {
    if (address >= span.address &&
        uint64_t(address) - span.address + size <= span.bytes.size())
      return span.bytes.data() + (address - span.address);
    if (span.address > address)
      break;
  }
  return nullptr;
}
std::optional<uint8_t> RecoveryEffectResources::byte(uint32_t address) const {
  const auto *p = find(address, 1);
  return p ? std::optional<uint8_t>(*p) : std::nullopt;
}
std::optional<uint16_t> RecoveryEffectResources::half(uint32_t address) const {
  const auto *p = find(address, 2);
  return p ? std::optional<uint16_t>((uint16_t(p[0]) << 8) | p[1])
           : std::nullopt;
}
std::optional<uint32_t> RecoveryEffectResources::word(uint32_t address) const {
  const auto *p = find(address, 4);
  return p ? std::optional<uint32_t>((uint32_t(p[0]) << 24) |
                                     (uint32_t(p[1]) << 16) |
                                     (uint32_t(p[2]) << 8) | p[3])
           : std::nullopt;
}
} // namespace ttyd
