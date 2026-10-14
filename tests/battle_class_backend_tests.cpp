#include "battle_class_backend.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main() {
  BattleClassAssets assets;
  assert(assets.load("romfs/battle-class-surfaces.bin",
                     "romfs/battle-class-icons.bin"));
  assert(assets.surface(0)->width == 40 && assets.surface(1)->height == 80 &&
         assets.icon(334)->width == 48);
  assert(!assets.icon(333) && !assets.surface(2));
  assert(
      !assets.load("romfs/no-such-atlas.bin", "romfs/battle-class-icons.bin") &&
      assets.ready());
  BattleClassBackend b;
  b.assets = &assets;
  b.begin();
  using F = BattleClassRasterService;
  auto call = [&](F f, std::vector<uint32_t> n = {}, std::vector<float> v = {},
                  std::optional<BattleActMatrix> m = {}) {
    return b.request({f, n, v, m});
  };
  BattleActMatrix matrix{};
  matrix[0] = matrix[5] = matrix[10] = 1;
  matrix[3] = 20;
  matrix[7] = 50;
  assert(call(F::LoadMatrix, {0}, {}, matrix));
  assert(call(F::MaterialColor, {1, 0xe1e1e1ff}));
  assert(call(F::TextureObject, {0x1234, 1}) == 1);
  assert(call(F::LoadTexture, {1, 0}));
  assert(call(F::Quad, {255, 255, 255, 255}, {64, 80, -64, 0}));
  assert(b.quads.size() == 1 && b.quads[0].vertices[0].x == 84 &&
         b.quads[0].vertices[1].x == -44);
  assert(b.quads[0].vertices[0].rgba == 0xe1e1e1ff &&
         b.quads[0].vertices[0].u == assets.surface(1)->u0);
  assert(call(F::Begin, {128, 0, 4}) && !b.complete());
  assert(!call(F::Begin, {128, 0, 4}));
  for (unsigned i = 0; i < 4; ++i)
    assert(call(F::Point, {0, 0, 0, 128}, {float(i), 0, 0, 0}));
  assert(b.complete() && b.quads.size() == 2 &&
         b.quads[1].vertices[0].rgba == 128);
  assert(call(F::Init2D));
  assert(call(F::Quad,{255,233,64,255},{0,0,1,1}));
  assert(b.quads.back().vertices[0].rgba==0xffe940ff);
  assert(call(F::Icon, {16, 334}, {}, matrix));
  assert(b.quads.back().texture == BattleClassTexture::ClassIcons);
  assert(b.quads.back().vertices[0].x == -4 &&
         b.quads.back().vertices[0].y == 98);
  assert(call(F::Icon, {48, 334}, {}, matrix) &&
         b.quads.back().sourceFlags == 48);
  const auto glyphStart = b.quads.size();
  MessageFont font;
  assert(font.load("romfs/font.pmfont"));
  b.font = &font;
  b.label = [](uint32_t address) { return address == 123 ? "Jump" : nullptr; };
  WindowFontMeasurement measured;
  assert(measurePlainWindowFont(font, "Jump", 2, measured));
  assert(call(F::FontWidth, {123}) == measured.width);
  assert(!call(F::FontString, {123}, {}, matrix));
  unsigned rngCalls = 0;
  b.random = [&](uint32_t bound) {
    assert(bound == 64);
    ++rngCalls;
    return 0u;
  };
  assert(call(F::FontStart));
  assert(call(F::FontColor, {0x808080ff}));
  assert(call(F::FontString, {123}, {}, matrix));
  assert(rngCalls == 8 && b.quads.size() == glyphStart + 4);
  const auto &glyph = b.quads[glyphStart];
  assert(glyph.texture == BattleClassTexture::Font &&
         glyph.vertices[0].rgba == 0x808080ff);
  assert(std::fabs(glyph.vertices[0].x - (20 - measured.width * .5f)) < .0001f);
  assert(!call(F::FontWidth, {999}));
  b.begin();
  assert(b.quads.empty() && b.complete());
  assert(!call(F::Quad, {255, 255, 255, 255}, {0, 0, 1, 1}));
}
