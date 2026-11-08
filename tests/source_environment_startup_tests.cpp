#include "source_environment_startup.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Palette = 0x8041ec90, Ready = 0x8041ec94,
                   Handle = 0x90001000, Payload = 0x90002000,
                   Other = 0x90003000;
struct F {
  std::map<uint32_t, uint8_t> bytes;
  std::shared_ptr<int> life = std::make_shared<int>(1);
  bool live = true, secondClear = false, archive = false, holdArchive = true,
       read = false, mutateCallback = false, rewritePublished = false;
  unsigned clears = 0, allocations = 0, requests = 0, closes = 0;
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
  void tpl(uint32_t a) {
    put(a, 0x0020af30);
    put(a + 4, 0);
    put(a + 8, 12);
  }
  F() {
    put(Palette, 0x12345678);
    put(Ready, 77);
    for (uint32_t a = 0x803e7278; a < 0x803e7458; ++a)
      bytes[a] = 0xaa;
    tpl(Payload);
    tpl(Other);
    put(Handle + 0x6c, Handle);
  }
  SourceEnvironmentStartupServices services() {
    SourceEnvironmentStartupServices s;
    auto &g = s.objects.gx;
    g.user = this;
    g.lifetime = life;
    g.current = [](void *p) { return static_cast<F *>(p)->live; };
    g.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      auto value = f.get(a);
      if (a == Handle + 0x6c && f.mutateCallback) {
        f.mutateCallback = false;
        f.put(Palette, Other);
      }
      return value;
    };
    g.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      return static_cast<F *>(p)->bytes[a];
    };
    g.store = [](void *p, uint32_t a, uint32_t n, uint32_t v) {
      auto &f = *static_cast<F *>(p);
      for (unsigned i = 0; i < n; ++i)
        f.bytes[a + i] = uint8_t(v >> (8 * (n - 1 - i)));
      if (a == Palette && v == Payload && f.rewritePublished)
        f.put(Palette, 0xdead0000);
      return true;
    };
    s.request =
        [](void *p,
           const SourceWindowTextureSetupCall &q) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      switch (q.function) {
      case 0x800050b4:
        assert(q.count == 3 && q.arguments[1] == 0 && q.arguments[2] == 0xf0);
        if (q.arguments[0] == 0x803e7368 && !f.secondClear)
          return {};
        for (uint32_t i = 0; i < 0xf0; ++i)
          f.bytes[q.arguments[0] + i] = 0;
        ++f.clears;
        return q.arguments[0];
      case 0x800782e4:
        assert(q.arguments[0] == 0x802f9c78);
        if (f.holdArchive)
          return {};
        return f.archive ? Payload : 0;
      case 0x80007388:
        return 0x8041f494;
      case 0x8026a0b8:
        assert(q.count == 2 && q.arguments[0] == 0x802f9c80 && q.destination);
        return 9;
      case 0x80144e88:
        assert(q.path && q.count == 2);
        return Handle;
      case 0x80144dcc:
        return 33; // Deliberately not32-aligned: allocate64, READ33.
      case 0x800300f0:
        assert(q.count == 2 && q.arguments[0] == 0 && q.arguments[1] == 64);
        ++f.allocations;
        return Payload;
      case 0x80144de4:
        assert((q.arguments ==
               std::array<uint32_t, 5>({Handle, Payload, 33, 0, 0x8019e0f0})));
        if (!f.read)
          return {};
        ++f.requests;
        return 0;
      case 0x80144dd4:
        assert(q.arguments[0] == Handle);
        ++f.closes;
        return 0;
      default:
        assert(false);
        return {};
      }
    };
    return s;
  }
};
void onceOnlyClears() {
  F f;
  auto s = f.services();
  SourceEnvironmentStartupCursor c;
  assert(!resumeSourceEnvironmentInit(s, c) && f.clears == 1 &&
         f.get(Palette) == 0x12345678 && f.get(Ready) == 77);
  assert(f.get(0x803e7278) == 0 && f.get(0x803e7368) == 0xaaaaaaaa);
  assert(!resumeSourceEnvironmentInit(s, c) && f.clears == 1);
  f.secondClear = true;
  assert(resumeSourceEnvironmentInit(s, c) && f.clears == 2 &&
         f.get(Palette) == 0 && f.get(Ready) == 0);
}
void logicalReadAndCallbackOrder() {
  F f;
  auto s = f.services();
  SourceEnvironmentStartupCursor setup;
  assert(!resumeSourceEnvironmentTextures(s, setup) && f.get(Palette) == 0 &&
         f.get(Ready) == 77);
  f.holdArchive = false;
  assert(!resumeSourceEnvironmentTextures(s, setup) && f.allocations == 1 &&
         f.get(Ready) == 77 && setup.pending() == 0x80144de4);
  assert(!resumeSourceEnvironmentTextures(s, setup) && f.allocations == 1);
  f.read = true;
  assert(resumeSourceEnvironmentTextures(s, setup) && f.requests == 1 &&
         f.get(Ready) == 77);
  f.mutateCallback = true;
  SourceEnvironmentStartupCursor callback;
  assert(resumeSourceEnvironmentCompletion(Handle + 0x40, s, callback));
  assert(f.closes == 1 && f.get(Other + 8) == Other + 12 &&
         f.get(Payload + 8) == 12 && f.get(Ready) == 1);
}
void retainedArchiveReturn() {
  F f;
  f.archive = true;
  f.holdArchive = false;
  f.rewritePublished = true;
  auto s = f.services();
  SourceEnvironmentStartupCursor c;
  assert(resumeSourceEnvironmentTextures(s, c));
  assert(f.get(Palette) == 0xdead0000 && f.get(Payload + 8) == Payload + 12 &&
         f.get(Ready) == 1 && !f.allocations && !f.requests);
}
} // namespace
int main() {
  onceOnlyClears();
  logicalReadAndCallbackOrder();
  retainedArchiveReturn();
}
