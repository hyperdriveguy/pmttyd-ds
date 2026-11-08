#include "source_fade_startup.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Global = 0x8041e620, A = 0x90000000, B = 0x90001000,
                   C = 0x90002000, Payload = 0x90003000, Other = 0x90004000,
                   Handle = 0x90005000;
struct F {
  std::map<uint32_t, uint8_t> bytes;
  std::map<uint32_t, unsigned> writes;
  std::vector<uint32_t> order;
  std::shared_ptr<int> life = std::make_shared<int>(1);
  bool clear = false, read = false, firstRowMutation = false,
       swapPublished = false, swapCallback = false;
  uint32_t blocked = 0;
  unsigned allocations = 0, clears = 0, requests = 0, closes = 0;
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
    put(Global, A);
    put(0x8041f7a8, 0);
    put(0x8041f7ac, 0x3f800000);
    put(A + 0x35c, 77);
    put(B + 0x358, Other);
    put(B + 0x35c, 88);
    put(Handle + 0x6c, Handle);
    tpl(Payload);
    tpl(Other);
  }
  SourceFadeStartupServices services() {
    SourceFadeStartupServices s;
    auto &g = s.objects.gx;
    g.user = this;
    g.lifetime = life;
    g.current = [](void *) { return true; };
    g.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      f.order.push_back(a);
      const auto v = f.get(a);
      if (a == Handle + 0x6c && f.swapCallback) {
        f.swapCallback = false;
        f.put(Global, B);
      }
      return v;
    };
    g.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      return static_cast<F *>(p)->bytes[a];
    };
    g.store = [](void *p, uint32_t a, uint32_t n, uint32_t v) {
      auto &f = *static_cast<F *>(p);
      if (a == f.blocked)
        return false;
      f.order.push_back(a);
      ++f.writes[a];
      for (unsigned i = 0; i < n; ++i)
        f.bytes[a + i] = uint8_t(v >> (8 * (n - 1 - i)));
      if (f.firstRowMutation && a == A + 0x74)
        f.put(0x8041f7ac, 0x40000000);
      if (f.firstRowMutation && a == A + 0xac) {
        f.put(Global, B);
        f.put(0x8041f7a8, 0x3f800000);
        f.put(0x8041f7ac, 0x40400000);
      }
      if (f.swapPublished && a == A + 0x358 && v == Payload)
        f.put(Global, B);
      return true;
    };
    s.request =
        [](void *p,
           const SourceWindowTextureSetupCall &q) -> std::optional<uint32_t> {
      auto &f = *static_cast<F *>(p);
      switch (q.function) {
      case 0x800300f0:
        ++f.allocations;
        if (q.callsite == 0x80013ba4) {
          assert(q.arguments[0] == 0 && q.arguments[1] == 0x360);
          return A;
        }
        assert(q.callsite == 0x80013b34 && q.arguments[0] == 0 &&
               q.arguments[1] == 64);
        return Payload;
      case 0x800050b4:
        assert(q.arguments[0] == A && q.arguments[1] == 0 &&
               q.arguments[2] == 0x360);
        if (!f.clear)
          return {};
        for (unsigned i = 0; i < 0x360; ++i)
          f.bytes[A + i] = 0;
        ++f.clears;
        return A;
      case 0x800782e4:
        assert(q.arguments[0] == 0x802bf79c);
        return f.swapPublished ? Payload : 0;
      case 0x80007388:
        return 0x8041f494;
      case 0x8026a0b8:
        assert(q.count == 2 && q.destination && q.arguments[0] == 0x802bf7a8);
        return 10;
      case 0x80144e88:
        assert(q.path && q.count == 2);
        return Handle;
      case 0x80144dcc:
        return 33;
      case 0x80144de4:
        assert((q.arguments ==
                std::array<uint32_t, 5>{Handle, Payload, 64, 0, 0x80013ca4}));
        if (!f.read)
          return {};
        ++f.requests;
        return 0;
      case 0x80144dd4:
        assert(q.arguments[0] == Handle);
        ++f.closes;
        f.put(Global, C);
        return 0;
      default:
        assert(false);
        return {};
      }
    };
    return s;
  }
};
void originalInitCapturesAndHolds() {
  F f;
  f.firstRowMutation = true;
  auto s = f.services();
  SourceFadeStartupCursor c;
  assert(!resumeSourceFadeInit(s, c) && f.allocations == 1 &&
         f.get(Global) == A);
  assert(!resumeSourceFadeInit(s, c) && f.allocations == 1);
  f.clear = true;
  f.blocked = B + 2 * 0xa8 + 0x78;
  assert(!resumeSourceFadeInit(s, c) && f.clears == 1 &&
         f.get(A + 0x98) == 0x40000000 && f.get(B + 0xa8 + 0x98) == 0x40000000);
  const auto accepted = f.writes[B + 2 * 0xa8 + 0x74];
  assert(accepted == 1);
  assert(!resumeSourceFadeInit(s, c) &&
         f.writes[B + 2 * 0xa8 + 0x74] == accepted && f.allocations == 1);
  f.blocked = 0;
  assert(resumeSourceFadeInit(s, c) && f.get(B + 0x350) == 0xffffffff &&
         f.get(B + 0x358) == 0 && f.get(B + 0x35c) == 0);
  for (unsigned row = 1; row < 5; ++row) {
    assert(f.get(B + row * 0xa8 + 0x24) == 0 &&
           f.get(B + row * 0xa8 + 0x98) == 0x40000000);
  }
  auto at = [&](uint32_t a) {
    for (unsigned i = 0; i < f.order.size(); ++i)
      if (f.order[i] == a)
        return i;
    assert(false);
    return 0u;
  };
  assert(at(0x8041f7a8) < at(A + 0x74) && at(A + 0x74) < at(0x8041f7ac));
}
void roundedReadAndManagerFirstCallback() {
  F f;
  auto s = f.services();
  SourceFadeStartupCursor setup;
  assert(!resumeSourceFadeTextures(s, setup) && f.allocations == 1 &&
         f.get(A + 0x35c) == 77);
  assert(!resumeSourceFadeTextures(s, setup) && f.allocations == 1);
  f.read = true;
  assert(resumeSourceFadeTextures(s, setup) && f.requests == 1 &&
         f.get(A + 0x35c) == 77);
  f.swapCallback = true;
  SourceFadeStartupCursor callback;
  assert(resumeSourceFadeCompletion(Handle + 0x40, s, callback));
  assert(f.closes == 1 && f.get(Payload + 8) == Payload + 12 &&
         f.get(Other + 8) == 12 && f.get(C + 0x35c) == 1 &&
         f.get(B + 0x35c) == 88);
}
void archiveUsesFreshManager() {
  F f;
  f.swapPublished = true;
  auto s = f.services();
  SourceFadeStartupCursor c;
  assert(resumeSourceFadeTextures(s, c));
  assert(f.get(Other + 8) == Other + 12 && f.get(Payload + 8) == 12 &&
         f.get(B + 0x35c) == 1 && !f.allocations);
}
} // namespace
int main() {
  originalInitCapturesAndHolds();
  roundedReadAndManagerFirstCallback();
  archiveUsesFreshManager();
}
