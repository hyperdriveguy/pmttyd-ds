#include "native_effect_process.hpp"
#include "native_ui_startup.hpp"
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
void actualFadeBeforeEffectInit() {
  auto process = NativeFieldAnimation::create(
      "romfs", NativeFileCache::create(), platform());
  assert(process && process->ready());
  auto ui = NativeUiStartup::create(process, "romfs");
  assert(ui && ui->resumeFont());
  auto effects = NativeEffectProcess::create(ui, "romfs");
  assert(effects && !effects->poolReady() && effects->initializeFade());
  auto &m = process->process().vm().memory;
  const auto manager = word(m, 0x8041e620);
  assert(manager);
  for (unsigned row = 0; row < 5; ++row) {
    assert(word(m, manager + row * 0xa8 + 0x74) == 0xffffffff &&
           word(m, manager + row * 0xa8 + 0x98) == 0x3f800000);
  }
  assert(word(m, manager + 0x350) == 0xffffffff &&
         word(m, manager + 0x358) == 0 && word(m, manager + 0x35c) == 0);
  assert(m.writeWord(manager + 0x350, 7) && effects->initializeFade() &&
         word(m, manager + 0x350) == 7 && !effects->poolReady());
  assert(effects->initialize() && effects->initializeEnvironment() &&
         !effects->beginFadeTextures());
  assert(ui->resumeWindow() && m.writeWord(manager + 0x35c, 77));
  assert(effects->beginFadeTextures() && word(m, manager + 0x35c) == 77 &&
         !effects->fadeTexturesReady() && effects->busy());
  assert(!effects->pumpFadeTextures(1) && effects->pumpFadeTextures(22719) &&
         effects->fadeTexturesReady() && !effects->busy());
  const auto palette = word(m, manager + 0x358);
  assert(word(m, palette + 4) == 4 && word(m, palette + 8) > palette &&
         word(m, manager + 0x35c) == 1);
  assert(effects->beginEnvironmentTextures() && effects->beginTextures());
  assert(effects->pumpEnvironmentTextures(UINT32_MAX) &&
         effects->pumpTextures(UINT32_MAX) &&
         effects->environmentTexturesReady() && effects->texturesReady() &&
         effects->fadeTexturesReady());
  assert(m.writeWord(0x8041e620, manager + 4) && !effects->current() &&
         !effects->fadeTexturesReady());
}
} // namespace
int main() { actualFadeBeforeEffectInit(); }
