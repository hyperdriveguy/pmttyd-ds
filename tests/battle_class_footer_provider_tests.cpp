#include "battle_class_footer_provider.hpp"
#include "battle_class_publication.hpp"
#include "file_texture_relocation.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iterator>
#include <map>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t Palette = 0x87100000, Camera = 0x90000,
                            Battle = 0x84000000;
  BattleClassFooterGpuResource resource;
  BattleClassAssets assets;
  BattleClassBackend batch;
  std::shared_ptr<const AnimationTextureResource> original;
  std::map<uint32_t, uint32_t> words;
  bool sinkReady = true, cameraReady = true, textureReady = true;
  unsigned emitted = 0, textureQueries = 0, cameraQueries = 0;
  BattleClassFooterTextureServices import() {
    return {this,
            [](void *p, const char *)
                -> std::optional<BattleClassFooterTextureAdmission> {
              return BattleClassFooterTextureAdmission{p, 512, 256, 0};
            },
            [](void *, const void *) {}};
  }
  BattleClassFooterQuadSink sink() {
    return {this, [](void *p, const BattleClassFooterAtlasQuad &q) {
              auto &f = *static_cast<Fixture *>(p);
              if (!f.sinkReady)
                return false;
              ++f.emitted;
              return f.batch.appendFooter(q);
            }};
  }
  BattleClassFooterQueryServices query() {
    return {
        this,
        [](void *p, std::optional<uint32_t> index) -> std::optional<uint32_t> {
          auto &f = *static_cast<Fixture *>(p);
          ++f.cameraQueries;
          assert(!index || *index == 8);
          return f.cameraReady ? std::optional<uint32_t>(Camera) : std::nullopt;
        },
        [](void *p, uint32_t palette,
           uint32_t index) -> std::optional<AnimationTPLImage> {
          auto &f = *static_cast<Fixture *>(p);
          ++f.textureQueries;
          return f.textureReady && palette == Palette ? f.original->image(index)
                                                      : std::nullopt;
        }};
  }
  void floating(uint32_t a, float v) {
    uint32_t n;
    std::memcpy(&n, &v, 4);
    words[a] = n;
  }
  Fixture(const char *root, const char *tpl) {
    assert(resource.load(root, import()));
    assert(assets.load("romfs/battle-class-surfaces.bin",
                       "romfs/battle-class-icons.bin"));
    batch.assets = &assets;
    original = AnimationTextureResource::load(tpl);
    assert(original && original->count() == 97);
    words[0x8041eb90] = Battle;
    words[Battle + 0x163f8] = 0x80000;
    words[0x800a0] = 0x81000;
    words[0x81000] = Palette;
    floating(0x80422234, .5f);
    floating(0x80422240, 0);
    floating(0x8042224c, 1);
    words[0x802ee400] = 0x43300000;
    words[0x802ee404] = 0;
    for (unsigned i = 0; i < 12; ++i)
      floating(Camera + 0x11c + 4 * i, (i == 0 || i == 5 || i == 10) ? 1 : 0);
  }
  BattleTexturePlaneServices services(BattleClassFooterProvider &provider) {
    BattleTexturePlaneServices s;
    s.actors.memory.user = this;
    s.actors.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words.at(a);
    };
    s.user = &provider;
    s.request = [](void *p, const BattleTexturePlaneRequest &q) {
      return static_cast<BattleClassFooterProvider *>(p)->request(q);
    };
    s.matrix = [](void *, BattleAudienceMatrixRequest &q) {
      if (q.function == 0x8029dd30) {
        q.output = q.left;
        return true;
      }
      assert(q.function == 0x8029dd64);
      for (unsigned r = 0; r < 3; ++r)
        for (unsigned c = 0; c < 4; ++c) {
          float v = c == 3 ? q.left[r * 4 + 3] : 0;
          for (unsigned k = 0; k < 3; ++k)
            v += q.left[r * 4 + k] * q.right[k * 4 + c];
          q.output[r * 4 + c] = v;
        }
      return true;
    };
    return s;
  }
};

