#include "battle_audience_aux_callback.hpp"
#include "event_gp_addresses.hpp"
#include "source_icon_matrix.hpp"
#include "source_icon_resources.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
int main() {
  SourceIconResources r;
  assert(!r.ready() && !r.word(0x8041e638) &&
         !r.half(SourceIconResources::Directory));
  assert(!r.load("romfs/missing-source-icons"));
  assert(r.load("romfs/source-icons") && r.count() == 613);
  assert(!r.load("romfs/source-icons"));
  assert(r.word(0x8041e634) == SourceIconResources::Directory &&
         r.word(0x8041e630) == SourceIconResources::Palette);
  assert(r.half(SourceIconResources::Directory) == 572 &&
         !r.half(SourceIconResources::Directory + 65536));
  assert(r.word(0x8041f864) == 0xffffffff && r.word(0x8041f86c) == 0x555555ff);
  for (unsigned i = 0; i < 6; ++i) {
    assert(r.half(0x80415040 + 2 * i) ==
           std::array<uint16_t, 6>{73, 74, 75, 76, 73, 77}[i]);
    assert(r.half(0x8041504c + 2 * i) ==
           std::array<uint16_t, 6>{103, 104, 105, 106, 103, 107}[i]);
  }
  SourceIconGXCall call;
  call.function = 0x802b7880;
  call.integers = {1};
  assert(!r.request(call, nullptr, nullptr));
  for (uint32_t index = 0; index < r.count(); ++index) {
    const auto image = r.image(index);
    assert(image && image->width && image->height);
    call = {};
    call.function = 0x80037a80;
    call.integers = {SourceIconResources::Palette, index};
    const auto token = r.request(call, nullptr, nullptr);
    assert(token == index + 1);
    call = {};
    call.function = 0x802b7880;
    call.integers = {*token};
    assert(!r.request(call, nullptr, [](void *, uint32_t) {
      assert(false);
      return true;
    })); // LOD not installed.
    call.function = 0x802b7544;
    call.integers = {*token, 1, 1};
    assert(r.request(call, nullptr, nullptr) == 0);
    call.function = 0x802b7880;
    call.integers = {*token};
    assert(!r.request(call, nullptr, [](void *, uint32_t) { return false; }));
    uint32_t expected = index;
    assert(r.request(call, &expected, [](void *u, uint32_t index) {
      assert(index == *static_cast<uint32_t *>(u));
      return true;
    }) == 0);
    call.function = 0x802b76c4;
    assert(r.request(call, nullptr, nullptr) == image->width);
    call.function = 0x802b76d4;
    assert(r.request(call, nullptr, nullptr) == image->height);
  }
  call = {};
  call.function = 0x80037a80;
  call.integers = {SourceIconResources::Palette, 613};
  assert(!r.request(call, nullptr, nullptr));
  call.integers = {SourceIconResources::Palette, 1, 1};
  assert(!r.request(call, nullptr, nullptr));
  call = {};
  call.function = 0x802b7544;
  call.integers = {1, 1, 1};
  call.reals[0] = 1;
  assert(!r.request(call, nullptr, nullptr));
  CameraManager cameras;
  cameras.initializeBoot(608, 480);
  EventGlobals globals;
  EventGpAddresses gp;
  assert(!sourceIconCameraIdentity(&cameras) &&
         !sourceIconCameraIdentity(&cameras, 13));
  assert(sourceIconCameraIdentity(&cameras, 8) == 0x84002000);
  assert(gp.registerGp(0x81000000, globals) &&
         gp.writeWord(0x80415fd0, 0x81000000));
  assert(gp.writeWord(0x8100001c, 17) && gp.writeWord(0x8100016c, 2));
  assert(sourceIconRuntimeWord(&cameras, gp, 0x8100001c) == 17);
  assert(!sourceIconRuntimeWord(&cameras, gp, 0x8400211c));
  auto *camera = cameras.camera(8);
  camera->viewPublished = true;
  camera->matrices.view = {{{1, 0, 0, 10}, {0, 1, 0, 20}, {0, 0, 1, 30}}};
  uint32_t ten;
  float value = 10;
  std::memcpy(&ten, &value, 4);
  assert(sourceIconRuntimeWord(&cameras, gp, 0x84002128) == ten);
  // Actual original directory/aliases/TPL descriptors feed the original
  // wrapper, using actual retained GP bytes and a published camera view.
  struct Bridge {
    SourceIconResources *resource;
    CameraManager *cameras;
    EventGpAddresses *gp;
    SourceMatrixIcon emitted;
    BattleActMatrix matrix;
    unsigned draws = 0;
  } bridge{&r, &cameras, &gp};
  SourceIconMatrixServices s;
  s.user = &bridge;
  s.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
    auto &b = *static_cast<Bridge *>(u);
    const auto value = b.resource->word(a);
    return value ? value : sourceIconRuntimeWord(b.cameras, *b.gp, a);
  };
  s.half = [](void *u, uint32_t a) {
    return static_cast<Bridge *>(u)->resource->half(a);
  };
  s.currentCamera = [](void *u) {
    return sourceIconCameraIdentity(static_cast<Bridge *>(u)->cameras, 8);
  };
  s.matrix = [](void *, BattleAudienceMatrixRequest &m) {
    BattleAudienceAuxServices math;
    bindAudienceAuxMathAdapter(math);
    return math.matrix(nullptr, m);
  };
  s.draw = [](void *u, const BattleActMatrix &m, const SourceMatrixIcon &icon) {
    auto &b = *static_cast<Bridge *>(u);
    b.emitted = icon;
    b.matrix = m;
    ++b.draws;
    return true;
  };
  BattleAudienceIconRequest request;
  request.function = 0x8001a6e0;
  request.icon = 73;
  request.matrix = {1, 0, 0, 3, 0, 1, 0, 4, 0, 0, 1, 5};
  SourceIconMatrixCursor cursor;
  assert(resumeSourceIconMatrix(request, s, cursor) && bridge.draws == 1 &&
         bridge.emitted.id == 73);
  const auto descriptor =
      SourceIconResources::Directory +
      *r.half(SourceIconResources::Directory + 2 * (75 + 1));
  assert(bridge.emitted.width == r.half(descriptor + 2) &&
         bridge.emitted.height == r.half(descriptor + 4));
  assert(bridge.matrix[3] == 13 && bridge.matrix[7] == 24 &&
         bridge.matrix[11] == 35);
  assert(sourceIconRuntimeWord(&cameras, gp, 0x8100001c) == 17);
}
