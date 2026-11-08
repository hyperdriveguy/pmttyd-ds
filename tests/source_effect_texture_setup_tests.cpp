#include "source_effect_texture_setup.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct F {
  std::map<uint32_t, uint8_t> bytes;
  std::vector<SourceWindowTextureSetupCall> calls;
  std::shared_ptr<int> life = std::make_shared<int>(1);
  bool live = true, holdArchive = true, holdRead = true;
  unsigned allocations = 0;
  uint32_t get(uint32_t a) {
    uint32_t v = 0;
    for (unsigned i = 0; i < 4; ++i)
      v = v << 8 | bytes[a + i];
    return v;
  }
  void put(uint32_t a, uint32_t v) {
    for (unsigned i = 0; i < 4; ++i)
      bytes[a + i] = uint8_t(v >> (24 - 8 * i));
  }
  F() {
    put(0x80415ee0, 0x90000000);
    put(0x80419a40, 0x90000100);
    put(0x80415fd0, 0x90000200);
    put(0x90000300, 0x0020af30);
    put(0x90000304, 0);
    put(0x90000308, 12);
  }
  SourceEffectTextureSetupServices services() {
    SourceEffectTextureSetupServices s;
    auto &g = s.objects.gx;
    g.user = this;
    g.lifetime = life;
    g.current = [](void *p) { return static_cast<F *>(p)->live; };
    g.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      return static_cast<F *>(p)->get(a);
    };
    g.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      return static_cast<F *>(p)->bytes[a];
    };
    g.store = [](void *p, uint32_t a, uint32_t n, uint32_t v) {
      auto &f = *static_cast<F *>(p);
      for (unsigned i = 0; i < n; ++i)
        f.bytes[a + i] = uint8_t(v >> (8 * (n - 1 - i)));
      return true;
    };
    s.request =
        [](void *p,
           const SourceWindowTextureSetupCall &q) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      if (q.function == 0x800782e4 && f.holdArchive)
        return {};
      if (q.function == 0x80144de4 && f.holdRead)
        return {};
      f.calls.push_back(q);
      switch (q.function) {
      case 0x800782e4:
        return q.arguments[0] == 0x802c1f38 ? 0x90000300 : 0;
      case 0x80007388:
        return 0x8041f494;
      case 0x8026a0b8:
        assert(q.destination && q.count == 3);
        assert(q.arguments[2] == 0x80425b0c);
        return 1;
      case 0x80144e88:
        assert(q.path);
        return 0x90000400;
      case 0x80144dcc:
        return 33;
      case 0x800300f0:
        assert(q.arguments[1] == 64);
        ++f.allocations;
        return 0x90000500;
      case 0x80144de4:
        assert(q.arguments[4] == 0x801d7a58);
        return 0;
      case 0x80144dd4:
        assert(q.arguments[0] == 0x90000400);
        f.put(0x80419a40, 0x90000600);
        return 0;
      default:
        assert(false);
        return {};
      }
    };
    return s;
  }
};
void retainedSetupAndIndependentCallback() {
  F f;
  SourceEffectTextureSetupCursor setup;
  auto s = f.services();
  f.put(0x90000200 + 0x16c, 7);
  assert(!resumeSourceEffectTextureSetup(s, setup) &&
         setup.pending() == 0x800782e4 && f.get(0x90000008) == 0 &&
         f.get(0x9000000c) == 0);
  f.holdArchive = false;
  assert(!resumeSourceEffectTextureSetup(s, setup) &&
         setup.pending() == 0x80144de4);
  assert(f.allocations == 1 && f.get(0x9000000c) == 1 &&
         f.get(0x90000308) == 0x9000030c && f.get(0x90000100) == 0x90000500 &&
         f.get(0x90000104) == 0);
  assert(!resumeSourceEffectTextureSetup(s, setup) && f.allocations == 1);
  f.holdRead = false;
  assert(resumeSourceEffectTextureSetup(s, setup) && !setup.active() &&
         f.get(0x90000104) == 0);
  // Real callback userdata captures handle before unpack and reloads the
  // manager after close. The test provider changes that pointer during close.
  f.put(0x90000400 + 0x6c, 0x90000400);
  f.put(0x90000500, 0x0020af30);
  f.put(0x90000504, 0);
  f.put(0x90000508, 12);
  SourceEffectTextureSetupCursor callback;
  assert(resumeSourceEffectTextureN64Completion(0x90000440, s, callback));
  assert(f.get(0x90000604) == 1 && f.get(0x90000104) == 0 &&
         f.get(0x90000508) == 0x9000050c);
}
} // namespace
int main() { retainedSetupAndIndependentCallback(); }