void originalMemoryWitness(const char *path) {
  std::ifstream file(path, std::ios::binary);
  assert(file);
  std::vector<uint8_t> raw{std::istreambuf_iterator<char>(file), {}};
  assert(raw.size() == 217920);
  std::map<uint32_t, uint8_t> bytes;
  constexpr uint32_t P = Fixture::Palette, B = Fixture::Battle;
  for (unsigned i = 0; i < raw.size(); ++i)
    bytes[P + i] = raw[i];
  auto put = [&](uint32_t a, uint32_t n) {
    for (unsigned i = 0; i < 4; ++i)
      bytes[a + i] = uint8_t(n >> (24 - 8 * i));
  };
  put(0x8041eb90, B);
  put(B + 0x163f8, 0x80000);
  put(0x800a0, 0x81000);
  put(0x81000, P);
  FileTextureRelocationServices relocation;
  relocation.memory.user = &bytes;
  relocation.memory.word = [](void *p, uint32_t a) {
    auto &b = *static_cast<std::map<uint32_t, uint8_t> *>(p);
    uint32_t v = 0;
    for (unsigned i = 0; i < 4; ++i)
      v = (v << 8) | b.at(a + i);
    return v;
  };
  relocation.memory.storeWord = [](void *p, uint32_t a, uint32_t n) {
    auto &b = *static_cast<std::map<uint32_t, uint8_t> *>(p);
    for (unsigned i = 0; i < 4; ++i)
      b[a + i] = uint8_t(n >> (24 - 8 * i));
  };
  relocation.memory.storeByte = [](void *p, uint32_t a, uint8_t n) {
    static_cast<std::map<uint32_t, uint8_t> *>(p)->at(a) = n;
  };
  relocation.byte = [](void *p, uint32_t a) {
    return static_cast<std::map<uint32_t, uint8_t> *>(p)->at(a);
  };
  FileTextureRelocationCursor c;
  assert(resumePackedFileTextureRelocation(P, relocation, c));
  BattleClassFooterMemoryReader reader{
      &bytes, [](void *p, uint32_t a) -> std::optional<uint32_t> {
        auto &b = *static_cast<std::map<uint32_t, uint8_t> *>(p);
        uint32_t n = 0;
        for (unsigned i = 0; i < 4; ++i) {
          auto v = b.find(a + i);
          if (v == b.end())
            return {};
          n = (n << 8) | v->second;
        }
        return n;
      }};
  const auto original = AnimationTextureResource::load(path);
  assert(original);
  for (unsigned i = 4; i <= 16; ++i) {
    const auto image = readBattleClassFooterTexture(reader, P, i),
               source = original->image(i);
    assert(image && source && image->imageHeader == P + source->imageHeader &&
           image->data == P + source->data && image->width == source->width &&
           image->height == source->height && image->format == 14 &&
           image->wrapS == source->wrapS &&
           image->minFilter == source->minFilter);
  }
  assert(!readBattleClassFooterTexture(reader, P + 4, 4));
  assert(!readBattleClassFooterTexture(reader, P, 3));
  const auto image = readBattleClassFooterTexture(reader, P, 4);
  assert(image);
  put(image->imageHeader + 4, 0);
  assert(!readBattleClassFooterTexture(reader, P, 4));
  put(image->imageHeader + 4, 14);
  bytes.erase(B + 0x163f8);
  assert(!readBattleClassFooterTexture(reader, P, 4));
}

