#include "native_logo_startup.hpp"
#include <cassert>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace &m, uint32_t a) {
  uint32_t v;
  assert(m.readWord(a, v));
  return v;
}
EventNativePlatform platform() {
  EventNativePlatform p;
  p.flushBlock = [](void *, uint32_t) {};
  p.systemCall = [](void *) {};
  p.truncate = [](void *, float v) { return int32_t(v); };
  p.subtractDouble = [](void *, double a, double b) {
    volatile double v = a - b;
    return v;
  };
  p.roundDoubleFloat = [](void *, double v) {
    volatile float f = float(v);
    return f;
  };
  return p;
}
void realLogoFileLifetime() {
  auto animation = NativeFieldAnimation::create(
      "romfs", NativeFileCache::create(), platform());
  assert(animation);
  assert(animation->files()->contains("./mariost.tpl", 4));
  auto ui = NativeUiStartup::create(animation, "romfs");
  assert(ui && ui->resumeFont());
  auto logo = NativeLogoStartup::bind(ui, "romfs");
  assert(logo && logo->current());
  // No platform provider: the entire real resource prefix executes, then holds
  // at OSGetResetCode. This is not a native reset/VI/source retrace receipt.
  assert(!logo->initialize() && logo->pending() &&
         logo->pendingAddress() == 0x8029b544 && !logo->initialized());
  auto &m = animation->process().vm().memory;
  const auto handle = logo->fileHandle();
  uint16_t refs;
  assert(handle && word(m, 0x8041ea80) == handle &&
         word(m, 0x8041ea84) == 0xffffffff);
  assert(m.readHalf(handle + 2, refs) && refs == 1);
  const auto palette = word(m, word(m, handle + 0xa0));
  assert(word(m, palette) == 0x0020af30 && word(m, palette + 4) == 26 &&
         word(m, palette + 8) > palette);
  assert(!logo->initialize() && logo->fileHandle() == handle &&
         m.readHalf(handle + 2, refs) && refs == 1);
  assert(!logo->exit());
  struct Backend {
    unsigned video = 0, textures = 0;
    bool live = true, exitReady = false, retraceCompleted = false;
  } backend;
  NativeLogoChildren children;
  children.user = &backend;
  children.lifetime = std::make_shared<int>(1);
  children.current = [](void *p) { return static_cast<Backend *>(p)->live; };
  children.call = [](void *p, BattleSequenceCall &q) {
    auto &b = *static_cast<Backend *>(p);
    switch (q.function) {
    case 0x8029b544:
    case 0x802a5088:
    case 0x8029c250:
      q.result = 0;
      return true;
    case 0x802a4420:
      assert(q.args[0] == 0x8030c230);
      ++b.video;
      return true;
    case 0x802a4c28:
      ++b.video;
      return true;
    case 0x802a3f58:
      // Completion may retire its provider. Retain its completed prefix across
      // the native post-call admission hold without repeating the retrace.
      if (!b.retraceCompleted) {
        ++b.video;
        b.retraceCompleted = true;
        b.live = false;
      }
      return true;
    default:
      // TEST-only whole-child receipts to exercise the file owner/Exit barrier.
      // Actual source texture and Mario providers are separately required.
      if (!b.exitReady)
        return false;
      ++b.textures;
      return true;
    }
  };
  assert(logo->configureChildren(children) &&
         !logo->configureChildren(children));
  assert(!logo->initialize() && !logo->current() &&
         logo->pendingAddress() == 0x802a3f58 && backend.video == 3);
  backend.live = true;
  assert(logo->initialize() && logo->initialized() && backend.video == 3);
  assert(!logo->exit() && logo->pendingAddress() == 0x80077bdc &&
         m.readHalf(handle + 2, refs) && refs == 1);
  backend.live = false;
  assert(!logo->current() && !logo->exit());
  backend.live = true;
  backend.exitReady = true;
  assert(logo->exit() && logo->exited() && backend.textures == 7 &&
         m.readHalf(handle + 2, refs) && refs == 0);
  assert(logo->exit() && backend.textures == 7 &&
         m.readHalf(handle + 2, refs) && refs == 0);
  assert(
      !NativeLogoStartup::bind(ui, "romfs")); // No reset/reimport of live work.
  // Root is borrowed from the existing process and needs its own backing proof.
  std::vector<uint8_t> oldRoot;
  assert(m.unregisterRam(0x8041f494, &oldRoot));
  auto replacementRoot = oldRoot;
  assert(m.registerRam(0x8041f494, std::move(replacementRoot)));
  assert(!logo->current() && !logo->exited() && !logo->exit());
}
} // namespace
int main() { realLogoFileLifetime(); }
