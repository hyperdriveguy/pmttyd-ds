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
void actualEnvironmentThenEffects(bool composeEffects) {
  auto process = NativeFieldAnimation::create(
      "romfs", NativeFileCache::create(), platform());
  assert(process && process->ready());
  auto ui = NativeUiStartup::create(process, "romfs");
  assert(ui && ui->resumeFont());
  auto effects = NativeEffectProcess::create(ui, "romfs");
  assert(effects && effects->initialize());
  auto &m = process->process().vm().memory;
  assert(m.writeWord(0x803e7274,
                     0x11223344)); // Last filename word, adjacent to new work.
  assert(effects->initializeEnvironment() && word(m, 0x8041ec90) == 0 &&
         word(m, 0x8041ec94) == 0 && word(m, 0x803e7274) == 0x11223344);
  assert(m.writeWord(0x803e7278, 0xaabbccdd) &&
         effects->initializeEnvironment() && word(m, 0x803e7278) == 0xaabbccdd);
  assert(!effects->beginEnvironmentTextures()); // Window's real shared archive
                                                // owner is not installed yet.
  assert(ui->resumeWindow() && m.writeWord(0x8041ec94, 77));
  assert(effects->beginEnvironmentTextures() && effects->busy() &&
         !effects->environmentTexturesReady());
  const auto palette = word(m, 0x8041ec90);
  assert(palette && word(m, 0x8041ec94) == 77);
  assert(effects->beginEnvironmentTextures() && word(m, 0x8041ec90) == palette);
  assert(!effects->pumpEnvironmentTextures(1) && word(m, 0x8041ec94) == 77);
  assert(effects->pumpEnvironmentTextures(37247) &&
         effects->environmentTexturesReady() && !effects->busy());
  assert(word(m, palette) == 0x0020af30 && word(m, palette + 4) == 11 &&
         word(m, palette + 8) > palette && word(m, 0x8041ec94) == 1);
  assert(effects->pumpEnvironmentTextures(0) &&
         effects->initializeEnvironment() && word(m, 0x8041ec90) == palette);
  if (composeEffects) {
    assert(effects->beginTextures() && effects->pumpTextures(266784 + 235776) &&
           effects->texturesReady() && effects->environmentTexturesReady());
    return;
  }
  // Check environment-only late archive proof before an effect owner can mask
  // it. Byte-equal replacement of the late-borrowed descriptor owner must
  // invalidate.
  std::vector<uint8_t> copy(160);
  for (unsigned i = 0; i < copy.size(); ++i)
    assert(m.readByte(0x803dbff0 + i, copy[i]));
  assert(m.unregisterRam(0x803dbff0) &&
         m.registerRam(0x803dbff0, std::move(copy)));
  assert(!effects->current() && !effects->environmentTexturesReady());
}
} // namespace
int main() {
  actualEnvironmentThenEffects(true);
  actualEnvironmentThenEffects(false);
}