int main(int argc, char **argv) {
  const char *root = argc > 1 ? argv[1] : "romfs";
  const char *tpl = argc > 2 ? argv[2] : "romfs/battle-class-common.tpl";
  originalMemoryWitness(tpl);
  Fixture f(root, tpl);
  BattleClassFooterProvider provider(f.resource, f.query(), f.sink());
  BattleActMatrix identity{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
  auto raster = [&](BattleClassRasterService function,
                    std::vector<uint32_t> integers = {},
                    std::vector<float> reals = {},
                    std::optional<BattleActMatrix> matrix = {}) {
    return provider.rasterRequest({function, integers, reals, matrix}, f.batch);
  };
  using R = BattleClassRasterService;
  assert(raster(R::LoadMatrix, {0}, {}, identity));
  assert(raster(R::TextureObject, {Fixture::Palette, 0}));
  assert(raster(R::LoadTexture, {0, 0}));
  assert(raster(R::Quad, {255, 255, 255, 255}, {-20, 20, 20, -20}));
  assert(raster(R::Init2D));
  assert(raster(R::Camera, {8}) == Fixture::Camera);
  assert(!raster(R::TextureObject, {Fixture::Palette + 1, 6}));
  f.textureReady = false;
  assert(!raster(R::TextureObject, {Fixture::Palette, 6}));
  f.textureReady = true;
  assert(raster(R::TextureObject, {Fixture::Palette, 6}) == 6);
  assert(raster(R::LoadTexture, {6, 0}));
  assert(raster(R::LoadMatrix, {0}, {}, identity));
  f.sinkReady = false;
  assert(!raster(R::Quad, {255, 233, 64, 255}, {-28, 12, 28, -12}));
  assert(f.batch.quads.size() == 1);
  f.sinkReady = true;
  assert(raster(R::Quad, {255, 233, 64, 255}, {-28, 12, 28, -12}));
  assert(f.batch.quads.size() == 2 &&
         f.batch.quads.back().texture == BattleClassTexture::Footer);
  assert(f.batch.quads.back().footer->vertices[0].rgba == 0xffe940ff);
  assert(raster(R::TextureObject, {Fixture::Palette, 0}));
  assert(raster(R::LoadTexture, {0, 0}));
  assert(raster(R::Quad, {255, 255, 255, 255}, {-20, 20, 20, -20}));
  assert(f.batch.quads.back().texture == BattleClassTexture::Surfaces);
  auto services = f.services(provider);
  BattleTexturePlaneCursor cursor;
  const uint32_t rgba = 0x11223344;
  f.sinkReady = false;
  assert(!resumePackedBattleTexturePlane2(identity, 4, rgba, services, cursor));
  assert(cursor.active() && f.batch.quads.size() == 3);
  const auto queries = f.textureQueries;
  assert(!resumePackedBattleTexturePlane2(identity, 4, rgba, services, cursor));
  assert(f.textureQueries == queries);
  f.sinkReady = true;
  assert(resumePackedBattleTexturePlane2(identity, 4, rgba, services, cursor));
  assert(!cursor.active() && f.batch.quads.size() == 4 && f.emitted == 2);
  const auto packet = f.batch.quads.back().footer;
  assert(packet && packet->index == 4 &&
         packet->rgb == BattleClassFooterRGB::Modulate);
  assert(packet->vertices[0].x == -40 && packet->vertices[0].y == 16 &&
         packet->vertices[2].x == 40 && packet->vertices[2].y == -16);
  for (const auto &v : packet->vertices)
    assert(v.rgba == rgba);
  assert(f.resource.accepts(*packet));
  assert(validateBattleClassFooterBatch(f.batch, &f.resource));
  assert(!validateBattleClassFooterBatch(f.batch, nullptr));
  BattleClassPublication publication;
  auto owner = std::make_shared<const uint8_t>(0);
  publication.bind(owner, 0x1234);
  assert(publication.publish({owner, 0x1234, 1}, f.batch));
  auto copied = publication.batch();
  assert(copied->quads.back().footer == packet);
  f.batch.quads.clear();
  assert(copied->quads.size() == 4 &&
         validateBattleClassFooterBatch(*copied, &f.resource));
  f.resource.retire();
  assert(!provider.request({}));
  assert(!f.resource.accepts(*packet));
  assert(!validateBattleClassFooterBatch(*copied, &f.resource));
}
