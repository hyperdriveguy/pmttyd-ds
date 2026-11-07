#pragma once
// Read-only actual original descriptor0 pixels; explicit host import-memory
// fixture.
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <vector>
namespace WindowNativeFixture {
uint32_t be(const std::vector<uint8_t> &v, unsigned a) {
  assert(a + 4 <= v.size());
  return uint32_t(v[a]) << 24 | uint32_t(v[a + 1]) << 16 |
         uint32_t(v[a + 2]) << 8 | v[a + 3];
}
std::vector<uint8_t> actualNativeAtlas() {
  std::ifstream iso("/home/carson/games/roms/G8ME01.iso", std::ios::binary);
  assert(iso);
  std::vector<uint8_t> b(321984);
  iso.seekg(947396944);
  iso.read(reinterpret_cast<char *>(b.data()), b.size());
  assert(iso && be(b, 0) == 0x20af30);
  const auto h = be(b, be(b, 8)), start = be(b, h + 8);
  std::array<std::array<uint8_t, 4>, 72 * 72> rgba{};
  auto rgb = [](unsigned n) {
    return std::array<uint8_t, 4>{uint8_t((n >> 11) * 255 / 31),
                                  uint8_t((n >> 5 & 63) * 255 / 63),
                                  uint8_t((n & 31) * 255 / 31), 255};
  };
  for (unsigned by = 0; by < 72; by += 8)
    for (unsigned bx = 0; bx < 72; bx += 8)
      for (unsigned sub = 0; sub < 4; ++sub) {
        const unsigned at = start + ((by / 8) * 9 + bx / 8) * 32 + sub * 8;
        const auto a = uint16_t(b[at] << 8 | b[at + 1]),
                   z = uint16_t(b[at + 2] << 8 | b[at + 3]);
        std::array<std::array<uint8_t, 4>, 4> c{rgb(a), rgb(z), {}, {}};
        for (unsigned k = 0; k < 3; ++k) {
          c[2][k] = uint8_t(a > z ? (2 * c[0][k] + c[1][k]) / 3
                                  : (c[0][k] + c[1][k]) / 2);
          c[3][k] = a > z ? uint8_t((c[0][k] + 2 * c[1][k]) / 3) : 0;
        }
        c[2][3] = 255;
        c[3][3] = a > z ? 255 : 0;
        for (unsigned y = 0; y < 4; ++y)
          for (unsigned x = 0; x < 4; ++x)
            rgba[(by + (sub / 2) * 4 + y) * 72 + bx + (sub % 2) * 4 + x] =
                c[b[at + 4 + y] >> (6 - 2 * x) & 3];
      }
  std::vector<uint8_t> out(128 * 128 * 4);
  for (unsigned y = 0; y < 128; ++y)
    for (unsigned x = 0; x < 128; ++x) {
      unsigned m = 0;
      for (unsigned k = 0; k < 3; ++k)
        m |= ((x >> k & 1) << (2 * k)) | ((y >> k & 1) << (2 * k + 1));
      auto at = 4 * (((y / 8) * 16 + x / 8) * 64 + m);
      for (unsigned k = 0; k < 4; ++k)
        out[at + 3 - k] = rgba[std::min(y, 71u) * 72 + std::min(x, 71u)][k];
    }
  return out;
}
} // namespace WindowNativeFixture
