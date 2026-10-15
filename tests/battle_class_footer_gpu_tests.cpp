#include "battle_class_footer_gpu.hpp"
#include <cassert>
#include <limits>
#include <vector>
using namespace ttyd;
struct Import {
  unsigned imports = 0, releases = 0;
  bool available = true;
  uint32_t width = 512, height = 256, format = 0;
  BattleClassFooterTextureServices services() {
    return {this,
            [](void *p, const char *path)
                -> std::optional<BattleClassFooterTextureAdmission> {
              auto &f = *static_cast<Import *>(p);
              ++f.imports;
              assert(std::string(path).find("battle-class-footers.t3x") !=
                     std::string::npos);
              if (!f.available)
                return {};
              return BattleClassFooterTextureAdmission{p, f.width, f.height,
                                                       f.format};
            },
            [](void *p, const void *id) {
              auto &f = *static_cast<Import *>(p);
              assert(id == p);
              ++f.releases;
            }};
  }
};
struct Sink {
  bool available = true;
  unsigned tries = 0;
  std::vector<BattleClassFooterAtlasQuad> quads;
  BattleClassFooterQuadSink services() {
    return {this, [](void *p, const BattleClassFooterAtlasQuad &q) {
              auto &s = *static_cast<Sink *>(p);
              ++s.tries;
              if (!s.available)
                return false;
              s.quads.push_back(q);
              return true;
            }};
  }
};
int main(int argc, char **argv) {
  const char *root = argc > 1 ? argv[1] : "romfs";
  Import importer;
  BattleClassFooterGpuResource resource;
  auto s = importer.services();
  assert(!resource.load("/no/footer", s) && !importer.imports);
  auto missing = s;
  missing.release = nullptr;
  assert(!resource.load(root, missing) && !importer.imports);
  importer.available = false;
  assert(!resource.load(root, s) && !resource.ready());
  importer.available = true;
  importer.width = 256;
  assert(!resource.load(root, s) && importer.releases == 1);
  importer.width = 512;
  importer.format = 14;
  assert(!resource.load(root, s) && importer.releases == 2);
  importer.format = 0;
  assert(resource.load(root, s) && resource.ready());
  const auto imports = importer.imports;
  assert(!resource.load(root, s) && importer.imports == imports);
  assert(resource.identity() == &importer && resource.surface(4) &&
         !resource.surface(3));
  BattleActMatrix matrix{1, 0, 0, 10, 0, 1, 0, 20, 0, 0, 1, 30};
  const std::array<std::array<float, 3>, 4> positions{
      {{-40, 16, 0}, {40, 16, 0}, {40, -16, 0}, {-40, -16, 0}}};
  auto uv =
      std::array<std::array<float, 2>, 4>{{{0, 0}, {1, 0}, {1, 1}, {0, 1}}};
  const std::array<uint32_t, 4> colors{
      {0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff00}};
  Sink sink;
  sink.available = false;
  assert(!resource.quad(4, matrix, positions, uv, colors,
                        BattleClassFooterRGB::Modulate, sink.services()) &&
         sink.quads.empty());
  sink.available = true;
  assert(resource.quad(4, matrix, positions, uv, colors,
                       BattleClassFooterRGB::Modulate, sink.services()));
  const auto first = sink.quads.front();
  assert(resource.accepts(first));
  assert(first.texture == &importer && first.index == 4 &&
         first.vertices[0].x == -30 && first.vertices[0].y == 36 &&
         first.vertices[0].z == 30);
  const auto *region = resource.surface(4);
  assert(
      first.vertices[0].u == region->u0 && first.vertices[0].v == region->v0 &&
      first.vertices[2].u == region->u1 && first.vertices[2].v == region->v1);
  for (unsigned i = 0; i < 4; ++i)
    assert(first.vertices[i].rgba == colors[i]);
  assert(resource.quad(11, matrix, positions, uv, colors,
                       BattleClassFooterRGB::Raster, sink.services()));
  assert(sink.quads.back().rgb == BattleClassFooterRGB::Raster);
  const auto attempts = sink.tries;
  assert(!resource.quad(17, matrix, positions, uv, colors,
                        BattleClassFooterRGB::Modulate, sink.services()));
  uv[0][0] = 2;
  assert(!resource.quad(4, matrix, positions, uv, colors,
                        BattleClassFooterRGB::Modulate, sink.services()));
  uv[0][0] = 0;
  matrix[0] = std::numeric_limits<float>::quiet_NaN();
  assert(!resource.quad(4, matrix, positions, uv, colors,
                        BattleClassFooterRGB::Modulate, sink.services()));
  assert(sink.tries == attempts);
  resource.retire();
  assert(!resource.ready() && !resource.surface(4) && importer.releases == 3);
  resource.retire();
  assert(importer.releases == 3);
  assert(!resource.accepts(first));
  assert(resource.load(root, s));
  assert(resource.identity() == first.texture &&
         resource.generation() != first.generation);
  assert(!resource.accepts(first));
  resource.retire();
  // Copied geometry remains stable, but retired native identity cannot be
  // drawn.
  assert(first.vertices[0].x == -30);
}
